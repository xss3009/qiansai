// app_common.h
#ifndef APP_COMMON_H
#define APP_COMMON_H

#include <rtthread.h>
#include "lvgl.h"
#include "gui_guider.h"


extern rt_mq_t chart_mq;
extern volatile rt_uint8_t temperature;
extern volatile rt_uint8_t c33id;
extern volatile rt_uint8_t c33flag;
// V2包数据
extern volatile uint8_t v2_data1, v2_data2, v2_data3;
extern volatile char v2_timestamp[26];
extern volatile uint8_t new_data_available;
extern volatile uint8_t temperature;
extern volatile uint8_t device_id;
extern volatile uint8_t status_flag;
extern volatile uint8_t sensor1_value;
extern volatile uint8_t sensor2_value;
extern volatile uint8_t sensor3_value;
extern volatile uint8_t sensor4_value;
extern volatile uint8_t sensor5_value;
extern volatile uint8_t sensor6_value;
extern volatile uint8_t sensor7_value;
// 函数声明
void init_chart_mq(void);
void process_chart_updates(void *parameter);

#endif
