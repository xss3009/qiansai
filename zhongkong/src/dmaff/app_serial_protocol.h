#ifndef APP_SERIAL_PROTOCOL_H
#define APP_SERIAL_PROTOCOL_H

#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"

// 串口协议状态机状态定义
typedef enum {
    STATE_WAIT_AA,      // 等待AA
    STATE_WAIT_BB,      // 等待BB
    STATE_WAIT_DATA,    // 等待数据
    STATE_WAIT_CC,      // 等待CC
    STATE_PACKET_COMPLETE, // 包完成
    STATE_WAIT_ID,
    STATE_WAIT_X,
    STATE_WAIT_Y,
    STATE_WAIT_FLAG,
    STATE_WAIT_29,
    STATE_WAIT_07,
    STATE_WAIT_50
} SerialParserState;

// 串口协议处理器
typedef struct {
    SerialParserState state; // 当前状态
    uint8_t data[4];           // 存储提取的数据
    rt_sem_t packet_sem;// 数据包完成信号量
    uint8_t data_index;
} SerialProtocolParser;




// 串口接收数据结构
typedef struct {
    uint8_t id;      // 设备ID
    uint8_t x;       // X坐标/位置
    uint8_t y;       // Y坐标/位置
    uint8_t flag;    // 控制标志位
} SerialPacketData;

// 声明全局数据结构（在其他文件中可以访问）
extern SerialPacketData g_serial_packet;



// 初始化串口协议处理器
void serial_protocol_init(void);

// 启动串口协议处理
void serial_protocol_start(void);

#endif // APP_SERIAL_PROTOCOL_H
