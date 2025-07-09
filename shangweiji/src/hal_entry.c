#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <string.h>
#include "rtdbg.h"
#include "use_common.h"

#define SAMPLE_UART_NAME       "uart4"    /* 串口设备名称 */
#define LED_PIN    BSP_IO_PORT_02_PIN_09 /* LED引脚 */
#define UART_RX_BUFFER_SIZE 128
#define TIMESTAMP_SIZE 26     // 时间戳字符串长度
#define SD_CD      BSP_IO_PORT_04_PIN_05

/* 全局变量 - 接收到的数据 */
// V1包数据
volatile uint8_t temperature = 0;
volatile uint8_t device_id = 0;
volatile uint8_t status_flag = 0;
volatile uint8_t sensor1_value = 0;
volatile uint8_t sensor2_value = 0;
volatile uint8_t sensor3_value = 0;  // 新增的传感器3值

volatile uint8_t packet_count = 0;     // 接收到的有效包计数

/* 状态机状态 */
typedef enum {
    // 通用状态
    STATE_WAIT_SYNC1,
    STATE_WAIT_SYNC2,
    STATE_WAIT_HEADER1,
    STATE_WAIT_HEADER2,

    // V1包状态
    STATE_WAIT_TEMPERATURE,
    STATE_WAIT_DEVICE_ID,
    STATE_WAIT_STATUS_FLAG,
    STATE_WAIT_SENSOR1,
    STATE_WAIT_SENSOR2,
    STATE_WAIT_SENSOR3,  // 新增状态：等待传感器3值

    // V2包状态
    STATE_WAIT_V2_DATA1,
    STATE_WAIT_V2_DATA2,
    STATE_WAIT_V2_DATA3,
    STATE_WAIT_V2_TIMESTAMP,

    // 通用结束状态
    STATE_WAIT_CR,
    STATE_WAIT_LF
} parser_state_t;

/* 接收上下文结构 */
struct receiver_ctx {
    parser_state_t state;        // 当前状态机状态
    rt_device_t serial;          // 串口设备句柄
    uint8_t rx_buffer[UART_RX_BUFFER_SIZE]; // 接收缓冲区

    // V1包临时存储
    uint8_t current_temperature;
    uint8_t current_device_id;
    uint8_t current_status_flag;
    uint8_t current_sensor1;
    uint8_t current_sensor2;
    uint8_t current_sensor3;    // 新增的传感器3临时存储

    // V2包临时存储
    uint8_t current_v2_data1;
    uint8_t current_v2_data2;
    uint8_t current_v2_data3;
    char current_timestamp[TIMESTAMP_SIZE];
    uint8_t timestamp_index;     // 时间戳字节计数
};

/* 串口接收回调函数 */
static rt_err_t uart_rx_callback(rt_device_t dev, rt_size_t size)
{
    return RT_EOK;
}

