/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_gpio
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

 
#ifndef _zf_gpio_h
#define _zf_gpio_h

#include "common.h"




#define GPIO_PIN_CONFIG (SPEED_50MHZ | OUT_PP)     //宏定义GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define GPIO_INT_CONFIG (IN_PULLUP)                 //宏定义GPIO中断引脚的默认配置，便于初始化GPIO中断时快速填写参数，如果需要其他参数可自行修改


typedef enum    // 枚举触发方式
{
    RISING = 0x08,
    FALLING = 0x0C,
    BOTH = 0x10
}TRIGGER_enum;

typedef enum    //枚举GPIO速度
{
    SPEED_10MHZ = 0x100,
    SPEED_2MHZ = 0x200,
    SPEED_50MHZ = 0x300
}GPIOSPEED_enum;


typedef enum    //枚举GPIO配置
{
    IN_AIN = 0x00,              //模拟输入模式
    IN_FLOAT = 0x04,            //浮空输入模式
    IN_PULLDOWN = 0x28,         //下拉输入
    IN_PULLUP = 0x48,           //上拉输入

    OUT_PP = 0x10,              //通用推挽输出模式
    OUT_OD = 0x14,              //通用开漏输出模式
    OUT_AF_PP = 0x18,           //复用功能推挽输出模式
    OUT_AF_OD = 0x1C,           //复用功能开漏输出模式

}GPIOMODE_enum;


typedef enum    //枚举端口方向
{
    GPI = 0, //定义管脚输入方向
    GPO = 1, //定义管脚输出方向
}GPIODIR_enum;



extern const uint32 GPION[4];

//获取GPIO地址
//X = 0代表A=端口 , X = 1代表B端口 ,依次类推
//X = 0,Y = 0代表 A0端口地址 ， X = 0,Y = 1代表 A8端口地址
#define GPIO_IDR_ADDR(X,Y)  (0x40010808 + X*0x400 + Y)

//GPIO复位
#define GPIO_PIN_RESET(x)   ((GPIO_TypeDef *)(GPION[(uint8)((x & 0xE0) >> 5)]))->BCR  = (uint16)(1 << (x & 0x1F))
//GPIO置位
#define GPIO_PIN_SET(x)     ((GPIO_TypeDef *)(GPION[(uint8)((x & 0xE0) >> 5)]))->BSHR  = (uint16)(1 << (x & 0x1F))
//中断标志位清除   
#define CLEAR_GPIO_FLAG(pin) GPIO_ClearPinsInterruptFlags(PORTPTR[pin>>5], 1<<(pin&0x1f));

void gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, uint32 pinconf);
void gpio_set(PIN_enum pin, uint8 dat);
uint8 gpio_get(PIN_enum pin);
void gpio_toggle(PIN_enum pin);
void gpio_interrupt_init(PIN_enum pin, TRIGGER_enum trigger, uint32 pinconf);




#endif
