/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-07-05     32705       the first version
 * 2025-07-06     32705       added smmmmmm and gmmmmmm variables
 * 2025-07-06     32705       added v2 packet support
 */
#ifndef __DMA_SENDER_H__
#define __DMA_SENDER_H__

#include <rtthread.h>
#include <rtdevice.h>

/* 定义V1发送数据结构 */
struct dma_packet_v1 {
    uint8_t sync1;         // 0xAA
    uint8_t sync2;         // 0xBB
    uint8_t header1;       // 0x54 (T的十六进制)
    uint8_t header2;       // 0x45 (E的十六进制)
    uint8_t temperature;   // 温度数据
    uint8_t device_id;     // 设备ID
    uint8_t status_flag;   // 状态标志
    uint8_t sensor1;       // 传感器值1 (smmmmmm)
    uint8_t sensor2;       // 传感器值2 (gmmmmmm)
    uint8_t sensor3;
    uint8_t newline[2];    // \r\n
};

/* 定义V2发送数据结构 */
struct dma_packet_v2 {
    uint8_t sync1;         // 0xAA
    uint8_t sync2;         // 0xBB
    uint8_t header1;       // 0x32 ('2'的十六进制)
    uint8_t header2;       // 0x23 ('#'的十六进制)
    uint8_t data1;         // V2数据1
    uint8_t data2;         // V2数据2
    uint8_t data3;         // V2数据3
    char    timestamp[26];
    uint8_t newline[2];    // \r\n
};

/* 初始化DMA发送器 */
rt_err_t dma_sender_init(const char *uart_name);

/* 通过DMA发送V1数据包 */
rt_err_t dma_send_packet_v1(uint8_t data_value);

/* 通过DMA发送V2数据包 */
rt_err_t dma_send_packet_v2(uint8_t data_value);

/* 获取当前使用的串口设备 */
rt_device_t get_dma_serial_device(void);



void send_sensor_data_repeatedly(uint8_t id, uint8_t temp, uint8_t humi);

/* 全局变量声明 */
extern volatile rt_uint8_t tmmmmmm;
extern volatile rt_uint8_t openid;
extern volatile rt_uint8_t openflag;
extern volatile rt_uint8_t smmmmmm;  // 传感器值1
extern volatile rt_uint8_t gmmmmmm;  // 传感器值2
extern volatile rt_uint8_t gmmmmmm2;  // 传感器值2
extern volatile rt_uint8_t v2_data1; // V2数据1
extern volatile rt_uint8_t v2_data2; // V2数据2
extern volatile rt_uint8_t v2_data3; // V2数据3
extern volatile char v2_time[26];
extern volatile rt_uint8_t SDCLLL;

#endif /* __DMA_SENDER_H__ */
