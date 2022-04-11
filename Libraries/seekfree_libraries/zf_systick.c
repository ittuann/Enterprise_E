/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_systick
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
#include "zf_systick.h"
#include "core_riscv.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      嘀嗒定时器延时
//  @param      time                us
//  @return     void
//  Sample usage:                   内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(uint64 time)
{
    SysTick->CTLR = 0;
    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CNTH0 = 0;
    SysTick->CNTH1 = 0;
    SysTick->CNTH2 = 0;
    SysTick->CNTH3 = 0;
    SysTick->CTLR = 1;          //启动系统计数器 systick（HCLK/8 时基） us

    while((*(volatile uint32*)0xE000F004) <= time);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick定时器启动
//  @param      void
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void systick_start(void)
{
    SysTick->CTLR = 0;
    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CTLR = 1;          //启动系统计数器 systick（HCLK/8 时基） us
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获得当前System tick timer的值
//  @return     返回当前System tick timer的值
//  Sample usage:               uint32 tim = systick_getval();
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(void)
{
    return (*(volatile uint32*)0xE000F004);
}



//vuint16 zf_delay_ms = sys_clk / 8000;
////----------------------------------------------
////软件延时
////-------------------------------------------
//void delay_ms(uint32 count)
//{
//    uint16 i;
//    do {
//            i = zf_delay_ms;
//            //i = sys_clk/6000;//参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
//            while(--i);
//       }while(--count);
//
//}
