#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <board.h>
#include "user_rtc.h"
#include "user_mb_app.h"
#include "user_modbus.h"
#include "user_FireControl.h"
#include "user_ShelfControl.h"

#define DBG_TAG "rtc"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define RTC_NAME "rtc"

DayData_Def dayData;

void rtc_init(void)
{
    rt_err_t ret       = RT_EOK;

    rt_device_t device = RT_NULL;

    device             = rt_device_find(RTC_NAME);
    if (!device)
    {
        rt_kprintf("find %s failed!\n", RTC_NAME);
    }

    if (rt_device_open(device, 0) != RT_EOK)
    {
        rt_kprintf("open %s failed!\n", RTC_NAME);
        return; // 添加错误返回
    }

    /* 设置日期 */
    ret = set_date(2025, 7, 16);
    rt_kprintf("set RTC date to 2025-07-16\n");
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
    }

    /* 设置时间 */
    ret = set_time(22, 46, 00);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
    }
}

void daily_alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    // rt_kprintf("daily alarm callback\n");
    collectDailyWarehouseData(&dayData);
}

void five_min_alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    // rt_kprintf("five min alarm callback\n");
     sampleShelvesEnvironmentalData(&dayData);
}

// void s_alarm_callback(rt_alarm_t alarm, time_t timestamp)
// {
//     time_t now;
//     get_timestamp(&now);
//     rt_kprintf("现在是: %.*s\r\n", 25, ctime(&now));
//     //  sampleShelvesEnvironmentalData(&dayData);
// }

void alarm_sample(void)
{
    // 创建每日0点闹钟
    struct rt_alarm_setup daily_setup = {
        .flag   = RT_ALARM_DAILY,
        .wktime = {
                   .tm_hour = 0, // 0点
            .tm_min  = 0, // 0分
            .tm_sec  = 0  // 0秒
        }
    };

    rt_alarm_t daily_alarm           = rt_alarm_create(daily_alarm_callback, &daily_setup);

    // 创建5分钟间隔闹钟（需要配合计数器）
    struct rt_alarm_setup min5_setup = {.flag   = RT_ALARM_MINUTE, // 每分钟触发+单次模式
                                        .wktime = {
                                            .tm_sec = 0            // 每分钟的0秒触发
                                        }};
    rt_alarm_t            min5_alarm = rt_alarm_create(five_min_alarm_callback, &min5_setup);

    // struct rt_alarm_setup s_setup    = {
    //        .flag = RT_ALARM_SECOND, // 每分钟触发+单次模式
    // };
    // rt_alarm_t s_alarm = rt_alarm_create(s_alarm_callback, &s_setup);

    // 启动俩个闹钟
    if (RT_NULL != daily_alarm)
        rt_alarm_start(daily_alarm);
    else
        rt_kprintf("daily alarm not found\n");

    if (RT_NULL != min5_alarm)
        rt_alarm_start(min5_alarm);
    else
        rt_kprintf("5 min alarm not found\n");

    // if (RT_NULL != s_alarm)
    //     rt_alarm_start(s_alarm);
    // else
    //     rt_kprintf("s alarm not found\n");

}
/* export msh cmd */
// MSH_CMD_EXPORT(alarm_sample, alarm sample);
// INIT_APP_EXPORT(rtc_init);
// INIT_APP_EXPORT(alarm_sample);
