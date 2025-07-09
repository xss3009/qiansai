#include <rtthread.h>
#include "user_modbus.h"
#include "user_mb_app.h"

#include "mb.h"
#include "mb_m.h"

static rt_list_t         device_list; // 设备列表
static struct rt_mutex   device_mutex;
static struct rt_mailbox mb, mb1;

static char mb_pool[4];
static char mb_pool1[4];

rt_list_t   *get_device_list(void) { return &device_list; } // 获取设备列表指针
rt_mutex_t   get_list_lock(void) { return &device_mutex; }
rt_mailbox_t get_mb(void) { return &mb; }
rt_mailbox_t get_mb1(void) { return &mb1; }

static void send_thread_entry(void *parameter)
{
    eMBMasterReqErrCode error_code = MB_MRE_NO_ERR; // Modbus 请求错误码
    pmb_sensor_def      pdevice, tmp;
    int                 val;

    // 主线程循环
    while (1)
    {
        rt_list_for_each_entry_safe(pdevice, tmp, &device_list, list)
        {
            // 添加指针有效性验证
            if (pdevice->slaveID < 1 || pdevice->slaveID > MB_MASTER_TOTAL_SLAVE_NUM)
            {
                rt_kprintf("Invalid slaveID: %d\n", pdevice->slaveID);
                continue;
            }

            // 发送Modbus输入寄存器读取请求
            if (pdevice->InputRegQuantity > 0)
            {
                error_code = eMBMasterReqReadInputRegister(
                    pdevice->slaveID, pdevice->InputRegAddress, pdevice->InputRegQuantity, RT_WAITING_FOREVER);
                /* 错误处理 */
                if (error_code != MB_MRE_NO_ERR)
                {
                    pdevice->status = sensorOffline;

#ifdef DEBUG_ENABLE
                    // 打印错误日志并计数器
                    rt_kprintf(
                        "eMBMasterReqReadInputRegister %d failed, error code: %d\n", pdevice->slaveID, error_code);
#endif
                    pdevice->BusErrorCount++;
                }
                else
                {
                    pdevice->status = sensorOnline; // 记录传感器状态
                }
            }

            // 发送Modbus线圈读取请求
            if (pdevice->CoilQuantity > 0)
            {
                error_code = eMBMasterReqReadCoils(
                    pdevice->slaveID, pdevice->CoilAddress, pdevice->CoilQuantity, RT_WAITING_FOREVER);

                /* 错误处理 */
                if (error_code != MB_MRE_NO_ERR)
                {
                    pdevice->status = sensorOffline;

#ifdef DEBUG_ENABLE
                    // 打印错误日志并计数器
                    rt_kprintf("eMBMasterReqReadCoils %d failed, error code: %d\n", pdevice->slaveID, error_code);
#endif
                    pdevice->BusErrorCount++;
                }
                else
                {
                    pdevice->status = sensorOnline; // 记录传感器状态
                }
            }

            // 发送Modbus离散输入读取请求
            if (pdevice->DiscreteInputQuantity > 0)
            {
                error_code = eMBMasterReqReadDiscreteInputs(pdevice->slaveID,
                                                            pdevice->DiscreteInputAddress,
                                                            pdevice->DiscreteInputQuantity,
                                                            RT_WAITING_FOREVER);
                /* 错误处理 */
                if (error_code != MB_MRE_NO_ERR)
                {
                    pdevice->status = sensorOffline;

#ifdef DEBUG_ENABLE
                    // 打印错误日志并计数器
                    rt_kprintf(
                        "eMBMasterReqReadDiscreteInputs %d failed, error code: %d\n", pdevice->slaveID, error_code);
#endif
                    pdevice->BusErrorCount++;
                }
                else
                {
                    pdevice->status = sensorOnline; // 记录传感器状态
                }
            }

            // 发送Modbus保持寄存器读取请求
            if (pdevice->HoldingRegquantity > 0)
            {
                error_code = eMBMasterReqReadHoldingRegister(
                    pdevice->slaveID, pdevice->HoldingRegAddress, pdevice->HoldingRegquantity, RT_WAITING_FOREVER);
                /* 错误处理 */
                if (error_code != MB_MRE_NO_ERR)
                {
                    pdevice->status = sensorOffline;
#ifdef DEBUG_ENABLE
                    // 打印错误日志并计数器
                    rt_kprintf(
                        "eMBMasterReqReadHoldingRegister %d failed, error code: %d\n", pdevice->slaveID, error_code);
#endif
                    pdevice->BusErrorCount++;
                }
                else
                {
                    pdevice->status = sensorOnline; // 记录传感器状态
                }
            }

            if (rt_mb_recv(&mb, (rt_ubase_t *)&val, 0) == RT_EOK)
            {
                if (val > 100)
                {
                    val = 100;
                }
                else if (val < 0)
                {
                    val = 0;
                }
                eMBMasterReqWriteHoldingRegister(24, 0, val, RT_WAITING_FOREVER);
            }

            if (rt_mb_recv(&mb1, (rt_ubase_t *)&val, 0) == RT_EOK)
            {
                if (val > 100)
                {
                    val = 100;
                }
                else if (val < 0)
                {
                    val = 0;
                }
                eMBMasterReqWriteHoldingRegister(24, 1, val, RT_WAITING_FOREVER);
            }
        }
    }
}

