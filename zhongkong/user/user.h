#ifndef __USER_H_
#define __USER_H_

/*
 *更改日志：
 *日期           作者       注意
 *2025-06-20     FHS       第一个版本
 */

#include <rtthread.h>
#include "user_modbus.h"

#define DEBUG_ENABLE

// 传感器数量定义
#define NUMBER_OF_DISTANCE_SENSORS             20 //距离传感器数量
#define NUMBER_OF_TEMPERATURE_HUMIDITY_SENSORS 20 //温湿度传感器数量
#define NUMBER_OF_BUZZERS                      24 //蜂鸣器数量
#define NUMBER_OF_FLAME_SENSORS                20 //火焰传感器数量
#define NUMBER_OF_SMOKE_SENSORS                2  //烟雾传感器数量
#define NUMBER_OF_COMBUSTIBLE_GAS_SENSORS      2  //可燃气体传感器数量
#define NUMBER_OF_AMBIENT_LIGHT_SENSORS        3  //环境光传感器数量

//火灾判断阈值
#define OFFLINE_THRESHOLD_FOR_FIRE_INCIDENTS 2   //离线阈值，用于判断是否发生火灾

#define AVERAGE_DETECTION_THRESHOLD_FOR_FIRES 25 //离线阈值，用于判断是否发生火灾
#define AVERAGE_COMBUSTIBLE_GAS_THRESHOLD     30 //离线阈值，用于判断是否发生火灾
#define AVERAGE_SMOKE_THRESHOLD               30 //离线阈值，用于判断是否发生火灾

//判断阈值
#define TEMPERATURE_THRESHOLD     25
#define HUMIDITY_THRESHOLD        50
#define LIGHT_INTENSITY_THRESHOLD 1000 //环境光强度阈值

// Modbus 通信参数配置
#define PORT_NUM      5         // 使用的串口端口号（如：COM5）
#define PORT_BAUDRATE 115200    // 串口通信波特率

#define PORT_PARITY MB_PAR_NONE // 无奇偶校验模式

// 线程优先级配置
#define MB_POLL_THREAD_PRIORITY 10                         // 轮询线程优先级（数值越小优先级越高）
#define MB_SEND_THREAD_PRIORITY RT_THREAD_PRIORITY_MAX - 1 // 发送线程最高优先级

// Modbus 输入寄存器配置
#define MB_SEND_REG_START 0 // 起始寄存器地址
#define MB_SEND_REG_NUM   4 // 每次读取的寄存器数量

// Modbus 线圈配置
#define MB_SEND_COIL_START 0 // 起始线圈地址
#define MB_SEND_COIL_NUM   5 // 每次读取的线圈数量

// Modbus 离散输入配置
#define MB_SEND_DISCRETE_START 0 // 起始离散输入地址
#define MB_SEND_DISCRETE_NUM   1 // 每次读取的离散输入数量

// 周期参数配置
#define MB_POLL_CYCLE_MS         8   // 轮询周期（单位：毫秒）
#define FIRE_DETECTION_CYCLE_MS  500 // 轮询周期（单位：毫秒）
#define BUS_ERROR_COUNT_JUDGMENT 200 // 总线错误计数判断阈值

#define MAXIMUM_AMOUNT_DATA 8        //最大采样数据量

typedef struct
{
    unsigned FireResetStatus    :1; /* 火灾复位状态 */
    unsigned SlaveDebugStatus   :1; /* 从机调试状态 */
    unsigned CombustibleGasState:1; /* 可燃气体状态 */
    unsigned SmokeState         :1; /* 烟雾状态 */
    unsigned FireState          :1; /* 火焰状态 */
} WarehouseStatus_Def;

typedef struct
{
    rt_uint32_t         BusErrorCount; /* 总线错误计数 */
    WarehouseStatus_Def status_flag;
} Warehouse_Def, *pWarehouse_Def;

#endif /* USER_H_ */
