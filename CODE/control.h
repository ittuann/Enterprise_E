/*
 *  control.h
 *
 *  Created on: 2021Äê1ÔÂ10ÈÕ
 *      Author: LBQ
 */

#ifndef _CONTROL_H_
#define _CONTRAL_H_

#include "MY_Bsp.h"
#include "define.h"

void LED_Beat(void);
void Beep_Beat(void);

void Angle_ElementBase_WritePID(void);
void Midline_Compensation(void);

void Magnet_Identify(void);
void Magnet_Process(void);

void Camera_Process(void);
uint8 Get_Prospect(void);
void Angle_Process(void);
float Regression_cal(uint8 startline, uint8 endline);
void Angle_All_Process(void);
void Angle_Front_Process(void);
void Angle_Part_Process(void);
void Sensor_Tracking(void);

void Dis_Control(void);
void Midline_Change(void);
void Speed_Change(void);
void Speed_Change_Near(void);
void ValidRow_Change(void);
void Gear_Control(void);

void Car_Run_Protect(int16 x, int16 y, int16 rotate, uint8 mode);
void Car_Advance(uint16 speed, uint16 Dis, uint8 mode);
void Car_Translae(uint16 speed, uint16 Dis, uint8 mode);
void Car_Turn(uint16 speed, uint16 angle, uint8 mode);
void Car_Stop(void);

void Car_Out_Garage(uint8 mode);
void Ramp_Control(void);
void Gyro_Circle_Control(void);
void Car_Rush_Turn(void);
void Car_Test(void);

void Motor_Alert(uint8 Err);
//void KEY_Change(uint8 Param, float Num);

#endif /* CODE_CONTROL_H_ */
