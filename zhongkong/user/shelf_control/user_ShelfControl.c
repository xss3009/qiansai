#include "user_ShelfControl.h"
#include "user_rtc.h"
#include "user_mb_app.h"
#include "user_modbus.h"
#include "control_algorithm.h"
#include "dma_sender.h"

void sampleShelvesEnvironmentalData(DayData_Def *dayData)
{
    // 声明Modbus数据缓冲区（来自外部文件）
    extern USHORT usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];
    //    extern UCHAR  ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS / 8];
    extern UCHAR  ucMDiscInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_DISCRETE_INPUT_NDISCRETES / 8];
    //    extern Warehouse_Def warehouse;

    static rtc_counting i              = 0; // 5分钟周期计数器
    static rtc_counting fiveMinCounter = 0; // 5分钟采样计数器
    rt_list_t          *sensor_list;

    //    Warehouse_Def *deviceStatus = &warehouse; // 传感器状态结构体

    time_t       now;                       // 当前时间戳
    rt_uint16_t  averageTemperature    = 0; // 温度平均值
    rt_uint16_t  averageHumidity       = 0; // 湿度平均值
    rt_uint16_t  averageCargo          = 0; // 货物状态平均值
    rtc_counting NumberOfOnlineSensors = 0; // 在线传感器计数器
    rt_err_t     result;
    pmb_sensor_def pdevice, tmp;

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

    i++;                    // 触发计数递增

    if (i >= SAMPLING_TIME) // 每5分钟触发一次（需要配合外部计数器逻辑）
    {
        // 获取并打印当前时间
        get_timestamp(&now);
#ifdef DEBUG_ENABLE
        rt_kprintf("现在是: %.*s\r\n", 25, ctime(&now));

        // 打印表格标题
        rt_kprintf("============================================================================================\n");
        rt_kprintf("|\t序号\t\t|\t温度\t|\t湿度\t|\t占用\t|\n");
#endif
        // 遍历所有传感器
        rt_list_for_each_entry_safe(pdevice, tmp, sensor_list, list)
        {
            if (pdevice->slaveID < 1 || pdevice->slaveID > MB_MASTER_TOTAL_SLAVE_NUM)
            {
                rt_kprintf("Invalid slaveID: %d\n", pdevice->slaveID);
                continue;
            }

            if (pdevice->status == sensorOnline)
            {

                if ((pdevice->type == ShelfSensors) || (pdevice->type == DeviceControlSensors) ||
                    (pdevice->type == ShelfAmbientLightSensor))
                {
#ifdef DEBUG_ENABLE
                    // 打印在线传感器数据（温度、湿度、货物状态）
                    rt_kprintf("|\t%d\t\t|\t%d.%d℃\t|\t%d.%d%\t|\t%s\t|\n",
                               pdevice->slaveID,
                               (usMRegInBuf[pdevice->slaveID - 1][0] / 10),                        // 温度整数部分
                               (usMRegInBuf[pdevice->slaveID - 1][0] % 10),                        // 温度小数部分
                               (usMRegInBuf[pdevice->slaveID - 1][1] / 10),                        // 湿度整数部分
                               (usMRegInBuf[pdevice->slaveID - 1][1] % 10),                        // 湿度小数部分
                               (ucMDiscInBuf[pdevice->slaveID - 1][0] & 0x01 ? "true" : "false")); // 货物存在状态

                    if(ucMDiscInBuf[pdevice->slaveID - 1][0]) {
                        uint8_t id = pdevice->slaveID;
                        uint8_t temp_val = usMRegInBuf[pdevice->slaveID - 1][0] / 10;
                        uint8_t humi_val = usMRegInBuf[pdevice->slaveID - 1][1] / 10;

                        // 立即发送五次
                        send_sensor_data_repeatedly(id, temp_val, humi_val);
                    }


#endif
                    // 累加计算平均值
                    averageTemperature += usMRegInBuf[pdevice->slaveID - 1][0];
                    averageHumidity    += usMRegInBuf[pdevice->slaveID - 1][1];
                    averageCargo       += ucMDiscInBuf[pdevice->slaveID - 1][0] & 0x01;
                    NumberOfOnlineSensors++;
                }
                else
                {
                    // 打印在线传感器数据（温度、湿度、货物状态）
                    rt_kprintf("|\t%d\t\t|\t%d.%d℃\t|\t%d.%d%\t|\t%s\t|\n",
                               pdevice->slaveID,
                               (usMRegInBuf[pdevice->slaveID - 1][0] / 10), // 温度整数部分
                               (usMRegInBuf[pdevice->slaveID - 1][0] % 10), // 温度小数部分
                               (usMRegInBuf[pdevice->slaveID - 1][1] / 10), // 湿度整数部分
                               (usMRegInBuf[pdevice->slaveID - 1][1] % 10), // 湿度小数部分
                               "N/A");                                      // 货物存在状态
                    averageTemperature += usMRegInBuf[pdevice->slaveID - 1][0];
                    averageHumidity    += usMRegInBuf[pdevice->slaveID - 1][1];
                    NumberOfOnlineSensors++;
                }
            }
            else
            {
                /* 处理离线传感器*/
#ifdef DEBUG_ENABLE
                // 处理离线传感器
                rt_kprintf("|\t%d\t\t|\t不在线\t|\t不在线\t|\t不在线\t|\n", pdevice->slaveID);
#endif
            }
        }

#ifdef DEBUG_ENABLE
        // 打印平均值数据
        rt_kprintf("|\t平均值为\t|\t%d.%d\t|\t%d.%d\t|\t%d%%\t|\n",
                   (averageTemperature / NumberOfOnlineSensors) / 10, // 平均温度整数
                   (averageTemperature / NumberOfOnlineSensors) % 10, // 平均温度小数
                   (averageHumidity / NumberOfOnlineSensors) / 10,    // 平均湿度整数
                   (averageHumidity / NumberOfOnlineSensors) % 10,    // 平均湿度小数
                   averageCargo * 100 / 16);                          // 货物存在平均概率

        rt_kprintf("============================================================================================\n");
        rt_kprintf("传感器在线数: %d\n", NumberOfOnlineSensors);
        cmmmmmm = NumberOfOnlineSensors;
#endif
        if (((averageTemperature / NumberOfOnlineSensors) / 10 > 29) ||
            ((averageHumidity / NumberOfOnlineSensors) / 10 > 70))
        {
            result = rt_mb_send(get_mb1(), 100);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mb_send ERR\n");
            }
        }
        else
        {
            result = rt_mb_send(get_mb1(), 0);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mb_send ERR\n");
            }
        }

        // 采样计数器处理
        fiveMinCounter++;
        if (fiveMinCounter >= SAMPLING_INTERVAL) // 达到采样间隔时保存数据
        {
#ifdef DEBUG_ENABLE
            rt_kprintf("保存当前采样周期数据到日统计结构体\n");
#endif
            // 保存当前采样周期数据到日统计结构体
            for (int num = 0; num < NUMBER_OF_MACHINES; num++)
            {
                dayData->temperature[num][dayData->counting] = usMRegInBuf[num][0];
                dayData->humidity[num][dayData->counting]    = usMRegInBuf[num][1];
                dayData->goods[num][dayData->counting]       = ucMDiscInBuf[num][0] & 0x01;
#ifdef DEBUG_ENABLE
                rt_kprintf("num:%d,temperature:%d,humidity:%d,goods:%d\n",
                           num + 1,
                           dayData->temperature[num][dayData->counting],
                           dayData->humidity[num][dayData->counting],
                           dayData->goods[num][dayData->counting]);
#endif
            }

            // 计算仓库平均值

            dayData->averageWarehouseTemperature[dayData->counting] = (averageTemperature / NumberOfOnlineSensors) / 10;
            dayData->averageWarehouseHumidity[dayData->counting]    = (averageHumidity / NumberOfOnlineSensors) / 10;
            dayData->averageWarehouseGoods[dayData->counting]       = averageCargo * 100 / NUMBER_OF_DISTANCE_SENSORS;
#ifdef DEBUG_ENABLE
            rt_kprintf("仓库平均温度：%d.%d℃\n",
                       dayData->averageWarehouseTemperature[dayData->counting],
                       dayData->averageWarehouseTemperature[dayData->counting] % 10);
            rt_kprintf("仓库平均湿度：%d.%d%%\n",
                       dayData->averageWarehouseHumidity[dayData->counting],
                       dayData->averageWarehouseHumidity[dayData->counting] % 10);
            rt_kprintf("仓库平均使用率：%d%%\n", dayData->averageWarehouseGoods[dayData->counting]);
            tmmmmmm = dayData->averageWarehouseTemperature[dayData->counting];
            smmmmmm = dayData->averageWarehouseHumidity[dayData->counting];
            ckmmmmm = dayData->averageWarehouseGoods[dayData->counting];

#endif

            dayData->counting++; // 日数据计数器递增
            fiveMinCounter = 0;  // 重置采样计数器
        }
        i = 0;                   // 重置5分钟周期计数器
    }
}

