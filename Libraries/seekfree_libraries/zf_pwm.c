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
#include "ch32v10x_rcc.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_tim.h"
#include "zf_pwm.h"
#include "zf_gpio.h"
#include "zf_systick.h"
#include "zf_qtimer.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM引脚初始化
//  @param      pwmch       PWM通道(可选择范围由zf_pwm.h内PWMCH_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void pwm_gpio_init(PWMCH_enum pwmch)
{
    switch(pwmch)
    {

        case PWM1_CH1_A8:
            gpio_init(A8, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM1_CH2_A9:
            gpio_init(A9, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM1_CH3_A10:
            gpio_init(A10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM1_CH4_A11:
            gpio_init(A11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;

//        case PWM1_CH1N_B13:
//            gpio_init(B13, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
//            break;
//        case PWM1_CH2N_B14:
//            gpio_init(B14, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
//            break;
//        case PWM1_CH3N_B15:
//            gpio_init(B15, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
//            break;
//
//        case PWM1_CH1N_A7:
//            gpio_init(A7, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
//            break;
//        case PWM1_CH2N_B0:
//            gpio_init(B0, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
//            break;
//        case PWM1_CH3N_B1:
//            gpio_init(B1, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
//            break;

        case PWM2_CH1_A0:
            gpio_init(A0, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM2_CH2_A1:
            gpio_init(A1, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM2_CH3_A2:
            gpio_init(A2, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM2_CH4_A3:
            gpio_init(A3, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;

        case PWM2_CH1_A15:
            gpio_init(A15, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM2_CH2_B3:
            gpio_init(B3, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM2_CH3_B10:
            gpio_init(B10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM2_CH4_B11:
            gpio_init(B11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;

        case PWM3_CH1_A6:
            gpio_init(A6, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH2_A7:
            gpio_init(A7, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH3_B0:
            gpio_init(B0, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH4_B1:
            gpio_init(B1, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;

        case PWM3_CH1_C6:
            gpio_init(C6, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH2_C7:
            gpio_init(C7, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH3_C8:
            gpio_init(C8, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH4_C9:
            gpio_init(C9, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;

        case PWM3_CH1_B4:
            gpio_init(B4, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM3_CH2_B5:
            gpio_init(B5, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;

        case PWM4_CH1_B6:
            gpio_init(B6, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM4_CH2_B7:
            gpio_init(B7, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM4_CH3_B8:
            gpio_init(B8, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        case PWM4_CH4_B9:
            gpio_init(B9, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
            break;
        default:
            //断言
            break;
    }
}







//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM初始化
//  @param      pwmch           PWM通道号及引脚
//  @param      freq            PWM频率
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_init(PWM1_CH1_A8, 50, 5000);   //初始化PWM1 通道1 使用引脚A8  输出PWM频率50HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWMCH_enum pwmch, uint32 freq, uint32 duty)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint16 match_temp;                                          //占空比值
    uint16 period_temp;                                         //周期值
    uint16 freq_div = 0;                                        //分频值

    pwm_gpio_init(pwmch);                                       //PWM引脚初始化

    if((pwmch >> 8) == 0x00)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    //使能定时器1时钟
    else if((pwmch >> 8) == 0x01)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //使能定时器2时钟
    else if((pwmch >> 8) == 0x02)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能定时器3时钟
    else if((pwmch >> 8) == 0x03)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //使能定时器4时钟

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //使能AFIO复用功能模块时钟

    //引脚重映射
    if((pwmch >> 4) == 0x10)        //PWM1的部分重映射
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
    else if((pwmch >> 4) == 0x11)   //PWM2的完全重映射
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if((pwmch >> 4) == 0x21)   //PWM3的完全重映射
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if((pwmch >> 4) == 0x22)   //PWM3的部分重映射
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    //获取系统主频
    sys_clk = 8000000 * (((RCC->CFGR0 >> 18)&0x0F) + 2);

    freq_div = (uint16)((sys_clk / freq) >> 16);                            //多少分频
    period_temp = (uint16)(sys_clk/(freq*(freq_div + 1)));                  //周期
    match_temp = period_temp * duty / PWM_DUTY_MAX;                         //占空比

    //初始化TIM1-4
    TIM_TimeBaseStructure.TIM_Period = period_temp - 1;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                         //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    //初始化TIM1-4 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = match_temp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;               //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_TimeBaseInit(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_TimeBaseStructure);  //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//        if(PWM1_CH1N_B13 <= pwmch)                                         //该通道为N通道
//        {
//            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //P通道
//            TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable;  //N通道
//        }
    if((pwmch & 0x03) == 0x00)                                          //通道选择
    {
        TIM_OC1Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );                       //定时器通道1初始化
        TIM_OC1PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable);               //定时器预装载配置
        TIM_OC1FastConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OC1FE);                             //比较捕获通道快速使能
    }
    else if((pwmch & 0x03) == 0x01)
    {
        TIM_OC2Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );
        TIM_OC2PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable);
        TIM_OC2FastConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OC2FE);                             //比较捕获通道快速使能
    }
    else if((pwmch & 0x03) == 0x02)
    {
        TIM_OC3Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );
        TIM_OC3PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable);
        TIM_OC3FastConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OC3FE);                             //比较捕获通道快速使能
    }
    else if((pwmch & 0x03) == 0x03)
    {
        TIM_OC4Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );
        TIM_OC4PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable);
        TIM_OC4FastConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OC4FE);                             //比较捕获通道快速使能
    }
    TIM_Cmd(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE);                                              //定时器使能
    TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE );
    //TIM_ARRPreloadConfig( TIM1, ENABLE );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM占空比设定
//  @param      pwmch           PWM通道号及引脚
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_duty(PWM1_CH1_A8, 5000);   //PWM1 通道1 使用引脚A8 占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty(PWMCH_enum pwmch, uint32 duty)
{
    uint16 match_temp;
    uint16 period_temp;
    //const uint32 TIMERN[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE};
    //获取周期值
    //计算占空比

    period_temp = ((TIM_TypeDef *) TIMERN[pwmch>>8])->ATRLR;    //获取定时器的周期值
    match_temp = period_temp * duty / PWM_DUTY_MAX;             //占空比

    if((pwmch >> 8) == 0x00)
    {
        (*(volatile uint32*)(0x40012C34 + (pwmch&0x03) * 0x04)) = match_temp;
        //0x40012C34 TIM1->CH1CVR
        //0x40012C38 TIM1->CH2CVR
        //0x40012C3C TIM1->CH3CVR
        //0x40012C40 TIM1->CH4CVR
    }
    else if((pwmch >> 8) == 0x01)
    {
        (*(volatile uint32*)(0x40000034 + (pwmch&0x03) * 0x04)) = match_temp;
        //0x40000034 TIM2->CH1CVR
        //0x40000038 TIM2->CH2CVR
        //0x4000003C TIM2->CH3CVR
        //0x40000040 TIM2->CH4CVR
    }
    else if((pwmch >> 8) == 0x02)
    {
        (*(volatile uint32*)(0x40000434 + (pwmch&0x03) * 0x04)) = match_temp;
        //0x40000434 TIM3->CH1CVR
        //0x40000438 TIM3->CH2CVR
        //0x4000043C TIM3->CH3CVR
        //0x40000440 TIM3->CH4CVR
    }
    else if((pwmch >> 8) == 0x03)
    {
        (*(volatile uint32*)(0x40000834 + (pwmch&0x03) * 0x04)) = match_temp;
        //0x40000834 TIM4->CH1CVR
        //0x40000838 TIM4->CH2CVR
        //0x4000083C TIM4->CH3CVR
        //0x40000840 TIM4->CH4CVR
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM频率设定
//  @param      pwmch           PWM通道号及引脚
//  @param      freq            PWM频率
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_freq(PWM1_CH1_A8, 60, 5000);   //使用引脚A8  输出PWM频率60HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_freq(PWMCH_enum pwmch, uint32 freq, uint32 duty)
{
    uint16 period_temp = 0;                                     //周期值
    uint16 freq_div = 0;                                        //分频值
    uint16 match_temp;

    freq_div = (uint16)((sys_clk / freq) >> 16);                //计算多少分频
    period_temp = (uint16)(sys_clk/(freq*(freq_div + 1)));      //计算周期

    ((TIM_TypeDef *) TIMERN[pwmch>>8])->ATRLR = period_temp - 1 ;
    ((TIM_TypeDef *) TIMERN[pwmch>>8])->PSC = freq_div;
    match_temp = period_temp * duty / PWM_DUTY_MAX;             //占空比
    if((pwmch >> 8) == 0x00)
    {
        (*(volatile uint32*)(0x40012C34 + (pwmch&0x03) * 0x04)) = match_temp;
    }
    else if((pwmch >> 8) == 0x01)
    {
        (*(volatile uint32*)(0x40000034 + (pwmch&0x03) * 0x04)) = match_temp;
    }
    else if((pwmch >> 8) == 0x02)
    {
        (*(volatile uint32*)(0x40000434 + (pwmch&0x03) * 0x04)) = match_temp;
    }
    else if((pwmch >> 8) == 0x03)
    {
        (*(volatile uint32*)(0x40000834 + (pwmch&0x03) * 0x04)) = match_temp;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM停止输出
//  @param      pwmch           PWM通道号及引脚
//  @return     void
//  Sample usage:               pwm_freq(PWM1_CH1_A8, 60, 5000);   //使用引脚A8  输出PWM频率60HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_stop(PWMCH_enum pwmch)
{
    TIM_Cmd(((TIM_TypeDef *) TIMERN[pwmch>>8]), DISABLE);         //定时器失能
}
