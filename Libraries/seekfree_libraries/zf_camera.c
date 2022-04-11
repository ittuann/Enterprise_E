/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_camera
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
#include "ch32v10x_dma.h"
#include "ch32v10x_rcc.h"
#include "ch32v10x_tim.h"
#include "ch32v10x.h"

#include "zf_gpio.h"
#include "zf_camera.h"
#include "zf_qtimer.h"

#include "SEEKFREE_UART_7725.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      摄像头DMA初始化
//  @param      dma_ch              DAM通道
//  @param      src_addr            源地址
//  @param      des_addr            目标地址
//  @param      size                数据长度
//  @return     void
//  Sample usage:                   camera_dma_init(DMA1_Channel4, GPIOA->ODR, GPIOC->ODR, 8);
//-------------------------------------------------------------------------------------------------------------------
void camera_dma_init(DMA_Channel_TypeDef* dma_ch,uint32 src_addr, uint32 des_addr, uint32 size)
{
    DMA_InitTypeDef DMA_InitStructure;

    if(DMA1_Channel1 == dma_ch || DMA1_Channel2 == dma_ch || DMA1_Channel3 == dma_ch || DMA1_Channel4 == dma_ch ||
       DMA1_Channel5 == dma_ch || DMA1_Channel6 == dma_ch || DMA1_Channel7 == dma_ch)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1总线初始化
    }
    else if(DMA2_Channel1 == dma_ch || DMA2_Channel2 == dma_ch || DMA2_Channel3 == dma_ch ||  DMA2_Channel4 == dma_ch ||
            DMA2_Channel5 == dma_ch)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMA2总线初始化
    }

    DMA_DeInit(dma_ch);

    //MDA配置初始化
    DMA_InitStructure.DMA_PeripheralBaseAddr = src_addr;                    //源地址
    DMA_InitStructure.DMA_MemoryBaseAddr = des_addr;                        //目标地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //外设作为源
    DMA_InitStructure.DMA_BufferSize = size;                                //传输多少个数据
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址不增加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存地址依次+1
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设每次传输一个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //内存每次传输一个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //非循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //优先级最高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //非内存到内存模式
    DMA_Init(dma_ch, &DMA_InitStructure);

    DMA_ITConfig(dma_ch, DMA_IT_TC, ENABLE);                                //配置DMA传输完成中断
    OV7725_UART_DMA_CH->CNTR = OV7725_UART_SIZE;                            //设置当前DMA传输的剩余数量，向下递减，该寄存器只能在DMA不工作时更改。
    DMA_Cmd(dma_ch, ENABLE);                                                //开启DMA1

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      摄像头GPIO触发定时器初始化
//  @param      timern              定时器通道
//  @param      pin                 引脚号
//  @return     void
//  Sample usage:                   camera_tim_etr_init(TIMER_1, A12);  //定时器ERT触发是固定的引脚禁止随意修改
//-------------------------------------------------------------------------------------------------------------------
void camera_tim_etr_init(TIMERN_enum timern, PIN_enum pin)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    gpio_init(pin, GPI, 0, IN_FLOAT);

    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit((TIM_TypeDef*)(TIMERN[timern]), &TIM_TimeBaseStructure);

    TIM_ETRClockMode1Config((TIM_TypeDef*)(TIMERN[timern]), TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x00);
    TIM_SelectInputTrigger((TIM_TypeDef*)(TIMERN[timern]), TIM_TS_ETRF);
    TIM_SelectSlaveMode((TIM_TypeDef*)(TIMERN[timern]), TIM_SlaveMode_Reset);                   //TIM从模式：触发信号的上升沿重新初始化计数器和触发寄存器的更新事件
    TIM_SelectMasterSlaveMode((TIM_TypeDef*)(TIMERN[timern]), TIM_MasterSlaveMode_Enable);      //启动定时器的被动触发

    TIM_Cmd((TIM_TypeDef*)(TIMERN[timern]), ENABLE);
    TIM_DMACmd((TIM_TypeDef*)(TIMERN[timern]), TIM_DMA_Trigger, ENABLE);                        //使能TIM_DMA
}











