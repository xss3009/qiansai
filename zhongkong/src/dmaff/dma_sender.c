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
#include "dma_sender.h"
#include <stdlib.h>
#include <time.h>

/* 内部全局变量 */
static const char *sender_uart_name = RT_NULL;
static rt_device_t serial = RT_NULL;
static uint8_t dma_tx_buffer[128];  // DMA发送缓冲区

/* 全局状态变量 - 外部可访问 */
volatile uint8_t tmmmmmm = 44;     // 温度值
volatile uint8_t openid = 0;       // 设备ID
volatile uint8_t openflag = 0;     // 状态标志
volatile uint8_t smmmmmm = 55;    // 新增传感器值1
volatile uint8_t gmmmmmm = 66;    // 新增传感器值2
volatile uint8_t gmmmmmm2 = 66;
volatile uint8_t hmmmmmm = 0;
volatile uint8_t cmmmmmm = 2;
volatile uint8_t fmmmmmm = 11;
volatile uint8_t fmmmmmm2 = 0;
volatile uint8_t ffmmmmm = 0;
volatile uint8_t ffmmmmm2 = 0;
volatile uint8_t fffmmmm = 0;
volatile uint8_t fffmmmm2 = 0;
volatile uint8_t ckmmmmm = 0;
volatile uint8_t krmmmmm = 0;
volatile uint8_t ywmmmmm = 0;
volatile uint8_t v2_data1 = 0;   // V2包数据1
volatile uint8_t v2_data2 = 0;   // V2包数据2
volatile uint8_t v2_data3 = 0;   // V2包数据3
volatile char v2_time[26];

/* 初始化DMA发送器 */
rt_err_t dma_sender_init(const char *uart_name)
{
    /* 保存串口名称 */
    sender_uart_name = uart_name;

    /* 查找串口设备 */
    serial = rt_device_find(sender_uart_name);
    if (!serial) {
        rt_kprintf("[DMA Sender] UART device '%s' not found!\n", sender_uart_name);
        return -RT_ERROR;
    }

    /* 以DMA模式打开串口 */
    if (rt_device_open(serial, RT_DEVICE_OFLAG_WRONLY | RT_DEVICE_FLAG_DMA_TX) != RT_EOK) {
        rt_kprintf("[DMA Sender] Failed to open UART in DMA mode!\n");
        return -RT_ERROR;
    }

    /* 初始化随机数种子 */
    srand(rt_tick_get());

    rt_kprintf("[DMA Sender] Initialized for UART '%s'\n", sender_uart_name);
    return RT_EOK;
}

/* 通过DMA发送V1数据包 */
rt_err_t dma_send_packet_v1(uint8_t data_value)
{
    if (!serial) {
        rt_kprintf("[DMA Sender] Not initialized!\n");
        return -RT_ERROR;
    }

    /* 准备完整数据包 - 添加了两个新字段 */
    struct dma_packet_v1 packet = {
        .sync1 = 0xAA,         // 同步字节1
        .sync2 = 0xBB,         // 同步字节2
        .header1 = 0x54,       // 'T'的十六进制
        .header2 = 0x45,       // 'E'的十六进制
        .temperature = tmmmmmm, // 温度值
        .device_id = openid,    // 设备ID
        .status_flag = openflag, // 状态标志
        .sensor1 = smmmmmm,    // 新增传感器值1
        .sensor2 = gmmmmmm,    // 新增传感器值2
        .sensor3 = gmmmmmm2,
        .sensor4 = hmmmmmm,
        .sensor5 = cmmmmmm,
        .sensor6 = fmmmmmm,
        .sensor7 = fmmmmmm2,
        .sensor8 = ffmmmmm,
        .sensor9 = ffmmmmm2,
        .sensor10 = fffmmmm,
        .sensor11 = fffmmmm2,
        .sensor12 = ckmmmmm,
        .sensor13 = krmmmmm,
        .sensor14 = ywmmmmm,
        .newline = {'\r', '\n'} // 换行符
    };

    /* 将数据包复制到DMA缓冲区 */
    rt_memcpy(dma_tx_buffer, &packet, sizeof(packet));

    /* 使用DMA发送数据包 */
    rt_size_t bytes_sent = rt_device_write(serial, 0, dma_tx_buffer, sizeof(packet));

    if (bytes_sent != sizeof(packet)) {
        rt_kprintf("[DMA Sender] Send v1 failed! Sent: %d/%d bytes\n",
                  bytes_sent, sizeof(packet));
        return -RT_ERROR;
    }

    /* 调试输出 - 增加两个新变量 */
    rt_kprintf("[DMA Sender] Sent v1: Temp=0x%02X, ID=0x%02X, Flag=0x%02X, "
                  "Sensor1=0x%02X, Sensor2=0x%02X, Sensor3=0x%02X, Sensor4=0x%02X\n, Sensor5=0x%02X\n, Sensor6=0x%02X\n, Sensor7=0x%02X\n",
                  tmmmmmm, openid, openflag, smmmmmm, gmmmmmm, gmmmmmm2, hmmmmmm, cmmmmmm, fmmmmmm, fmmmmmm2);

    return RT_EOK;
}

rt_err_t dma_send_packet_v2(uint8_t data_value)
{
    if (!serial) {
        rt_kprintf("[DMA Sender] Not initialized!\n");
        return -RT_ERROR;
    }

    /* 准备V2数据包 - 改为手动赋值 */
    struct dma_packet_v2 packet;
    packet.sync1 = 0xAA;
    packet.sync2 = 0xBB;
    packet.header1 = 0x32;   // '2'
    packet.header2 = 0x23;   // '#'
    packet.data1 = v2_data1;
    packet.data2 = v2_data2;
    packet.data3 = v2_data3;

    // 复制时间戳内容而不是指针
    rt_strncpy(packet.timestamp, v2_time, sizeof(packet.timestamp));
    packet.timestamp[sizeof(packet.timestamp) - 1] = '\0'; // 确保终止

    packet.newline[0] = '\r';
    packet.newline[1] = '\n';

    /* 将数据包复制到DMA缓冲区 */
    rt_memcpy(dma_tx_buffer, &packet, sizeof(packet));

    /* 使用DMA发送数据包 */
    rt_size_t bytes_sent = rt_device_write(serial, 0, dma_tx_buffer, sizeof(packet));

    if (bytes_sent != sizeof(packet)) {
        rt_kprintf("[DMA Sender] Send v2 failed! Sent: %d/%d bytes\n",
                  bytes_sent, sizeof(packet));
        return -RT_ERROR;
    }

    /* 完整的调试输出 */
    rt_kprintf("Sent V2包: 数据1=0x%02X, 数据2=0x%02X, 数据3=0x%02X, 时间=%s\n",
              v2_data1, v2_data2, v2_data3, v2_time);

    return RT_EOK;
}

void send_sensor_data_repeatedly(uint8_t id, uint8_t temp, uint8_t humi)
{
    // 获取并设置时间戳
    time_t now;
    time(&now);
    rt_strncpy(v2_time, ctime(&now), sizeof(v2_time) - 1);
    v2_time[sizeof(v2_time) - 1] = '\0'; // 确保终止符

    // 重复发送5次
    for (int i = 0; i < 5; i++) {
        v2_data1 = id;
        v2_data2 = temp;
        v2_data3 = humi;

        // 直接发送V2包
        dma_send_packet_v2(0);

        // 短延时确保发送完成
        rt_thread_mdelay(50);
    }
}



/* 获取当前使用的串口设备 */
rt_device_t get_dma_serial_device(void)
{
    return serial;
}
