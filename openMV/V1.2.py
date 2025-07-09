#########################################################################
# 功能：基于vison board实现任务
# 主要功能：
# 计算物体体积
# 识别二维码
# 版本：v1.2
# 作者：LJW
# 邮箱：liaojiawei2024@163.com
# 更新时间：2025.6.25
# 更新内容：
##########################################################################

import sensor
import math
import lcd
import image
import time
import machine
import ustruct
from machine import UART, Pin

# 初始化LCD显示屏
lcd.init()

# 系统参数配置
Goods_high = 30         # 物体高度（单位：cm）
REAL_SIZE_CM = 19.0     # 二维码实际物理尺寸（边长cm）

# 相机硬件参数（根据OV7725传感器规格设置）
SENSOR_WIDTH_MM = 5.6   # 传感器感光区域宽度
FOCAL_LENGTH_MM = 3.7   # 镜头焦距
IMAGE_WIDTH_PIXELS = 320  # 有效图像宽度（像素）
MEASURE_DISTANCE = 20 #物体和摄像头的距离（单位：cm）
task_flag = 1  # 任务标志位

# 定义位置和半径的变化阈值
position_threshold = 4  # 如果对象位置变化超过这个值，则认为是新的位置
radius_threshold = 4    # 这里定义了radius_threshold，但似乎在代码中没有使用
MAX_GHANGE_THRRSHOLD = 80  # 如果对象位置变化超过这个值，则认为变化过大，不进行更新
prev_x, prev_y = None, None  # 初始化前一个对象位置的坐标

#LAB色彩空间：棕色阈值
color_threshold =(34, 18, -4, 11, 11, 127)
black_threshold = [(11, 32, -42, 1, -15, 15)]

#色块ID定义
last_valid_position = None
POSITION_CHANGE_THRESHOLD = 30
color_id = 0          # 色块标识符
prev_has_blob = False # 前次检测状态
current_id = 0        # 已存在初始化
position_stable_counter = 0

#color_flag说明:
#color_flag = 2时，是还没开始识别
#color_flag = 1时，是货物运送中
#color_flag = 0时，是正在识别货物
color_flag = 2        

uart = UART(2, baudrate=115200)

# 颜色块发送函数修正
def cb_usart(id,x, y,cb_flag):
    frame = ustruct.pack("<BBBBBBBB",
        0xAA,
        0x29,
        0x07,
        0x50,
        id,
        x,
        y,
        cb_flag
        )
    checksum = sum(frame) % 256
    uart.write(frame + ustruct.pack("B", checksum))

# 卡尔曼滤波参数配置（四个角点分别配置）
# 对色块进行滤波
corners_kf_params = [
    {  # 左上角
        'x_Q': 0.01, 'x_R': 0.1, 'x_x': 0, 'x_P': 1,
        'y_Q': 0.01, 'y_R': 0.1, 'y_x': 0, 'y_P': 1
    },
    {  # 右上角
        'x_Q': 0.01, 'x_R': 0.1, 'x_x': 0, 'x_P': 1,
        'y_Q': 0.01, 'y_R': 0.1, 'y_x': 0, 'y_P': 1
    },
    {  # 右下角
        'x_Q': 0.01, 'x_R': 0.1, 'x_x': 0, 'x_P': 1,
        'y_Q': 0.01, 'y_R': 0.1, 'y_x': 0, 'y_P': 1
    },
    {  # 左下角
        'x_Q': 0.01, 'x_R': 0.1, 'x_x': 0, 'x_P': 1,
        'y_Q': 0.01, 'y_R': 0.1, 'y_x': 0, 'y_P': 1
    }
]

# 定义卡尔曼滤波函数，用于平滑坐标值
def kalman_filter(x, Q, R, prev_x, prev_P):
    """
    卡尔曼滤波器实现
    参数:
        x - 当前测量值
        Q - 过程噪声协方差
        R - 测量噪声协方差
        prev_x - 上一状态估计值
        prev_P - 上一状态协方差
    返回:
        x_estimated - 滤波后的坐标值
        P_estimated - 更新后的协方差
    """
    P = prev_P + Q  # 预测误差协方差
    K = P / (P + R)  # 卡尔曼增益
    x_estimated = prev_x + K * (x - prev_x)  # 更新状态估计值
    P_estimated = (1 - K) * P  # 更新误差协方差
    return x_estimated, P_estimated  # 返回过滤后的坐标值和误差协方差



