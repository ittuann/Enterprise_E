/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            小钻风
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        IAR 8.3 or MDK 5.28
 * @Taobao          https://seekfree.taobao.com/
 * @date            2019-02-18
 * @note
                                        接线定义：
                    ------------------------------------
                                                模块管脚                          单片机管脚
                        SDA(51的RX)         查看SEEKFREE_UART_7725.h文件中的OV7725_UART_COF_UART_TX宏定义
                        SCL(51的TX)         查看SEEKFREE_UART_7725.h文件中的OV7725_UART_COF_UART_RX宏定义
                                                场中断(VSY)          查看SEEKFREE_UART_7725.h文件中的OV7725_UART_VSYNC_PIN宏定义
                                                行中断(HREF)         不接
                                                像素中断(PCLK)       查看SEEKFREE_UART_7725.h文件中的OV7725_UART_PCLK宏定义
                                                数据口(D0-D7)        查看SEEKFREE_UART_7725.h文件中的OV7725_UART_DATA_PIN宏定义
                    ------------------------------------

                                                默认分辨率是                    160*120
                                                默认FPS              50帧
 ********************************************************************************************************************/

#include "zf_systick.h"
#include "zf_uart.h"
#include "zf_gpio.h"
#include "zf_nvic.h"
#include "zf_camera.h"

#include "SEEKFREE_UART_7725.h"

uint8 ov7725_uart_image_bin[OV7725_UART_H][OV7725_UART_W/8];
//uint8 ov7725_uart_image_dec[OV7725_UART_H][OV7725_UART_W];

#define OV7725_Threshold    0x40

uint16 OV7725_CFG[OV7725_CONFIG_FINISH][2]=
{
    {OV7725_RESERVE,           0},
    {OV7725_CONTRAST,   OV7725_Threshold},
    {OV7725_FPS,             150},
    {OV7725_COL,   OV7725_UART_W},
    {OV7725_ROW,   OV7725_UART_H},

    {OV7725_INIT,              0}
};


uint16 OV7725_GET_CFG[OV7725_CONFIG_FINISH-1][2]=
{
    {OV7725_RESERVE,           0},
    {OV7725_CONTRAST,          0},
    {OV7725_FPS,               0},
    {OV7725_COL,               0},
    {OV7725_ROW,               0}
};

uint8   ov7725_uart_receive[3];
uint8   ov7725_uart_receive_num = 0;
uint8   ov7725_uart_receive_flag = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头串口中断函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       该函数在ISR文件 串口3中断程序被调用
//-------------------------------------------------------------------------------------------------------------------

