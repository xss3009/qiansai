#include "user_FireControl.h"
#include "user_mb_app.h"
#include "user.h"
#include "user_modbus.h"
#include "control_algorithm.h"

static struct rt_event event;
static rt_timer_t      fire_detection_timer;

static rt_thread_t fire_detection_thread;
static PID_TypeDef LED_PID = {
    .Kp = 2.5,
    .Ki = 0.08,
    .Kd = 0.02,
};

Warehouse_Def warehouse;

//后续改为静态函数
static void fire_detection_timer_callback(void *parameter)
{
    // 秒级控制火情，及时应对情况
    extern UCHAR  ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS / 8];
    extern USHORT usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];

    static rt_uint8_t i = 0, temp = 0;

    rt_err_t result;

    rt_uint32_t alarmCount[NUMBER_OF_MACHINES] = {0};

    Warehouse_Def *deviceStatus                = &warehouse; // 传感器状态结构体

    time_t now;                                              // 当前时间戳

    rt_uint8_t fireProbability                 = 0;          // 火情检测概率
    rt_uint8_t fireCount                       = 0;          // 火情检测计数

    rt_uint8_t aFireMayOccur                   = 0;          // 可能发生火情标志

    rt_uint32_t AverageCombustibleGasDetection = 0;
    rt_uint32_t AverageSmokeDetection          = 0;
    rt_uint32_t AverageLightIntensity          = 0;

    rt_list_t     *sensor_list;
    pmb_sensor_def pdevice, tmp;

    PAlgorithmDevice Dev = GetAlgorithmDevice();

    // 火灾报警复位处理
    if (deviceStatus->status_flag.FireResetStatus == 1)
    {
        // 遍历所有传感器复位报警计数器
        for (rt_uint8_t i = 0; i < NUMBER_OF_MACHINES; i++)
        {
            alarmCount[i] = 0;
        }
        deviceStatus->status_flag.FireResetStatus = 0;
    }

    if ((deviceStatus->BusErrorCount >= BUS_ERROR_COUNT_JUDGMENT) &&
        (deviceStatus->status_flag.SlaveDebugStatus == 0))      // 总线错误计数判断且从机处于调试模式
    {
        if (deviceStatus->status_flag.CombustibleGasState == 1) // 可燃气体状态为1
        {
            /* 添加可燃气体状态判断处理 */
#ifdef DEBUG_ENABLE
            rt_kprintf("可能发生爆炸\n");
#endif
        }
        else
        {
            /* 添加总线错误计数超过阈值处理 */
#ifdef DEBUG_ENABLE
            rt_kprintf("总线错误计数超过阈值，请查看传感器是否离线\n");

#endif
        }
    }

    // 获取并打印当前时间
    get_timestamp(&now);
#ifdef DEBUG_ENABLE
    rt_kprintf("现在是: %.*s\r\n", 25, ctime(&now));

    rt_kprintf("火情检测：\n");
    rt_kprintf(
        "============================================================================================================="
        "=======\n");
    rt_kprintf("|\t序号\t\t|\t火焰检测\t|\t烟雾检测\t|\t可燃气体检测\t|\t环境光\t\t|\n");
