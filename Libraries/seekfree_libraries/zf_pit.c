/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_pit
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

#include "zf_qtimer.h"
#include "zf_pit.h"
#include "zf_nvic.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器周期中断
//  @param      timern      定时器通道
//  @param      ms          定时周期(1-32767)
//  @return     void
//  Sample usage:           timer_pit_interrupt(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_interrupt_ms(TIMERN_enum timern, uint16 ms)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //时钟使能
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                //时钟使能
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                //时钟使能
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);                //时钟使能

    //定时器TIM3初始化,计数器
    //ATRLR寄存器设置为1 ，则计数0和1，一共2次
    assert(ms);


    TIM_TimeBaseStructure.TIM_Period = sys_clk/2000 - 1;                    //计数44000
    TIM_TimeBaseStructure.TIM_Prescaler = 2*ms - 1;                      //设置为1MHZ计数一次,也就是1us计数一次

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERN[timern], &TIM_TimeBaseStructure);//根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig((TIM_TypeDef *)TIMERN[timern],TIM_IT_Update,ENABLE );      //使能指定的TIM中断,允许更新中断
    TIM_ClearITPendingBit((TIM_TypeDef *)TIMERN[timern], TIM_IT_Update);

    //中断优先级NVIC设置
    if(TIMER_1 == timern)
        nvic_init(TIM1_UP_IRQn, 1, 2, ENABLE);
    else if(TIMER_2 == timern)
        nvic_init(TIM2_IRQn, 1, 2, ENABLE);
    else if(TIMER_3 == timern)
        nvic_init(TIM3_IRQn, 1, 2, ENABLE);
    else if(TIMER_4 == timern)
        nvic_init(TIM4_IRQn, 1, 2, ENABLE);

    TIM_Cmd((TIM_TypeDef *)TIMERN[timern], ENABLE);  //使能TIMx

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器周期中断
//  @param      timern      定时器通道
//  @param      us          定时周期(1-65535)
//  @return     void
//  Sample usage:           timer_pit_interrupt(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_interrupt_us(TIMERN_enum timern, uint16 us)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //时钟使能
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                //时钟使能
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                //时钟使能
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);                //时钟使能

    //定时器TIM3初始化,计数器
    //ATRLR寄存器设置为1 ，则计数0和1，一共2次
    assert(us);


    TIM_TimeBaseStructure.TIM_Period = sys_clk/1000000 - 1;                 //计数88次
    TIM_TimeBaseStructure.TIM_Prescaler = us - 1;                           //分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERN[timern], &TIM_TimeBaseStructure);//根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig((TIM_TypeDef *)TIMERN[timern],TIM_IT_Update,ENABLE );      //使能指定的TIM中断,允许更新中断
    TIM_ClearITPendingBit((TIM_TypeDef *)TIMERN[timern], TIM_IT_Update);

    //中断优先级NVIC设置
    if(TIMER_1 == timern)
        nvic_init(TIM1_UP_IRQn, 1, 2, ENABLE);
    else if(TIMER_2 == timern)
        nvic_init(TIM2_IRQn, 1, 2, ENABLE);
    else if(TIMER_3 == timern)
        nvic_init(TIM3_IRQn, 1, 2, ENABLE);
    else if(TIMER_4 == timern)
        nvic_init(TIM4_IRQn, 1, 2, ENABLE);

    TIM_Cmd((TIM_TypeDef *)TIMERN[timern], ENABLE);  //使能TIMx

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      us计时器开始
//  @param      timern      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     void
//  Sample usage:           pit_start_us(TIMER_1);  //定时器0 开始计数，每以us计数一次
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_start_us(TIMERN_enum timern)
{
    if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //时钟使能
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                //时钟使能
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                //时钟使能
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);                //时钟使能

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 65535;                               //计数88次
    TIM_TimeBaseStructure.TIM_Prescaler = sys_clk/1000000 - 1;              //分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERN[timern], &TIM_TimeBaseStructure);//根据指定的参数初始化TIMx的时间基数单位
    ((TIM_TypeDef *)TIMERN[timern])->CNT = 0;
    TIM_Cmd((TIM_TypeDef *)TIMERN[timern], ENABLE);  //使能TIMx
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取计时器值
//  @param      timern      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     uint32      计数值
//  Sample usage:           本函数通常用户无需关心，如果需要获取计数值，应该调用h文件的宏定义函数（带有时间单位）
//-------------------------------------------------------------------------------------------------------------------
uint16 timer_pit_get_us(TIMERN_enum timern)
{
    return ((TIM_TypeDef *)TIMERN[timern])->CNT;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭PIT定时器
//  @param      timern      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     void
//  Sample usage:           关闭定时器并清除标志位
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_close(TIMERN_enum timern)
{
    TIM_Cmd((TIM_TypeDef *)TIMERN[timern], DISABLE);  //使能TIMx
}



