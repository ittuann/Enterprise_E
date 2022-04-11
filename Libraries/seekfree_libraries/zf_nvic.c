/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_nvic
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

#include "zf_nvic.h"
#include "ch32v10x_misc.h"
#include "board.h"





//-------------------------------------------------------------------------------------------------------------------
//  @brief      中断初始化
//  @param      irqn                        中断号，可以查看CH32V10x.h文件中的IRQn_Type枚举体定义
//  @param      preemption_priority         抢占优先级值越小，优先级越高 (可选参数最高0，最低1)
//  @param      sub_priority                次优先级值越小，优先级越高     (可选参数0-7，最高0，最低7)
//  @param      status                      使能或者失能
//  @return     void
//  Sample usage:                           nvic_init(EXTI0_IRQn,0, 0, ENABLE); //外部中断0使能,抢占优先级最高，次优先级最高。
//-------------------------------------------------------------------------------------------------------------------
void nvic_init(IRQn_Type irqn,uint8 preemption_priority, uint8 sub_priority,uint8 status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                                  //中断号设置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preemption_priority;   //抢占优先级值越小，优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;               //次优先级值越小，优先级越高
    NVIC_InitStructure.NVIC_IRQChannelCmd = status;                             //使能
    NVIC_Init(&NVIC_InitStructure);

    //保存状态值
    irq_status.ienr0 |= (PFIC->ISR[0] & 0xFFFFF000);    //[31:12]
    irq_status.ienr1 |= (PFIC->ISR[1] & 0x0FFFFFFF);    //[27:0]
    if(irq_status.flag == 1)
    {
        close_all_irq();
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭所有中断，
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void close_all_irq(void)
{
    //写1，清空状态
    PFIC->IRER[0] = 0xffffffff;
    PFIC->IRER[1] = 0xffffffff;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      恢复之前关闭的中断值
//  @param      irq_status       当前保留中断值
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void sys_recover_irq(void)
{
    PFIC->IENR[0] = irq_status.ienr0;
    PFIC->IENR[1] = irq_status.ienr1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭总中断
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 DisableGlobalIRQ(void)
{
    uint8 state;
    irq_status.flag = 1;
    if((irq_status.ienr0 == 0) && (irq_status.ienr1 == 0))   state = 0;  //没有就返回0
    else state = 1;  //有中断就返回1
    close_all_irq();
    return state;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开总中断
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void EnableGlobalIRQ(uint8 state)
{
    if(state == 0)
    {
        irq_status.flag = 0;
        sys_recover_irq();
    }
}

