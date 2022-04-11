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

#include "ch32v10x_rcc.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_usart.h"
#include "ch32v10x_misc.h"
#include "zf_gpio.h"
#include "zf_uart.h"
#include "zf_nvic.h"


const uint32 UARTN[] = {USART1_BASE, USART2_BASE, USART3_BASE ,UART4_BASE};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口引脚初始化
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void uart_gpio_init(UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    if(tx_pin == UART1_TX_A9 && rx_pin == UART1_RX_A10)
    {
        gpio_init(A9, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(A10, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART1_TX_B6)
    {
        gpio_init(B6, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(B7, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART2_TX_A2)
    {
        gpio_init(A2, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(A3, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART3_TX_B10)
    {
        gpio_init(B10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(B11, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART3_TX_C10)
    {
        gpio_init(C10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(C11, GPI, 0, IN_FLOAT);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口初始化
//  @param      uartn       串口通道
//  @param      baud        波特率
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           uart_init(UART_1, 115200, UART1_TX_A9, UART1_RX_A10); //串口1初始化引脚号,TX为A9,RX为A10
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    USART_InitTypeDef USART_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //引脚初始化
    uart_gpio_init(tx_pin, rx_pin);

    if(UART1_TX_B6 == tx_pin && UART1_RX_B7 == rx_pin)                         //引脚重映射
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    if(UART3_TX_C10 == tx_pin && UART3_RX_C11 == rx_pin)
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

    //串口总线使能
    if(UART_1 == uartn) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(UART_2 == uartn) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else if(UART_3 == uartn) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //串口参数配置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //使能串口
    USART_Init((USART_TypeDef*)UARTN[uartn], &USART_InitStructure);
    USART_Cmd((USART_TypeDef*)UARTN[uartn], ENABLE);


}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      uartn       串口通道
//  @param      dat         串口数据
//  @return     void
//  Sample usage:           uart_putchar(UART_1, 0x43);        //串口1发送0x43。
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTN_enum uartn, uint8 dat)
{
    while((((USART_TypeDef*)UARTN[uartn])->STATR & USART_FLAG_TXE)==0);
    ((USART_TypeDef*)UARTN[uartn])->DATAR = dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn       串口通道
//  @param      buff        要发送的数组地址
//  @param      len         数据长度
//  @return     void
//  Sample usage:           uart_putbuff(UART_1, buff, 10);     //串口1发送10个buff数组。
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff(UARTN_enum uartn, uint8 *buff, uint32 len)
{
    while(len--)
        uart_putchar(uartn, *buff++);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn       串口通道
//  @param      str         字符串首地址
//  @return     void
//  Sample usage:           uart_putstr(UART_1, (uint8 *)"12345")   //串口1发送12345这个字符串
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr(UARTN_enum uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口接收中断
//  @param      uartn       串口通道
//  @param      status      使能或者失能
//  @return     void
//  Sample usage:           uart_rx_irq(UART_1, ENABLE);        //打开串口1接收中断
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq(UARTN_enum uartn,uint8 status)
{
    USART_ITConfig(((USART_TypeDef*)UARTN[uartn]), USART_IT_RXNE, ENABLE);
    nvic_init(USART1_IRQn + uartn, 1, 0, status);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口发送中断
//  @param      uartn       串口通道
//  @param      status      使能或者失能
//  @return     void
//  Sample usage:           uart_tx_irq(UART_1, DISABLE);       //关闭串口1发送 中断
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_irq(UARTN_enum uartn,uint8 status)
{
    USART_ITConfig(((USART_TypeDef*)UARTN[uartn]), USART_IT_TXE, ENABLE);
    nvic_init(USART1_IRQn + uartn, 1, 0, status);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（whlie等待）
//  @param      uartn           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     void
//  Sample usage:               uint8 dat; uart_getchar(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar(UARTN_enum uartn, uint8 *dat)
{
    while((((USART_TypeDef*)UARTN[uartn])->STATR & USART_FLAG_RXNE) == 0);
    *dat = (uint16_t)(((USART_TypeDef*)UARTN[uartn])->DATAR & (uint16_t)0x01FF);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（查询接收）
//  @param      uartn           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     uint8           1：接收成功   0：未接收到数据
//  Sample usage:               uint8 dat; uart_query(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query(UARTN_enum uartn, uint8 *dat)
{
    if((((USART_TypeDef*)UARTN[uartn])->STATR & USART_FLAG_RXNE) != 0)
    {
        *dat = (uint16_t)(((USART_TypeDef*)UARTN[uartn])->DATAR & (uint16_t)0x01FF);
        return 1;
    }
    return 0;
}