#endif
get:
    rt_mutex_take(get_list_lock(), RT_WAITING_FOREVER);

    sensor_list = get_device_list();

    if (rt_list_isempty(sensor_list))
    {
        rt_kprintf("第一次获取链表失败\n");
        sensor_list = get_device_list();
        if (rt_list_isempty(sensor_list))
        {
            rt_kprintf("第二次获取链表失败\n");
            rt_mutex_release(get_list_lock());
            return;
        }
    }
    rt_mutex_release(get_list_lock());
    //遍历所有火焰传感器
    rt_list_for_each_entry_safe(pdevice, tmp, sensor_list, list)
    {
        if (pdevice->slaveID < 1 || pdevice->slaveID > NUMBER_OF_MACHINES)
        {
            rt_kprintf("Invalid slaveID: %d\n", pdevice->slaveID);
            goto get;
        }

        if (pdevice->status == sensorOnline)
        {
            // 如果火焰传感器检测到火焰
            if (ucMCoilBuf[pdevice->slaveID - 1][1])
            {
                //对应的传感器计数加1
                alarmCount[pdevice->slaveID - 1]++;
                fireCount++;
            }

            if (pdevice->slaveID > 16 && pdevice->slaveID <= 18)
            {
                rt_kprintf("| \t%d\t\t|\t%s\t\t|\t%s\t\t|\t%d%%\t\t|\t%s\t\t|\n",
                           pdevice->slaveID,
                           (ucMCoilBuf[pdevice->slaveID - 1][1] ? "true" : "false"),
                           "N/A",                                             // 烟雾检测未实现
                           usMRegInBuf[pdevice->slaveID - 1][3] * 100 / 4096, // 可燃气体检测
                           "N/A");
                AverageCombustibleGasDetection += usMRegInBuf[pdevice->slaveID - 1][3];
            }
            else if (pdevice->slaveID > 16 && pdevice->slaveID <= 20)
            {
                rt_kprintf("| \t%d\t\t|\t%s\t\t|\t%d%%\t\t|\t%s\t\t|\t%s\t\t|\n",
                           pdevice->slaveID,
                           (ucMCoilBuf[pdevice->slaveID - 1][1] ? "true" : "false"),
                           usMRegInBuf[pdevice->slaveID - 1][3] * 100 / 4096, // 烟雾检测
                           "N/A",                                             // 可燃气体检测未实现
                           "N/A");
                AverageSmokeDetection += usMRegInBuf[pdevice->slaveID - 1][3];
            }
            else if (pdevice->slaveID > 16 && pdevice->slaveID <= 23)
            {
                rt_kprintf("| \t%d\t\t|\t%s\t\t|\t%s\t\t|\t%s\t\t|\t%d\t\t|\n",
                           pdevice->slaveID,
                           (ucMCoilBuf[pdevice->slaveID - 1][1] ? "true" : "false"),
                           "N/A", // 烟雾检测未实现
                           "N/A", // 可燃气体检测未实现
                           usMRegInBuf[pdevice->slaveID - 1][4]);
                AverageLightIntensity += usMRegInBuf[pdevice->slaveID - 1][4];
            }
            else
            {
#ifdef DEBUG_ENABLE
                rt_kprintf("| \t%d\t\t|\t%s\t\t|\t%s\t\t|\t%s\t\t|\t%s\t\t|\n",
                           pdevice->slaveID,
                           (ucMCoilBuf[pdevice->slaveID - 1][1] ? "true" : "false"),
                           "N/A", // 烟雾检测未实现
                           "N/A", // 可燃气体检测未实现
                           "N/A");
#endif
            }
        }
        else
        {
#ifdef DEBUG_ENABLE
            rt_kprintf(
                "| \t%d\t\t|\t%s\t\t|\t%s\t\t|\t%s\t\t|\t%s\t\t|\n", pdevice->slaveID, "离线", "离线", "离线", "离线");

#endif

            if ((alarmCount[pdevice->slaveID - 1] > 1) && pdevice->status == sensorOffline)
            {
                aFireMayOccur++; // 如果检测到传感器离线且报警次数超过1次，则可能发生火情
#ifdef DEBUG_ENABLE
                rt_kprintf("传感器 %d 离线，报警次数：%d\n", pdevice->slaveID, alarmCount[pdevice->slaveID - 1]);
#endif
            }
        }
    }

    fireProbability        = fireCount * 100 / NUMBER_OF_FLAME_SENSORS; // 平均火焰检测概率
    AverageLightIntensity /= NUMBER_OF_AMBIENT_LIGHT_SENSORS;