/* 状态机处理函数 */
static void process_received_byte(struct receiver_ctx *ctx, uint8_t byte)
{
    switch (ctx->state) {
        case STATE_WAIT_SYNC1:
            if (byte == 0xAA) {
                ctx->state = STATE_WAIT_SYNC2;
            }
            break;

        case STATE_WAIT_SYNC2:
            if (byte == 0xBB) {
                ctx->state = STATE_WAIT_HEADER1;
            } else {
                ctx->state = STATE_WAIT_SYNC1;
            }
            break;

        case STATE_WAIT_HEADER1:
            // 保存header1，继续等待header2
            ctx->current_v2_data1 = byte; // 临时复用这个字段
            ctx->state = STATE_WAIT_HEADER2;
            break;

        case STATE_WAIT_HEADER2:
            // 判断包类型
            if (ctx->current_v2_data1 == 0x54 && byte == 0x45) {  // V1包: 'T' 'E'
                ctx->state = STATE_WAIT_TEMPERATURE;
            }
            else if (ctx->current_v2_data1 == 0x32 && byte == 0x23) {  // V2包: '2' '#'
                ctx->state = STATE_WAIT_V2_DATA1;
                rt_kprintf("识别为V2包\n");
            }
            else {
                // 未知包类型，重置状态机
                ctx->state = STATE_WAIT_SYNC1;
            }
            break;

        /******************** V1包处理 ********************/
        case STATE_WAIT_TEMPERATURE:
            ctx->current_temperature = byte;
            ctx->state = STATE_WAIT_DEVICE_ID;
            break;

        case STATE_WAIT_DEVICE_ID:
            ctx->current_device_id = byte;
            ctx->state = STATE_WAIT_STATUS_FLAG;
            break;

        case STATE_WAIT_STATUS_FLAG:
            ctx->current_status_flag = byte;
            ctx->state = STATE_WAIT_SENSOR1;
            break;

        case STATE_WAIT_SENSOR1:
            ctx->current_sensor1 = byte;
            ctx->state = STATE_WAIT_SENSOR2;
            break;

        case STATE_WAIT_SENSOR2:
            ctx->current_sensor2 = byte;
            ctx->state = STATE_WAIT_SENSOR3;  // 新增：等待传感器3值
            break;

        case STATE_WAIT_SENSOR3:  // 新增状态处理
            ctx->current_sensor3 = byte;
            ctx->state = STATE_WAIT_CR;
            break;

        /******************** V2包处理 ********************/
        case STATE_WAIT_V2_DATA1:
            ctx->current_v2_data1 = byte;
            ctx->state = STATE_WAIT_V2_DATA2;
            break;

        case STATE_WAIT_V2_DATA2:
            ctx->current_v2_data2 = byte;
            ctx->state = STATE_WAIT_V2_DATA3;
            break;

        case STATE_WAIT_V2_DATA3:
            ctx->current_v2_data3 = byte;
            ctx->state = STATE_WAIT_V2_TIMESTAMP;
            ctx->timestamp_index = 0; // 重置时间戳索引
            break;

        case STATE_WAIT_V2_TIMESTAMP:
            // 接收时间戳字节
            if (ctx->timestamp_index < TIMESTAMP_SIZE) {
                ctx->current_timestamp[ctx->timestamp_index] = byte;
                ctx->timestamp_index++;
            }

            // 检查是否接收完所有时间戳字节
            if (ctx->timestamp_index >= TIMESTAMP_SIZE) {
                // 确保字符串正确终止
                ctx->current_timestamp[TIMESTAMP_SIZE-1] = '\0';
                ctx->state = STATE_WAIT_CR;
            }
            break;

        /******************** 通用结束状态 ********************/
        case STATE_WAIT_CR:
            if (byte == '\r') {
                ctx->state = STATE_WAIT_LF;
            } else {
                // 不符合预期，重置状态机
                ctx->state = STATE_WAIT_SYNC1;
            }
            break;

        case STATE_WAIT_LF:
            if (byte == '\n') {
                // 完整数据包接收完成，更新全局变量
                packet_count++;

                // 判断包类型并处理
                if (ctx->current_v2_data1 == 0x54) { // V1包
                    temperature = ctx->current_temperature;
                    device_id = ctx->current_device_id;
                    status_flag = ctx->current_status_flag;
                    sensor1_value = ctx->current_sensor1;
                    sensor2_value = ctx->current_sensor2;
                    sensor3_value = ctx->current_sensor3;  // 新增传感器3值

                    rt_kprintf("V1包: 温度=0x%02X, ID=0x%02X, 状态=0x%02X, 传感器1=0x%02X, 传感器2=0x%02X, 传感器3=0x%02X\n",
                              temperature, device_id, status_flag,
                              sensor1_value, sensor2_value, sensor3_value);
                }
                else { // V2包
                    v2_data1 = ctx->current_v2_data1;
                    v2_data2 = ctx->current_v2_data2;
                    v2_data3 = ctx->current_v2_data3;
                    rt_strncpy((char*)v2_timestamp, ctx->current_timestamp, TIMESTAMP_SIZE);
                    v2_timestamp[TIMESTAMP_SIZE-1] = '\0';

                    rt_kprintf("V2包: 数据1=0x%02X, 数据2=0x%02X, 数据3=0x%02X, 时间=%s\n",
                              v2_data1, v2_data2, v2_data3, v2_timestamp);
                }
            }
            // 无论是否接收到LF，都重置状态机
            ctx->state = STATE_WAIT_SYNC1;
            break;

        default:
            ctx->state = STATE_WAIT_SYNC1;
            break;
    }
}

