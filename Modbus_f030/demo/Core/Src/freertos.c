/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

// 模块名称 : 嵌入式竞赛版Modbus传感器程序
// 文件名称 : freertos.c
// 作    者 : FHS
// 版    本 : V1.0
// 创建时间 : 2025-06-19
// 最后修改 : 2025-06-19
// 功能描述 : FreeRTOS源文件
// 历史记录 : 2025-06-19 V1.0
//====================================================================
//  日期       |   版本   |   作者    |   描述
//--------------------------------------------------------------------
// 2025-06-19 |  V1.0    |   FHS     | 修改完成第一版
//====================================================================

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uart_device.h"
#include "modbus.h"
#include "errno.h"
#include "adc.h"
#include "usart.h"
#include "semphr.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define USE_QSB
#define PM
//#define USE_ENV_MONITOR_SENSOR 1
//#define USE_TMP_HUMI_SENSOR 1

#ifdef USE_SWITCH_SENSOR

#define SLAVE_ADDR         1
#define NB_BITS            5
#define NB_INPUT_BITS      3
#define NB_REGISTERS       0
#define NB_INPUT_REGISTERS 0

#endif

#ifdef USE_ENV_MONITOR_SENSOR
extern ADC_HandleTypeDef hadc;

#define SLAVE_ADDR         2
#define NB_BITS            5
#define NB_INPUT_BITS      0
#define NB_REGISTERS       0
#define NB_INPUT_REGISTERS 2

#endif

#ifdef USE_TMP_HUMI_SENSOR

void        AHT20Task(void *argument);
static void aht20_get_datas(uint16_t *temp, uint16_t *humi);

#define SLAVE_ADDR         3
#define NB_BITS            5
#define NB_INPUT_BITS      0
#define NB_REGISTERS       0
#define NB_INPUT_REGISTERS 2

#endif

#ifdef USE_QSB

SemaphoreHandle_t AHT20Mutex;

static void AHT20Task(void *argument);
static void adc_sensorTask(void *argument);
static void aht20_get_datas(uint16_t *temp, uint16_t *humi);
static void BH1750Task(void *argument);
static void BH1750_get_datas(uint16_t *light);
static void Sensor_Parsing_Task(void *argument);

#define SLAVE_ADDR    24
#define NB_BITS       5
#define NB_INPUT_BITS 1
#define NB_REGISTERS  2

#ifdef BH1750

#define NB_INPUT_REGISTERS 5

#elif defined(PM)

#define NB_INPUT_REGISTERS 14

#elif

#define NB_INPUT_REGISTERS 4

#endif

#endif

#ifdef BH1750

SemaphoreHandle_t BH1750Mutex;

// BH1750 I2C地址
#define BH1750_ADDR_LOW  0x23 // ADDR引脚接地
#define BH1750_ADDR_HIGH 0x5C // ADDR引脚接VCC
#define BH1750_I2C_ADDR  BH1750_ADDR_HIGH

// BH1750操作码
#define BH1750_POWER_DOWN       0x00
#define BH1750_POWER_ON         0x01
#define BH1750_RESET            0x07
#define BH1750_CONT_H_RES_MODE  0x10 // 连续高分辨率模式
#define BH1750_CONT_H_RES_MODE2 0x11 // 连续高分辨率模式2
#define BH1750_CONT_L_RES_MODE  0x13 // 连续低分辨率模式
#define BH1750_ONE_H_RES_MODE   0x20 // 单次高分辨率模式
#define BH1750_ONE_H_RES_MODE2  0x21 // 单次高分辨率模式2
#define BH1750_ONE_L_RES_MODE   0x23 // 单次低分辨率模式

// 测量结果结构体
typedef struct
{
    float    lux; // 光照强度(lux)
    uint16_t raw; // 原始数据
} BH1750_Data;

