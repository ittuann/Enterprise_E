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

#include "ch32v10x_rcc.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_exti.h"
#include "zf_gpio.h"

#define GPIOx_BASE      GPIOA_BASE//偏移0x0400

const uint32 GPION[4] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO初始化
//  @param      pin         选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
//  @param      dir         引脚的方向   输出：GPO   输入：GPI
//  @param      dat         引脚初始化时设置的电平状态，输出时有效 0：低电平 1：高电平
//  @param      pinconf     引脚配置（可设置参数由zf_gpio.h文件内GPIOSPEED_enum与GPIOMODE_enum枚举值确定，多个条件使用 | 相或）
//  @return     void
//  Sample usage:           gpio_init(D0, GPO, 1, GPIO_PIN_CONFIG);//D0初始化为GPIO功能、输出模式、输出高电平、速度100MHZ 推挽输出
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, uint32 pinconf)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8 io_group = (pin & 0xE0) >> 5;
    uint8 io_pin = pin & 0x1F;


    if(PIN_NULL == pin) return;
    //GPIO总线使能
    if(0 == (pin >> 5))      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(1 == (pin >> 5)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(2 == (pin >> 5)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(3 == (pin >> 5)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(4 == (pin >> 5)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(5 == (pin >> 5)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(6 == (pin >> 5)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);


    //设置引脚号
    GPIO_InitStructure.GPIO_Pin = (uint16)(1 << io_pin);

    if(GPO == dir)
    {
        //只有输出需要设置速度
        if((uint16)pinconf >> 8 == 0) GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)(GPIO_Speed_50MHz);  //防止没有填写这个参数,默认设置50Mhz
        else GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)((uint16)pinconf >> 8);                       //已经填写了该参数。根据值来设置
    }

    //设置模式
    GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)((uint16)pinconf & 0xff);
    GPIO_Init((GPIO_TypeDef *)(GPION[io_group]), &GPIO_InitStructure);

    if(GPO == dir)
    {
        //只有输出需要设置引脚状态
        GPIO_WriteBit((GPIO_TypeDef *)(GPION[io_group]), (uint16)(1 << io_pin), dat);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO输出设置
//  @param      pin         引脚号选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
//  @param      dat         引脚的电平状态，输出时有效 0：低电平 1：高电平
//  @return     void
//  Sample usage:           gpio_set(D0, 0);//D0输出低电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(PIN_enum pin, uint8 dat)
{
    uint8 io_group = (pin & 0xE0) >> 5;
    uint8 io_pin = pin & 0x1F;

    if (dat == 1)
         ((GPIO_TypeDef *)(GPION[io_group]))->BSHR = 1 << io_pin;
    else
         ((GPIO_TypeDef *)(GPION[io_group]))->BCR = 1 << io_pin;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO状态获取
//  @param      pin         选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
//  @return     uint8       0：低电平 1：高电平
//  Sample usage:           uint8 status = gpio_get(D0);//获取D0引脚电平
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get(PIN_enum pin)
{
    uint8 io_group = (pin & 0xE0) >> 5;
    uint8 io_pin = pin & 0x1F;

    if((((GPIO_TypeDef *)(GPION[io_group]))->INDR & (1 << io_pin)) != 0)
    {
        return 1;
    }
    return 0;


}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO 翻转
//  @param      pin         选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
//  @return     void        
//  Sample usage:           gpio_toggle(D5);//D5引脚电平翻转
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle(PIN_enum pin)
{
    uint8 io_group = (pin & 0xE0) >> 5;
    uint8 io_pin = pin & 0x1F;

    ((GPIO_TypeDef *)(GPION[io_group]))->OUTDR ^= (uint16)(1 << io_pin);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO 外部中断初始化
//  @param      pin         选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
//  @param      trigger     中断触发方式 选择范围参见h文件TRIGGER_enum枚举
//  @return     void
//  Sample usage:           gpio_interrupt_init(D0, RISING, GPIO_INT_CONFIG);//D0上升沿中断
//-------------------------------------------------------------------------------------------------------------------
void gpio_interrupt_init(PIN_enum pin,TRIGGER_enum trigger, uint32 pinconf)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    gpio_init(pin, GPI, 0, pinconf);
    GPIO_EXTILineConfig(pin >> 5, pin & 0x1F);              //引脚中断号配置
    EXTI_InitStructure.EXTI_Line = 1 << (pin & 0x1F);       //引脚号配置
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //触发模式
    EXTI_InitStructure.EXTI_Trigger = trigger;              //触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //使能或者失能
    EXTI_Init(&EXTI_InitStructure);
}


