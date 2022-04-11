/*
 *  pid.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "define.h"
#include "pid.h"

//PID Statement
Motor_PIDTypeDef_t Mortor1_PID, Mortor2_PID, Mortor3_PID, Mortor4_PID;
PIDTypeDef_t Angle, Gyro_Angle;
FFCTypeDef_t FFC;

float   ki_index = 1.000f;      //积分分离倍率
int16   Dead_zone = 15;         //积分死区
//积分死区应该通过实际工程中可容忍的最大误差来计算出
//也可以不严谨的以电机稳定运行后，此时输入的误差大小作为积分死区

/*
 * @name:void PID_Init(void);
 * @function: PID initialize
 * @param:none
 * @return:none
 */
void PID_Init(void)
{
    /* 电机环 */
    Motor_PID_Parameter_Init(&Mortor1_PID);
    Motor_PID_Parameter_Init(&Mortor2_PID);
    Motor_PID_Parameter_Init(&Mortor3_PID);
    Motor_PID_Parameter_Init(&Mortor4_PID);
    Mortor1_PID.i_Max = Mortor1_PID.i_Max
    = Mortor1_PID.i_Max = Mortor1_PID.i_Max = 1000;

    /* 陀螺仪环 */
    PID_Parameter_Init(&Gyro_Angle);
    GyroOutPut = 0;

    /* 转向环 */
    FusionErr = 0.000f;
    PID_Parameter_Init(&Angle);
    AngleOutPut = 0;
    Angle.i_Max = 400.0f;
    Angle.out_Max = (float)Mortor_RPM_Max;

    /* 前馈补偿 */
    FFC.Rin = FFC.LastRin = FFC.PerrRin = 0.000f;
    FFC.FOutput = 0.000f;

    /* PID参数初始化  */
    MortorPID_Parameter_Write(PID1_Parameter_M1, PID1_Parameter_M2, PID1_Parameter_M3, PID1_Parameter_M4);
    AnglePID_Parameter_Write(Angle1_PID1_40_Parameter);
}

/*
 * @name:void Motor_Realize_PID(PIDTypeDef_t *pid);
 * @function: 位置式PID控制
 * @param:none
 * @return:none
 */
void Motor_Realize_PID(Motor_PIDTypeDef_t *pid, uint8 type)
{
    int16 Mid_Err = 3;      //中等误差
    float Kif = 0.0f;       //Ki
    float Online_K = 1.070f;

//    if (Mortor1.Speed_Idealy == 0) {
        if (type == 1) {
            Kif = 4.8289;
        } else if (type == 2) {
            Kif = 5.2741;
        } else if (type == 3) {
            Kif = 4.9904;
        } else if (type == 4) {
            Kif = 5.1102;
        }
//    } else {
//        if (type == 1) {
//            Kif = 5.5101;
//        } else if (type == 2) {
//            Kif = 4.6743;
//        } else if (type == 3) {
//            Kif = 4.9406;
//        } else if (type == 4) {
//            Kif = 4.3160;
//        }
//    }

    /* 误差求取 */
    pid->Err_Now = pid->EX_Val - pid->Now_Val;    //计算当前误差
    pid->Err_ABS = ABS(pid->Err_Now);

    /* 误差限幅 */
    if(pid->Err_ABS > 64) {
        pid->Err_Now = pid->Err_Now >  64 ?  64 : pid->Err_Now;
        pid->Err_Now = pid->Err_Now < -64 ? -64 : pid->Err_Now;
    }

    pid->Err_ABS = ABS(pid->Err_Now);
    pid->Err_Sum += pid->Err_Now;

    /* 积分限幅 */
    if (ABS(pid->Err_Sum) > pid->i_Max) {
        if (pid->Err_Sum > 0) {
            pid->Err_Sum = pid->i_Max;
        } else {
            pid->Err_Sum = -pid->i_Max;
        }
    }

    /* 专家PID */
    //偏差在朝向偏差绝对值增大的方向变化(偏差越来越大), 或者偏差一直为某一固定值
    if ((pid->Err_Now * (pid->Err_Now - pid->Err_Last) > 0 &&
            pid->Err_Last * (pid->Err_Last - pid->Err_LastLast) > 0) ||
            (pid->Err_Now - pid->Err_Last) == 0) {
        if (pid->Err_ABS >= Mid_Err) {
            pid->Output = pid->Kp * pid->Err_Now
                        + pid->Err_Sum * Kif
                        + pid->Kd * (pid->Err_Now - pid->Err_Last);
            pid->Output = pid->Output * Online_K;
        }
        //偏差是向绝对值增大的方向变化, 但是偏差绝对值本身并不是很大
        else if (pid->Err_ABS < Mid_Err) {
            pid->Output = pid->Kp * pid->Err_Now
                        + pid->Err_Sum * Kif
                        + pid->Kd * (pid->Err_Now - pid->Err_Last);
        }
    }
    //偏差的绝对值向减小的方向变化
//        else if(pid->Err_Now * (pid->Err_Now - pid->Err_Last) < 0 &&
//                 (pid->Err_Now - pid->Err_Last) * (pid->Err_Last - pid->Err_LastLast) > 0) {
//            //此时保持控制器输出不变即可
//        }
    //偏差处于极限状态
    else if(pid->Err_Now * (pid->Err_Now - pid->Err_Last) < 0 &&
             ((pid->Err_Now - pid->Err_Last) * (pid->Err_Last - pid->Err_LastLast) < 0)) {

        if (pid->Err_Now > Mid_Err || pid->Err_Now < -Mid_Err) {
            pid->Output = pid->Kp * pid->Err_Now
                        + pid->Err_Sum * Kif
                        + pid->Kd * (pid->Err_Now - pid->Err_Last);
            pid->Output = pid->Output * Online_K;
        } else {
            pid->Output = pid->Kp * pid->Err_Now
                        + pid->Err_Sum * Kif
                        + pid->Kd * (pid->Err_Now - pid->Err_Last);
        }
    }
    //正常情况
    else {
        pid->Output = pid->Kp * pid->Err_Now
                    + pid->Err_Sum * Kif
                    + pid->Kd * (pid->Err_Now - pid->Err_Last);
    }

    /* 输出限幅 */
    pid->Output = pid->Output > Mortor_PWM_Max ? Mortor_PWM_Max : pid->Output;
    pid->Output = pid->Output < Mortor_PWM_Min ? Mortor_PWM_Min : pid->Output;

    /* 更新上次值 */
    pid->Err_Last = pid->Err_Now;       //更新上次误差
    pid->Err_LastLast = pid->Err_Last;
    pid->Output_Last = pid->Output;
}