void ov7725_cof_uart_interrupt(void)
{

    ov7725_uart_receive[ov7725_uart_receive_num] = ((USART_TypeDef*)UARTN[OV7725_UART_COF_UART])->DATAR & 0x01FF;
    ov7725_uart_receive_num++;

    if(1==ov7725_uart_receive_num && 0XA5!=ov7725_uart_receive[0])  ov7725_uart_receive_num = 0;
    if(3 == ov7725_uart_receive_num)
    {
        ov7725_uart_receive_num = 0;
        ov7725_uart_receive_flag = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:                   在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------

void ov7725_uart_vsync(void)
{
    OV7725_UART_DMA_CH->CNTR = OV7725_UART_SIZE;             //设置当前DMA传输的剩余数量，向下递减，该寄存器只能在DMA不工作时更改。
    OV7725_UART_DMA_CH->CFGR |= DMA_CFGR1_EN;                //开启DMA1
}

vuint8 ov7725_uart_finish_flag = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头DMA完成中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:                   在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_dma(void)
{
    ov7725_uart_finish_flag = 1;                             //摄像头数据采集置1
    OV7725_UART_DMA_CH->CFGR &= (uint16)(~DMA_CFGR1_EN);     //关闭DMA1
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头DMA初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_dma_init(void)
{
    uint8 data_pin,num;

    //摄像头数据引脚初始化
    data_pin = (uint8)(((OV7725_UART_DATA_PIN - (GPIOA_BASE+8)) >> 10) << 5) +
                (((OV7725_UART_DATA_PIN - (GPIOA_BASE+8)) & 0x0F) << 3);

    for(num=7; num>0; num--)
    {
        gpio_init((PIN_enum)(num + data_pin), GPI, 0, IN_FLOAT);
    }


 	//DMA摄像头初始化
 	camera_dma_init(OV7725_UART_DMA_CH,(uint32)OV7725_UART_DATA_PIN, (uint32)camera_buffer_addr, OV7725_UART_SIZE);
    nvic_init(OV7725_UART_DMA_IRQN, 0, 0, ENABLE);          //中断配置
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头定时器捕获触发初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_tim1_etr_init(void)
{
    camera_tim_etr_init(TIMER_1, OV7725_UART_PCLK);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头场中断初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_vsync_init(void)
{   
    gpio_interrupt_init(OV7725_UART_VSYNC_PIN, FALLING, IN_PULLUP);
    nvic_init(OV7725_UART_VSYNC_IRQN, 1, 0, ENABLE);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风对摄像头内部寄存器进行写操作
//  @param      uartn   摄像头内部寄存器地址
//  @param      buff    需要写入的数据
//  @return     void
//  @since      v1.0
//  Sample usage:       调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void ov7725_set_all_config(UARTN_enum uartn, uint16 buff[OV7725_CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];

    ov7725_uart_receive_flag = 0;

    for(i=0; i<OV7725_CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp           = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;

        uart_putbuff(uartn, send_buffer, 4);
        systick_delay_ms(10);
    }


    while(!ov7725_uart_receive_flag)    systick_delay_ms(1);
    ov7725_uart_receive_flag = 0;
    while((0xff != ov7725_uart_receive[1]) || (0xff != ov7725_uart_receive[2]))systick_delay_ms(1);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风获取摄像头内部配置信息
//  @param      uartn   摄像头串口
//  @param      buff    需要读取的数据
//  @return     void
//  @since      v1.0
//  Sample usage:       调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void ov7725_get_all_config(UARTN_enum uartn, uint16 buff[OV7725_CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];

    for(i=0; i<OV7725_CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = OV7725_GET_STATUS;
        temp           = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;

        uart_putbuff(uartn, send_buffer,4);

        systick_delay_ms(10);

        while(!ov7725_uart_receive_flag)    systick_delay_ms(1);

        ov7725_uart_receive_flag = 0;

        buff[i][1] = ov7725_uart_receive[1]<<8 | ov7725_uart_receive[2];
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头固件版本
//  @param      uartn               选择使用的串口
//  @return     (uint16)camera_id   返回摄像头ID
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
//-------------------------------------------------------------------------------------------------------------------
uint16 ov7725_uart_get_id(UARTN_enum uartn)
{
    uint16 temp;
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = OV7725_GET_WHO_AM_I;
    temp           = 0;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(uartn, send_buffer, 4);

    while(!ov7725_uart_receive_flag)    systick_delay_ms(1);
    ov7725_uart_receive_flag = 0;

    return ((uint16)(ov7725_uart_receive[1]<<8) | ov7725_uart_receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头固件版本
//  @param      uartn       选择使用的串口
//  @return     uint16      版本号
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
//-------------------------------------------------------------------------------------------------------------------
uint16 ov7725_get_version(UARTN_enum uartn)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = OV7725_GET_STATUS;
    temp           = OV7725_GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(uartn, send_buffer, 4);

    while(!ov7725_uart_receive_flag)    systick_delay_ms(1);
    ov7725_uart_receive_flag = 0;

    return ((uint16)(ov7725_uart_receive[1]<<8) | ov7725_uart_receive[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      新版小钻风初始化
//  @return     uint8       初始化结果
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_uart_init(void)
{
    camera_type = CAMERA_BIN_UART;//设置连接摄像头类型
    camera_buffer_addr = ov7725_uart_image_bin[0];
    uart_init(OV7725_UART_COF_UART, 9600, OV7725_UART_COF_UART_TX, OV7725_UART_COF_UART_RX);
    uart_rx_irq(OV7725_UART_COF_UART, ENABLE);

    EnableGlobalIRQ(0);

    //设置所有参数
    ov7725_uart_receive_flag = 0;
    ov7725_set_all_config(OV7725_UART_COF_UART, OV7725_CFG);
    //获取所有参数
    ov7725_uart_receive_flag = 0;
    ov7725_get_all_config(OV7725_UART_COF_UART, OV7725_GET_CFG);

    DisableGlobalIRQ();

    //DMA初始化
    ov7725_uart_dma_init();
    //GPIO触发定时器初始化,PLCK引脚初始化
    ov7725_uart_tim1_etr_init();
    //VSYNC初始化
    ov7725_uart_vsync_init();

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头未解压图像发送至上位机查看图像
//  @param      *imgaddr            压缩图像数据地址
//  @param      *imgsize            图像大小(直接填写OV7725_UART_SIZE)
//  @return     void
//  @since      v1.0
//  Sample usage:                   调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_7725(UARTN_enum uartn, void *imgaddr, uint32 imgsize)
{
    uint32 i;
    uart_putchar(uartn,0x00);
    uart_putchar(uartn,0xff);
    uart_putchar(uartn,0x01);
    uart_putchar(uartn,0x01);
    for(i=0; i<(imgsize); i++)
    {
        uart_putchar(uartn,((uint8 *)(imgaddr))[i]);
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头数据解压函数
//  @param      *data1              源地址
//  @param      *data2              目的地址
//  @return     void
//  @since      v1.0
//  Sample usage:                   Image_Decompression(da1,dat2[0]);//将一维数组dat1的内容解压到二维数组dat2里.
//-------------------------------------------------------------------------------------------------------------------
void Image_Decompression(uint8 *data1,uint8 *data2)
{
    uint8  temp[2] = {0,255};
    uint16 lenth = OV7725_UART_SIZE;
    uint8  i = 8;


    while(lenth--)
    {
        i = 8;
        while(i--)
        {
            *data2++ = temp[(*data1 >> i) & 0x01];
        }
        data1++;
    }
}






