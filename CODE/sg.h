/*
 *  sg.h
 *
 *  Created on: 2021Äê1ÔÂ10ÈÕ
 *      Author: LBQ
 */
#ifndef CODE_INC_SG_H_
#define CODE_INC_SG_H_

#include "MY_Bsp.h"

#define PWM_SG PWM2_CH1_A0

extern  uint16  Servo_Max;
extern  uint16  Servo_Min;
extern  int16   SG_Test_Temp;

extern  uint16  Servo_Center_Front;
extern  uint16  Servo_Center_Back;
extern  uint16  Servo_Turn90_Left;
extern  uint16  Servo_Turn90_Right;

extern  void SG_Init(void);
extern  void ServoCtrl(uint16 duty);

extern  void SG_Test(void);

extern  void SG_Turn_Front(void);
extern  void SG_Turn_Back(void);
extern  void SG_Turn90_Left(void);
extern  void SG_Turn90_Right(void);

#endif /* CODE_SG_H_ */