/*
 * @name:void Realize_PID(PIDTypeDef_t *pid);
 * @function: 位置式PID控制(角度环
 * @param:none
 * @return:none
 * @notion: P比例I积分D微分
 */
void Realize_PID(PIDTypeDef_t *pid)
{
    float   StaticDif_Err = 3.0f;       //静差(余差)
    float   StaticDif_Out = 3.0f;       //输出静差
    float   Lpf1Factor_d = 0.700f;      //微分项低通滤波系数
    float   Lpf1Factor_Out = 0.850f;    //输出项低通滤波系数
    float   Online_K = 1.075f;
    float   Offine_K = 0.65f;
    float   S_Err = 20.0f;              //小误差
    float   Mid_Err = 35.0f;            //中等误差
    float   L_Err = 75.0f;              //大误差

    pid->Err_Now = FusionErr;
    pid->Err_ABS = fabsf(pid->Err_Now);

    /* 误差限幅 */
//    if(pid->Err_ABS > 45) {
//        pid->Err_Now = pid->Err_Now >  45 ?  45 : pid->Err_Now;
//        pid->Err_Now = pid->Err_Now < -45 ? -45 : pid->Err_Now;
//    }

    /* 误差较小时取消误差(积分死区) */
    if(pid->Err_ABS < StaticDif_Err) {
        pid->Err_Now = 0;
    }

    pid->Err_ABS = fabsf(pid->Err_Now);
    pid->Err_Sum += pid->Err_Now;

    /* 简易屏幕显示当前状态 */
    if(pid->Err_Now < -8.0f) {
        Car_State_Flag = 4;         //左转
    } else if(pid->Err_Now > 8.0f) {
        Car_State_Flag = 5;         //右转
    } else {
        Car_State_Flag = 1;
    }

    /* 状态切换消除积分 */
    //如切换PID模式，则建议消除积分项以避免积分饱

    /* 三段式积分分离 */
//    if(pid->Err_ABS > 30 & pid->Err_ABS < 35) {
//        ki_index = 0.600f;
//    } else if(pid->Err_ABS > 25 & pid->Err_ABS < 30) {
//        ki_index = 0.900f;
//    } else {
//        ki_index = 1.000f;
//    }

    /* 遇限削弱积分 */
//    if(pid->Err_ABS > 35) {
//        ki_index = 0.000f;
//    }

    /* 变速积分 */

    /* 积分抗饱和 */
    pid->Err_Sum = pid->Err_Sum >  pid->i_Max ?  pid->i_Max : pid->Err_Sum;
    pid->Err_Sum = pid->Err_Sum < -pid->i_Max ? -pid->i_Max : pid->Err_Sum;
//    pid->Output_i = pid->Output_i >  pid->i_Max ?  pid->i_Max : pid->Output_i;
//    pid->Output_i = pid->Output_i < -pid->i_Max ? -pid->i_Max : pid->Output_i;

    /* 不完全微分(微分项) */
    pid->Output_d = pid->Kd * (pid->Err_Now - pid->Err_Last) * Lpf1Factor_d + pid->Output_dd * (1.000f - Lpf1Factor_d);

    if (isnanf(pid->Output_d) || isinff(pid->Output_d)) {
        if (isnanf(pid->Output_dd) || isinff(pid->Output_dd)) {
            pid->Output_d = 0;
        } else {
            pid->Output_d = pid->Output_dd;
        }
    }

    pid->Output_dd = pid->Output_d;

    /* PID计算 */
//    pid->Output = pid->Kp * pid->Err_Now
//                    + ki_index * pid->Err_Sum
//                    + pid->Output_d;

    /* 专家PID */
    //小偏差
    if (pid->Err_ABS < S_Err) {
        //小偏差弱调 误差较小, 调小比例
        pid->Output = Offine_K * pid->Kp * pid->Err_Now
                    + Offine_K * ki_index * pid->Ki * pid->Err_Sum
                    + Offine_K * pid->Output_d;
    }
    //大偏差
    else if (pid->Err_ABS > L_Err) {
        pid->Output = pid->Kp * pid->Err_Now
                    + ki_index * pid->Ki * pid->Err_Sum
                    + pid->Output_d;
    }
    //正常偏差
    else {
        //偏差在朝向偏差绝对值增大的方向变化(偏差越来越大), 或者偏差一直为某一固定值
        if (((int16)pid->Err_Now * ((int16)pid->Err_Now - (int16)pid->Err_Last) > 0 &&
                (int16)pid->Err_Last * ((int16)pid->Err_Last - (int16)pid->Err_LastLast) > 0) ||
                ((int16)pid->Err_Now - (int16)pid->Err_Last) == 0) {

            if (pid->Err_Now > Mid_Err || pid->Err_Now < -Mid_Err) {
                pid->Output = Online_K * pid->Kp * pid->Err_Now
                            + Online_K * ki_index * pid->Ki * pid->Err_Sum
                            + Online_K * pid->Output_d;
            }
            //偏差是向绝对值增大的方向变化, 但是偏差绝对值本身并不是很大
            else if (pid->Err_Now < Mid_Err && pid->Err_Now > -Mid_Err) {
                pid->Output = pid->Kp * pid->Err_Now
                            + ki_index * pid->Ki * pid->Err_Sum
                            + pid->Output_d;
            }
        }
        //正常情况
        else {
            pid->Output = pid->Kp * pid->Err_Now
                        + ki_index * pid->Ki * pid->Err_Sum
                        + pid->Output_d;
        }
    }


    /* 输出限幅 */
    if (fabsf(pid->Output) > pid->out_Max) {
        pid->Output = pid->Output >  pid->out_Max ?  pid->out_Max : pid->Output;
        pid->Output = pid->Output < -pid->out_Max ? -pid->out_Max : pid->Output;
    }

    /* 不完全微分(输出) */
    pid->Output = Lpf1Factor_Out * pid->Output + (1.000f - Lpf1Factor_Out) * pid->Output_Last;

    if (isnanf(pid->Output) || isinff(pid->Output)) {
        if (isnanf(pid->Output_Last) || isinff(pid->Output_Last)) {
            pid->Output = 0;
        } else {
            pid->Output = pid->Output_Last;
        }
    }

    /* 消除静差 */
    if(fabs(pid->Output) < StaticDif_Out) {
        pid->Output = 0.0f;
    }

    AngleOutPut = (int16)(pid->Output);

    /* 更新上次值 */
    pid->Err_Last = pid->Err_Now;         // 更新上次误差
    pid->Err_LastLast = pid->Err_Last;
    pid->Output_Last = pid->Output;
}