// 设置测量模式
uint8_t BH1750_SetMode(I2C_HandleTypeDef *hi2c, uint8_t mode)
{
    if (HAL_I2C_Master_Transmit(hi2c, BH1750_I2C_ADDR << 1, &mode, 1, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint8_t BH1750_Init(I2C_HandleTypeDef *hi2c)
{
    // 发送上电命令
    uint8_t cmd = BH1750_POWER_ON;
    if (HAL_I2C_Master_Transmit(hi2c, BH1750_I2C_ADDR << 1, &cmd, 1, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // 设置默认测量模式
    return BH1750_SetMode(hi2c, BH1750_CONT_H_RES_MODE);
}

// 重置传感器
uint8_t BH1750_Reset(I2C_HandleTypeDef *hi2c)
{
    uint8_t cmd = BH1750_RESET;
    return HAL_I2C_Master_Transmit(hi2c, BH1750_I2C_ADDR << 1, &cmd, 1, 100);
}

// 读取光照强度
uint8_t BH1750_ReadLight(I2C_HandleTypeDef *hi2c, BH1750_Data *result)
{
    uint8_t data[2] = {0};

    // 读取两个字节数据
    if (HAL_I2C_Master_Receive(hi2c, BH1750_I2C_ADDR << 1, data, 2, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // 组合原始数据
    result->raw = (data[0] << 8) | data[1];

    // 转换为lux值 (分辨率=1.2)
    result->lux = result->raw / 1.2f;

    return HAL_OK;
}

#endif

#ifdef PM

// 解析颗粒物数量(0.1升空气中)
typedef struct
{
    // PM浓度值（μg/m³）
    uint16_t pm1_0_cf1; // CF=1环境下的PM1.0
    uint16_t pm2_5_cf1; // CF=1环境下的PM2.5
    uint16_t pm10_cf1;  // CF=1环境下的PM10
    uint16_t pm1_0_atm; // 大气环境下的PM1.0
    uint16_t pm2_5_atm; // 大气环境下的PM2.5（主要监测值）
    uint16_t pm10_atm;  // 大气环境下的PM10

    // 颗粒物数量（0.1升空气中）
    uint16_t particles_0_3um; // 直径>0.3μm的颗粒数
    uint16_t particles_0_5um; // 直径>0.5μm的颗粒数
    uint16_t particles_1_0um; // 直径>1.0μm的颗粒数
    uint16_t particles_2_5um; // 直径>2.5μm的颗粒数
    uint16_t particles_5_0um; // 直径>5.0μm的颗粒数
    uint16_t particles_10um;  // 直径>10μm的颗粒数
} PM_SensorData;

#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
#ifdef USE_QSB
    osThreadNew(AHT20Task, NULL, &defaultTask_attributes);
    osThreadNew(adc_sensorTask, NULL, &defaultTask_attributes);
#ifdef BH1750
    osThreadNew(BH1750Task, NULL, &defaultTask_attributes);
#endif

#ifdef PM
    osThreadNew(Sensor_Parsing_Task, NULL, &defaultTask_attributes);
#endif

#endif
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */

#ifdef USE_QSB

//**********************************************************//
//CRC校验类型： CRC8
//多项式： X8+X5+X4+1
//Poly:0011 0001 0x31
unsigned char Calc_CRC8(unsigned char *message, unsigned char Num)
{
    unsigned char i;
    unsigned char byte;
    unsigned char crc = 0xFF;
    for (byte = 0; byte < Num; byte++)
    {
        crc ^= (message[byte]);
        for (i = 8; i > 0; --i)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
} //

static uint32_t g_temp, g_humi;

static void aht20_get_datas(uint16_t *temp, uint16_t *humi)
{
    *temp = g_temp;
    *humi = g_humi;
}

static void AHT20Task(void *argument)
{
    uint8_t cmd[] = {0xAC, 0x33, 0x00};
    uint8_t datas[7];
    uint8_t crc;

    extern I2C_HandleTypeDef hi2c1;

    AHT20Mutex = xSemaphoreCreateMutex();

    vTaskDelay(10); /* wait for ready */

    while (1)
    {
        xSemaphoreTake(AHT20Mutex, portMAX_DELAY);
        if (HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, 0x70, cmd, 3, 100))
        {
            vTaskDelay(100); /* wait for ready */
            if (HAL_OK == HAL_I2C_Master_Receive(&hi2c1, 0x70, datas, 7, 100))
            {
                xSemaphoreGive(AHT20Mutex);
                /* cal crc */
                crc = Calc_CRC8(datas, 6);
                if (crc == datas[6])
                {
                    /* ok */
                    g_humi = ((uint32_t)datas[1] << 12) | ((uint32_t)datas[2] << 4) | ((uint32_t)datas[3] >> 4);
                    g_temp = (((uint32_t)datas[3] & 0x0f) << 16) | ((uint32_t)datas[4] << 8) | ((uint32_t)datas[5]);

                    g_humi = g_humi * 100 * 10 / 0x100000;       /* 0.1% */
                    g_temp = g_temp * 200 * 10 / 0x100000 - 500; /* 0.1C */
                }
            }
        }

        vTaskDelay(20);
    }
}

#ifdef BH1750

static BH1750_Data light_data;

static void BH1750_get_datas(uint16_t *light) { *light = (uint16_t)light_data.lux; }

static void BH1750Task(void *argument)
{
    extern I2C_HandleTypeDef hi2c1;

    BH1750Mutex = xSemaphoreCreateMutex();

    xSemaphoreTake(BH1750Mutex, portMAX_DELAY);
    BH1750_Init(&hi2c1);
    BH1750_SetMode(&hi2c1, BH1750_ONE_H_RES_MODE);
    xSemaphoreGive(BH1750Mutex);

    vTaskDelay(10); /* wait for ready */

    while (1)
    {
        xSemaphoreTake(BH1750Mutex, portMAX_DELAY);
        BH1750_SetMode(&hi2c1, BH1750_ONE_H_RES_MODE);
        xSemaphoreGive(BH1750Mutex);

        vTaskDelay(120);

        xSemaphoreTake(BH1750Mutex, portMAX_DELAY);
        BH1750_ReadLight(&hi2c1, &light_data);
        xSemaphoreGive(BH1750Mutex);

        vTaskDelay(500);
    }
}

#endif

#define FRAME_SIZE         32
#define DOUBLE_BUFFER_SIZE (FRAME_SIZE) // 双缓冲大小
#define PROCESS_QUEUE_SIZE 8            // 处理队列深度

// FreeRTOS对象
SemaphoreHandle_t uartIdleSem;

// 使用双缓冲结构
typedef struct
{
    uint8_t active_buffer;                 // 当前活动缓冲区 (0 或 1)
    uint8_t buffer[2][DOUBLE_BUFFER_SIZE]; // 双DMA缓冲区
} DoubleBuffer_t;

DoubleBuffer_t    dma_buffer;
SemaphoreHandle_t buffer_mutex;

// 开启DMA接收
void Start_DMA_Reception(void)
{
    // 开启串口空闲中断
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);

    // 开启DMA接收(双缓冲模式)
    HAL_UART_Receive_DMA(&huart5, dma_buffer.buffer[dma_buffer.active_buffer], DOUBLE_BUFFER_SIZE);
}

void Handle_UART_Idle(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART5) // 根据您的实际USART修改
    {
        // 清除空闲标志
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        // 声明上下文切换标志
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // 正确使用FromISR版本API
        xSemaphoreTakeFromISR(uartIdleSem, &xHigherPriorityTaskWoken);

        // 停止当前DMA传输
        HAL_UART_DMAStop(&huart5);

        // 切换缓冲区
        dma_buffer.active_buffer = (dma_buffer.active_buffer + 1) % 2;
        HAL_UART_Receive_DMA(&huart5, dma_buffer.buffer[dma_buffer.active_buffer], DOUBLE_BUFFER_SIZE);

        // 如果有更高优先级任务就绪，立即切换
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

static PM_SensorData pm_data;

static void PM_get_datas(PM_SensorData *pm) { *pm = pm_data; }

// 解析传感器数据
void Parse_Sensor_Data(uint8_t *data)
{
    // 验证帧头(0x42, 0x4D)
    if (data[0] != 0x42 || data[1] != 0x4D)
        return;

    // 计算校验和(前30字节求和)
    uint32_t calc_checksum = 0;
    for (int i = 0; i < 30; i++)
        calc_checksum += data[i];

    // 获取帧中的校验和
    uint16_t frame_checksum = (data[30] << 8) | data[31];

    if (calc_checksum == frame_checksum)
    {
        // 解析PM浓度值(μg/m³)
        pm_data.pm1_0_cf1       = (data[4] << 8) | data[5];
        pm_data.pm2_5_cf1       = (data[6] << 8) | data[7];
        pm_data.pm10_cf1        = (data[8] << 8) | data[9];
        pm_data.pm1_0_atm       = (data[10] << 8) | data[11];
        pm_data.pm2_5_atm       = (data[12] << 8) | data[13];
        pm_data.pm10_atm        = (data[14] << 8) | data[15];

        // 解析颗粒物数量(0.1升空气中)
        pm_data.particles_0_3um = (data[16] << 8) | data[17];
        pm_data.particles_0_5um = (data[18] << 8) | data[19];
        pm_data.particles_1_0um = (data[20] << 8) | data[21];
        pm_data.particles_2_5um = (data[22] << 8) | data[23];
        pm_data.particles_5_0um = (data[24] << 8) | data[25];
        pm_data.particles_10um  = (data[26] << 8) | data[27];

        /* 在此处使用传感器数据
    printf("PM2.5(CF=1): %d μg/m³\r\n", pm2_5_cf1);
    printf("PM2.5(ATM): %d μg/m³\r\n", pm2_5_atm);
    printf(">0.3μm颗粒: %d/0.1L\r\n", particles_0_3um);
    */
    }
}

static void Sensor_Parsing_Task(void *argument)
{
    uint8_t i   = 0;
    uartIdleSem = xSemaphoreCreateBinary();

    for (;;)
    {
        // 从队列获取数据帧（阻塞等待）
        if (xSemaphoreGive(uartIdleSem) == pdTRUE)
        {
            i = (dma_buffer.active_buffer + 1) % 2;
            // 解析数据（此处执行时间较长）
            Parse_Sensor_Data(dma_buffer.buffer[i]);
        }
    }
}

static uint8_t beep_flag   = 0;
static uint8_t gas_flag    = 0;
static uint8_t beep_status = 0;

static void adc_sensorTask(void *argument)
{
    uint8_t i = 0, j = 0;

    while (1)
    {
        if (beep_flag)
        {
            i++;
            if (i >= 5)
            {
                i = 0;
            }
        }
#if QT
        else if (gas_flag)
        {
            i++;
            switch (j)
            {
                case 0:
                {
                    if (i >= 1)
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                        j++;
                        i = 0;
                    }
                    break;
                }
                case 1:
                {
                    if (i >= 1)
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); //beep1
                        j++;
                        i = 0;
                    }
                    break;
                }
                case 2:
                {
                    if (i >= 1)
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); //beep1
                        j++;
                        i = 0;
                    }
                    break;
                }
                case 3:
                {
                    if (i >= 1)
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); //beep1
                        j++;
                        i = 0;
                    }
                    break;
                }
                case 4:
                {
                    if (i >= 6)
                    {
                        j = 0;
                        i = 0;
                    }
                    break;
                }
            }
        }
#endif

        else if (!beep_status)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); //beep1
        }

        vTaskDelay(100);
    }
}
#endif

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    // uint16_t          adc_value[2] = {0};
    GPIO_PinState     val;        // GPIO状态暂存变量
    uint8_t          *query;      // Modbus查询数据缓冲区指针
    modbus_t         *ctx;        // Modbus上下文对象
    int               rc;         // 操作返回值
    modbus_mapping_t *mb_mapping; // Modbus寄存器映射表