void collectDailyWarehouseData(DayData_Def *dayData)
{
    time_t      now;                                          // 当前时间戳
    rt_uint32_t averageTemperature[NUMBER_OF_MACHINES] = {0}; // 各机器温度总和
    rt_uint32_t averageHumidity[NUMBER_OF_MACHINES]    = {0}; // 各机器湿度总和
    rt_uint16_t averageCargo[NUMBER_OF_MACHINES]       = {0}; // 各机器货物状态总和

    // 仓库级统计变量
    rt_uint32_t warehouseThermometerCount              = 0; // 仓库温度计数
    rt_uint32_t warehouseHumidityCount                 = 0; // 仓库湿度计数
    rt_uint32_t warehouseCargoCount                    = 0; // 仓库货物状态计数

    get_timestamp(&now);
#ifdef DEBUG_ENABLE
    rt_kprintf("现在是: %.*s", 25, ctime(&now));

    // 打印每日统计结果
    rt_kprintf("============================================================================================\n");
    rt_kprintf("|\t编号\t|\t温度\t|\t湿度\t|\t占用率\t|\n");
#endif

    // 遍历所有机器计算日均值
    for (int i = 0; i < NUMBER_OF_MACHINES; i++)
    {
        // 累加该机器所有采样周期的数据
        for (int j = 0; j < dayData->counting; j++)
        {
            averageTemperature[i] += dayData->temperature[i][j];
            averageHumidity[i]    += dayData->humidity[i][j];
            averageCargo[i]       += dayData->goods[i][j];
        }

        // 计算该机器各参数的日平均值
        dayData->averageDailyTemperature[i] = averageTemperature[i] / dayData->counting;
        dayData->averageDailyHumidity[i]    = averageHumidity[i] / dayData->counting;
        dayData->averageDailyGoods[i]       = averageCargo[i] * 100 / dayData->counting;
#ifdef DEBUG_ENABLE
        rt_kprintf("|\t%d\t|\t%d℃\t|\t%d%\t|\t%d%\t|\n",
                   i + 1,
                   dayData->averageDailyTemperature[i] / 10,
                   dayData->averageDailyHumidity[i] / 10,
                   dayData->averageDailyGoods[i]);
#endif
    }
#ifdef DEBUG_ENABLE
    rt_kprintf("============================================================================================\n");
#endif
    // 计算仓库级全局平均值
    for (int i = 0; i < dayData->counting; i++)
    {
        warehouseThermometerCount += dayData->averageWarehouseTemperature[i];
        warehouseHumidityCount    += dayData->averageWarehouseHumidity[i];
        warehouseCargoCount       += dayData->averageWarehouseGoods[i];
    }

    // 存储最终仓库级统计结果
    dayData->DailyWarehouseTemperature = warehouseThermometerCount / dayData->counting;
    dayData->DailyWarehouseHumidity    = warehouseHumidityCount / dayData->counting;
    dayData->DailyWarehouseGoods       = warehouseCargoCount / dayData->counting;
#ifdef DEBUG_ENABLE
    // 打印每日统计结果
    rt_kprintf("|\t仓库温度\t|\t仓库湿度\t|\t仓库使用率\t|\n");
    rt_kprintf("|\t%d℃\t\t|\t%d%\t\t|\t%d%\t\t|\n",
               dayData->DailyWarehouseTemperature,
               dayData->DailyWarehouseHumidity,
               dayData->DailyWarehouseGoods);
    rt_kprintf("============================================================================================\n");

    rt_kprintf("计数: %d", dayData->counting);
#endif
    dayData->counting = 0;
}
