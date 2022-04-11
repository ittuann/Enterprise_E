/*
 *  pid.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */
#ifndef _PID_H_
#define _PID_H_

#include "MY_Bsp.h"

//Structure
typedef struct {
    float   Now_Val;            //当前值
    float   Last_Val;           //上次值
    float   EX_Val;             //期望值

    float   Err_Now;            //当前误差
    float   Err_ABS;            //当前误差绝对值
    float   Err_Last;           //上次误差
    float   Err_LastLast;       //上上次误差
    float   Err_Sum;            //误差累积

    float   Kp;                 //比例系数
    float   Ki;                 //积分系数
    float   Kd;                 //微分系数

    float   Output_p;           //比例项输出
    float   Output_i;           //积分项输出
    float   Output_d;           //微分项输出
    float   Output_dd;          //上次微分项输出
    float   p_Max;              //比例项输出限幅
    float   i_Max;              //积分项限幅
    float   d_Max;              //微分项输出限幅
    float   out_Max;            //输出限幅
    float   Output;             //输出
    float   Output_Last;        //上次输出值

    float   DKp;                //动态KP
} PIDTypeDef_t;

typedef struct {
    int16   Now_Val;            //当前速度
    int16   EX_Val;             //期望速度
    int16   Err_Now;            //当前误差
    int16   Err_ABS;            //当前误差
    int16   Err_Last;           //上次误差
    int16   Err_LastLast;       //上上次误差
    int16   Err_Sum;            //误差累积
    int16   Kp;                 //比例系数
    int16   Ki;                 //积分系数
    int16   Kd;                 //微分系数
    int16   i_Max;              //积分项限幅
    int16   Output;             //输出占空比
    int16   Output_Last;        //上次输出值
} Motor_PIDTypeDef_t;
//用于整型和字符型变量，提高运算速度

typedef struct {
    float Rin;
    float LastRin;
    float PerrRin;
    float FOutput;
} FFCTypeDef_t;

//Parameter
extern  Motor_PIDTypeDef_t Mortor1_PID, Mortor2_PID, Mortor3_PID, Mortor4_PID;
extern  PIDTypeDef_t Angle, Gyro_Angle;
extern  FFCTypeDef_t FFC;

#define Param_Kp    0
#define Param_Ki    1
#define Param_Kd    2
#define Param_Kl    3

extern  float   ki_index;  //积分分离倍率

//Function
extern  void PID_Init(void);
extern  void PID_Parameter_Init(PIDTypeDef_t *pid);
extern  void Motor_PID_Parameter_Init(Motor_PIDTypeDef_t *pid);

extern  void MortorPID_Parameter_Write(int16 *M1_Param, int16 *M2_Param, int16 *M3_Param, int16 *M4_Param);
extern  void AnglePID_Parameter_Write(float *Param);

extern  void Motor_Realize_PID(Motor_PIDTypeDef_t *pid, uint8 type); //位置式PID(电机
extern  void Realize_PID(PIDTypeDef_t *pid);             //位置式PID
extern  void Gyro_Realize_PID(PIDTypeDef_t *pid, float Expect); //位置式PID(陀螺仪
extern  void DynamicRealize_PID(PIDTypeDef_t *pid);      //位置式动态PID

extern  void Incremental_PID(PIDTypeDef_t *pid);         //增量式PID
extern  void Nonlinear_Realize_PID(PIDTypeDef_t *pid);   //非线性PID
extern  void Changable_PID(PIDTypeDef_t *pid);           //变积分PID
extern  void DA_Incremental_PID(PIDTypeDef_t *pid);      //微分先行式增量式PID
extern  void FeedforwardController(FFCTypeDef_t *pid);   //前馈补偿控制

#endif /* CODE_PID_H_ */
