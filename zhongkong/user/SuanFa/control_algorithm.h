#ifndef __CONTROL_ALGORITHM_H
#define __CONTROL_ALGORITHM_H

// 增量式PID的比例值
#define INCREMENT_KP 20000
#define INCREMENT_KI 2000
#define INCREMENT_KD 0

// 位置式PID的比例值
#define POSITION_KP 7.03125f
#define POSITION_KI 0.0703f
#define POSITION_KD 0.35f

// 位置式PID的积分限幅值
#define POSITION_CLIPPING 2.5

typedef struct
{
    float K; //
    float FusionValue;
} CF_TypeDef, *PCF_TypeDef;

// 低通滤波器结构体
typedef struct
{
    float prev_value;
    float alpha;
} LPF_TypeDef, *PLPF_TypeDef;

// 卡尔曼滤波器结构体
typedef struct
{
    float x; // 状态估计
    float P; // 估计误差协方差
    float Q; // 过程噪声协方差
    float R; // 测量噪声协方差
    float K; // 卡尔曼增益
} KF_TypeDef, *PKF_TypeDef;

typedef struct
{
    float  Kp;                     // 比例
    float  Ki;                     // 积分
    float  Kd;                     // 微分
    double ActualValue;            // 实际值
    double DeviationValue;         // 当前偏差值
    double NextDeviationValue;     // 下一个偏差值
    double LastDeviationValue;     // 最后一个偏差值
    double IntegralDeviationValue; // 偏差值积分
} PID_TypeDef, *PPID_TypeDef;

typedef struct AlgorithmDevice
{
    // PPID_TypeDef PID_A;
    // PPID_TypeDef PID_B;
    // PPID_TypeDef PID_C;
    // PPID_TypeDef PID_D;
    // PPID_TypeDef PID_Vx;
    // PPID_TypeDef PID_Vy;
    // PPID_TypeDef PID_Vz;
    // PCF_TypeDef  CF_Sx;
    // PCF_TypeDef  CF_Sy;
    // PCF_TypeDef  CF_FusionX;
    // PCF_TypeDef  CF_FusionY;
    void (*Set)(PID_TypeDef *PID, float Kp, float Ki, float Kd);
    void (*Init)(void);
    void (*Clear)(PID_TypeDef *PID);
    float (*limit_float)(float insert, float low, float high);
    int (*limit_int)(int insert, int low, int high);
    double (*double_abs)(double insert);
    float (*float_abs)(float insert);
    float (*int_abs)(float insert);
    double (*StopJudging)(double position, double target);
    void (*KalmanFilter_Init)(KF_TypeDef *kf, float q, float r);
    float (*KalmanFilter)(KF_TypeDef *kf, float measurement);
    float (*ComplementaryFilter)(CF_TypeDef *CF, float angle_m, float gyro_m);
    float (*LowPassFilter)(LPF_TypeDef *filter, float input);
    float (*HybridFilter_KF_LPF)(KF_TypeDef *kf, LPF_TypeDef *lpf, float raw);
    float (*HybridFilter_KF_CF)(KF_TypeDef *kf, CF_TypeDef *cf, float acc, float gyro);
    float (*ThreeStageFilter)(LPF_TypeDef *lpf, CF_TypeDef *cf, KF_TypeDef *kf, float acc, float gyro, float raw);
    float (*Incremental_PID)(PID_TypeDef *PID, float Encoder, float Target);
    double (*Position_PID)(PID_TypeDef *PID, double position, double target);

} AlgorithmDevice, *PAlgorithmDevice;

PAlgorithmDevice GetAlgorithmDevice(void);

#endif /* __CONTROL_ALGORITHM_H */
