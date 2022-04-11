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
#ifndef _zf_systick_h
#define _zf_systick_h

#include "common.h"



//------------------------------------以下宏定义用于SYSTICK延时------------------------------------
#define systick_delay_ms(time)      systick_delay(time * (sys_clk/8000))    //设置SYSTICK延时时间  单位ms
#define systick_delay_us(time)      systick_delay(time * (sys_clk/8000000)) //设置SYSTICK延时时间  单位us


//------------------------------------以下宏定义用于获取当前时间------------------------------------
#define systick_getval_ms()         systick_getval()/(sys_clk/8000)         //获取当前计时时间  单位ms
#define systick_getval_us()         systick_getval()/(sys_clk/8000000)      //获取当前计时时间  单位us
#define systick_getval_ns()         systick_getval()/(sys_clk/8000000000)   //获取当前计时时间  单位ns


void systick_start(void);
void systick_delay(uint64 time);
uint32 systick_getval(void);


//void delay_ms(uint32 count);

#endif
