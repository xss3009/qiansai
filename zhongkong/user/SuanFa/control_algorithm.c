#include "control_algorithm.h"
//#include "control_system.h"

/*
 * 文件名称：control_encoder.c
 * 摘要：算法函数库
 *
 * 修改历史     	版本号        	Author       		修改内容
 *--------------------------------------------------------------
 * 2024/11/14		v1.0			FHS				移植加修改注释
 * 2024/11/20		v1.1			FHS			删除无关形参，补充注释
 * 2024/11/24		v1.2			FHS			   添加虚位置零函数
 * 2024/11/27		v1.3			FHS			 Yaw轴PID参数整定完成
 * 2024/11/28		v1.4			FHS			 增加一阶互补滤波算法
 * 2025/05/13		v1.5			FHS			 增加一阶低通滤波算法
 * 2025/05/14		v1.6			FHS			 增加卡尔曼滤波算法
 * 2025/05/14		v1.7			FHS			 增加卡尔曼+低通滤波混合等算法
 *--------------------------------------------------------------
 */

// static PID_TypeDef PID_A, PID_B, PID_C, PID_D, PID_Vx, PID_Vy, PID_Vz; // PID结构体
// static CF_TypeDef  CF_Sx, CF_Sy, CF_FusionX, CF_FusionY;               // CF结构体
/*
 *  函数名：PID_Set
 *  功能描述：设置PID的比例，微分，积分
 *  输入参数：PID_TypeDef 的指针
 *           Kp 比例
 *           Ki 微分
 *           Kd 积分
 *  输出参数：无
 *  返回值: 无
 */
static void PID_Set(PID_TypeDef *PID, float Kp, float Ki, float Kd)
{
    PID->Kp = Kp;
    PID->Ki = Ki;
    PID->Kd = Kd;
}

/*
 *  函数名：PID_Clear
 *  功能描述：除PID的比例，微分，积分之外 的变量清零函数
 *  输入参数：PID_TypeDef 的指针
 *  输出参数：无
 *  返回值: 无
 */
static void PID_Clear(PID_TypeDef *PID)
{
    PID->ActualValue            = 0.00f;
    PID->DeviationValue         = 0.00f;
    PID->LastDeviationValue     = 0.00f;
    PID->NextDeviationValue     = 0.00f;
    PID->IntegralDeviationValue = 0.00f;
}

/*
 *  函数名：PID_Init
 *  功能描述：PID结构体初始化函数
 *  输入参数：无
 *  输出参数：无
 *  返回值: 无
 */
static void PID_Init(void)
{
    // PID_Set(&PID_A, INCREMENT_KP, INCREMENT_KI, INCREMENT_KD);
    // PID_Clear(&PID_A);
    // PID_Set(&PID_B, INCREMENT_KP, INCREMENT_KI, INCREMENT_KD);
    // PID_Clear(&PID_B);
    // PID_Set(&PID_C, INCREMENT_KP, INCREMENT_KI, INCREMENT_KD);
    // PID_Clear(&PID_C);
    // PID_Set(&PID_D, INCREMENT_KP, INCREMENT_KI, INCREMENT_KD);
    // PID_Clear(&PID_D);
    // PID_Set(&PID_Vx, POSITION_KP, POSITION_KI, POSITION_KD);
    // PID_Clear(&PID_Vx);
    // PID_Set(&PID_Vy, POSITION_KP, POSITION_KI, POSITION_KD);
    // PID_Clear(&PID_Vy);
    // PID_Set(&PID_Vz, 0.08599633f, 0.0085f, 0.0335f);
    // PID_Clear(&PID_Vz);
    // CF_Sx.K      = 0.98;
    // CF_Sy.K      = 0.98;
    // CF_FusionX.K = 0.0;
    // CF_FusionY.K = 0.0;
}
/*
 *  函数名：target_limit_double
 *  功能描述：双精度浮点限幅函数
 *  输入参数：insert需要限幅的形参
 *           low  限幅最小值
 *           high 限幅最大值
 *  输出参数：无
 *  返回值: 限幅后的值
 */
static double target_limit_double(double insert, double low, double high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}

/*
 *  函数名：target_limit_float
 *  功能描述：单精度浮点限幅函数
 *  输入参数：insert需要限幅的形参
 *           low  限幅最小值
 *           high 限幅最大值
 *  输出参数：无
 *  返回值: 限幅后的值
 */
static float target_limit_float(float insert, float low, float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}

/*
 *  函数名：target_limit_int
 *  功能描述：整形限幅函数
 *  输入参数：insert需要限幅的形参
 *           low  限幅最小值
 *           high 限幅最大值
 *  输出参数：无
 *  返回值: 限幅后的值
 */
static int target_limit_int(int insert, int low, int high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}

/*
 *  函数名：double_abs
 *  功能描述：浮点型数据计算绝对值
 *  输入参数：insert 需要转为绝对值的形参
 *  输出参数：无
 *  返回值: 输入数的绝对值
 */
