/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_uart
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
#ifndef _zf_uart_h
#define _zf_uart_h

#include "common.h"


//此枚举定义不允许用户修改
//初始化的时候，必须使用同一组引脚例如，UART1_TX_A9和UART1_RX_A10是一组引脚
typedef enum //枚举串口引脚
{

    UART1_TX_A9 = 0x00,     UART1_RX_A10,   //默认引脚
    UART1_TX_B6,            UART1_RX_B7,    //重映射

    UART2_TX_A2 = 0x10,     UART2_RX_A3,    //默认引脚

    UART3_TX_B10 = 0x20,    UART3_RX_B11,   //默认引脚
    UART3_TX_C10,           UART3_RX_C11    //部分重映射

}UARTPIN_enum;


typedef enum //枚举串口引脚
{
    UART_1 = 0,
    UART_2,
    UART_3
}UARTN_enum;



extern const uint32 UARTN[];



void uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin);
void uart_putchar(UARTN_enum uartn, uint8 dat);
void uart_putbuff(UARTN_enum uartn, uint8 *buff, uint32 len);
void uart_putstr(UARTN_enum uartn, const uint8 *str);
void uart_getchar(UARTN_enum uartn, uint8 *dat);
uint8 uart_query(UARTN_enum uartn, uint8 *dat);
void uart_tx_irq(UARTN_enum uartn,uint8 status);
void uart_rx_irq(UARTN_enum uartn,uint8 status);
//void uart_set_handle(UARTN_enum uartn, lpuart_handle_t *handle, lpuart_transfer_callback_t callback, uint8 *tx_buff, uint32 tx_count, uint8 *rx_buff, uint32 rx_count);




#endif
