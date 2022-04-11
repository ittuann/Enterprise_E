/*
 *  screen.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "screen.h"
#include "SEEKFREE_18TFT.h"

//  @param      x              0-159
//  @param      y              0-7
#define PageMax 4       //屏幕最大页数
#define HMI_RowMax 6    //HMI最大行数

uint8 Select_Screen_Row = 4;            //HMI选择当前行数
uint8 HMI_Finish_Flag = 0;              //HMI结束标志位
int16 Select_Screen_Data = 0;           //HMI选择当前行数据
uint8 Screen_Clear_0nce_Falg = 0;       //暂存屏幕清屏一次标志位
uint8 Screen_Show_Menu_StartMod = 1;    //暂存初始状态选择标志位
uint8 Char_WRITE_FLAG = 1;              //字符串写入标志位
uint8 Delay_WRITE_FLAG = 0;             //延时写入标志位
uint8 Screen_Fast_Close_Flag = 1;       //高速关闭屏幕标志位
uint8 Screen_Close_Flag = 1;            //按键关闭屏幕标志位
int8 KEY_Scan_Menu_Now = 0;             //暂存当前按键返回值
int8 KEY_Scan_Menu_Temp = 0;            //暂存按键累加值


/*
 * @name:void HMI_Display(uint8 startmod)
 * @function: HMI_Display
 * @param: none
 * @return: none
 */
void HMI_Display(uint8 startmod)
{
    if (HMI_Finish_Flag == 0) {
        KEY_Scan_Menu_Now = KEY_Scan_Screen();

        if (KEY_Scan_Menu_Now != 0) {
//            Beep_Buzzing(25, 1);
        }

        if(KEY_Scan_Menu_Now  == 10) {
            Select_Screen_Data = 0;
            Select_Screen_Row += 1;
            KEY_Scan_Menu_Now = 0;
        }   //修改下一行

        /* 显示内容 */
        if (Photo_Finsh_Flag == 1) {
            lcd_displayimage032(img[0], OV7725_UART_W, OV7725_UART_H);
            midline_show();        //中线辅助显示
        }   //摄像头

        /* 调试用 */
//        if (ICM20602_Offset_Finished) {
            lcd_showint16(100, 0, ICM_Offset.gyro.z);
            lcd_showint32(100, 1, offect_temp6, 5);
            lcd_showuint8(100, 2, Offect_Count);
//            lcd_showint16(100, 3, (int16)ICM_Treated.gyro.z);
//        }   //检查陀螺仪零偏
//        lcd_showint8(80 , 4, (int8)AngleErr);
//        lcd_showint16(80, 5, (int16)ELCErr);
//        lcd_showint8(80 , 6, (int8)FusionErr);
//        lcd_showint16(60, 7, AngleOutPut);

        if (Char_WRITE_FLAG == 1) {
            lcd_showstr(0 , 4, "1OG:");
            lcd_showstr(80, 4, "2OD:");
            lcd_showstr(0 , 5, "3Gr:");
            lcd_showstr(0 , 6, "4S :");
            lcd_showstr(80, 6, "5SX:");
            lcd_showstr(0 , 7, "6SC:");
            Char_WRITE_FLAG = 0;
        }
        lcd_showuint8(35, 4, Out_Garage);       //是否出库 0是出1是不出(出库完成
        lcd_showuint8(115, 4, Out_Garage_Dir);  //出库方向 1是左2是右
        lcd_showuint8(35, 5, Gear_Flag);        //是否加速 0是不加1是加
        lcd_showuint8(35, 6, Speed_Set_y_Flag); //正常速度档位 1-40;2-45;3-50
        lcd_showuint8(115, 6, Speed_Set_x_Flag);//横向速度档位 1-35;2-40
        lcd_showuint8(35, 7, Speed_Set_c_Flag); //环内速度档位 1-45;2-50

        if (Select_Screen_Row == 4) {
            Select_Screen_Data = Out_Garage;
            Select_Screen_Data += KEY_Scan_Menu_Now;
            Out_Garage = Select_Screen_Data;
            Out_Garage = (Out_Garage <= 0) ? 0 : ( (Out_Garage >= 1) ? 1 : Out_Garage);
        }   //修改当前第一行 (初始为第4行
        if (Select_Screen_Row == 5) {
            Select_Screen_Data = Out_Garage_Dir;
            Select_Screen_Data += KEY_Scan_Menu_Now;
            Out_Garage_Dir = Select_Screen_Data;
            Out_Garage_Dir = (Out_Garage_Dir <= 1) ? 1 : ( (Out_Garage_Dir >= 2) ? 2 : Out_Garage_Dir);
        }
        if (Select_Screen_Row == 6) {
            Select_Screen_Data = Gear_Flag;
            Select_Screen_Data += KEY_Scan_Menu_Now;
            Gear_Flag = Select_Screen_Data;
            Gear_Flag = (Gear_Flag <= 0) ? 0 : ( (Gear_Flag >= 3) ? 3 : Gear_Flag);
        }
        if (Select_Screen_Row == 7) {
            Select_Screen_Data = Speed_Set_y_Flag;
            Select_Screen_Data += KEY_Scan_Menu_Now;
            Speed_Set_y_Flag = Select_Screen_Data;
            Speed_Set_y_Flag = (Speed_Set_y_Flag <= 1) ? 1 : ( (Speed_Set_y_Flag >= 7) ? 7 : Speed_Set_y_Flag);
        }
        if (Select_Screen_Row == 8) {
            Select_Screen_Data = Speed_Set_x_Flag;
            Select_Screen_Data += KEY_Scan_Menu_Now;
            Speed_Set_x_Flag = Select_Screen_Data;
            Speed_Set_x_Flag = (Speed_Set_x_Flag <= 1) ? 1 : ( (Speed_Set_x_Flag >= 6) ? 6 : Speed_Set_x_Flag);
        }
        if (Select_Screen_Row == 9) {
            Select_Screen_Data = Speed_Set_c_Flag;
            Select_Screen_Data += KEY_Scan_Menu_Now;
            Speed_Set_c_Flag = Select_Screen_Data;
            Speed_Set_c_Flag = (Speed_Set_c_Flag <= 1) ? 1 : ( (Speed_Set_c_Flag >= 6) ? 6 : Speed_Set_c_Flag);
        }

        /* 完成修改 */
        if (Select_Screen_Row > (HMI_RowMax + 3)) {
//            Beep_Buzzing(50, 1);
            HMI_Finish_Flag = 2;
        }

        lcd_showint8(120, 7, (Select_Screen_Row - 3));
    }

    if (HMI_Finish_Flag == 2) {
        lcd_clear(GREEN);
        Char_WRITE_FLAG = 1;
        HMI_Finish_Flag = 1;
    }   //完成修改并清除当前屏幕显示内容

    if (HMI_Finish_Flag) {
//        Screen_Show_Menu(startmod);
        Screen_Show_Temp();
    }   //切换至正常显示
}