static double double_abs(double insert)
{
    if (insert >= 0)
        return insert;
    else
        return -insert;
}

/*
 *  函数名：float_abs
 *  功能描述：浮点型数据计算绝对值
 *  输入参数：insert 需要转为绝对值的形参
 *  输出参数：无
 *  返回值: 输入数的绝对值
 */
static float float_abs(float insert)
{
    if (insert >= 0)
        return insert;
    else
        return -insert;
}

/*
 *  函数名：int_abs
 *  功能描述：整型数据计算绝对值
 *  输入参数：insert 需要转为绝对值的形参
 *  输出参数：无
 *  返回值: 输入数的绝对值
 */
static float int_abs(float insert)
{
    if (insert >= 0)
        return insert;
    else
        return -insert;
}

/*--------------------------------------------------------------
 *  函数名：KalmanFilter_Init
 *  功能描述：初始化卡尔曼滤波器参数
 *  输入参数：kf - 卡尔曼滤波器结构体指针
 *          q - 过程噪声
 *          r - 测量噪声（建议Q=0.001, R=0.1）
 *-------------------------------------------------------------*/
static void KalmanFilter_Init(KF_TypeDef *kf, float q, float r)
{
    kf->Q = q;
    kf->R = r;
    kf->x = 0;
    kf->P = 0;
    kf->K = 0;
}

/*--------------------------------------------------------------
 *  函数名：KalmanFilter_Update
 *  功能描述：卡尔曼滤波器更新步骤
 *  输入参数：kf - 卡尔曼滤波器结构体指针
 *          measurement - 测量值
 *  输出参数：无
 *  返回值: 滤波后的估计值
 *  算法流程：
 *    1. 预测阶段：x = x, P = P + Q
 *    2. 更新阶段：
 *       K = P / (P + R)
 *       x = x + K*(measurement - x)
 *       P = (1 - K)*P
 *-------------------------------------------------------------*/
static float KalmanFilter(KF_TypeDef *kf, float measurement)
{
    // 预测
    kf->P += kf->Q;

    // 更新
    kf->K  = kf->P / (kf->P + kf->R);
    kf->x += kf->K * (measurement - kf->x);
    kf->P *= (1 - kf->K);

    return kf->x;
}

/*--------------------------------------------------------------
 *  函数名：ComplementaryFilter
 *  功能描述：一阶互补传感器融合算法
 *  输入参数：CF       - 滤波器结构体指针
 *          angle_m - 加速度计测量的角度（单位：度）
 *          gyro_m  - 陀螺仪测量的角速度（单位：度/秒）
 *  输出参数：无
 *  返回值: 融合后的角度估计值（单位：度）
 *  算法原理：
 *    融合加速度计的静态特性与陀螺仪的动态特性
 *    FusionValue = K * angle_m + (1-K) * (prev_Fusion + gyro_m * dt)
 *    其中 dt=0.005s 对应200Hz采样率
 *-------------------------------------------------------------*/
static float ComplementaryFilter(CF_TypeDef *CF, float angle_m, float gyro_m)
{
    // 计算陀螺仪积分项：当前融合值 + 角速度积分（0.005s时间常数）
    const float gyro_integral = CF->FusionValue + gyro_m * 0.005f;

    // 一阶互补融合公式：加速度计权重K，陀螺仪权重(1-K)
    CF->FusionValue           = CF->K * angle_m         // 加速度计分量
                      + (1.0f - CF->K) * gyro_integral; // 陀螺仪积分分量

    return CF->FusionValue;
}

/*--------------------------------------------------------------
 *  函数名：LowPassFilter
 *  功能描述：一阶低通数字滤波器
 *  输入参数：filter - 滤波器结构体指针(需包含前次值和滤波系数)
 *          input  - 原始输入信号
 *  输出参数：无
 *  返回值: 滤波后的信号值
 *  算法公式：y(n) = α * x(n) + (1-α) * y(n-1)
 *          其中 α 为滤波系数(取值范围0-1)，值越小滤波效果越强
 *-------------------------------------------------------------*/
static float LowPassFilter(LPF_TypeDef *filter, float input)
{
    // 更新滤波器值：新值 = 系数α*当前输入 + (1-α)*前次输出
    filter->prev_value = filter->alpha * input + (1 - filter->alpha) * filter->prev_value;
    return filter->prev_value; // 返回滤波后的信号值
}

// 在现有滤波算法结构体后添加混合滤波接口
/*--------------------------------------------------------------
 *  函数名：HybridFilter_KF_LPF
 *  功能描述：卡尔曼+低通滤波混合方案
 *  输入参数：kf - 卡尔曼滤波器
 *          lpf - 低通滤波器
 *          raw - 原始传感器数据
 *  返回值: 双重滤波后的数据
 *  处理流程：原始数据 → 低通滤波 → 卡尔曼滤波
 *-------------------------------------------------------------*/
static float HybridFilter_KF_LPF(KF_TypeDef *kf, LPF_TypeDef *lpf, float raw)
{
    float lpf_out = LowPassFilter(lpf, raw);
    return KalmanFilter(kf, lpf_out);
}