#ifdef DEBUG_ENABLE
    rt_kprintf("|\t平均概率\t|\t%d%%\t\t|\t%d%%\t\t|\t%d%%\t\t|\t%d\t\t|\n",
               fireProbability,
               AverageSmokeDetection = AverageSmokeDetection * 100 / 4096 / NUMBER_OF_SMOKE_SENSORS,
               AverageCombustibleGasDetection =
                   AverageCombustibleGasDetection * 100 / 4096 / NUMBER_OF_COMBUSTIBLE_GAS_SENSORS,
               AverageLightIntensity);
#endif

    i++;
    if (i >= 4)
    {
        if (Dev->int_abs((int)usMRegInBuf[22][4] - LIGHT_INTENSITY_THRESHOLD) > 100)
        {
            int output  = Dev->Incremental_PID(&LED_PID, usMRegInBuf[22][4], LIGHT_INTENSITY_THRESHOLD);

            output      = Dev->limit_int(output, -2, 2);

            temp       += output;

            temp        = Dev->limit_int(temp, 0, 100);

            result      = rt_mb_send(get_mb(), temp);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mb_send ERR\n");
            }
        }
    }
    // 如果可能发生火情标志大于等于阈值，或者火焰检测概率大于等于50%，则触发报警
    if ((aFireMayOccur >= OFFLINE_THRESHOLD_FOR_FIRE_INCIDENTS) ||
        (fireProbability >= AVERAGE_DETECTION_THRESHOLD_FOR_FIRES) ||
        (AverageCombustibleGasDetection >= AVERAGE_COMBUSTIBLE_GAS_THRESHOLD) ||
        (AverageSmokeDetection >= AVERAGE_SMOKE_THRESHOLD))
    {
#ifdef DEBUG_ENABLE
        rt_kprintf("仓库内有火灾发生！覆盖区域约为：%d%%\n", fireProbability);
#endif
        // rt_event_send(&event, 0x01);
    }
#ifdef DEBUG_ENABLE
    rt_kprintf(
        "============================================================================================================="
        "=======\n");
#endif
}

static void FireAlarm_entry(void *parameter)
{
    rt_uint32_t         event_id;
    eMBMasterReqErrCode error_code = MB_MRE_NO_ERR;

    while (1)
    {
        //所有带有警报功能的Modbus传感器报警
        if (rt_event_recv(&event, 0x01, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &event_id) ==
            RT_EOK)
        {
            for (int i = 0; i < NUMBER_OF_BUZZERS; i++)
            {
                error_code = eMBMasterReqWriteCoil(i + 1, 1, COIL_HIGH, RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)
                {
                    /*发生火情后传感器警报启动失败处理*/
#ifdef DEBUG_ENABLE
                    rt_kprintf("蜂鸣器%d报警开启失败\n", i + 1);
#endif
                }
            }
            rt_thread_mdelay(100);
        }
    }
}

rt_err_t fire_detection_init(void)
{
    // 创建周期为FIRE_DETECTION_CYCLE_MS毫秒的定时器
    fire_detection_timer = rt_timer_create("fire_tmr",
                                           fire_detection_timer_callback,
                                           RT_NULL,
                                           FIRE_DETECTION_CYCLE_MS,
                                           RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);

    if (rt_event_init(&event, "event", RT_IPC_FLAG_PRIO) != RT_EOK)
    {
        rt_kprintf("rt_event_init error.\n");
        return -1;
    }

    fire_detection_thread = rt_thread_create("fire_detection_thread", //线程名字
                                             FireAlarm_entry,         //入口函数
                                             RT_NULL,                 //入口函数参数
                                             512,                     //栈大小
                                             22,                      //线程优先级
                                             5);

    if ((fire_detection_timer != RT_NULL))
    {
        rt_timer_start(fire_detection_timer);
        rt_thread_startup(fire_detection_thread);
        return RT_EOK;
    }
    else
    {
        rt_kprintf("fire_detection_init error\n");
    }

    return -RT_ERROR;
}
// INIT_APP_EXPORT(fire_detection_init);
