#include "app_serial_protocol.h"
#include <rtthread.h>
#include <string.h>
#include "dma_sender.h"


// 协议定义
#define HEADER_AA 0xAA
#define HEADER_BB 0x29
#define FOOTER_CC 0x07

// 串口协议处理器实例
static SerialProtocolParser parser;

// 环形缓冲区用于接收数据
#define RX_BUF_SIZE 128
static struct rt_ringbuffer rx_rb;
static rt_uint8_t rx_buf[RX_BUF_SIZE];
static struct rt_semaphore data_sem;



SerialPacketData g_serial_packet = {
    .id = 0,
    .x = 0,
    .y = 0,
    .flag = 0
};



/* 串口接收回调函数 */
void user_uart3_callback(uart_callback_args_t *p_args)
{
    if (p_args->event == UART_EVENT_RX_CHAR)
    {
        uint8_t data = (uint8_t)p_args->data;

        // 将接收到的数据放入环形缓冲区
        rt_ringbuffer_put(&rx_rb, &data, 1);

        // 释放信号量通知有数据到达
        rt_sem_release(&data_sem);
    }
}

/* 状态机处理数据包 */
static void process_byte(uint8_t byte)
{
    switch (parser.state) {
        case STATE_WAIT_AA:
            if (byte == HEADER_AA) {
                rt_kprintf("AA");
                parser.state = STATE_WAIT_29;
            }
            break;
        case STATE_WAIT_29:
                    if (byte == 0x29) {
                        rt_kprintf("29");
                        parser.state = STATE_WAIT_07;
                    } else {
                        parser.state = STATE_WAIT_AA; // 重置状态
                    }
                    break;

        case STATE_WAIT_07:
            if (byte == 0x07) {
                parser.state = STATE_WAIT_50;
            } else {
                parser.state = STATE_WAIT_AA; // 重置状态
            }
            break;

        case STATE_WAIT_50:
                            if (byte == 0x50) {
                                rt_kprintf("50");
                                parser.state = STATE_WAIT_ID;
                                parser.data_index = 0;
                            } else {
                                parser.state = STATE_WAIT_AA; // 重置状态
                            }
                            break;
        case STATE_WAIT_ID:
                        parser.data[parser.data_index++] = byte;
                        parser.state = STATE_WAIT_X;
                            break;
        case STATE_WAIT_X:
                    parser.data[parser.data_index++] = byte;
                    parser.state = STATE_WAIT_Y;
                    break;

                case STATE_WAIT_Y:
                    parser.data[parser.data_index++] = byte;
                    parser.state = STATE_WAIT_FLAG;
                    break;

                case STATE_WAIT_FLAG:
                    parser.data[parser.data_index++] = byte;
                    parser.state = STATE_PACKET_COMPLETE;
                    break;


        case STATE_PACKET_COMPLETE:
            // 不应停留在此状态，应重置
            parser.state = STATE_WAIT_AA;
            break;
    }
}

/* 串口数据处理线程 */
static void serial_data_thread(void *parameter)
{
    uint8_t data;
    rt_size_t len;

    while (1) {
        // 等待数据到达
        rt_sem_take(&data_sem, RT_WAITING_FOREVER);

        // 处理所有可用数据
        while ((len = rt_ringbuffer_get(&rx_rb, &data, 1)) > 0)  // 修复条件判断
        {
            process_byte(data);

            // 如果包完成，处理完成后重置状态机
            if (parser.state == STATE_PACKET_COMPLETE) {
                g_serial_packet.id = parser.data[0];
                g_serial_packet.x = parser.data[1];
                g_serial_packet.y = parser.data[2];
                g_serial_packet.flag = parser.data[3];

//                                rt_kprintf("Received packet: ID=%d, X=%d, Y=%d, Flag=%d\n",
//                                        g_serial_packet.id, g_serial_packet.x, g_serial_packet.y, g_serial_packet.flag);
                                openid = g_serial_packet.id;
                                openflag = g_serial_packet.flag;
                parser.state = STATE_WAIT_AA; // 重置状态机
            }
        }
    }
}

/* 串口发送数据包 */
void serial_send_packet(uint8_t data)
{
    uint8_t packet[] = {HEADER_AA, HEADER_BB, data, FOOTER_CC};
    R_SCI_UART_Write(&g_uart3_ctrl, packet, sizeof(packet));
}

/* 串口测试线程 - 定期发送测试数据 */
static void serial_test_thread(void *parameter)
{
    uint8_t test_data = 0;

//    while (1) {
////        // 发送测试数据包
////        serial_send_packet(test_data);
////        rt_kprintf("Sent test packet with data: 0x%02X\n", test_data);
////
////        // 递增测试数据
////        test_data = (test_data + 1) % 0xFF;
////
////        // 每1秒发送一次
////        rt_thread_mdelay(1000);
//    }
}

// 初始化串口协议处理器
void serial_protocol_init(void)
{
    // 初始化状态机
    parser.state = STATE_WAIT_AA;
        parser.data_index = 0;
        memset(parser.data, 0, sizeof(parser.data));

        g_serial_packet.id = 0;
            g_serial_packet.x = 0;
            g_serial_packet.y = 0;
            g_serial_packet.flag = 0;
    // 初始化环形缓冲区
    rt_ringbuffer_init(&rx_rb, rx_buf, RX_BUF_SIZE);

    // 初始化数据信号量
    rt_sem_init(&data_sem, "data_sem", 0, RT_IPC_FLAG_FIFO);

    // 注册串口回调函数
    R_SCI_UART_CallbackSet(&g_uart3_ctrl, user_uart3_callback, NULL, NULL);
}

// 启动串口协议处理
void serial_protocol_start(void)
{
    // 创建并启动数据处理线程
    rt_thread_t data_tid = rt_thread_create(
        "serial_data",
        serial_data_thread,
        RT_NULL,
        1024,
        19,
        10
    );
    if (data_tid) rt_thread_startup(data_tid);

    // 创建并启动测试线程
    rt_thread_t test_tid = rt_thread_create(
        "serial_test",
        serial_test_thread,
        RT_NULL,
        1024,
        25,
        10
    );
    if (test_tid) rt_thread_startup(test_tid);
}
