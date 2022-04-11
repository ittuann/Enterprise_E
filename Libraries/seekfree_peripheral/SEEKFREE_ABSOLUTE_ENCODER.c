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
                    模块管脚                                    单片机管脚
                    SCK                          查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_SCK_PIN 宏定义
                    MISO                         查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_MISO_PIN宏定义
                    MOSI                 查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_MOSI_PIN宏定义
                    CS                   查看SEEKFREE_ABSOLUTE_ENCODER.H文件ABS_ENCODER_SPI_PCS_PIN  宏定义
                    ------------------------------------
 ********************************************************************************************************************/

#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "zf_systick.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      通过SPI写一个byte,同时读取一个byte
//  @param      byte        发送的数据    
//  @return     uint8       return 返回status状态
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_wr_byte(uint8 byte)
{
    spi_mosi(ABS_ENCODER_SPI_NUM, &byte, &byte, 1);
    return(byte);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      将val写入cmd对应的寄存器地址,同时返回status字节
//  @param      cmd         命令字
//  @param      val         待写入寄存器的数值
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void spi_w_reg_byte(uint8 cmd, uint8 val, PIN_enum cs_pin)
{
    GPIO_PIN_RESET(cs_pin);
    cmd |= ABS_ENCODER_SPI_W;
    spi_wr_byte(cmd);
    spi_wr_byte(val);
    GPIO_PIN_SET(cs_pin);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取cmd所对应的寄存器地址
//  @param      cmd         命令字
//  @param      *val        存储读取的数据地址
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void spi_r_reg_byte(uint8 cmd, uint8 *val, PIN_enum cs_pin)
{
    GPIO_PIN_RESET(cs_pin);
    cmd |= ABS_ENCODER_SPI_R;
    spi_wr_byte(cmd);
    *val = spi_wr_byte(0);
    GPIO_PIN_SET(cs_pin);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      写入一个数据到编码器的寄存器
//  @param      cmd         寄存器地址
//  @param      *val        写入数据的地址
//  @return     uint8       0：程序  1：失败
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 encoder_spi_w_reg_byte(uint8 cmd, uint8 val, PIN_enum cs_pin)
{
    uint8 dat;
    GPIO_PIN_RESET(cs_pin);
    cmd |= ABS_ENCODER_SPI_W;
    spi_wr_byte(cmd);
    spi_wr_byte(val);
    GPIO_PIN_SET(cs_pin);
    systick_delay_us(1);
    GPIO_PIN_RESET(cs_pin);
    dat = spi_wr_byte(0x00);
    spi_wr_byte(0x00);
    GPIO_PIN_SET(cs_pin);
    
    if(val != dat)  return 1;   //写入失败
    return 0;                   //写入成功
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取寄存器
//  @param      cmd         寄存器地址
//  @param      *val        存储读取的数据地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_spi_r_reg_byte(uint8 cmd, uint8 *val, PIN_enum cs_pin)
{
    GPIO_PIN_RESET(cs_pin);
    cmd |= ABS_ENCODER_SPI_R;
    spi_wr_byte(cmd);
    spi_wr_byte(0x00);

    GPIO_PIN_SET(cs_pin);
    systick_delay_us(1);
    GPIO_PIN_RESET(cs_pin);
    *val = spi_wr_byte(0x00);
    spi_wr_byte(0x00);
    GPIO_PIN_SET(cs_pin);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置零偏
//  @param      zero_position  需要设置的零偏
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void set_zero_position_spi(uint16 zero_position, PIN_enum cs_pin)
{    
    zero_position = (uint16)(4096 - zero_position);
    zero_position = zero_position << 4;
    encoder_spi_w_reg_byte(ZERO_L_REG,(uint8)zero_position, cs_pin);//设置零位
    encoder_spi_w_reg_byte(ZERO_H_REG,zero_position>>8, cs_pin);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取当前角度
//  @param      void
//  @return     uint16       返回角度值0-4095 对应0-360°
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 encoder_angle_spi(PIN_enum cs_pin)
{
    uint16 angle;
    angle = 0;
    GPIO_PIN_RESET(cs_pin);
    angle = (uint16)spi_wr_byte(0x00);
    angle <<= 8;                                //存储高八位
    angle |= (uint16)spi_wr_byte(0x00);  //存储低八位
    GPIO_PIN_SET(cs_pin);
    
    return angle>>4;                               //12位精度，因此右移四位
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器1速度获取
//  @param      cs_pin       编码器1的CS引脚
//  @return     uint16       返回角度值-511-511
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int16 encoder1_speed_spi(PIN_enum cs_pin)
{
    static uint16 old_angle ,new_angle;
    uint16 angle_diff;

    //获取本次角度数据
    GPIO_PIN_RESET(cs_pin);
    new_angle = (uint16)spi_wr_byte(0x00);
    new_angle <<= 8;                           //存储高八位
    new_angle |= (uint16)spi_wr_byte(0x00);    //存储低八位
    GPIO_PIN_SET(cs_pin);
    new_angle >>= 6;

    angle_diff = (old_angle - new_angle)&0x3ff;
    old_angle = new_angle;                      //保存这次


    return ((int16)angle_diff - ((angle_diff & 0x200)<<1));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器2速度获取
///  @param      cs_pin      编码器2的CS引脚
//  @return     uint16       返回角度值-511-511
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int16 encoder2_speed_spi(PIN_enum cs_pin)
{
    static uint16 old_angle ,new_angle;
    uint16 angle_diff;

    //获取本次角度数据
    GPIO_PIN_RESET(cs_pin);
    new_angle = (uint16)spi_wr_byte(0x00);
    new_angle <<= 8;                           //存储高八位
    new_angle |= (uint16)spi_wr_byte(0x00);    //存储低八位
    GPIO_PIN_SET(cs_pin);
    new_angle >>= 6;

    angle_diff = (old_angle - new_angle)&0x3ff;
    old_angle = new_angle;                      //保存这次


    return ((int16)angle_diff - ((angle_diff & 0x200)<<1));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器3速度获取
//  @param      cs_pin       编码器3的CS引脚
//  @return     uint16       返回角度值-511-511
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int16 encoder3_speed_spi(PIN_enum cs_pin)
{
    static uint16 old_angle ,new_angle;
    uint16 angle_diff;

    //获取本次角度数据
    GPIO_PIN_RESET(cs_pin);
    new_angle = (uint16)spi_wr_byte(0x00);
    new_angle <<= 8;                           //存储高八位
    new_angle |= (uint16)spi_wr_byte(0x00);    //存储低八位
    GPIO_PIN_SET(cs_pin);
    new_angle >>= 6;

    angle_diff = (old_angle - new_angle)&0x3ff;
    old_angle = new_angle;                      //保存这次


    return ((int16)angle_diff - ((angle_diff & 0x200)<<1));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器4速度获取
//  @param      cs_pin       编码器4的CS引脚
//  @return     uint16       返回角度值-511-511
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int16 encoder4_speed_spi(PIN_enum cs_pin)
{
    static uint16 old_angle ,new_angle;
    uint16 angle_diff;

    //获取本次角度数据
    GPIO_PIN_RESET(cs_pin);
    new_angle = (uint16)spi_wr_byte(0x00);
    new_angle <<= 8;                           //存储高八位
    new_angle |= (uint16)spi_wr_byte(0x00);    //存储低八位
    GPIO_PIN_SET(cs_pin);
    new_angle >>= 6;

    angle_diff = (old_angle - new_angle)&0x3ff;
    old_angle = new_angle;                      //保存这次


    return ((int16)angle_diff - ((angle_diff & 0x200)<<1));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_self5_check(PIN_enum cs_pin)
{
    uint8 val;
    uint8 i;
    uint8 dat[] = {0,0,0,0xC0,0xFF,0x1C};
    do
    {
        for(i = 0; i < 6; i++)
        {
            encoder_spi_w_reg_byte(i+1, dat[i], cs_pin);
            systick_delay_ms(10);
        }

        encoder_spi_r_reg_byte(6, &val, cs_pin);
        systick_delay_ms(10);
        //卡在这里原因有以下几点
        //1 编码器坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
    }while(0x1C != val);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器初始化函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_spi(PIN_enum cs_pin)
{
    //初始化SPI，使用软件控制CS引脚
    spi_init(ABS_ENCODER_SPI_NUM, ABS_ENCODER_SPI_SCK_PIN, ABS_ENCODER_SPI_MOSI_PIN, ABS_ENCODER_SPI_MISO_PIN, cs_pin, 0, FPCLK_DIV_8);

    encoder_self5_check(cs_pin);

    encoder_spi_w_reg_byte(DIR_REG, 0x00, cs_pin);       //设置旋转方向 正转数值变小：0x00   反转数值变大：0x80
    set_zero_position_spi(0, cs_pin);                    //设置零偏
}

