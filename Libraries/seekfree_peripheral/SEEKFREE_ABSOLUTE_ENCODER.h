/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            绝对式编码器
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看LPC546XX_config.h文件内版本宏定义
 * @Software        IAR 8.3 or MDK 5.24a
 * @Target core     LPC54606J512BD100
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-01-13
 * @note
                    接线定义：
                    ------------------------------------
                    模块管脚                单片机管脚
                    SCK                 查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_SCK_PIN 宏定义
                    MISO                查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_MISO_PIN宏定义
                    MOSI                查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_MOSI_PIN宏定义
                    CS                  查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_PCS_PIN  宏定义
                    ------------------------------------
 ********************************************************************************************************************/

#ifndef _SEEKFREE_ABSOLUTE_ENCODER_H
#define _SEEKFREE_ABSOLUTE_ENCODER_H


#include "headfile.h"
#include "My_Bsp.h"


#define ABS_ENCODER_SPI_NUM         SPI_2
#define ABS_ENCODER_SPI_SCK_PIN     SPI2_SCK_PB13
#define ABS_ENCODER_SPI_MISO_PIN    SPI2_MISO_PB14
#define ABS_ENCODER_SPI_MOSI_PIN    SPI2_MOSI_PB15

//#define ABS_ENCODER_SPI_PC1_PIN     C9
//#define ABS_ENCODER_SPI_PC2_PIN     C8
//#define ABS_ENCODER_SPI_PC3_PIN     B12
//#define ABS_ENCODER_SPI_PC4_PIN     A8

#define ABS_ENCODER_SPI_CSN(x)      ((x == 0)? (GPIO_PIN_RESET(ABS_ENCODER_SPI_PCS_PIN))  :(GPIO_PIN_SET(ABS_ENCODER_SPI_PCS_PIN)))


//角度传感器参数
#define ABS_ENCODER_SPI_W       0x80
#define ABS_ENCODER_SPI_R       0x40


#define ZERO_L_REG              0x00
#define ZERO_H_REG              0x01
#define DIR_REG                 0X09




void encoder_init_spi(PIN_enum cs_pin);
uint16 encoder_angle_spi(PIN_enum cs_pin);
int16 encoder1_speed_spi(PIN_enum cs_pin);
int16 encoder2_speed_spi(PIN_enum cs_pin);
int16 encoder3_speed_spi(PIN_enum cs_pin);
int16 encoder4_speed_spi(PIN_enum cs_pin);


#endif

