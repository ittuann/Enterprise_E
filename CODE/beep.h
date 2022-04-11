/*
 *  beep.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef _BEEP_H
#define _BEEP_H

//include
#include "MY_Bsp.h"

/************************BEEP**********************/

#define BEEP_PIN    D2
#define BEEP_ON     gpio_set(BEEP_PIN, 1)
#define BEEP_OFF    gpio_set(BEEP_PIN, 0)
#define BEEP_TOGGLE gpio_toggle(BEEP_PIN)
#define BEEP_STATE  gpio_get(BEEP_PIN)

//function
extern  void Beep_Init(void);
extern  void Beep_Buzzing(unsigned char time,unsigned short buzz_num);

extern  void Beep_Boot_Success(void);
extern  void Beep_Car_Stop(void);


/************************LED**********************/

////define
#define LED0_PIN B15
//#define LED1_PIN B15
//#define LED2_PIN B15
//#define LED3_PIN B15

typedef enum
{
  LED0   = 0,
  LED1   = 1,
  LED2   = 2,
  LED3   = 3,
  LEDALL = 4
} LEDn_e;

typedef enum
{
  ON  = 0,  //亮
  OFF = 1,  //灭
  RVS = 2,  //反转
} LEDs_e;

#define LED0_ON   	  gpio_set(LED0_PIN, 0)
#define LED0_OFF      gpio_set(LED0_PIN, 1)
#define LED0_TOGGLE   gpio_toggle(LED0_PIN)
//#define LED1_ON   	  gpio_set(LED1_PIN, 0)
//#define LED1_OFF      gpio_set(LED1_PIN, 1)
//#define LED1_TOGGLE   gpio_toggle(LED1_PIN)
//#define LED2_ON   	  gpio_set(LED2_PIN, 0)
//#define LED2_OFF      gpio_set(LED2_PIN, 1)
//#define LED2_TOGGLE   gpio_toggle(LED2_PIN)
//#define LED3_ON   	  gpio_set(LED3_PIN, 0)
//#define LED3_OFF      gpio_set(LED3_PIN, 1)
//#define LED3_TOGGLE   gpio_toggle(LED3_PIN)

//function
extern  void LED_Init(void);
extern  void LED_Blink(unsigned char led_num,unsigned char blink_num, unsigned short time);
extern  void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);

#endif /* CODE_BEEP_H_ */
