/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_pwm
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

#ifndef _zf_pwm_h
#define _zf_pwm_h

#include "common.h"

typedef enum
{
    //同一个PWMP模块不同通道只能输出相同频率的PWM，占空比可设置不同
    //例如PWM1_CH1_PA8与PWM1_CH3_PA10属于同一个子模块，频率只能一样，但是占空比可以不一样

    //----------------PWM1----------------
    //以下为TIMER1的PWM的一组通道，不允许通道混用。
    //例如：PWM1_CH2_PA9与PWM1_CH1N_PB13不能同时使用。
    PWM1_CH1_A8 = 0x0000,
    PWM1_CH2_A9,
    PWM1_CH3_A10,
    PWM1_CH4_A11,


    //----------------PWM2----------------
    //以下为TIMER2的PWM的一组通道，不允许通道混用。
    //例如：PWM2_CH1_A15与PWM2_CH3_A2不能同时使用。
    PWM2_CH1_A0 = 0x0100,
    PWM2_CH2_A1,
    PWM2_CH3_A2,
    PWM2_CH4_A3,

    //以下为TIMER2的PWM的一组通道，不允许通道混用。
    PWM2_CH1_A15 = 0x0110,     //完全映射
    PWM2_CH2_B3,               //完全映射
    PWM2_CH3_B10,              //完全映射
    PWM2_CH4_B11,              //完全映射

    //----------------PWM3----------------
    ///以下为TIMER3的PWM的一组通道，不允许通道混用。
    //例如：PWM3_CH1_PA6与PWM3_CH4_C9不能同时使用。
    PWM3_CH1_A6 = 0x0200,
    PWM3_CH2_A7,
    PWM3_CH3_B0,
    PWM3_CH4_B1,

    //以下为TIMER3的PWM的一组通道，不允许通道混用。
    PWM3_CH1_C6 = 0x0210,      //完全映射
    PWM3_CH2_C7,               //完全映射
    PWM3_CH3_C8,               //完全映射
    PWM3_CH4_C9,               //完全映射

    //以下为TIMER3的PWM的一组通道，不允许通道混用。
    PWM3_CH1_B4 = 0x0220,      //部分映射
    PWM3_CH2_B5,               //部分映射

    //----------------PWM4----------------
    //以下为TIMER4的PWM的一组通道。
    PWM4_CH1_B6 = 0x0300,
    PWM4_CH2_B7,
    PWM4_CH3_B8,
    PWM4_CH4_B9

}PWMCH_enum;

#define PWM_DUTY_MAX 10000

void pwm_init(PWMCH_enum pwmch, uint32 freq, uint32 duty);
void pwm_duty(PWMCH_enum pwmch, uint32 duty);
void pwm_freq(PWMCH_enum pwmch, uint32 freq, uint32 duty);
void pwm_stop(PWMCH_enum pwmch);


#endif
