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
#ifndef _zf_spi_h
#define _zf_spi_h

#include "common.h"


//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
    SPI_1 = 0x00,
    SPI_2 = 0x01
}SPIN_enum;


typedef enum //枚举串口号
{
    //----------SPI1----------
    SPI1_SCK_PA5 = 0x00,
    SPI1_MOSI_PA7,
    SPI1_MISO_PA6,

    //重映射
    SPI1_SCK_PB3 = 0x10,
    SPI1_MOSI_PB5,
    SPI1_MISO_PB4,

    //----------SPI2----------
    SPI2_SCK_PB13 = 0x20,
    SPI2_MOSI_PB15,
    SPI2_MISO_PB14,

    SPI_MISO_NULL
}SPI_PIN_enum;



typedef enum //枚举串口号
{
    FPCLK_DIV_2 = 0x00,
    FPCLK_DIV_4,
    FPCLK_DIV_8,
    FPCLK_DIV_16,
    FPCLK_DIV_32,
    FPCLK_DIV_64,
    FPCLK_DIV_128,
    FPCLK_DIV_256,
}SPI_SPEED_enum;

void spi_init(
        SPIN_enum spi_n,
        SPI_PIN_enum sck_pin,
        SPI_PIN_enum mosi_pin,
        SPI_PIN_enum miso_pin,
        PIN_enum cs_pin,
        uint8 mode,
        SPI_SPEED_enum BaudRatePrescaler);
void spi_mosi(SPIN_enum spi_n, uint8 *modata, uint8 *midata, uint32 len);




#endif