#ifdef USE_QSB
    // 启动ADC校准（嵌赛专用）
    HAL_ADCEx_Calibration_Start(&hadc);

#endif
    // 初始化Modbus RTU上下文
    ctx = modbus_new_st_rtu("uart1", 115200, 'N', 8, 1);
    modbus_set_slave(ctx, SLAVE_ADDR); // 设置从机地址

    // 分配Modbus ADU缓冲区
    query      = pvPortMalloc(MODBUS_RTU_MAX_ADU_LENGTH);

    // 创建Modbus寄存器映射表
    mb_mapping = modbus_mapping_new_start_address(0, NB_BITS, 0, NB_INPUT_BITS, 0, NB_REGISTERS, 0, NB_INPUT_REGISTERS);

    // 建立Modbus连接
    rc         = modbus_connect(ctx);
    if (rc == -1)
    {
        //fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        vTaskDelete(NULL);
        ; // 异常退出
    }

    // 主任务循环
    for (;;)
    {
        // 接收Modbus请求（过滤无效查询）
        do
        {
            rc = modbus_receive(ctx, query);
            /* Filtered queries return 0 */
        } while (rc == 0);

        // 错误处理（保留CRC校验错误连接）
        if (rc == -1 && errno != EMBBADCRC)
        {
            /* Quit */
            continue;
        }

        /* update values of registers
         * a. read gpio
         * b. update reg
         */
        // 发送Modbus响应

#ifdef USE_SWITCH_SENSOR
        /* key1 */
        val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        if (val == GPIO_PIN_RESET)
        {
            mb_mapping->tab_input_bits[0] = 1;
        }
        else
        {
            mb_mapping->tab_input_bits[0] = 0;
        }

        /* key2 */
        val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
        if (val == GPIO_PIN_RESET)
        {
            mb_mapping->tab_input_bits[1] = 1;
        }
        else
        {
            mb_mapping->tab_input_bits[1] = 0;
        }

        /* key3 */
        val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
        if (val == GPIO_PIN_RESET)
        {
            mb_mapping->tab_input_bits[2] = 1;
        }
        else
        {
            mb_mapping->tab_input_bits[2] = 0;
        }

#endif

#ifdef USE_QSB
        /* 更新ADC采样值到输入寄存器 */
#ifdef PM

#else
        for (int i = 0; i < 2; i++)
        {
            HAL_ADC_Start(&hadc);
            if (HAL_OK == HAL_ADC_PollForConversion(&hadc, 100))
            {
                mb_mapping->tab_input_registers[i + 2] = HAL_ADC_GetValue(&hadc);
            }
        }

        // 火焰传感器阈值检测（通道0）
        if (mb_mapping->tab_input_registers[2] <= 600 || beep_flag)
        {
            mb_mapping->tab_bits[1] = 1; // 设置线圈寄存器1
            mb_mapping->tab_bits[0] = 1;
            beep_flag               = 1; // 触发蜂鸣器标志
        }

#endif

#ifdef QT
        // 货物距离传感器通道1阈值检测（离散输入）如果是气体传感器则是气体阈值
        if (mb_mapping->tab_input_registers[3] >= 1500)
        {
            gas_flag                      = 1;
            mb_mapping->tab_input_bits[0] = 1;
        }
        else
        {
            gas_flag                      = 0;
            mb_mapping->tab_input_bits[0] = 0;
        }
#else
        if (mb_mapping->tab_input_registers[3] <= 600)
        {
            mb_mapping->tab_input_bits[0] = 1;
        }
        else
        {
            mb_mapping->tab_input_bits[0] = 0;
        }
#endif
#endif

#ifdef USE_QSB
        /* 
         * AHT20温湿度数据更新（非实时采集）
         * 从全局变量获取预处理好的传感器数据
         * 寄存器分配：
         *   [0] 温度值（0.1℃精度）
         *   [1] 湿度值（0.1%精度） 
         * 注意：此处仅更新寄存器值，实际采集在AHT20Task线程完成
         */
        uint16_t temp, humi;
        aht20_get_datas(&temp, &humi);             // 从全局变量获取最新数据
        mb_mapping->tab_input_registers[0] = temp; // 更新温度输入寄存器
        mb_mapping->tab_input_registers[1] = humi; // 更新湿度输入寄存器
#ifdef PM
        PM_SensorData fPM_Data;

        PM_get_datas(&fPM_Data);
        mb_mapping->tab_input_registers[2]  = fPM_Data.pm1_0_cf1;
        mb_mapping->tab_input_registers[3]  = fPM_Data.pm2_5_cf1;
        mb_mapping->tab_input_registers[4]  = fPM_Data.pm10_cf1;
        mb_mapping->tab_input_registers[5]  = fPM_Data.pm1_0_atm;
        mb_mapping->tab_input_registers[6]  = fPM_Data.pm2_5_atm;
        mb_mapping->tab_input_registers[7]  = fPM_Data.pm10_atm;
        mb_mapping->tab_input_registers[8]  = fPM_Data.particles_0_3um;
        mb_mapping->tab_input_registers[9]  = fPM_Data.particles_0_5um;
        mb_mapping->tab_input_registers[10] = fPM_Data.particles_1_0um;
        mb_mapping->tab_input_registers[11] = fPM_Data.particles_2_5um;
        mb_mapping->tab_input_registers[12] = fPM_Data.particles_5_0um;
        mb_mapping->tab_input_registers[13] = fPM_Data.particles_10um;

#endif

#endif

#ifdef BH1750
        uint16_t light;
        BH1750_get_datas(&light);
        mb_mapping->tab_input_registers[4] = light; // 更新湿度输入寄存器
#endif

        rc = modbus_reply(ctx, query, rc, mb_mapping);
        if (rc == -1)
        {
            //break;
        }
        /* 设备控制段 */
#ifdef USE_SWITCH_SENSOR
        /* switch1 */
        if (mb_mapping->tab_bits[0])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);   //switch1
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); //switch1

                                                                  /* switch2 */
        if (mb_mapping->tab_bits[1])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);   //switch2
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //switch2

        if (mb_mapping->tab_bits[2])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); //LED1
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);   //LED1

        if (mb_mapping->tab_bits[3])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //LED2
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);   //LED2

        if (mb_mapping->tab_bits[4])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); //LED3
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);   //LED3
#endif

