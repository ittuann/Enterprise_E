/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_timer
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
#include "ch32v10x_misc.h"

#include "zf_gpio.h"
#include "zf_qtimer.h"
#include "zf_nvic.h"

const uint32 TIMERN[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE};



//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器引脚初始化
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void timer_gpio_quad_init(TIMER_PIN_enum phaseA, TIMER_PIN_enum phaseB)
{
    if(TIMER1_CHA_A8 == phaseA)       gpio_init(A8, GPI, 0, IN_FLOAT);
    else if(TIMER2_CHA_A0 == phaseA)  gpio_init(A0, GPI, 0, IN_FLOAT);
    else if(TIMER2_CHA_A15 == phaseA) gpio_init(A15, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHA_A6 == phaseA)  gpio_init(A6, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHA_C6 == phaseA)  gpio_init(C6, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHA_B4 == phaseA)  gpio_init(B4, GPI, 0, IN_FLOAT);
    else if(TIMER4_CHA_B6 == phaseA)  gpio_init(B6, GPI, 0, IN_FLOAT);

    if(TIMER1_CHB_A9 == phaseB)       gpio_init(A9, GPI, 0, IN_FLOAT);
    else if(TIMER2_CHB_A1 == phaseB)  gpio_init(A1, GPI, 0, IN_FLOAT);
    else if(TIMER2_CHB_B3 == phaseB)  gpio_init(B3, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHB_A7 == phaseB)  gpio_init(A7, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHB_C7 == phaseB)  gpio_init(C7, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHB_B5 == phaseB)  gpio_init(B5, GPI, 0, IN_FLOAT);
    else if(TIMER4_CHB_B7 == phaseB)  gpio_init(B7, GPI, 0, IN_FLOAT);

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器解码初始化
//  @param      timern      定时器枚举体
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           推荐使用正交解码编码器。
//-------------------------------------------------------------------------------------------------------------------
void timer_quad_init(TIMERN_enum timern, TIMER_PIN_enum phaseA, TIMER_PIN_enum phaseB)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    if(TIMER_1 == timern)       RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//使能定时器1的时钟
    else  if(TIMER_2 == timern) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2的时钟
    else  if(TIMER_3 == timern) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟
    else  if(TIMER_4 == timern) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟

    timer_gpio_quad_init(phaseA, phaseB);                                           //初始化引脚

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                            //使能AFIO复用功能模块时钟

    if(TIMER2_CHA_A15 == phaseA && TIMER2_CHB_B3 == phaseB)                         //引脚重映射
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else    if(TIMER3_CHA_C6 == phaseA && TIMER3_CHB_C7 == phaseB)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else    if(TIMER3_CHA_B4 == phaseA && TIMER3_CHB_B5 == phaseB)
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //预分频器
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //重复计数器
    TIM_TimeBaseInit((TIM_TypeDef*)(TIMERN[timern]), &TIM_TimeBaseStructure);       //初始化结构体
    TIM_EncoderInterfaceConfig((TIM_TypeDef*)(TIMERN[timern]), TIM_EncoderMode_TI2 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式

    TIM_Cmd((TIM_TypeDef*)(TIMERN[timern]), ENABLE);                                //定时器使能
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器编码器解码取值
//  @param      timern      定时器枚举体
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int16 timer_quad_get(TIMERN_enum timern)
{
    int16 count = ((TIM_TypeDef*)(TIMERN[timern]))->CNT;
    return count;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器的计数器清空
//  @param      timern      定时器枚举体
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void timer_quad_clear(TIMERN_enum timern)
{
    ((TIM_TypeDef*)(TIMERN[timern]))->CNT = 0;
}