/*
 * @name:void Realize_PID(PIDTypeDef_t *pid);
 * @function: 位置式PID控制(陀螺仪
 * @param:none
 * @return:none
 */
void Gyro_Realize_PID(PIDTypeDef_t *pid, float Expect)
{
    float StaticDif_Err = 2.0f;     //静差(余差)
    float StaticDif_Out = 3.0f;

    /* 参数写入 */
    AnglePID_Parameter_Write(Gyro_PID_Parameter);

    /* 误差求取 */
    pid->Now_Val = ICM_Treated.gyro.z * Gyro_K;
    pid->EX_Val = (float)Expect;
    pid->Err_Now = pid->EX_Val - (float)pid->Now_Val;
    pid->Err_ABS = fabsf(pid->Err_Now);

    /* 误差限幅 */

    /* 误差较小时取消误差 */
    if (pid->Err_ABS < StaticDif_Err) {
        pid->Err_Now = 0.0f;
    }

    pid->Err_Sum += pid->Err_Now;

    /* PID计算 */
    pid->Output = -(pid->Kp * pid->Err_Now
                    + pid->Ki* ki_index * pid->Err_Sum
                    + pid->Kd * (pid->Err_Now - pid->Err_Last));

    /* 输出限幅 */

    /* 输出项一阶低通滤波 */
    pid->Output = 0.800f * pid->Output + 0.200f * pid->Output_Last;

    /* 消除静差 */
    if(fabs(pid->Output) < StaticDif_Out) {
        pid->Output = 0.0f;
    }

    GyroOutPut = (int16)(pid->Output);

    /* 更新上次值 */
    pid->Err_Last = pid->Err_Now;         // 更新上次误差
    pid->Output_Last = pid->Output;
}