def get_rect_corners(rect_tuple):
    x, y, w, h = rect_tuple
    return [
        (x, y),
        (x + w, y),
        (x + w, y + h),
        (x, y + h)
    ]

def cb_task():
    global color_id, prev_has_blob, last_valid_position, current_id, color_flag, prev_has_blob

    black_blobs = img.find_blobs(black_threshold, merge=True, margin=10)

    if not black_blobs:  # 没有检测到黑色框
        return

    largest_black = max(black_blobs, key=lambda b: b.pixels())
    bx, by, bw, bh = largest_black.rect()

    black_rect = largest_black.rect()  # 获取黑色框区域



    target_blobs = img.find_blobs([color_threshold],
                                roi=(bx, by, bw, bh),  # 添加ROI限制
                                pixels_threshold=20,
                                area_threshold=20,
                                merge=True)


    # # 物体存在状态检测（放在目标色块检测之后）
    current_has_blob = len(target_blobs) > 0

    if current_has_blob and not prev_has_blob:
        current_id += 1
        color_flag =  0

    # 状态变化时重置跟踪器
    if not current_has_blob and prev_has_blob:
        last_valid_position = None
        color_flag = 1

    # 更新历史状态记录
    prev_has_blob = current_has_blob  # 这行放在条件判断之后

    if not target_blobs:  # 没有检测到色块
        cb_usart(0,0,0,0)  # 发送（0,0）坐标，保持心跳帧数
        return

    largest_target = max(target_blobs, key=lambda b: b.pixels())

    abs_rect = (
        largest_target.x(),
        largest_target.y(),
        largest_target.w(),
        largest_target.h()
    )
    raw_corners = get_rect_corners(abs_rect)

    filtered_corners = []
    for i in range(4):
        x_raw, y_raw = raw_corners[i]
        params = corners_kf_params[i]

        # X轴卡尔曼滤波
        x_filt, x_P = kalman_filter(x_raw,
                                    params['x_Q'], params['x_R'],
                                    params['x_x'], params['x_P'])
        # Y轴卡尔曼滤波
        y_filt, y_P = kalman_filter(y_raw,
                                    params['y_Q'], params['y_R'],
                                    params['y_x'], params['y_P'])

        # 更新滤波器状态
        corners_kf_params[i].update({
            'x_x': x_filt, 'x_P': x_P,
            'y_x': y_filt, 'y_P': y_P
        })
        filtered_corners.append((x_filt, y_filt))

    # 计算中心坐标
    xs = [c[0] for c in filtered_corners]
    ys = [c[1] for c in filtered_corners]
    center_x = int(sum(xs) / 4)
    center_y = int(sum(ys) / 4)

    # ID更新逻辑


    # 绘制检测图形
    img.draw_rectangle(black_rect, color=(0,255,0))
    img.draw_rectangle(abs_rect, color=(0,0,255))
    img.draw_cross(center_x, center_y, color=127, size=5)


    # 通过串口发送坐标

    cb_usart(current_id,center_x, center_y,color_flag)

# 初始化摄像头传感器
sensor.reset()  # 重置摄像头
sensor.set_pixformat(sensor.RGB565)  # 设置彩色图像格式（16位RGB）
sensor.set_framesize(sensor.QVGA)    # 设置图像分辨率320x240
sensor.set_windowing((320, 240))     # 设置取景窗口尺寸
sensor.skip_frames(time=2000)        # 等待摄像头稳定
sensor.set_vflip(True)  # 垂直翻转（适应安装方向）
sensor.set_hmirror(True)  # 水平镜像（适应安装方向）
sensor.set_auto_gain(False)  # 关闭自动增益
sensor.set_auto_whitebal(False)  # 关闭自动白平衡
clock = time.clock()  # 创建时钟对象计算FPS

#任务流程
#启动任务后，寻找视野范围内的棕色色块，通过串口发送给stm32坐标位置
while(True):
    clock.tick()
    img = sensor.snapshot()
    gray_img = img.copy().to_grayscale()

    if task_flag == 1:  # 任务标志位为1，任务启动
        cb_task()
    lcd.display(img)

