/*
 *  motor.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef _SPEED_CONTROL_H_
#define _SPEED_CONTROL_H_

//include
#include "MY_Bsp.h"

#define Speed_Flag  1           // 1:PID 2:PWM
#define Board_Flag  1           // 1:自己的 2:逐飞

//***************************PWM*************************************/

//#define     F1_PIN      A15 //前后 以显示屏显示方向为准
//#define     F2_PIN      B3
//#define     B1_PIN      B10
//#define     B2_PIN      B11
//
//#define     F1_PWM      PWM4_CH1_B6
//#define     F2_PWM      PWM4_CH2_B7
//#define     B1_PWM      PWM4_CH3_B8
//#define     B2_PWM      PWM4_CH4_B9

#define     F1_PIN      A15 //前后 以显示屏显示方向为准
#define     F2_PIN      B3
#define     B1_PIN      B10
#define     B2_PIN      B11

#define     F1_PWM      PWM4_CH1_B6
#define     F2_PWM      PWM4_CH2_B7
#define     B1_PWM      PWM4_CH3_B8
#define     B2_PWM      PWM4_CH4_B9

//structure
typedef struct {
    int16 Speed_Idealx;
    int16 Speed_Idealy;
    int16 Speed_Idealr;
    int16 Speed_Ideal;
    int16 Speed_Read;
    int16 Speed_Last;
    int16 PWM_Write;
    int16 Acc;
    int32 Distance;
} MortorTypedef_t;

//Statement
extern MortorTypedef_t Mortor1, Mortor2, Mortor3, Mortor4;

//Function
extern  void Speed_Control_Init(void);
extern  void SpeedPID_Control(void);
extern  void Car_Run(int16 x, int16 y, int16 rotate);

//Function
extern  void Motor_PWM_Init(void);
extern  void Mototr_Parameter_Init(MortorTypedef_t *sptr);
extern  void MortorType_Init(void);
extern  void Mortor_Prevent_Madness(void);
extern  void Motor_PWM_Write(void);

extern  void FSS_Print_PWM(void);
extern  void Wireless_Send(void);

#endif /* CODE_MOTOR_H_ */