/*--------------------------------------------------------------
 *  函数名：HybridFilter_KF_CF
 *  功能描述：卡尔曼+互补滤波混合方案
 *  输入参数：kf - 卡尔曼滤波器
 *          cf - 互补滤波器 
 *          acc - 加速度计原始值
 *          gyro - 陀螺仪原始值
 *  返回值: 融合后的角度估计
 *  处理流程：互补滤波 → 卡尔曼平滑
 *-------------------------------------------------------------*/
static float HybridFilter_KF_CF(KF_TypeDef *kf, CF_TypeDef *cf, float acc, float gyro)
{
    float cf_out = ComplementaryFilter(cf, acc, gyro);
    return KalmanFilter(kf, cf_out);
}

/*--------------------------------------------------------------
 *  函数名：ThreeStageFilter
 *  功能描述：三级级联滤波
 *  输入参数：raw - 原始传感器数据
 *  返回值: 多重滤波后的数据
 *  处理流程：低通滤波 → 互补滤波 → 卡尔曼滤波
 *-------------------------------------------------------------*/
static float ThreeStageFilter(LPF_TypeDef *lpf, CF_TypeDef *cf, KF_TypeDef *kf, float acc, float gyro, float raw)
{
    float stage1 = LowPassFilter(lpf, raw);
    float stage2 = ComplementaryFilter(cf, acc, stage1);
    return KalmanFilter(kf, stage2);
}

/*
 *  函数名：Incremental_PID
 *  功能描述：增量式PID控制器
 *  增量式PID公式：pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
 *                e(k)代表本次偏差
 *                e(k-1)代表上一次的偏差
 *                e(k-2)代表上俩次的偏差
 *  输入参数：PID_TypeDef 的指针
 *           Encoder 编码器测量值(实际速度)
 *           Target 目标速度
 *  输出参数：无
 *  返回值: 电机PWM
 */
static float Incremental_PID(PID_TypeDef *PID, float Encoder, float Target)
{
    PID->DeviationValue  = Target - Encoder; // 计算偏差
    PID->ActualValue    += PID->Kp * (PID->DeviationValue - PID->NextDeviationValue) + PID->Ki * PID->DeviationValue +
                        PID->Kd * (PID->DeviationValue - (2 * PID->NextDeviationValue) + PID->LastDeviationValue);
    PID->NextDeviationValue = PID->DeviationValue; // 保存上一次偏差
    //    PID->ActualValue        = target_limit_float(PID->ActualValue, -PWM_CLIPPING, PWM_CLIPPING);
    return (float)PID->ActualValue;
}

/*
 *  函数名：PID_StopJudging
 *  功能描述：PID停止虚位置零
 *  输入参数：position 传感器返回值
 *           target 目标值
 *  输出参数：无
 *  返回值: 电机PWM
 */
double PID_StopJudging(double position, double target)
{
    if (((double_abs(target) - double_abs(position)) < 0) && ((double_abs(target) - double_abs(position)) > -0.05))
    {
        return target;
    }
    return position;
}

/*
 *  函数名：Position_PID
 *  功能描述：位置式PID控制器
 *  增量式PID公式：pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
 *                e(k)代表本次偏差
 *                e(k-1)代表上一次的偏差
 *                ∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
 *  输入参数：PID_TypeDef 的指针
 *           position 测量的位置信息
 *           target 目标位置
 *  输出参数：无
 *  返回值: 电机PWM
 */
static double Position_PID(PID_TypeDef *PID, double position, double target)
{
    PID->DeviationValue          = target - position;
    PID->IntegralDeviationValue += PID->DeviationValue;
    PID->IntegralDeviationValue =
        target_limit_double(PID->IntegralDeviationValue, -POSITION_CLIPPING, POSITION_CLIPPING);
    PID->ActualValue = PID->Kp * PID->DeviationValue + PID->Ki * PID->IntegralDeviationValue +
                       PID->Kd * (PID->DeviationValue - PID->NextDeviationValue);
    PID->NextDeviationValue = PID->DeviationValue;
    return PID->ActualValue; // 输出
}

struct AlgorithmDevice g_tAlgorithmDevice = {
    // &PID_A,
    //                                              &PID_B,
    //                                              &PID_C,
    //                                              &PID_D,
    //                                              &PID_Vx,
    //                                              &PID_Vy,
    //                                              &PID_Vz,
    PID_Set,
    PID_Init,
    PID_Clear,
    target_limit_float,
    target_limit_int,
    double_abs,
    float_abs,
    int_abs,
    PID_StopJudging,
    KalmanFilter_Init,
    KalmanFilter,
    ComplementaryFilter,
    LowPassFilter,
    HybridFilter_KF_LPF,
    HybridFilter_KF_CF,
    ThreeStageFilter,
    Incremental_PID,
    Position_PID

};

PAlgorithmDevice GetAlgorithmDevice(void) { return &g_tAlgorithmDevice; }