/*
 * @name:void DynamicRealize_PID(PIDTypeDef_t *pid);
 * @function: 位置式动态PID控制(角加速度
 * @param:none
 * @return:none
 */
void DynamicRealize_PID(PIDTypeDef_t *pid)
{
    float StaticDif_Err = 2.0f;        //静差(余差)
    float StaticDif_Out = 3.0f;

    /* 根据元素的参数写入 */
    AnglePID_Parameter_Write(Angle2_PID1_Parameter);

    /* 误差求取 */
    Sensor_Tracking();

    pid->Err_Now = FusionErr;
    pid->Err_ABS = fabsf(pid->Err_Now);

    /* 误差较小时取消误差 */
    if(pid->Err_ABS < StaticDif_Err) {
        pid->Err_Now = 0.0f;
    }

    /* 误差限幅 */
//    if(pid->Err_ABS > 45.0f) {
//        pid->Err_Now = pid->Err_Now>  45.0f ?  45.0f : pid->Err_Now;
//        pid->Err_Now = pid->Err_Now< -45.0f ? -45.0f : pid->Err_Now;
//    }

    /* 计算动态DKp */
    pid->DKp = 0.000f;
    pid->DKp = 1.0f * pid->Err_Now * pid->Err_Now / pid->Kp + pid->Ki;
    //P值与差值成二次函数关系，此处P和I不是PID参数，而是动态PID参数

    /* PID计算 */
    pid->Output = 0.000f;
    pid->Output = pid->DKp * pid->Err_Now
                    + pid->Kd * (0.800f * pid->Err_Now + 0.800f * pid->Err_Last);    //只用PD

    /* 输出限幅 */
    if (fabsf(pid->Output) > pid->out_Max) {
        pid->Output = pid->Output >  pid->out_Max ?  pid->out_Max : pid->Output;
        pid->Output = pid->Output < -pid->out_Max ? -pid->out_Max : pid->Output;
    }

    /* 消除静差 */
    if(fabs(pid->Output) < StaticDif_Out) {
        pid->Output = 0.0f;
    }

    AngleOutPut = (int16)(pid->Output);

    /* 更新上次值 */
    pid->Err_Last = pid->Err_Now;
}


/*
 * @name:void MortorPID_Parameter_Write(float *Mx_Param);
 * @function:PID Parameter Write
 * @param:parameter
 * @return:none
 */
void MortorPID_Parameter_Write(int16 *M1_Param, int16 *M2_Param, int16 *M3_Param, int16 *M4_Param)
{
    Mortor1_PID.Kp = M1_Param[Param_Kp];
    Mortor1_PID.Ki = M1_Param[Param_Ki];
    Mortor1_PID.Kd = M1_Param[Param_Kd];

    Mortor2_PID.Kp = M2_Param[Param_Kp];
    Mortor2_PID.Ki = M2_Param[Param_Ki];
    Mortor2_PID.Kd = M2_Param[Param_Kd];

    Mortor3_PID.Kp = M3_Param[Param_Kp];
    Mortor3_PID.Ki = M3_Param[Param_Ki];
    Mortor3_PID.Kd = M3_Param[Param_Kd];

    Mortor4_PID.Kp = M4_Param[Param_Kp];
    Mortor4_PID.Ki = M4_Param[Param_Ki];
    Mortor4_PID.Kd = M4_Param[Param_Kd];
}