/* 串口接收线程 */
static void uart_receive_thread_entry(void *parameter)
{
    struct receiver_ctx *ctx = (struct receiver_ctx *)parameter;

    while (1) {
        // 尝试从串口读取数据
        rt_size_t read_size = rt_device_read(ctx->serial, 0, ctx->rx_buffer, UART_RX_BUFFER_SIZE);

        if (read_size > 0) {
            // 处理接收到的每个字节
            for (int i = 0; i < read_size; i++) {
                process_received_byte(ctx, ctx->rx_buffer[i]);
            }
        }

        // 短延时，让出CPU
        rt_thread_mdelay(10);
    }
}

void hal_entry(void)
{
    rt_kprintf("\n二号板子 - 数据接收端\n");
    rt_kprintf("支持V1和V2两种数据包格式\n");
    rt_kprintf("V1包格式: AA BB 54 45 [温度] [ID] [状态] [传感器1] [传感器2] [传感器3] \\r\\n\n");
    rt_kprintf("V2包格式: AA BB 32 23 [数据1] [数据2] [数据3] [时间戳] \\r\\n\n");

    /* 初始化接收上下文 */
    static struct receiver_ctx ctx = {
        .state = STATE_WAIT_SYNC1,
        // V1包初始化
        .current_temperature = 0,
        .current_device_id = 0,
        .current_status_flag = 0,
        .current_sensor1 = 0,
        .current_sensor2 = 0,
        .current_sensor3 = 0,  // 初始化新增的传感器3值
        // V2包初始化
        .current_v2_data1 = 0,
        .current_v2_data2 = 0,
        .current_v2_data3 = 0,
        .timestamp_index = 0
    };
    // 初始化时间戳缓冲区
    memset(ctx.current_timestamp, 0, TIMESTAMP_SIZE);
    ctx.current_timestamp[TIMESTAMP_SIZE-1] = '\0';

    /* 查找并打开串口设备 */
    ctx.serial = rt_device_find(SAMPLE_UART_NAME);
    if (!ctx.serial) {
        rt_kprintf("错误：找不到UART4设备！\n");
        return;
    }

    /* 配置串口参数 - 115200, 8N1 */
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    config.baud_rate = BAUD_RATE_115200;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.parity = PARITY_NONE;

    if (rt_device_control(ctx.serial, RT_DEVICE_CTRL_CONFIG, &config) != RT_EOK) {
        rt_kprintf("错误：串口配置失败！\n");
        return;
    }

    /* 以中断接收模式打开串口 */
    if (rt_device_open(ctx.serial, RT_DEVICE_FLAG_INT_RX) != RT_EOK) {
        rt_kprintf("错误：无法以中断模式打开UART4！\n");
        return;
    }

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(ctx.serial, uart_rx_callback);

    /* 创建接收线程 */
    rt_thread_t thread = rt_thread_create("uart_rcv",
                                         uart_receive_thread_entry,
                                         &ctx,
                                         1024,  // 栈大小
                                         20,    // 优先级
                                         10);   // 时间片

    if (thread) {
        rt_thread_startup(thread);
    } else {
        rt_kprintf("错误：创建接收线程失败！\n");
        return;
    }

    /* 配置LED引脚 */
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SD_CD, PIN_MODE_INPUT);

    /* 主循环 - 显示接收到的数据 */
    while (1) {
        // 控制LED闪烁表示系统运行中
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(100);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(900);
    }
}
