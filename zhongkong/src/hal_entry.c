/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2024-03-11    Wangyuqiang   first version
 * 2025-07-06    32705         added V2 packet support with SDCLLL condition
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <board.h>
#include "user_FireControl.h"
#include "dma_sender.h"

#define LED_PIN_0 BSP_IO_PORT_14_PIN_3 /* Onboard LED pins */
#define LED_PIN_1 BSP_IO_PORT_14_PIN_0 /* Onboard LED pins */
#define LED_PIN_2 BSP_IO_PORT_14_PIN_1 /* Onboard LED pins */

volatile uint8_t SDCLLL = 0;  // V2包发送条件标志

void hal_entry(void)
{
    extern void rtc_init(void);
    extern void alarm_sample(void);
    extern void fire_detection_init(void);

    rt_kprintf("\nHello RT-Thread!\n");
    rt_kprintf("==================================================\n");
    rt_kprintf("\nHello RT-Thread!\n");
    rt_kprintf("==================================================\n");
    rt_kprintf("This is a iar project which mode is xspi0 execution!\n");
    rt_kprintf("==================================================\n");
    rt_kprintf("DMA Sender mode: Always send V1 packets, send V2 only when SDCLLL=1\n");
    rt_kprintf("==================================================\n");

    rtc_init();
    alarm_sample();
    fire_detection_init();

    // 初始化DMA发送器
    if (dma_sender_init("uart4") != RT_EOK) {
        rt_kprintf("Failed to initialize DMA sender!\n");
        return;
    }

    uint8_t counter = 0;
    fsp_err_t err = R_SCI_UART_Open(&g_uart3_ctrl, &g_uart3_cfg);
    if (err != FSP_SUCCESS) {
        rt_kprintf("UART3 open failed: %d\n", err);
        return;
    }

    // 初始化并启动串口协议处理
    serial_protocol_init();
    serial_protocol_start();

    while (1)
    {
        rt_pin_write(LED_PIN_0, PIN_HIGH);
        rt_pin_write(LED_PIN_1, PIN_HIGH);
        rt_pin_write(LED_PIN_2, PIN_HIGH);
        rt_thread_mdelay(1000);
        rt_pin_write(LED_PIN_0, PIN_LOW);
        rt_pin_write(LED_PIN_1, PIN_LOW);
        rt_pin_write(LED_PIN_2, PIN_LOW);
        rt_thread_mdelay(1000);





        // 总是发送V1数据包
        if (dma_send_packet_v1(counter) == RT_EOK) {
            rt_kprintf("Sent V1 packet\n");
        }

        // 只有当SDCLLL=1时才发送V2数据包
        if (SDCLLL == 1) {
            if (dma_send_packet_v2(counter) == RT_EOK) {
                rt_kprintf("Sent V2 packet (SDCLLL=1)\n");
            }



        }

        /* 计数器递增 */
        counter++;

        /* 延时控制发送速率 */
        rt_thread_mdelay(400);
    }
}