/*
 * @name:void Screen_Show_Menu(void);
 * @function:循环屏幕显示菜单
 * @param:none
 * @return:none
 */
void Screen_Show_Menu(uint8 startmod)
{
    KEY_Scan_Menu_Now = KEY_Scan_Screen();

    if(KEY_Scan_Menu_Now  == 10) {
        Screen_Close_Flag = !Screen_Close_Flag;
        Screen_Clear_0nce_Falg = 1;
        Char_WRITE_FLAG = 1;
        KEY_Scan_Menu_Now = 0;
        Beep_Buzzing(25, 1);
    }   //按下中键开关屏幕

    if (Screen_Close_Flag) {
        KEY_Scan_Menu_Temp += KEY_Scan_Menu_Now;

        if (Screen_Show_Menu_StartMod == 1) {
            KEY_Scan_Menu_Temp = startmod;
            Screen_Show_Menu_StartMod = 0;
        }   //选择初始显示页数

        if (KEY_Scan_Menu_Now != 0) {
            lcd_clear(WHITE);
            Char_WRITE_FLAG = 1;
            Delay_WRITE_FLAG = 1;
            Beep_Buzzing(25, 1);
        }   //切换页数时清屏

        if(KEY_Scan_Menu_Temp >  PageMax)   KEY_Scan_Menu_Temp = 0;
        if(KEY_Scan_Menu_Temp <= -1)        KEY_Scan_Menu_Temp = PageMax;   //循环显示

        switch(KEY_Scan_Menu_Temp) {
            case(0):    Screen_Show_Debug();    break;
            case(1):    Screen_Show_Camera();   break;
            case(2):    Screen_Show_ELC();      break;
            case(3):    Screen_Show_Motor();    break;
            case(4):    Screen_Show_Gyro();     break;

            default:    lcd_showint8(0, 0, KEY_Scan_Menu_Temp);
        }   //切换屏幕页数

        lcd_showint8(120, 7, KEY_Scan_Menu_Temp);   //显示当前页数
    }

    if (Screen_Clear_0nce_Falg == 1) {
        lcd_clear(WHITE);
        Char_WRITE_FLAG = 1;
        Screen_Clear_0nce_Falg = 0;
    }   //清屏一次
}

