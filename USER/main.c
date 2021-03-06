/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * 进取号E
 *
 * 郭誉博 卢佰奇 张晓龙
 ********************************************************************************************************************/
//#pragma message("E")

#include "My_Bsp.h"

int main(void)
{
    DisableGlobalIRQ();
    board_init();           //初始化MPU、时钟 、串口
    systick_delay_ms(100);  //延时100ms，等待主板其他外设上电成功
    if(My_Bsp_Init() == 1) {
       Beep_Boot_Success();
    }
    EnableGlobalIRQ(0);

    while(1)
    {
        Camera_Process();

//        SG_Test();          //舵机按键调试

//        Screen_Show_Menu(3);
//        Screen_Show_Temp();
        HMI_Display(1);

//        Wireless_Send();    //匿名上位机显示波形
    }
}
