#ifndef USER_RTC_H
#define USER_RTC_H

#include <rtthread.h>
#include "user_modbus.h"
#include "user.h"

//调试模式


#ifdef DEBUG_ENABLE

#define SAMPLING_INTERVAL 1 //单位5分钟

#define SAMPLING_TIME 1     //

#else

#define SAMPLING_INTERVAL 12 //单位5分钟

#define SAMPLING_TIME 5      //单位分钟

#endif

typedef rt_uint8_t rtc_counting;

typedef struct
{
    rt_uint16_t temperature[NUMBER_OF_MACHINES][MAXIMUM_AMOUNT_DATA]; // 各设备温度采样数据（设备编号 × 采样点）
    rt_uint16_t averageDailyTemperature[NUMBER_OF_MACHINES];          // 各设备每日温度平均值
    rt_uint16_t humidity[NUMBER_OF_MACHINES][MAXIMUM_AMOUNT_DATA];    // 各设备湿度采样数据
    rt_uint16_t averageDailyHumidity[NUMBER_OF_MACHINES];             // 各设备每日湿度平均值
    rt_uint8_t  goods[NUMBER_OF_MACHINES][MAXIMUM_AMOUNT_DATA];       // 各设备货物数量采样数据
    rt_uint8_t  averageDailyGoods[NUMBER_OF_MACHINES];                // 各设备每日货物数量平均值
    rt_uint16_t averageWarehouseTemperature[MAXIMUM_AMOUNT_DATA];     // 仓库每小时温度平均值（24小时）
    rt_uint16_t DailyWarehouseTemperature;                            // 仓库当日温度日均值
    rt_uint16_t averageWarehouseHumidity[MAXIMUM_AMOUNT_DATA];        // 仓库每小时湿度平均值
    rt_uint16_t DailyWarehouseHumidity;                               // 仓库当日湿度日均值
    rt_uint8_t  averageWarehouseGoods[MAXIMUM_AMOUNT_DATA];           // 仓库每小时货物数量平均值
    rt_uint8_t  DailyWarehouseGoods;                                  // 仓库当日货物数量日均值
    rt_uint8_t  fireCount;                                            // 火灾报警次数统计
    rt_uint8_t  counting;                                             // 数据采样计数器（当前存储位置）
    rt_uint32_t OfflineCount[NUMBER_OF_MACHINES];                     // 设备离线计数
} DayData_Def;

#endif /* USER_RTC_H */