void Screen_Show_Temp(void)
{
    KEY_Scan_Menu_Now = KEY_Scan_Screen();

    if(KEY_Scan_Menu_Now  == 10) {
        Screen_Close_Flag = !Screen_Close_Flag;
        Screen_Clear_0nce_Falg = 1;
        Char_WRITE_FLAG = 1;
        KEY_Scan_Menu_Now = 0;
        Beep_Buzzing(25, 1);
    }   //按下中键开关屏幕

    if (!Screen_Close_Flag) {
        //显示内容
        if (Photo_Finsh_Flag) {
            lcd_displayimage7725(ov7725_uart_image_bin[0], OV7725_UART_W, OV7725_UART_H);
            midline_show();
        }

        lcd_showuint8(90, 0, valid_row);
        lcd_showuint16(90, 1, TFmin_Dist);
//        lcd_showint8(0 , 4, Start_Part);
//        lcd_showint8(90, 4, End_Part);

        lcd_showstr(0 , 4, "LL:");  //[0]
        lcd_showstr(80, 4, "RR:");  //[3]
        lcd_showstr(0 , 5, "L:");   //[1]
        lcd_showstr(80, 5, "R:");   //[2]
        lcd_showuint16(25 , 4, ELC_Slide[0][0]);
        lcd_showuint16(105, 4, ELC_Slide[3][0]);
        lcd_showuint16(25 , 5, ELC_Slide[1][0]);
        lcd_showuint16(105, 5, ELC_Slide[2][0]);
//
        lcd_showint16(0 , 6, (int16)FusionErr);
        lcd_showint16(90, 6, AngleOutPut);
//        lcd_showint16(90, 6, CarSpeed_a);
        lcd_showint16(0 , 7, FrontErr);
        lcd_showint16(90, 7, speedline_err_abs);

//        lcd_showfloat(80, 4, ICM_Treated.gyro.z, 3, 3);
//        lcd_showint32(0 , 5, (int32)Attitude_Angel_X, 6);
//        lcd_showint16(0 , 6, Attitude_Yaw);
    }

    if (Screen_Clear_0nce_Falg == 1) {
        lcd_clear(WHITE);
        Char_WRITE_FLAG = 1;
        Screen_Clear_0nce_Falg = 0;
    }   //清屏一次
}

/*
 * @name: void Screen_Show_Menu_SpeedBase(uint8 startmod, uint8 Speed);
 * @function: 高速时关闭屏幕 上电后计时1s后开启屏幕
 * @param: none
 * @return: none
 */
void Screen_Show_Menu_SpeedBase(uint8 startmod, uint8 Speed)
{
    if (CarSpeed_x > Speed && Screen_Fast_Close_Flag == 1 && Screen_Close_Flag == 0 && Screen_Slow_Start == 0) {
        lcd_clear(BLACK);
//        Beep_Buzzing(25, 1);
        Char_WRITE_FLAG = 1;
        Screen_Fast_Close_Flag = 0;
    }
//    else if (CarSpeed_x > Speed && Screen_Fast_Close_Flag == 0 && Screen_Slow_Start == 0) {
//        Screen_Fast_Close_Flag = 0;
//    }
    else if (Screen_Slow_Start == 1) {
        Screen_Fast_Close_Flag = 0;
    }
    else {
        if(Screen_Fast_Close_Flag == 0 && Screen_Close_Flag == 0 && Screen_Slow_Start == 0) {
            Screen_Fast_Close_Flag = 1;
            Char_WRITE_FLAG = 1;
            lcd_clear(WHITE);
        }
        Screen_Show_Menu(startmod);
    }
}

/*
 * @name:void Screen_Show_Debug(void);
 * @function:屏幕显示DEBUG
 * @param:none
 * @return:none
 */