static void mb_master_poll(void *parameter)
{
    eMBMasterInit(MB_RTU, PORT_NUM, PORT_BAUDRATE, PORT_PARITY);
    eMBMasterEnable();

    while (1)
    {
        eMBMasterPoll();

        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

static int mb_master_sample(int argc, char **argv)
{
    static rt_uint8_t is_init = 0;

    rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

    rt_err_t result;

    rt_list_init(&device_list);

    result = rt_mutex_init(&device_mutex, "device_mutex", RT_IPC_FLAG_PRIO);
    if (result != RT_EOK)
    {
        rt_kprintf("rt_mutex_init failed: %d\n", result);
        return -1;
    }

    /* 初始化邮箱 */
    result = rt_mb_init(&mb,                 //邮箱对象的句柄
                        "mbt",               //邮箱的名字
                        &mb_pool[0],         //内存池指向 mb_pool
                        sizeof(mb_pool) / 4, //邮箱中能容纳的邮件数量,每封邮件占四字节
                        RT_IPC_FLAG_FIFO);   //如果有多个线程等待，按照先来先得到的方法分配邮件
    if (result != RT_EOK)
    {
        rt_kprintf("rt_mb_init ERR\n");
        return -1;
    }

    /* 初始化邮箱 */
    result = rt_mb_init(&mb1,                 //邮箱对象的句柄
                        "mbt1",               //邮箱的名字
                        &mb_pool1[0],         //内存池指向 mb_pool
                        sizeof(mb_pool1) / 4, //邮箱中能容纳的邮件数量,每封邮件占四字节
                        RT_IPC_FLAG_FIFO);    //如果有多个线程等待，按照先来先得到的方法分配邮件
    if (result != RT_EOK)
    {
        rt_kprintf("rt_mb_init ERR\n");
        return -1;
    }

    for (int i = 0; i < NUMBER_OF_MACHINES; i++)
    {
        /* 初始化设备参数 */
        pmb_sensor_def devices = rt_malloc(sizeof(mb_sensor_def));
        RT_ASSERT(devices != RT_NULL);

        rt_list_init(&devices->list);

        devices->slaveID = i + 1;
        devices->status  = sensorOffline;
        if (i < 16)
        {
            devices->type                  = ShelfSensors;

            devices->CoilAddress           = 0x0000;                              // 设置实际线圈地址
            devices->CoilQuantity          = NUMBER_OF_SENSOR_COILS_ON_THE_SHELF; // 设置线圈数量

            devices->DiscreteInputAddress  = 0x0000;
            devices->DiscreteInputQuantity = NUMBER_OF_SENSOR_DISCRETE_INPUTS_ON_THE_SHELF;   // 设置离散输入数量

            devices->InputRegAddress       = 0x0000;                                          // 设置输入寄存器地址
            devices->InputRegQuantity      = NUMBER_OF_SENSOR_INPUT_REGISTERS_ON_THE_SHELF;   // 设置输入寄存器数量

            devices->HoldingRegAddress     = 0x0000;                                          // 设置保持寄存器地址
            devices->HoldingRegquantity    = NUMBER_OF_SENSOR_HOLDING_REGISTERS_ON_THE_SHELF; // 设置保持寄存器数量
        }
        else if (i < 18)
        {
            devices->type                  = CombustibleGasSensors;

            devices->CoilAddress           = 0x0000;                                 // 设置实际线圈地址
            devices->CoilQuantity          = NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_COILS; // 设置线圈数量

            devices->DiscreteInputAddress  = 0x0000;
            devices->DiscreteInputQuantity = NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_DISCRETE_INPUTS;   // 设置离散输入数量

            devices->InputRegAddress       = 0x0000;                                             // 设置输入寄存器地址
            devices->InputRegQuantity      = NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_INPUT_REGISTERS;   // 设置输入寄存器数量

            devices->HoldingRegAddress     = 0x0000;                                             // 设置保持寄存器地址
            devices->HoldingRegquantity    = NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_HOLDING_REGISTERS; // 设置保持寄存器数量
        }
        else if (i < 20)
        {
            devices->type                  = SmokeSensors;

            devices->CoilAddress           = 0x0000;                       // 设置实际线圈地址
            devices->CoilQuantity          = NUMBER_OF_SMOKE_SENSOR_COILS; // 设置线圈数量

            devices->DiscreteInputAddress  = 0x0000;
            devices->DiscreteInputQuantity = NUMBER_OF_SMOKE_SENSOR_DISCRETE_INPUTS;   // 设置离散输入数量

            devices->InputRegAddress       = 0x0000;                                   // 设置输入寄存器地址
            devices->InputRegQuantity      = NUMBER_OF_SMOKE_SENSOR_INPUT_REGISTERS;   // 设置输入寄存器数量

            devices->HoldingRegAddress     = 0x0000;                                   // 设置保持寄存器地址
            devices->HoldingRegquantity    = NUMBER_OF_SMOKE_SENSOR_HOLDING_REGISTERS; // 设置保持寄存器数量
        }
        else if (i < 23)
        {
            devices->type                  = ShelfAmbientLightSensor;

            devices->CoilAddress           = 0x0000;                                     // 设置实际线圈地址
            devices->CoilQuantity          = NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_COILS; // 设置线圈数量

            devices->DiscreteInputAddress  = 0x0000;
            devices->DiscreteInputQuantity = NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_DISCRETE_INPUTS; // 设置离散输入数量

            devices->InputRegAddress       = 0x0000;                                               // 设置输入寄存器地址
            devices->InputRegQuantity      = NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_INPUT_REGISTERS; // 设置输入寄存器数量

            devices->HoldingRegAddress     = 0x0000;                                               // 设置保持寄存器地址
            devices->HoldingRegquantity = NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_HOLDING_REGISTERS;  // 设置保持寄存器数量
        }
        else if (i < 24)
        {
            devices->type                  = DeviceControlSensors;

            devices->CoilAddress           = 0x0000;                                // 设置实际线圈地址
            devices->CoilQuantity          = NUMBER_OF_DEVICE_CONTROL_SENSOR_COILS; // 设置线圈数量

            devices->DiscreteInputAddress  = 0x0000;
            devices->DiscreteInputQuantity = NUMBER_OF_DEVICE_CONTROL_SENSOR_DISCRETE_INPUTS;   // 设置离散输入数量

            devices->InputRegAddress       = 0x0000;                                            // 设置输入寄存器地址
            devices->InputRegQuantity      = NUMBER_OF_DEVICE_CONTROL_SENSOR_INPUT_REGISTERS;   // 设置输入寄存器数量

            devices->HoldingRegAddress     = 0x0000;                                            // 设置保持寄存器地址
            devices->HoldingRegquantity    = NUMBER_OF_DEVICE_CONTROL_SENSOR_HOLDING_REGISTERS; // 设置保持寄存器数量
        }
        /* 将设备节点插入链表尾部 */
        rt_list_insert_after(device_list.prev, &devices->list);
    }

    rt_kprintf("Device list initialized with %d nodes\n", NUMBER_OF_MACHINES);

    if (is_init > 0)
    {
        rt_kprintf("sample is running\n");
        return -RT_ERROR;
    }
    tid1 = rt_thread_create("md_m_poll", mb_master_poll, RT_NULL, 512, MB_POLL_THREAD_PRIORITY, 10);
    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else
    {
        goto __exit;
    }

    tid2 = rt_thread_create("md_m_send", send_thread_entry, RT_NULL, 1024, MB_SEND_THREAD_PRIORITY - 2, 10);
    if (tid2 != RT_NULL)
    {
        rt_thread_startup(tid2);
    }
    else
    {
        goto __exit;
    }

    is_init = 1;
    return RT_EOK;

__exit:
    if (tid1)
        rt_thread_delete(tid1);
    if (tid2)
        rt_thread_delete(tid2);

    return -RT_ERROR;
}

INIT_APP_EXPORT(mb_master_sample);
