/*
 *  beep.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "beep.h"

/**********************BEEP***********************/

/*
 * @name:void Beep_Init(void);
 * @function: beep pin initialize
 * @param:none
 * @return:none
 */
void Beep_Init(void)
{
	gpio_init(BEEP_PIN, GPO, 0,GPIO_PIN_CONFIG);

	BEEP_OFF;
}

/*
 * @name:void Beep_Buzzing(uint8_t time_ms,uint16_t buzz_num);
 * @function: 蜂鸣
 * @param:  time_ms:发声时间   buzz_num:发声次数
 * @return:none
 */
void Beep_Buzzing(uint8_t time_ms, uint16_t buzz_num)
{
    BEEP_OFF;
	for(buzz_num = buzz_num * 2; buzz_num > 0; buzz_num--) {
        BEEP_TOGGLE;
        delay_ms(time_ms);
	}
	BEEP_OFF;
}

/*
 * @name:void Beep_Boot_Success(void);
 * @function:开机蜂鸣
 * @param:none
 * @return:none
 */
void Beep_Boot_Success(void)
{
    Beep_Buzzing(50, 2);
//    printf("Consuming Time is %d \n", Tim_Count);   //printf输出必须带上换行符
//    printf("Initialize Successfully\r\n");
//    uart_putstr(DEBUG_UART, (uint8 *)"InitDone");
}

/*
 * @name:void Beep_Car_Stop(void);
 * @function:停车蜂鸣
 * @param:none
 * @return:none
 */
void Beep_Car_Stop(void)
{
    BEEP_ON;
    systick_delay_ms(50);
    BEEP_OFF;
}


/**********************LED***********************/


/*
 * @name:void LED_Init(void);
 * @function:  initialize led
 * @param:none
 * @return:none
 */
void LED_Init(void)
{
    gpio_init(LED0_PIN, GPO, 1, GPIO_PIN_CONFIG);
//    gpio_init(LED1_PIN, GPO, 1, GPIO_PIN_CONFIG);
//    gpio_init(LED2_PIN, GPO, 1, GPIO_PIN_CONFIG);
//    gpio_init(LED3_PIN, GPO, 1, GPIO_PIN_CONFIG);

    LED_Ctrl(LEDALL, OFF);
}

/*
 * @name:void LED_Blink(uint8_t led_num,uint8_t bink_num, uint16_t time);
 * @function: LED Blink
 * @param:led number ,blink number, time
 * @return: none
 */
void LED_Blink(uint8 led_num,uint8 blink_num, uint16 time_ms)
{

    for( ; blink_num > 0; blink_num--) {
        switch(led_num) {
            case(0): LED0_ON;break;
//            case(1): LED1_ON;break;
//            case(2): LED2_ON;break;
//            case(3): LED3_ON;break;
        };
        systick_delay_ms(time_ms);
        switch(led_num) {
            case(0): LED0_OFF;break;
//            case(1): LED1_OFF;break;
//            case(2): LED2_OFF;break;
//            case(3): LED3_OFF;break;
        };
        systick_delay_ms(time_ms);
    }
}

/*
 * @name:void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
 * @function:LED_Ctrl
 * @param:LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
 * @return: none
 */
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
      switch(LEDno) {
          case LED0:
//            if(sta==ON)        gpio_set(LED0_PIN,0);
//            else if(sta==OFF)  gpio_set(LED0_PIN,1);
//            else if(sta==RVS) (gpio_get(LED0_PIN))?(gpio_set(LED0_PIN,0)):(gpio_set(LED0_PIN,1));
            break;

//          case LED1:
//            if(sta==ON)        gpio_set(LED1_PIN,0);
//            else if(sta==OFF)  gpio_set(LED1_PIN,1);
//            else if(sta==RVS) (gpio_get(LED1_PIN))?(gpio_set(LED1_PIN,0)):(gpio_set(LED1_PIN,1));
//            break;

//          case LED2:
//            if(sta==ON)        gpio_set(LED2_PIN,0);
//            else if(sta==OFF)  gpio_set(LED2_PIN,1);
//            else if(sta==RVS) (gpio_get(LED2_PIN))?(gpio_set(LED2_PIN,0)):(gpio_set(LED2_PIN,1));
//            break;

//          case LED3:
//            if(sta==ON)        gpio_set(LED3_PIN,0);
//            else if(sta==OFF)  gpio_set(LED3_PIN,1);
//            else if(sta==RVS) (gpio_get(LED3_PIN))?(gpio_set(LED3_PIN,0)):(gpio_set(LED3_PIN,1));
//            break;

          case LEDALL:
            if(sta==ON) {
                gpio_set(LED0_PIN,0);
//                gpio_set(LED1_PIN,0);
//                gpio_set(LED2_PIN,0);
//                gpio_set(LED3_PIN,0);
            } else if(sta==OFF) {
                gpio_set(LED0_PIN,1);
//                gpio_set(LED1_PIN,1);
//                gpio_set(LED2_PIN,1);
//                gpio_set(LED3_PIN,1);
            } else if(sta==RVS) {
              (gpio_get(LED0_PIN))?(gpio_get(LED0_PIN)):(gpio_get(LED0_PIN));
//              (gpio_get(LED1_PIN))?(gpio_get(LED1_PIN)):(gpio_get(LED1_PIN));
//              (gpio_get(LED2_PIN))?(gpio_get(LED2_PIN)):(gpio_get(LED2_PIN));
//              (gpio_get(LED3_PIN))?(gpio_get(LED3_PIN)):(gpio_get(LED3_PIN));
            }
            break;

          default:
            break;
      }
}