void Screen_Show_Debug(void)
{
    if (Photo_Finsh_Flag == 1) {
        midline_show();        //中线辅助显示
    }

    if (Char_WRITE_FLAG == 1) {
        lcd_showstr(0 , 4, "Ir:");
        Char_WRITE_FLAG = 0;
    }
    lcd_showuint8(25, 4, Improper_Row_Count);
    lcd_showint16(0 , 5, Attitude_Yaw);
}

/*
 * @name:void Screen_Show_Camera(void);
 * @function:屏幕显示摄像头
 * @param:none
 * @return:none
 */
void Screen_Show_Camera(void)
{
    if(Char_WRITE_FLAG == 1) {
        lcd_showstr(0 , 4, "Vr:");
        lcd_showstr(80, 4, "Ir:");
        lcd_showstr(0 , 5, "Er:");
        lcd_showstr(80, 5, "O :");
        Char_WRITE_FLAG = 0;
    }

    /* 处理后图像显示 */
    if (Photo_Finsh_Flag == 1) {
        lcd_displayimage032(img[0], OV7725_UART_W, OV7725_UART_H);
//        lcd_displayimage7725(img[0], OV7725_UART_W, OV7725_UART_H);
//        lcd_displayimage032_zoom(img[0], MT9V03X_W, MT9V03X_H, 80, 60);
        midline_show();        //中线辅助显示
    }

    lcd_showuint8(100, 1, FrontErr);
    lcd_showuint8(100, 2, speedline_err_abs);
    lcd_showuint8(100, 3, Gear);

    lcd_showuint8(25 , 4, valid_row);
    lcd_showuint8(105, 4, Improper_Row_Count);
    lcd_showint8(25  , 5, (int8)FusionErr);
    lcd_showint16(105, 5, AngleOutPut);

//    Screen_Show_Car_State(0,7);
}

/*
 * @name:void Screen_Show_Motor(void);
 * @function:屏幕显示调试内容
 * @param:none
 * @return:none
 */
void Screen_Show_Motor(void)
{
    Screen_Show_Car_State(0,0);

    lcd_showint16(0, 1, (int16)Mortor1.Speed_Read);
    lcd_showint16(0, 2, (int16)Mortor1_PID.Err_Now);
    lcd_showint16(0, 3, (int16)Mortor1_PID.Output);

    lcd_showint16(60, 1, (int16)Mortor2.Speed_Read);
    lcd_showint16(60, 2, (int16)Mortor2_PID.Err_Now);
    lcd_showint16(60, 3, (int16)Mortor2_PID.Output);

    lcd_showint16(0, 5, (int16)Mortor3.Speed_Read);
    lcd_showint16(0, 6, (int16)Mortor3_PID.Err_Now);
    lcd_showint16(0, 7, (int16)Mortor3_PID.Output);

    lcd_showint16(60, 5, (int16)Mortor4.Speed_Read);
    lcd_showint16(60, 6, (int16)Mortor4_PID.Err_Now);
    lcd_showint16(60, 7, (int16)Mortor4_PID.Output);

    lcd_showint16(5, 4, (int16)CarSpeed_x);
    lcd_showint16(55, 4, (int16)CarSpeed_y);
    lcd_showint16(105, 4, (int16)CarSpeed_a);
}

/*
 * @name:void Screen_Show_ELC(void);
 * @function:屏幕显示电磁
 * @param:none
 * @return:none
 */
void Screen_Show_ELC(void)
{
    if(Char_WRITE_FLAG == 1) {
        lcd_showstr(0 , 0, "LL:");  //[0]
        lcd_showstr(80, 0, "RR:");  //[3]
        lcd_showstr(0 , 1, "L:");   //[1]
        lcd_showstr(80, 1, "R:");   //[2]

        Char_WRITE_FLAG = 0;        //写入失能
    }
    lcd_showuint16(25 , 0, ELC_Guiyi[0]);
    lcd_showuint16(105, 0, ELC_Guiyi[3]);
    lcd_showuint16(25 , 1, ELC_Guiyi[1]);
    lcd_showuint16(105, 1, ELC_Guiyi[2]);


    lcd_showuint16(0, 3, ELC_Slide[0][0]);
    lcd_showuint16(0, 4, ELC_Slide[1][0]);
    lcd_showuint16(0, 5, ELC_Slide[2][0]);
    lcd_showuint16(0, 6, ELC_Slide[3][0]);

    lcd_showuint16(80, 3, ELC_KF[0]);
    lcd_showuint16(80, 4, ELC_KF[1]);
    lcd_showuint16(80, 5, ELC_KF[2]);
    lcd_showuint16(80, 6, ELC_KF[3]);

    lcd_showfloat(0, 7, ELCErr, 3, 1);
}

