#ifndef __USER_MODBUS_H
#define __USER_MODBUS_H

#include <rtthread.h>
#include "user.h"

//  Modbus 从机数量
#define NUMBER_OF_MACHINES 24 //从机数量

#define COIL_HIGH 0xFF00
#define COIL_LOW  0x0000

// Modbus 货架传感器从机寄存器地址和数量定义
#define NUMBER_OF_SENSOR_COILS_ON_THE_SHELF             5 //货架传感器线圈数量
#define NUMBER_OF_SENSOR_DISCRETE_INPUTS_ON_THE_SHELF   1 //货架传感器离散输入数量
#define NUMBER_OF_SENSOR_INPUT_REGISTERS_ON_THE_SHELF   4 //货架传感器输入寄存器数量
#define NUMBER_OF_SENSOR_HOLDING_REGISTERS_ON_THE_SHELF 0 //货架传感器保持寄存器数量

// Modbus 可燃气体传感器从机寄存器地址和数量定义
#define NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_COILS             5 //可燃气体传感器线圈数量
#define NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_DISCRETE_INPUTS   1 //可燃气体传感器离散输入数量
#define NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_INPUT_REGISTERS   4 //可燃气体传感器输入寄存器数量
#define NUMBER_OF_COMBUSTIBLE_GAS_SENSOR_HOLDING_REGISTERS 0 //可燃气体传感器保持寄存器数量

// Modbus 烟雾传感器从机寄存器地址和数量定义
#define NUMBER_OF_SMOKE_SENSOR_COILS             5 //烟雾传感器线圈数量
#define NUMBER_OF_SMOKE_SENSOR_DISCRETE_INPUTS   1 //烟雾传感器离散输入数量
#define NUMBER_OF_SMOKE_SENSOR_INPUT_REGISTERS   4 //烟雾传感器输入寄存器数量
#define NUMBER_OF_SMOKE_SENSOR_HOLDING_REGISTERS 0 //烟雾传感器保持寄存器数量

// Modbus 货架环境光传感器从机寄存器地址和数量定义
#define NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_COILS             5 //货架环境光传感器线圈数量
#define NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_DISCRETE_INPUTS   1 //货架环境光传感器离散输入数量
#define NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_INPUT_REGISTERS   5 //货架环境光传感器输入寄存器数量
#define NUMBER_OF_SHELF_AMBIENT_LIGHT_SENSOR_HOLDING_REGISTERS 0 //货架环境光传感器保持寄存器数量

// Modbus 设备控制传感器从机寄存器地址和数量定义
#define NUMBER_OF_DEVICE_CONTROL_SENSOR_COILS             5  //设备控制传感器线圈数量
#define NUMBER_OF_DEVICE_CONTROL_SENSOR_DISCRETE_INPUTS   1  //设备控制传感器离散输入数量
#define NUMBER_OF_DEVICE_CONTROL_SENSOR_INPUT_REGISTERS   14 //设备控制传感器
#define NUMBER_OF_DEVICE_CONTROL_SENSOR_HOLDING_REGISTERS 2  //设备控制传感器保持寄存器数量

/* 传感器状态枚举定义 */
typedef enum
{
    sensorOffline = 0, /* 传感器离线状态 */
    sensorOnline,      /* 传感器在线状态 */
} mb_sensor_status_t;

typedef enum
{
    ShelfSensors,            /* 货架传感器 */
    CombustibleGasSensors,   /* 可燃气体传感器 */
    SmokeSensors,            /* 烟雾传感器 */
    ShelfAmbientLightSensor, /* 货架环境光传感器 */
    DeviceControlSensors,    /* 设备控制传感器 */
} mb_sensortype_def;

typedef struct
{
    rt_list_t list;                    /* 设备节点链表 */

    rt_uint8_t         slaveID;        /* 从机ID */
    mb_sensor_status_t status;         /* 从机状态 */
    mb_sensortype_def  type;           /* 从机类型 */
    rt_uint32_t        BusErrorCount;  /* 总线错误计数 */

    rt_uint16_t CoilAddress;           /* 线圈寄存器地址 */
    rt_uint16_t CoilQuantity;          /* 线圈寄存器数量 */

    rt_uint16_t DiscreteInputAddress;  /* 离散输入寄存器地址 */
    rt_uint16_t DiscreteInputQuantity; /* 离散输入寄存器数量 */

    rt_uint16_t InputRegAddress;       /* 输入寄存器地址 */
    rt_uint16_t InputRegQuantity;      /* 输入寄存器数量 */

    rt_uint16_t HoldingRegAddress;     /* 保持寄存器地址 */
    rt_uint16_t HoldingRegquantity;    /* 保持寄存器数量 */

} mb_sensor_def, *pmb_sensor_def;

rt_list_t  *get_device_list(void);
rt_mutex_t  get_list_lock(void);
rt_mailbox_t get_mb(void);
rt_mailbox_t get_mb1(void);

#endif /* USER_MODBUS_H */