/*
 * @name:void AnglePID_Parameter_Write(float *Mx_Param);
 * @function:PID Parameter Write
 * @param:parameter
 * @return:none
 */
void AnglePID_Parameter_Write(float *Param)
{
    Angle.Kp        = Param[Param_Kp];
    Angle.Ki        = Param[Param_Ki];
    Angle.Kd        = Param[Param_Kd];
    Angle.out_Max   = Param[Param_Kl];
}

/*
 * @name:void PID_Parameter_Init(float *Mx_Param);
 * @function:PID Parameter Write
 * @param:parameter
 * @return:none
 */
void PID_Parameter_Init(PIDTypeDef_t *pid)
{
    pid->Now_Val        = 0.0f;
    pid->EX_Val         = 0.0f;
    pid->Err_Now        = 0.0f;
    pid->Err_ABS        = 0.0f;
    pid->Err_Last       = 0.0f;
    pid->Err_LastLast   = 0.0f;
    pid->Err_Sum        = 0.0f;
    pid->Kp             = 0.0f;
    pid->Ki             = 0.0f;
    pid->Kd             = 0.0f;
    pid->Output_p       = 0.0f;
    pid->Output_i       = 0.0f;
    pid->Output_d       = 0.0f;
    pid->Output_dd      = 0.0f;
    pid->Output         = 0.0f;
    pid->Output_Last    = 0.0f;
}

void Motor_PID_Parameter_Init(Motor_PIDTypeDef_t *pid)
{
    pid->Now_Val        = 0;
    pid->EX_Val         = 0;
    pid->Err_Now        = 0;
    pid->Err_ABS        = 0;
    pid->Err_Last       = 0;
    pid->Err_LastLast   = 0;
    pid->Err_Sum        = 0;
    pid->Kp             = 0;
    pid->Ki             = 0;
    pid->Kd             = 0;
    pid->Output         = 0;
    pid->Output_Last    = 0;
}

//////////////////////////////////////////////////////////////////////
////////////////////////////////没用到////////////////////////////////
//////////////////////////////////////////////////////////////////////



/*
 * @name:void Incremental_PID(PIDTypeDef_t *pid);
 * @function: 增量式PID控制
 * @param:none
 * @return:none
 */
void Incremental_PID(PIDTypeDef_t *pid)
{
    float StaticDif_Err = 1.0f;     //静差(余差)
    float StaticDif_Out = 1.0f;     //静差

    /* 误差求取 */
    pid->Err_Now = pid->EX_Val - pid->Now_Val;    //计算当前误差
    pid->Err_ABS = ABS(pid->Err_Now);

    /* 误差较小时取消误差 */
    if(pid->Err_ABS < StaticDif_Err) {
        pid->Err_Now = 0;
    }

    /* 速度误差限幅 */
    if(pid->Err_ABS > 64) {
        pid->Err_Now = pid->Err_Now >  64 ?  64 : pid->Err_Now;
        pid->Err_Now = pid->Err_Now < -64 ? -64 : pid->Err_Now;
    }

    /* PID计算 */
    pid->Output += pid->Kp * (pid->Err_Now - pid->Err_Last)
                      + pid->Ki * (pid->Err_Now)
                      + pid->Kd * (pid->Err_Now - 2 * pid->Err_Last + pid->Err_LastLast);

    //可以串联BangBang_PID

    /* 输出限幅 */
    pid->Output = pid->Output > Mortor_PWM_Max ? Mortor_PWM_Max : pid->Output;
    pid->Output = pid->Output < Mortor_PWM_Min ? Mortor_PWM_Min : pid->Output;

    /* 消除静差 */
    if(fabs(pid->Output) < StaticDif_Out) {
        pid->Output = 0.0f;
    }

    /* 更新上次值 */
    pid->Err_Last = pid->Err_Now;       //更新上次误差
    pid->Err_LastLast = pid->Err_Last;  //更新上上次误差
}

/*
 * @name:void Nonlinear_Realize_PID(PIDTypeDef_t *pid);
 * @function: 非线性PID控制
 * @param:none
 * @return:none
 */