/*
 * @name:void Screen_Show_Gyro(void);
 * @function:屏幕显示陀螺仪
 * @param:none
 * @return:none
 */
void Screen_Show_Gyro(void)
{
    if(Char_WRITE_FLAG == 1) {
        lcd_showstr(0, 0, "Ax:");
        lcd_showstr(0, 1, "Ay:");
        lcd_showstr(0, 2, "Az:");
        lcd_showstr(0, 3, "Gx:");
        lcd_showstr(0, 4, "Gy:");
        lcd_showstr(0, 5, "Gz:");

        Char_WRITE_FLAG = 0;        //写入失能
    }
//        lcd_showint16(25, 0, ICM_Treated.accdata.x);
//        lcd_showint16(25, 1, ICM_Treated.accdata.y);
//        lcd_showint16(25, 2, ICM_Treated.accdata.z);
//        lcd_showint16(25, 3, ICM_Treated.gyro.x);
//        lcd_showint16(25, 4, ICM_Treated.gyro.y);
        lcd_showint16(25, 5, (int16)ICM_Treated.gyro.z);

//        lcd_showfloat(25, 5, ICM_Treated.gyro.z, 3, 3);
//        lcd_showint16(25, 6, (int16)Gyro_Z_Last[0]);
//        lcd_showint32(0, 7, (int32)Attitude_Angel_X, 6);
        lcd_showint16(0, 7, Attitude_Yaw);
}

/*
 * @name:void Screen_Show_Car_State(uint16 x,uint16 y)
 * @function:屏幕显示小车当前状态
 * @param:none
 * @return:none
 */
void Screen_Show_Car_State(uint16 x,uint16 y)
{
    switch(Car_State_Flag) {
        case 0: lcd_showstr(x,y,"0");       break;
        case 1: lcd_showstr(x,y,"Direht");  break;
        case 2: lcd_showstr(x,y,"S_Dir");   break;
        case 3: lcd_showstr(x,y,"F_Dir");   break;
        case 4: lcd_showstr(x,y,"Left");    break;
        case 5: lcd_showstr(x,y,"Right");   break;

        case 100: lcd_showstr(x,y,"Stop");  break;
        case 102: lcd_showstr(x,y,"F_Dir"); break;
        case 103: lcd_showstr(x,y,"FF_Dir");break;
        case 104: lcd_showstr(x,y,"DF_Dir");break;

        default:    lcd_showuint8(x,y,Car_State_Flag);  break;
    }
}

//-----------------------------------USER--------------------------------------------//

/*
 * @name:void Image_DrawXLine(int16 x,uint16 color,uint16 size)
 * @function:绘制一条垂直于X轴的直线
 * @param:none
 * @return:none
 */
void Image_DrawXLine(int16 x, int16 length_y, uint16 color, uint16 size)
{
    int16 _y = 0;
//    for(_y = 0;_y < OV7725_UART_W;_y ++)
    for(_y = 0;_y < length_y;_y ++)
    {
        if(size == 1)
            lcd_drawpoint(x,_y,color);
        else if(size == 3)
        {
            lcd_drawpoint(x-1,_y,color);
            lcd_drawpoint(x,_y,color);
            lcd_drawpoint(x+1,_y,color);
        }
        else if(size == 5)
        {
            lcd_drawpoint(x-2,_y,color);
            lcd_drawpoint(x-1,_y,color);
            lcd_drawpoint(x,_y,color);
            lcd_drawpoint(x+1,_y,color);
            lcd_drawpoint(x+2,_y,color);
        }
    }
}
void Image_DrawYLine(int16 length_x, int16 y, uint16 color, uint16 size)
{
    int16 _x = 0;
//    for(_x = 0;_x < OV7725_UART_H;_x ++)
    for(_x = 0;_x < length_x;_x ++)
    {
        if(size == 1)
            lcd_drawpoint(_x,y,color);
        else if(size == 3)
        {
            lcd_drawpoint(_x,y - 1,color);
            lcd_drawpoint(_x,y,color);
            lcd_drawpoint(_x,y + 1,color);
        }
        else if(size == 5)
        {
            lcd_drawpoint(_x,y - 2,color);
            lcd_drawpoint(_x,y - 1,color);
            lcd_drawpoint(_x,y,color);
            lcd_drawpoint(_x,y + 1,color);
            lcd_drawpoint(_x,y + 2,color);
        }
    }
}

