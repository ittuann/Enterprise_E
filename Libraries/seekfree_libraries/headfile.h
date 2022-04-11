/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		headfile
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.26
 * @Target core		NXP RT1021DAG5A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 ********************************************************************************************************************/
 
#ifndef _headfile_h
#define _headfile_h

#include "common.h"
#include <stdint.h>
#include "board.h"

//------CH32V103 SDK头文件
#include "ch32v10x.h"
#include "ch32v10x_adc.h"
#include "ch32v10x_bkp.h"
#include "ch32v10x_crc.h"
#include "ch32v10x_dbgmcu.h"
#include "ch32v10x_dma.h"
#include "ch32v10x_exti.h"
#include "ch32v10x_flash.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_i2c.h"
#include "ch32v10x_iwdg.h"
#include "ch32v10x_pwr.h"
#include "ch32v10x_rcc.h"
#include "ch32v10x_rtc.h"
#include "ch32v10x_spi.h"
#include "ch32v10x_tim.h"
#include "ch32v10x_usart.h"
#include "ch32v10x_wwdg.h"
#include "ch32v10x_usb.h"
#include "ch32v10x_usb_host.h"
#include "ch32v10x_misc.h"


//------逐飞科技单片机外设驱动头文件
#include "zf_adc.h"
#include "zf_camera.h"
#include "zf_flash.h"
#include "zf_gpio.h"
#include "zf_nvic.h"
#include "zf_pwm.h"
#include "zf_spi.h"
#include "zf_systick.h"
#include "zf_qtimer.h"
#include "zf_uart.h"
#include "zf_pit.h"

//------逐飞科技产品驱动头文件
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_PRINTF.h"
#include "SEEKFREE_UART_7725.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"

#endif