void Nonlinear_Realize_PID(PIDTypeDef_t *pid)
{
    uint16  errabs = 0;
    float   e = 2.71828183f;
    int8    Nonlinear_Kp1 = 0;
    int8    Nonlinear_Kd1 = 0;
    int8    Nonlinear_Kd2 = 1;
    float   Nonlinear_Kd3 = 0.15f;

    int8    StaticDif_Out = 1;     //静差(余差)

    /* 根据元素的参数写入 */
//    Angle_ElementBase_WritePID();

    /* 误差求取 */
    Angle_Part_Process();

    pid->Err_Now = AngleErr;
    pid->Err_ABS = fabsf(pid->Err_Now);
    errabs = (uint16)pid->Err_ABS;

    /* 误差限幅 */
    if(pid->Err_ABS > 45) {
        pid->Err_Now = pid->Err_Now>= 45? 45:pid->Err_Now;
        pid->Err_Now = pid->Err_Now<=-45?-45:pid->Err_Now;
    }

    /* PID计算 */
    pid->Output = -(Nonlinear_Kp1 + pid->Kp * (float)(10000 - SechErr[errabs]) / 10000.0f
                 + pid->Ki * (SechErr[errabs] / 10000.0f)
                 + Nonlinear_Kd1 + pid->Kd / (1.000f + Nonlinear_Kd2 * pow(e, Nonlinear_Kd3 * (Nonlinear_Kd3 * pid->Err_Now))));

    /* 输出限幅 */
    pid->Output = pid->Output >  pid->out_Max ?  pid->out_Max : pid->Output;
    pid->Output = pid->Output < -pid->out_Max ? -pid->out_Max : pid->Output;

    /* 消除静差 */
    if(fabsf(pid->Output) < StaticDif_Out) {
        pid->Output = 0;
    }

    /* 自己做的 */
    AngleOutPut = (int16)(0.8f * pid->Output + 0.2f * AngleOutPut);     //Angle_Last
}

/*
 * @name:void Changable_PID(PIDTypeDef_t *pid);
 * @function: 变积分PID
 * @param:none
 * @return:none
 * @notion:e大i小
 */
void Changable_PID(PIDTypeDef_t *pid)
{
    float change_kib = 4;

    ki_index = pid->Ki;

    if (pid->Err_Now +  pid->Err_Last > 0)
    {
        ki_index = (pid->Ki) - (pid->Ki) / (1 + exp(change_kib - 0.2 * fabs(pid->Err_Now)));    //变积分控制
    }

    pid->Output = pid->Kp * (pid->Err_Now - pid->Err_Last)
                      + pid->Ki * ki_index * (pid->Err_Now)
                      + pid->Kd * (pid->Err_Now - 2 * pid->Err_Last + pid->Err_LastLast);
}

/*
 * @name:void DA_Incremental_PID(PIDTypeDef_t *pid);
 * @function: 微分先行式增量式PID控制
 * @param:none
 * @return:none
 */
void DA_Incremental_PID(PIDTypeDef_t *pid)
{
//    float c1, c2, c3, temp;
//
//    float gama = 0.8f; //微分先行滤波系数, 在0~1之间
//
//    pid->Error_Now = pid->EX_Val - pid->Now_Val;
//
//    temp = gama * pid->Kd + pid->Kp;
//    c3 = pid->Kd / temp;
//    c2 = (pid->Kd +pid->Kp) / temp;
//    c1 = gama * c3;
//
//    pid->DeltaPv = pid->Now_Val - pid->LastDeltaPv;
//
//    pid->Incr_Val = pid->Kp * (pid->Error_Now - pid->Err_Last)
//                      + pid->Ki * pid->Error_Now
//                      + c1 * pid->Error_d + c2 * pid->DeltaPv * c3 * pid->LastDeltaPv;
//
//    pid->Output_Val += pid->Incr_Val;
//
//    pid->LastDeltaPv = pid->DeltaPv;
//    pid->Err_Last = pid->Error_Now;
}

/*
 * @name:void FeedForwardController(FFCTypeDef_t *pid);
 * @function: 前馈补偿控制
 * @param:none
 * @return:none
 */
void FeedforwardController(FFCTypeDef_t *pid)
{
    float FFCa = 1.0f, FFCb = 1.0f;

    pid->FOutput = FFCa * (pid->Rin - pid->LastRin) + FFCb * (pid->Rin - 2 * pid->LastRin + pid->PerrRin);

    pid->PerrRin= pid->LastRin;
    pid->LastRin= pid->Rin;
}