/*
 * @name:void Screen_Drawline(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint16 color)
 * @function:屏幕划线
 * @param:none
 * @return:none
 */
void Screen_Drawline(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint16 color)
{
    float x;
    //制作y = kx + p 直线
    float k = 0.0000f;
    float p = 0.0000f;
    if(x1 == x2)
    {
        for(int i = MIN(y1,y2);i < MAX(y1,y2);i ++)
        {
            lcd_drawpoint(x1,i,color);
        }
    }
    else if(y1 == y2)
    {
        for(int i = MIN(x1,x2);i < MAX(x1,x2);i ++)
        {
            lcd_drawpoint(i,y1,color);
        }
    }
    else
    {
        k = (float)(y2 - y1) / (float)(x2 - x1);
        p = y1 - k*(float)x1;
        for(int i = MIN(y1,y2);i < MAX(y1,y2);i ++)
        {
            x = (float)(i - p) / k;
            lcd_drawpoint((int16)x,i,color);
        }
    }
}

/*
 * @name:void Image_Show_CentreRectangle(uint16 x,uint16 y,uint16 color,uint16 length,uint16 wide)
 * @function:绘制中心矩形
 * @param:none
 * @notice:注意不要超过图像边框
 */
void Image_Show_CentreRectangle(uint16 x,uint16 y,uint16 color,uint16 length,uint16 wide)
{
    uint16 i;
    if(length%2==0) length++;
    if(wide%2==0)   wide++;
    for(i=(x-(length-1)/2);i<=(x+(length-1)/2);i++)
    {
        lcd_drawpoint(i,y-(wide-1)/2,color);
        lcd_drawpoint(i,y+(wide-1)/2,color);
    }
    for(i=(y-(wide-1)/2);i<=(y+(wide-1)/2);i++)
    {
        lcd_drawpoint(x-(length-1)/2,i,color);
        lcd_drawpoint(x+(length-1)/2,i,color);
    }
}

/*
 * @name:void Image_Show_CornerRectangle(uint16 x_1,uint16 y_1,uint16 x_2,uint16 y_2,uint16 color)
 * @function:绘制边角矩形
 * @param:none
 * @notice:注意必须按照规则
 */
void Image_Show_CornerRectangle(uint16 x_1,uint16 y_1,uint16 x_2,uint16 y_2,uint16 color)
{
    uint16 i,x_s,y_s,x_e,y_e;
    if(x_1 > x_2) {
        x_s = x_2;x_e = x_1;
    }
    else{
        x_s = x_1;x_e = x_2;
    }
    if(y_1 > y_2) {
        y_s = y_2;y_e = y_1;
    }
    else{
        y_s = y_1;y_e = y_2;
    }
    for(i=x_s;i<=x_e;i++)
    {
        lcd_drawpoint(i,y_s,color);
        lcd_drawpoint(i,y_e,color);
    }
    for(i=y_s;i<=y_e;i++)
    {
        lcd_drawpoint(x_s,i,color);
        lcd_drawpoint(x_e,i,color);
    }
}

/*
 * @name:void Image_Show_X(uint16 x,uint16 y,uint16 color,uint16 size)
 * @function:绘制边角矩形
 * @param:none
 * @notice:none
 */
void Image_Show_X(uint16 x,uint16 y,uint16 color,uint16 size)
{
    unsigned short i;
    if(size%2==0) size++;
    //中点
    lcd_drawpoint(x,y,color);
    //边缘
    for(i = 1;i <= ((size-1)/2);i ++)
    {
        lcd_drawpoint(x-i,y-i,color);
        lcd_drawpoint(x+i,y-i,color);
        lcd_drawpoint(x-i,y+i,color);
        lcd_drawpoint(x+i,y+i,color);
    }
}

void Screen_Smile(void)
{
    if(Screen_Show_Menu_StartMod == 1)
    {
        lcd_clear(YELLOW);
        Screen_Show_Menu_StartMod = 0;
    }

    Image_DrawYLine(40,30,BLACK,5);
    Image_DrawYLine(40,100,BLACK,5);
}
