/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_spi
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
#include "ch32v10x_spi.h"
#include "ch32v10x_rcc.h"
#include "ch32v10x_gpio.h"

#include "zf_gpio.h"
#include "zf_spi.h"
#include "stddef.h"



const uint32 SPIN[2] = {SPI1_BASE, SPI2_BASE};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI引脚初始化
//  @param      sck_pin     SCK引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      mosi_pin    MOSI引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      miso_pin    MISO引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      cs_pin      片选引脚(可选择范围由common.h内PIN_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void spi_gpio_init(SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, PIN_enum cs_pin)
{
    if(SPI1_SCK_PA5 == sck_pin) gpio_init(A5, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI1_SCK_PB3 == sck_pin) gpio_init(B3, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI2_SCK_PB13 == sck_pin) gpio_init(B13, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else assert(0);         //无此参数进入断言

    if(SPI1_MOSI_PA7 == mosi_pin) gpio_init(A7, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI1_MOSI_PB5 == mosi_pin) gpio_init(B5, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI2_MOSI_PB15 == mosi_pin) gpio_init(B15, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else assert(0);         //无此参数进入断言

    if(SPI1_MISO_PA6 == miso_pin) gpio_init(A6, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI1_MISO_PB4 == miso_pin) gpio_init(B4, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI2_MISO_PB14 == miso_pin) gpio_init(B14, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI_MISO_NULL == miso_pin)  miso_pin = SPI_MISO_NULL;
    else  assert(0);         //无此参数进入断言


    gpio_init(cs_pin, GPO, 1, SPEED_50MHZ|OUT_PP);

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI初始化
//  @param      spi_n               选择SPI模块   (SPI_1-SPI_2)
//  @param      sck_pin             SCK引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      mosi_pin            MOSI引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      miso_pin            MISO引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      cs_pin              片选引脚(可选择范围由common.h内PIN_enum枚举值确定) 使用软件CS引脚
//  @param      mode                SPI模式 0：CPOL=0 CPHA=0    1：CPOL=0 CPHA=1   2：CPOL=1 CPHA=0   3：CPOL=1 CPHA=1 //具体细节可自行百度
//  @param      BaudRatePrescaler   SPI的速率设置为系统时钟的多少分频
//  @return     void
//  Sample usage:               spi_init(SPI_1, SPI1_SCK_PA5, SPI1_MOSI_PA7, SPI1_MISO_PA6, A0, 0, FPCLK_DIV_2);//硬件SPI初始化  模式0 波特率为系统时钟频率/2
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为50000
//-------------------------------------------------------------------------------------------------------------------
void spi_init(
        SPIN_enum spi_n,
        SPI_PIN_enum sck_pin,
        SPI_PIN_enum mosi_pin,
        SPI_PIN_enum miso_pin,
        PIN_enum cs_pin,
        uint8 mode,
        SPI_SPEED_enum BaudRatePrescaler)
{

    SPI_InitTypeDef  SPI_InitStructure;

    if(SPI_1 == spi_n)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);                //SPI1时钟使能，复用总线使能
    }
    else if(SPI_2 == spi_n)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                //SPI2时钟使能
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                //复用总线使能
    }

    spi_gpio_init(sck_pin, mosi_pin, miso_pin, cs_pin);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       //设置SPI的数据大小:SPI发送接收8位帧结构

    switch(mode)
    {
        case 0:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      //串行同步时钟的空闲状态为低电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        }break;
        case 1:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      //串行同步时钟的空闲状态为低电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
        }break;
        case 2:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     //串行同步时钟的空闲状态为高电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        }break;
        case 3:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     //串行同步时钟的空闲状态为高电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
        }break;
    }

    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               //主机模式，使用软件控制NSS引脚
    SPI_InitStructure.SPI_BaudRatePrescaler = BaudRatePrescaler << 3;       //定义波特率预分频的值
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                //CRC值计算的多项式
    SPI_Init((SPI_TypeDef *)(SPIN[spi_n]), &SPI_InitStructure);             //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    if(SPI1_SCK_PB3 == sck_pin) GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    SPI_Cmd(((SPI_TypeDef *)(SPIN[spi_n])), ENABLE);                         //使能SPI外设

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      spi_n           选择SPI模块   (SPI_1-SPI_2)
//  @param      modata          发送的数据缓冲区地址
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      len             发送的字节数
//  @return     void
//  @since      v2.0
//  Sample usage:               spi_mosi(SPI_1, buf, buf, 1);    //发送modata的内容，并接收到midata里，长度为1字节
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi(SPIN_enum spi_n, uint8 *modata, uint8 *midata, uint32 len)
{
   // while((((SPI_TypeDef *)(SPIN[spi_n]))->STATR & SPI_I2S_FLAG_TXE) == RESET);
    while(len--)
    {
        ((SPI_TypeDef *)(SPIN[spi_n]))->DATAR = *(modata++);
        while((((SPI_TypeDef *)(SPIN[spi_n]))->STATR & SPI_I2S_FLAG_TXE) == RESET);
        if(NULL != midata)
        {
            while((((SPI_TypeDef *)(SPIN[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
            *(midata++) = ((SPI_TypeDef *)(SPIN[spi_n]))->DATAR;
        }
    }
}

