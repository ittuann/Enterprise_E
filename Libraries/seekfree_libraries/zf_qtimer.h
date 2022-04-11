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

#ifndef _zf_timer_h
#define _zf_timer_h

#include "common.h"


typedef enum    //TIMER引脚枚举
{
    TIMER1_CHA_A8,
    TIMER1_CHB_A9,
    TIMER2_CHA_A0,
    TIMER2_CHB_A1,
    TIMER2_CHA_A15,              //完全映射
    TIMER2_CHB_B3,               //完全映射

    TIMER3_CHA_A6,
    TIMER3_CHB_A7,

    TIMER3_CHA_C6,               //完全映射
    TIMER3_CHB_C7,               //完全映射
    TIMER3_CHA_B4,               //部分映射
    TIMER3_CHB_B5,               //部分映射

    TIMER4_CHA_B6,
    TIMER4_CHB_B7,

}TIMER_PIN_enum;

//此枚举定义不允许用户修改
typedef enum
{
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
}TIMERN_enum;

extern const uint32 TIMERN[] ;

void timer_quad_init(TIMERN_enum timern, TIMER_PIN_enum phaseA, TIMER_PIN_enum phaseB);
int16 timer_quad_get(TIMERN_enum timern);
void timer_quad_clear(TIMERN_enum timern);




#endif