#ifdef USE_ENV_MONITOR_SENSOR
        /* beep1 */
        if (mb_mapping->tab_bits[0])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);   //beep1
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); //beep1

        /* beep2 */
        if (mb_mapping->tab_bits[1])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);   //beep2
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); //beep2

        if (mb_mapping->tab_bits[2])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); //LED1
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);   //LED1

        if (mb_mapping->tab_bits[3])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //LED2
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);   //LED2

        if (mb_mapping->tab_bits[4])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); //LED3
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);   //LED3
#endif

#ifdef USE_QSB

        // 蜂鸣器控制（线圈寄存器0）
        if (mb_mapping->tab_bits[0] && !beep_flag)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); //beep1
            beep_status = 1;
        }
        else if (!beep_flag)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); //beep1
            beep_status = 0;
        }

        // 着火报警控制（线圈寄存器1）
        if (mb_mapping->tab_bits[1])
            beep_flag = 1;
        else if (!mb_mapping->tab_bits[1] && !beep_status)
        {
            beep_flag               = 0;
            mb_mapping->tab_bits[0] = 0;
        }
        else
            beep_flag = 0;

        // LED控制（线圈寄存器2）
        if (mb_mapping->tab_bits[2])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); //LED1
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);   //LED1

        // LED控制（线圈寄存器3）
        if (mb_mapping->tab_bits[3])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //LED2
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);   //LED2

        // LED控制（线圈寄存器4）
        if (mb_mapping->tab_bits[4])
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); //LED3
        else
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);   //LED3

				if(((int16_t)mb_mapping->tab_registers[0] >= 0) && ((int16_t)mb_mapping->tab_registers[0] <= 100))
						TIM3->CCR1 = mb_mapping->tab_registers[0];
				else if((int16_t)mb_mapping->tab_registers[0] < 0)
				{
						TIM3->CCR1 = 0;
						mb_mapping->tab_registers[0] = 0;
				}
				else if((int16_t)mb_mapping->tab_registers[0] > 100)
				{
						TIM3->CCR1 = 100;
						mb_mapping->tab_registers[0] = 100;
				}

				if(((int16_t)mb_mapping->tab_registers[1] >= 0) && ((int16_t)mb_mapping->tab_registers[1] <= 100))
						TIM3->CCR2 = mb_mapping->tab_registers[1];
				else if((int16_t)mb_mapping->tab_registers[1] < 0)
				{
						TIM3->CCR2 = 0;
						mb_mapping->tab_registers[1] = 0;
				}
				else if((int16_t)mb_mapping->tab_registers[1] > 100)
				{
						TIM3->CCR2 = 100;
						mb_mapping->tab_registers[1] = 100;
				}

#endif
    }

    // 资源释放
    modbus_mapping_free(mb_mapping); // 释放寄存器映射表
    vPortFree(query);                // 释放ADU缓冲区
    /* For RTU */
    modbus_close(ctx); // 关闭Modbus连接
    modbus_free(ctx);  // 释放Modbus上下文

    vTaskDelete(NULL); // 安全删除任务

  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

