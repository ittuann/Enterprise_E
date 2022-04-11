/*
 *  key.h
 *
 *  Created on: 2021Äê1ÔÂ10ÈÕ
 *      Author: LBQ
 */

#ifndef _key_h
#define _key_h

#include "MY_Bsp.h"

/**********************KEY***********************/

//Key Pin
//#define KEY_1_PIN       C8
//#define KEY_2_PIN       C9
//#define KEY_3_PIN       B8
//#define KEY_4_PIN       B9
//#define KEY_5_PIN       A0

#define KEY_1_PIN       B2
#define KEY_2_PIN       B5
#define KEY_3_PIN       B4

//Pin Mode Get Define
#define KEY_1_GEt    gpio_get(KEY_1_PIN)
#define KEY_2_GEt    gpio_get(KEY_2_PIN)
#define KEY_3_GEt    gpio_get(KEY_3_PIN)
//#define KEY_4_GEt    gpio_get(KEY_4_PIN)
//#define KEY_5_GEt    gpio_get(KEY_5_PIN)

//Exit Define
#define KEY1_EXTI_IRQN EXTI2_IRQn
#define KEY2_EXTI_IRQN EXTI4_IRQn
#define KEY3_EXTI_IRQN EXTI9_5_IRQn

//Envent Define
#define KEY_1_PRESS     1
#define KEY_2_PRESS     2
#define KEY_3_PRESS     3
//#define KEY_4_PRESS     4
//#define KEY_5_PRESS     5

//Function
extern  void KEY_Init(void);
extern  uint8 KEY_Scan(void);
extern  int8 KEY_Scan_Screen(void);
extern  uint8 KEY_Scan_Long(void);
extern  void KEY_Test(void);


/**********************DIP***********************/

////Dip Pin
//#define DIP_1_PIN       A4
//#define DIP_2_PIN       B11
//#define DIP_3_PIN       B12
//
////Pin Mode Get Define
//#define DIP_1_GEt    gpio_get(DIP_1_PIN)
//#define DIP_2_GEt    gpio_get(DIP_2_PIN)
//#define DIP_3_GEt    gpio_get(DIP_3_PIN)
//
//#define DIP_1_ON        1
//#define DIP_2_ON        2
//#define DIP_3_ON        3
//
////Function
//extern  void Boot_DIP_Scan(void);
//extern  uint8 DIP_Scan(void);
//extern  unsigned char Check_DIP_1(void);
//extern  unsigned char Check_DIP_2(void);
//extern  unsigned char Check_DIP_3(void);

#endif /* CODE_KEY_H_ */
