/*
 *  control.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */
 
#include "MY_Bsp.h"
#include "control.h"
#include "define.h"

/*
 * @name:void LED_Beat(void)
 * @function: LED控制
 * @param:none
 * @return:none
 */
void LED_Beat(void)
{

}

/*
 * @name:void Beep_Beat(void)
 * @function: 蜂鸣器控制
 * @param:none
 * @return:none
 */
void Beep_Beat(void)
{
    Beep_Flag = 0;

    /* 短&长直道加速蜂鸣 */
//    if (Car_State_Flag == 103)  Beep_Flag = 1;  //Far
//    if (Car_State_Flag == 102)  Beep_Flag = 1;  //Near
//    if (Car_State_Flag == 104)  Beep_Flag = 1;  //Dis
//    if (Gear != 0 && Gear == GearCount) Beep_Flag = 1;  //加速
//    if (Gear != 0)              Beep_Flag = 1;
    /* 元素蜂鸣 */
    if (Ramp_Flag)              Beep_Flag = 1;  //坡道
//    if (circle_flag  != 0)       Beep_Flag = 1;  //环岛
    if((right_circle_flag==3)||(left_circle_flag==3)) Beep_Flag = 1;
    if((right_circle_flag==5)||(left_circle_flag==5)) Beep_Flag = 1;
//    if(stop_flag==1)                                 Beep_Flag = 1;
//    if (flag_y_cross != 0)       Beep_Flag = 1;  //三叉
    if (flag_c != 0)            Beep_Flag = 1;  //三叉
//    if (flag_cross != 0)        Beep_Flag = 1;  //十字
//    if (ELC_Protect_Count > 99) Beep_Flag = 1;  //出赛道

    /* 测试 */
//    if (Gyro_Integral > 1)      Beep_Flag = 1;    //出现不能用的点
//    if (Fork_Road_Count > 2)    Beep_Flag = 1;    //两个三叉
//    if (FusionErr > 40.0f)      Beep_Flag = 1;
//    if (AngleOutPut >= 99)      Beep_Flag = 1;

    if (Beep_Flag == 1) {
        BEEP_ON;
    } else {
        BEEP_OFF;
    }
}

/*
 * @name:void Angle_ElementBase_WritePID(void);
 * @function: 根据元素和速度情况分别写入PID参数
 * @param:parameter
 * @return:none
 */
void Angle_ElementBase_WritePID(void)
{
    if((circle_flag == 1 && left_circle_flag == 4) || (circle_flag == 2 && right_circle_flag == 4)) {
        /* 环岛 */
        if (Speed_Set_c == 45) {
            AnglePID_Parameter_Write(Angle1_PID3_45_Parameter);
        } else if (Speed_Set_c == 50) {
            AnglePID_Parameter_Write(Angle1_PID3_50_Parameter);
        } else if (Speed_Set_c == 55) {
            AnglePID_Parameter_Write(Angle1_PID3_55_Parameter);
        } else if (Speed_Set_c == 60) {
            AnglePID_Parameter_Write(Angle1_PID3_60_Parameter);
        } else if (Speed_Set_c == 65) {
            AnglePID_Parameter_Write(Angle1_PID3_65_Parameter);
        } else if (Speed_Set_c == 70) {
            AnglePID_Parameter_Write(Angle1_PID3_70_Parameter);
        } else {
            AnglePID_Parameter_Write(Angle1_PID3_Parameter);
        }
    } else if (left_circle_flag == 3 || right_circle_flag == 3) {
        /* 入环 */
        if (Speed_Set_in == 45) {
            AnglePID_Parameter_Write(Angle1_PID4_45_Parameter);
        } else if (Speed_Set_in == 50) {
            AnglePID_Parameter_Write(Angle1_PID4_50_Parameter);
        } else if (Speed_Set_in == 55) {
            AnglePID_Parameter_Write(Angle1_PID4_55_Parameter);
        }
    } else if (flag_y_cross == 1) {
        /* 横向 */
        if (Speed_Set_x == 35) {
            AnglePID_Parameter_Write(Angle1_PID2_35_Parameter);
        } else if (Speed_Set_x == 40) {
            AnglePID_Parameter_Write(Angle1_PID2_40_Parameter);
        } else if (Speed_Set_x == 45) {
            AnglePID_Parameter_Write(Angle1_PID2_45_Parameter);
        } else if (Speed_Set_x == 50) {
            AnglePID_Parameter_Write(Angle1_PID2_50_Parameter);
        } else if (Speed_Set_x == 55) {
            AnglePID_Parameter_Write(Angle1_PID2_55_Parameter);
        } else if (Speed_Set_x == 60) {
            AnglePID_Parameter_Write(Angle1_PID2_60_Parameter);
        } else {
            AnglePID_Parameter_Write(Angle1_PID2_Parameter);
        }
    } else if (stop_flag >= 3){
        /* 入库 */
        AnglePID_Parameter_Write(Angle1_PID6_Parameter);
    } else {
        /* 正常 */
        if (Car_Direction == 1) {
            /* 正向 */
            if (Speed_Set_y == 40) {
                AnglePID_Parameter_Write(Angle1_PID1_40_Parameter);
            } else if (Speed_Set_y == 45) {
                AnglePID_Parameter_Write(Angle1_PID1_45_Parameter);
            } else if (Speed_Set_y == 50) {
                AnglePID_Parameter_Write(Angle1_PID1_50_Parameter);
            } else if (Speed_Set_y == 55) {
                AnglePID_Parameter_Write(Angle1_PID1_55_Parameter);
            } else if (Speed_Set_y == 60) {
                AnglePID_Parameter_Write(Angle1_PID1_60_Parameter);
            } else if (Speed_Set_y == 65) {
                AnglePID_Parameter_Write(Angle1_PID1_65_Parameter);
            } else if (Speed_Set_y == 70) {
                AnglePID_Parameter_Write(Angle1_PID1_70_Parameter);
            } else {
                AnglePID_Parameter_Write(Angle1_PID1_Parameter);
            }
        } else {
            /* 反向 */
        }
    }
}

/*
 * @name:void Middle_Compensation(void)
 * @function:环岛中线补偿
 * @param:none
 * @return:none
 */
void Midline_Compensation(void)
{
    if(left_circle_flag == 3) {
        /* 左环岛入环 */
        middle_standard = 41;
    }else if (left_circle_flag == 4) {
        /* 左环岛环内巡线 */
        middle_standard = 42;
    } else if (right_circle_flag == 3 || right_circle_flag == 4) {
        /* 右环岛 */
        middle_standard = 37;
    } else if (flag_y_cross == 1 && (Fork_Road_Count == 2 || Fork_Road_Count == 3)) {
        /* 左三叉 */
        middle_standard = 35;
    } else if (flag_y_cross == 1 && (Fork_Road_Count == 1 || Fork_Road_Count == 4)) {
        /* 右三叉 */
        middle_standard = 39;
    } else {
        /* 正常 */
        middle_standard = 41;
    }
}

/*
 * @name:void Magnet_Identify(void);
 * @function:电磁判断
 * @param:none
 * @return:none
 */
void Magnet_Identify(void)
{
    Guiyi();

    ELC_Analyse();
}

/*
 * @name:void Magnet_Process(void);
 * @function:电磁寻迹
 * @param:none
 * @return:none
 */
void Magnet_Process(void)
{
    subInside  = ELC_Value[1] - ELC_Value[2];   //内侧差
    sumInside  = ELC_Value[1] + ELC_Value[2];   //内侧和
    subOutside = ELC_Value[0] - ELC_Value[3];   //外侧差
    sumOutside = ELC_Value[0] + ELC_Value[3];   //外侧和

//    ELCErr = (float)subOutside / sumOutside;    //差比和计算电磁误差
//    ELCErr = sqrtf(ELC_Guiyi[0]) - sqrtf(ELC_Guiyi[3]) / sumOutside;

//    ELCErr = ELCErr * 10.0f;   //将误差归到-40-40
}

/*
 * @name:uint8 Camera_Process(void);
 * @function:摄像头处理
 * @param:none
 * @return:none
 */
void Camera_Process(void)
{
    if(ov7725_uart_finish_flag) {
        Image_Decompression(ov7725_uart_image_bin[0], img[0]); //解压后 黑 0 白 255

        get_mid_line(0);        //中线处理函数

        ov7725_uart_finish_flag = 0;
        Photo_Finsh_Flag = 1;
    } else {
        Photo_Finsh_Flag = 0;
    }
}

/*
 * @name:uint8 Get_Prospect(void);
 * @function:根据速度和动作和图像确定前瞻(动态有效行)
 * @param:none
 * @return:aim_line
 */
uint8 Get_Prospect(void)
{
    if(AngleErr > 7.0f) {
        if(Mortor_MaxSpeed > 70)        aim_line = 29;
        else if(Mortor_MaxSpeed > 90)   aim_line = 28;
        else if(Mortor_MaxSpeed > 100)  aim_line = 27;
        else                            aim_line = aim_line + 0;
    }
    else if(AngleErr < -7.0f) {
        if(Mortor_MaxSpeed > 70)        aim_line = 29;
        else if(Mortor_MaxSpeed > 90)   aim_line = 28;
        else                            aim_line = aim_line + 0;
    }
    else                                aim_line = aim_line + 0;

    /* 速度越快有效行越远 */
//    if (CarSpeed_x>50) {
//        aim_line = (int)(-0.2 * (CarSpeed_x - 50) + aim_line);
//    }

    if ((valid_row + 2) > aim_line) {
        prospect_row = (uint8)(valid_row + (OV7725_UART_H - valid_row)/3);
    } else {
        prospect_row = aim_line;
    }

    return prospect_row;
}

/*
 * @name:void Angle_Process(void)
 * @function:取三行打脚行
 * @param:none
 * @return:none
 */
void Angle_Process(void)
{
    aim_line1 = Get_Prospect();

    AngleErr = ((5 * middle_line[aim_line1] +
                 3 * middle_line[aim_line1 + 1] +
                 2 * middle_line[aim_line1 + 2]) / (10.0f)) - middle_standard;
}

/*
 * @name: float Regression_cal(int startline, int endline)
 * @function: 最小二乘法
 * @param: startline是起始行,endline是截至行
 * @notice: y = Ax + B (A是斜率B是截距
 */
float Regression_cal(uint8 startline, uint8 endline)
{
    uint8 i = 0;
    uint16 SumX = 0, SumY = 0, SumLines = 0;
    float SumUp = 0.000f, SumDown = 0.000f, avrX = 0.000f, avrY = 0.000f;
    float B = 0.000f;
    float A = 0.000f;

    SumLines = endline - startline;

    for(i = startline; i < endline; i++) {
        SumX += i;
        SumY += middle_line[i];
    }

    avrX = SumX / SumLines;
    avrY = SumY / SumLines;

    SumUp = 0;
    SumDown = 0;
    for(i = startline; i < endline; i++) {
        SumUp += (middle_line[i] - avrY) * (i - avrX);
        SumDown += (i - avrX) * (i - avrX);
    }

    if(SumDown == 0) {
        B = 0;
    } else {
        B = (int)(SumUp / SumDown);
    }

    A = (SumY - B * SumX) / SumLines;

//    B = (B < -40) ? -40 : (B > 40 ? 40 : B);  //限幅

    Regression_A = A;
    Regression_B = B;

    return B;
}

/*
 * @name:void Angle_All_Process(void);
 * @function:加权计算全图像中线偏差
 * @param:none
 * @return:none
 */
void Angle_All_Process(void)
{
    uint8 i = 0;
    int16 line_sum = 0;         //中线之和
    int16 weight_sum = 0;       //权重之和
    uint8 front_valid_row = 0;  //图像前部分取到的行数
    int16 front_linesum = 0;    //图像前部分中线之和
    uint8 front_add = 0;        //图像前部分中线计数
//    uint8 Turn_K = 30;

    AngleErr = 0.000f;
    FrontErr = 0;
    Slope = 0.0f;

    /* 环岛中线补偿 */
    Midline_Compensation();

    front_valid_row = valid_row + (start_row - valid_row) / 3;  //取图像前三分之一

    for(i = start_row; i > valid_row + 1; i --) {
        weight_sum += weight1[i];
        line_sum += weight1[i] * middle_line[i];

        if(i < front_valid_row) {
            front_linesum += (middle_line[i] - middle_standard);
            front_add++;
        }   //图像前1/3
    }   //从下往上扫 0是最顶行

//    i = start_row;
//    while(i > valid_row) {
//        weight_sum += weight1[i];
//        line_sum += weight1[i] * (middle_line[i] - middle_standard);
//        i--;
//    }   //另一种写法

    if(weight_sum == 0)     weight_sum = 1;
    if(front_add == 0)      front_add = 1;  //防止运算时除0

    midlinenow  = (float)line_sum / weight_sum - middle_standard;
    midline_fff = midline_ff;
    midline_ff  = midline_f;
    midline_f   = midlinenow;

//    AngleErr = (int)((midline_fff * 0.50f + midline_ff * 0.30f + midline_f * 0.20f) * Turn_K);
    AngleErr = midline_fff * 0.50f + midline_ff * 0.30f + midline_f * 0.20f;

//    if (valid_row>45 && valid_row<55) {
//        Slope = Regression_cal(valid_row+3, start_row);
//    } else if ((valid_row + 3 > 15) && valid_row<45) {
//        Slope = Regression_cal(valid_row+3, start_row);
//    } else {
//        Slope = Regression_cal(15, start_row);
//    }
//    AngleErr = (int)((AngleErr * 8 + Slope * 2) / 10);

    FrontErr = front_linesum / front_add;
}

/*
 * @name:void Angle_Front_Process(void);
 * @function: 取图像有效行下三分之一
 * @param:none
 * @return:none
 */
void Angle_Front_Process(void)
{
    uint8 i = 0;
    uint8 front_valid_row = 0;  //图像前部分取到的行数
    int16 front_linesum = 0;    //图像前部分中线之和
    uint8 front_add = 0;        //图像前部分中线计数

    FrontErr = 0;

    front_valid_row = valid_row + (start_row - valid_row) / 3;  //取图像前三分之一

    for(i = valid_row + 1; i < front_valid_row; i++) {
        front_linesum += (middle_line[i] - middle_standard);
        front_add++;
    }   //从上往下扫到有效图像部分前1/3

    if(front_add == 0)      front_add = 1;  //防止极限情况运算除0
    FrontErr = front_linesum / front_add;

}

/*
 * @name:void Angle_All_Process(void);
 * @function:用部分图像计算偏差
 * @param:none
 * @return:none
 */
void Angle_Part_Process(void)
{
    uint8   i = 0;
    float   Part_Err_Sum = 0.000f;  //图像部分误差加和
    float   Err_K = 2.0f;           //测试下来范围在1.0-3.0比较好
    uint16  Part_Ave = 0;           //范围内中线平均值
    uint8   Part_Used = 0;          //图像部分误差使用的行数

    AngleErr = 0.000f;
    memset(Part_Err, 0, sizeof(Part_Err));
    Improper_Row_Count = 0;
    memset(Improper_Row, 0, sizeof(Improper_Row));

    Midline_Compensation();     //环岛中线补偿

//    if (Start_Part > start_row) {
//        Start_Part = start_row - 2;
//    }   //起始行限幅

    if (flag_y_cross != 0) {
        End_Part = Start_Part - PartUsed_X;
    } else {
        End_Part = Start_Part - PartUsed;
    }   //横向和正常给不同使用行数
    if (End_Part < valid_row) {
        End_Part = valid_row + 2;
    }   //截至行限幅

    Part_Used = Start_Part - End_Part;

    for (i = Start_Part; i > End_Part; i--) {    //从下往上扫
        Part_Err[Start_Part - i] = (float)(middle_line[i] - middle_standard) * 100.0f / ((float)img_real_width[i] / 2.0f); //0是用的最底行
        Part_Ave += middle_line[i];
//        if (i == End_Part - 2) {
//            Part_Err[Start_Part - i] = Part_Err[Start_Part - i] * 1.2f;
//        }   //放大截至行误差
    }   //相当于计算百分比
        //PartUsed 相当于前瞻, 10行适合低速, 高速可以取15, 区域越小越灵敏
        //可以从图像倒数第5行向上扫

    Part_Ave = Part_Ave / Part_Used; //计算范围内中线平均值
    for (i = Start_Part; i > End_Part; i--) {
        if (ABS(middle_line[i] - Part_Ave) > 10) {
            Improper_Row[Start_Part - i] = middle_line[i];
            Part_Used--;
            Improper_Row_Count++;
        }
    }   //记录偏离中线均值10以上的点 滤掉跳变点

    for (i = 0; i < (Part_Used / 5); i++) {
            Part_Err[i] = 0.000f;
    }   //最底下1/5行置零, 为了平滑

    for (i = 0; i < Part_Used; i++) {
        if (Improper_Row[i] == 0) {
            Part_Err_Sum += Part_Err[i];
        }
    }   //累加非跳变点

    AngleErr  = Part_Err_Sum * Err_K / (float)Part_Used;
    AngleErr2 = AngleErr1;
    AngleErr1 = AngleErr0;
//    AngleErr0 = AngleErr;
    AngleErr0 = Part_Err_Sum * Err_K / (float)Part_Used;;

    AngleErr = AngleErr0 * 0.70f + AngleErr1 * 0.20f + AngleErr2 * 0.10f;

    if (isnanf(AngleErr) || isinff(AngleErr)) {
        if (isnanf(AngleErr1) || isinff(AngleErr1)) {
            if (isnanf(AngleErr2) || isinff(AngleErr2)) {
                AngleErr  = Part_Err_Sum * Err_K / (float)Part_Used;
                AngleErr0 = AngleErr1 = AngleErr2 = AngleErr;
                if (isnanf(AngleErr) || isinff(AngleErr)) {
                    AngleErr = AngleErr0 = AngleErr1 = AngleErr2 = 0.000f;
                }
            } else {
                AngleErr = AngleErr0 = AngleErr1 = AngleErr2;
            }
        } else {
            AngleErr = AngleErr0 = AngleErr1;
        }
    }
}

/*
 * @name: void Sensor_Tracking(void)
 * @function: 传感器循迹
 * @param: none
 * @return: none
 */
void Sensor_Tracking(void)
{
    float lpf1_factor = 0.900f;

//    Angle_Process();
//    Angle_All_Process();
    Angle_Part_Process();

    if (Gear_Flag) {
        if (valid_row < 40) {
            Angle_Front_Process();
        }
    }

    Magnet_Process();

    FusionErr = AngleErr;
//    if (AngleErr > 1.0f && ELCErr > 1.0f) {
//        FusionErr = AngleErr * 0.800f + ELCErr * 0.200f;
//    }
//    if (AngleErr < -1.0f && ELCErr < -1.0f) {
//        FusionErr = AngleErr * 0.800f + ELCErr * 0.200f;
//    }   //电磁误差与摄像头同向时，电磁误差*0.2+摄像头*0.8 不同向则信任摄像头
//    if (circle_flag == 0 && flag_y_cross == 0) {
//        FusionErr = AngleErr;
//    } //环岛三叉内仅使用摄像头

    FusionErr = lpf1_factor * FusionErr + (1.000f - lpf1_factor) * FusionErr_Last;

    FusionErr_Last = FusionErr;

    if (isnanf(FusionErr) || isinff(FusionErr)) {
        if (isnanf(FusionErr_Last) || isinff(FusionErr_Last)) {
            if (isnanf(AngleErr) || isinff(AngleErr)) {
                if (isnanf(AngleErr1) || isinff(AngleErr1)) {
                    if (isnanf(AngleErr2) || isinff(AngleErr2)) {
                        FusionErr = FusionErr_Last = AngleErr = AngleErr0 = AngleErr1 = AngleErr2 = 0.000f;
                    } else {
                        FusionErr = FusionErr_Last = AngleErr = AngleErr0 = AngleErr1 = AngleErr2;
                    }
                } else {
                    FusionErr = FusionErr_Last = AngleErr = AngleErr0 = AngleErr1;
                }
            }
        } else {
            FusionErr = FusionErr_Last;
        }
    }
}

/*
 * @name: void Dis_Control(void)
 * @function: 根据距离的控制
 * @param: none
 * @return: none
 */
void Dis_Control(void)
{
    uint8   SpeedZone = 7;
    uint16  CountZone = 128;

    if (ABS((int16)(fabsf(CarSpeed_a))) >= SpeedZone) {
        Dis_Acc_Count = 0;
    }
    if (ABS((int16)(fabsf(CarSpeed_a))) < SpeedZone) {
        Dis_Acc_Count ++;
    }

    if (Dis_Acc_Count >= 500) {
        Dis_Acc_Count = 500;
    }

    if (Dis_Acc_Count > CountZone) {
        Car_State_Flag = 104;
        DisGear = 3;
    }   //实际走过一定距离的直道, 就算是直道
        //有滞后性, 加速不要给多
}

/*
 * @name: void Midline_Change(void)
 * @function: 加速
 * @param: none
 * @return: none
 */
void Midline_Change(void)
{
    uint8 i = 0;
    uint8 startrow = 10;
    uint8 endrow = 30;
    uint8 RowCount = 0;
    uint8 RowCount2 = 0;
    uint8 OffsetCount = 0;

    if (valid_row > startrow) {
        startrow = valid_row;
    }

    for (i = startrow; i < endrow; i++) {
        if ((endrow - startrow) < 5) {
            break;
        }   //行数过少取消判断

        if (middle_standard - left_black[i] > 4 && right_black[i] - middle_standard > 4) {
            RowCount2++;
        } else if (middle_standard - left_black[i] > 2 && right_black[i] - middle_standard > 2) {
            RowCount++;
        } else {
            break;
        }

        if (RowCount == endrow - startrow) {
//            MidlineGear = 8;
            Gear = 8;
        }
        if (RowCount2 == endrow - startrow) {
//            MidlineGear = 10;
            Gear = 10;
        }
    }   //判断中线上半部与赛道两边界无焦点即可加速

    for (i = startrow; i < endrow; i++) {
        if ((ABS(middle_line[i] - middle_standard) > 2) && (middle_line[i] - middle_line[i+1] < 2)) {
            OffsetCount++;
        } else {
            break;
        }

        if (OffsetCount == endrow - startrow) {
            Gear = Gear * 0.5f;
        }
    }   //直道漂移后中线全部偏向一侧
}

/*
 * @name: void Speed_Change(void)
 * @function: 直道加速
 * @param: none
 * @return: none
 */
void Speed_Change(void)
{
    int8    speedline_err_real = 0;
    uint8   stright_err_abs = 0;
    uint8   stright2_err_abs = 0;

    speedline_err_real = (int8)((5 * middle_line[speed_line] +
                                 2 * middle_line[speed_line + 1] +
                                 3 * middle_line[speed_line - 1]) / (10.0f) - middle_standard);
    speedline_err_abs = ABS(speedline_err_real); //偏差值,用于控速

    stright_err_abs = ABS((uint8)((5 * middle_line[stright_speed_line] +
                                   2 * middle_line[stright_speed_line + 1] +
                                   3 * middle_line[stright_speed_line - 1]) / (10.0f) - middle_standard)); //偏差值,用于判断是否为直道

    stright2_err_abs = ABS((uint8)((5 * middle_line[30] +
                                    2 * middle_line[30 + 1] +
                                    3 * middle_line[30 - 1]) / (10.0f) - middle_standard)); //偏差值,用于判断是否为直道

    speedline_err_abs = MAX(speedline_err_abs, stright_err_abs);
    speedline_err_abs = MAX(speedline_err_abs, stright2_err_abs);

    /*************长直道加速*******************/
    if(speedline_err_abs < 10) {
//        SlGear = 6;
        Gear = 6;
        Car_State_Flag = 103;
    }
    if (speedline_err_abs < 7) {
//        SlGear = 8;
        Gear = 8;
    }
}

/*
 * @name: void Speed_Change_Near(void)
 * @function: 短直道加速
 * @param: none
 * @return: none
 */
void Speed_Change_Near(void)
{
    if (ABS(FrontErr) < 10) {
//        SnGear = 3;
        Gear = 3;
        Car_State_Flag = 102;
    }
    if (ABS(FrontErr) < 6) {
//        SnGear = 6;
        Gear = 6;
    }
}

/*
 * @name: void ValidRow_Change(void)
 * @function: 有效行判断加速
 * @param: none
 * @return: none
 */
void ValidRow_Change(void)
{
    if (valid_row <= 11) {
        VrGear = 6;
    }
}

/*
 * @name: void Gear_Control(void)
 * @function: 加速总控制
 * @param: none
 * @return: none
 */
void Gear_Control(void)
{
    Gear = 0;               //加速先置零
    MidlineGear = SlGear = VrGear = SnGear = DisGear = 0;

    if (Gear_Flag && circle_flag == 0) {//不是环岛则开启直道加速控制
//        Dis_Control();              //距离判断加速
        if (Gear < 20) {
            Speed_Change_Near();    //顶部图像判断加速
//            ValidRow_Change();      //有效行判断加速
            Speed_Change();         //全局图像判断加速
            Midline_Change();       //中线判断加速
        }

//        if (SlGear >= Gear)     Gear = SlGear;
//        if (VrGear >= Gear)     Gear = VrGear;
//        if (MidlineGear >= Gear)Gear = MidlineGear;
//        if (SnGear >= Gear)     Gear = SnGear;
//        if (DisGear >= Gear)    Gear = DisGear;

//        if (flag_y_cross != 0 && Gear != 0) {
//            Gear = MAX(MIN(CarSpeed_x * 1.1f, Speed_Set_y + Gear), 0);  //平滑加速
//        }

//        if (Gear > GearCount) {
//            GearCount += 1;
//            Gear = GearCount;
//        }   //平滑加速
//        if (Gear < GearCount)   GearCount = Gear;
//        if (Gear == 0)          GearCount = 0;

        if (AngleOutPut > 35 || CarSpeed_a > 30 || fabsf(FusionErr) > 55.0f || valid_row > 17) {
            Gear = 0;
        }   //误判情况加速置零

        if (Gear_Flag == 1) {
            Gear = Gear * 0.5f;
        }   //慢速挡位加速
        if (Gear_Flag == 3) {
            Gear = Gear * 1.1f;
        }   //高速挡位加速

        if (flag_c) {
            Gear = Gear * 0.2f;
        }   //三叉预判断减少加速
        if (flag_y_cross != 0) {
            if (Gear_Flag == 3) {
                Gear = Gear * 0.4f;
            } else {
                Gear = 0;
            }
        }   //三叉内不加速

//        if (stop_flag > 3) {
//            Gear = 0;
//        }   //入库不加速
    }
}

/*
 * @name:void Car_Run_Protect(int16 x, int16 y, int16 rotate, uint8 mode)
 * @function: 带电磁出赛道防护的Car_Run
 * @param:none
 * @return:none
 */
void Car_Run_Protect(int16 x, int16 y, int16 rotate, uint8 mode)
{
    if (mode == 0) {
        ELC_Protect_State = 0;
        Car_Run(x, y, rotate);
        ELC_Protect_Count = 0;
    }

    if (mode == 1) {
        if (MaxELC < 444 && ELC_Protect_Count < 150) {
             ELC_Protect_Count++;
        } else {
            if (ELC_Protect_Count > 1) {
                ELC_Protect_Count--;
            }
        }

        if (ELC_Protect_Count > 50 && stop_flag == 0) {
            ELC_Protect_State = 1;
//            Car_Run(0, 0, 0);
            Car_Stop();
        } else if (stop_flag >= 3 && ELC_Protect_Count > 100) {
            ELC_Protect_State = 1;
//            Car_Run(0, 0, 0);
            Car_Stop();
            //入库停车保护灵敏度降低
        } else {
            ELC_Protect_State = 0;
            Car_Run(x, y, rotate);
            Car_Protect = 0;
        }

//        if (ABS(x - CarSpeed_x) > 10 && speed_read_tmp[Mortor_MinSpeedNum] > 3000) {
//            Car_Stop();
//        }   //堵转保护 不好用
    }

    if (!Out_Garage) {
        ELC_Protect_State = 0;
        Car_Protect = 0;
        ELC_Protect_Count = 0;
    }
}

/*
 * @name: void Car_Advance(int16 speed, int16 Dis, uint8 mode)
 * @function: 前进一定距离
 * @param: 1是前 2是后
 * @return:none
 */
void Car_Advance(uint16 speed, uint16 Dis, uint8 mode)
{
    if (mode == 1) {
        if ((int16)(CarTran_x) < Dis) {
            CarTran_x += CarSpeed_x;
            Car_Run(speed, 0, 0);
        } else if ((int16)(CarTran_x) >= Dis) {
            Car_Run(0, 0, 0);
            Car_AdvanceX_Flag = 1;
        }
    }   /* 前 */

    if (mode == 2) {
        if ((int16)(CarTran_x) > -Dis) {
            CarTran_x += CarSpeed_x;
            Car_Run(-speed, 0, 0);
        } else if ((int16)(CarTran_x) <= -Dis) {
            Car_Run(0, 0, 0);
            Car_AdvanceX_Flag = 1;
        }
    }   /* 后 */
}

/*
 * @name: void Car_Translae(uint8 mode, uint16 Dis)
 * @function: 平移一定距离
 * @param: 1是左 2是右
 * @return:none
 */
void Car_Translae(uint16 speed, uint16 Dis, uint8 mode)
{
    if (mode == 1) {
        if ((int16)(CarTran_y) > -Dis) {
            CarTran_y += CarSpeed_y;
            Car_Run(0, -speed, 0);
        } else if((int16)(CarTran_y) <= -Dis) {
            Car_Run(0, 0, 0);
            Car_AdvanceY_Flag = 1;
        }
    }   /* 左 */

    if (mode == 2) {
        if ((int16)(CarTran_y) < Dis) {
            CarTran_y += CarSpeed_y;
            Car_Run(0, speed, 0);
        } else if((int16)(CarTran_y) >= Dis) {
            Car_Run(0, 0, 0);
            Car_AdvanceY_Flag = 1;
        }
    }   /* 右 */
}

/*
 * @name: void Car_Turn(int16 speed, int16 angle, uint8 mode);
 * @function: 转一定角度
 * @param: 1是左 2是右
 * @return:none
 */
void Car_Turn(uint16 speed, uint16 angle, uint8 mode)
{
    if (mode == 1) {
        Gyro_Integral = 1;
        Attitude_Err = angle - Attitude_Yaw;
        if (Attitude_Err > 0) {
            Car_Run(0, 0, -speed);
        } else {
            Car_Run(0, 0, 0);
            Gyro_Integral = 0;
            Car_Turn_Flag = 1;
        }
    }   /* 左 */

    if (mode == 2) {
        Gyro_Integral = 1;
        Attitude_Err = angle + Attitude_Yaw;
        if (Attitude_Err > 0) {
            Car_Run(0, 0, speed);
        } else {
            Car_Run(0, 0, 0);
            Gyro_Integral = 0;
            Car_Turn_Flag = 1;
        }
    }   /* 右 */

}

/*
 * @name:void Car_Stop(void);
 * @function: 停车
 * @param；none
 * @return:none
 */
void Car_Stop(void)
{
    Mortor1.Speed_Ideal = Mortor2.Speed_Ideal
    = Mortor3.Speed_Ideal = Mortor4.Speed_Ideal = 0;

    if (ABS(Mortor_MaxSpeed) < 20 || ABS(CarSpeed_x) < 20) {
        Mortor1.PWM_Write = Mortor2.PWM_Write
        = Mortor3.PWM_Write = Mortor4.PWM_Write =  0;
        Car_State_Flag = 100;

        Car_Protect = 1;    //失能电机PID
        Motor_PWM_Write();
    }   //至少刹车到设定速度或以下
}

/*
 * @name:void Car_Out_Garage(void);
 * @function:出库
 * @param: 1是左 2是右
 * @return:none
 */
void Car_Out_Garage(uint8 mode)
{
    /* 简易摄像头 */
//    if (Angle.Angle_Err_ABS <= 10 && valid_row <= 20) {
//        Out_Garage = 1;
//    }

    /* 编码器 */
    Car_Translae(50, 2500, mode);
    if (Car_AdvanceY_Flag == 1) {
        Out_Garage = 1;
        CarTran_y = 0;
        Car_AdvanceY_Flag = 0;
    }
}

/*
 * @name: void Ramp_Control(void);
 * @function: none
 * @param: none
 * @return: none
 */
void Ramp_Control(void)
{
    if (!circle_flag && !flag_y_cross && !ELC_Protect_State && valid_row < 13 && TFmin_Dist <= 29) {
        Ramp_Flag = 1;
    }   //不是环岛不是三叉也没有停车保护 且有效行足够高 则识别坡道

    if (Ramp_TimeCount != 0 && Ramp_TimeCount <= 36) {
        Ramp_TimeCount++;
        Ramp_Flag = 0;
    } else if (Ramp_TimeCount > 36) {
        Ramp_TimeCount = 0;
    }   //坡道结束后一段时间内不再识别第二个坡道

    if (Ramp_Flag) {
        CarTran_x += CarSpeed_x;    //识别坡道后开始记步

        if (CarTran_x > 5000.0f) {
            Gear = 0;
        }   //过了坡道前半部份后关闭加速

        if (CarTran_x > 1000.0f && CarTran_x < 7000.0f) {
            if (AngleOutPut >  10)  AngleOutPut =  10;
            if (AngleOutPut < -10)  AngleOutPut = -10;
        }   //坡道内转角限幅

//        if (CarTran_x > 8000.0f && TFmin_Dist <= 28) {
//            CarTran_x = 0;
//            Ramp_TimeCount++;
//            Ramp_Flag = 0;
//        }   //识别下坡结束坡道

        if (CarTran_x > 11000.0f) {
            CarTran_x = 0;
            Ramp_TimeCount++;
            Ramp_Flag = 0;
        }   //坡道记步足够后结束坡道

//        if (CarTran_x > 8000.0f && TFmin_Dist >= 40 && TFmin_Dist <= 45) {
//            CarTran_x = 0;
//            Ramp_Flag = 0;
//        }   //走过一定距离后识别正常赛道结束坡道
    }

}

/*
 * @name: void Gyro_Circle_Control(void);
 * @function: 陀螺仪出入环
 * @param: none
 * @return: none
 */
void Gyro_Circle_Control(void)
{
    if (circle_flag != 0) {

        Gyro_Integral = 1;
        Attitude_Err = ABS(Attitude_Yaw);

        Gear = 0;

        /* 左环 */
        if (circle_flag == 1) {
            if (Attitude_Err >= 600) {
                Gyro_Repair_End_Flag = 1;
            }   //陀螺仪大于35度入环补线结束
            if (Attitude_Err >= 2000) {
                Gyro_StartIdOutCir_Flag = 1;
            }   //陀螺仪大于200度开始识别出环
            if (Attitude_Err >= 2500) {
                Gyro_TimeOutCir_Flag = 1;
            }   //陀螺仪大于248度强制开始出环
            if (Attitude_Err >= 3500) {
                Gyro_Trans_Flag = 1;
            }   //陀螺仪大于310度开始偏移
            if (Attitude_Err >= 3550) {
                Gyro_Integral = 0;
                Gyro_Trans_Flag = 0;
                Gyro_OutCircle_Flag = 1;
            }   //陀螺仪大于356度出环完成
            //判断小环岛和大环岛 可以记录车身0-60度时间或是编码器脉冲数
        }
        /* 右环 */
        if (circle_flag == 2) {
            if (Attitude_Err >= 600) {
                Gyro_Repair_End_Flag = 1;
            }
            if (Attitude_Err >= 2000) {
                Gyro_StartIdOutCir_Flag = 1;
            }
            if (Attitude_Err >= 2500) {
                Gyro_TimeOutCir_Flag = 1;
            }
            if (Attitude_Err >= 3500) {
                Gyro_Trans_Flag = 1;
            }
            if (Attitude_Err >= 3550) {
                Gyro_Integral = 0;
                Gyro_Trans_Flag = 0;
                Gyro_OutCircle_Flag = 1;
            }
        }
}

    //姿态出环
    if (circle_flag == 0) {
        OutCircle_Inertial = 0;
        OutCircle_InertialCount= 0;
    }   //记录清零

    /* 用入环 */
//    if (right_circle_flag == 3 || left_circle_flag == 3) {
//        Speed_CircleIn += CarSpeed_x;
//        OutCircle_Inertial += Angle.Output;
//        OutCircle_InertialCount++;
//    }   //记录入环后转角输出
//    if ((right_circle_flag == 4 || left_circle_flag == 4) && OutCircle_InertialCount!= 1) {
//        Speed_CircleIn = Speed_CircleIn / OutCircle_InertialCount;
//        OutCircle_Inertial = OutCircle_Inertial / OutCircle_InertialCount;
//        OutCircle_InertialCount = 1;
//    }   //用入环计算环岛内平均打角
    /* 用环内 */
//    if (right_circle_flag == 4 || left_circle_flag == 4) {
    if (Attitude_Err > 1000 && Attitude_Err < 2250 && (right_circle_flag == 4 || left_circle_flag == 4)) {
        OutCircle_Inertial += Angle.Output;
        OutCircle_InertialCount++;
    }   //记录环内转角输出
    if ((right_circle_flag == 5 || left_circle_flag == 5) && OutCircle_InertialCount != 1) {
        OutCircle_Inertial = OutCircle_Inertial / (float)OutCircle_InertialCount;
        OutCircle_InertialCount = 1;
    }   //用环内计算环岛内平均打角

    //短时间内不再识别第二个环岛
    if (left_circle_flag == 6 || right_circle_flag == 6) {
        OutCircle_Time = 1;
    }
    if (OutCircle_Time !=  0) {
        OutCircle_Time++;   //出环后开始计时
        if (OutCircle_Time < 100) {
            Gear = 0;       //出环后0.4s内不加速
        }
    }
    if ((left_circle_flag == 3 || right_circle_flag == 3) && OutCircle_Time != 0) {
        if (OutCircle_Time < 200) {
            left_circle_flag = right_circle_flag = 0;
            circle_flag = 0;
        }   //0.8s内不再入第二个环
    }
    if (OutCircle_Time >= 205) {
        OutCircle_Time = 0;
    }   //0.82s后停止计时
}

void Speed_Flag_Set(void)
{
    //正常速度
    if (Speed_Set_y_Flag == 1) {
        Speed_Set_y = 40;
    } else if (Speed_Set_y_Flag == 2) {
        Speed_Set_y = 45;
    } else if (Speed_Set_y_Flag == 3) {
        Speed_Set_y = 50;
    } else if (Speed_Set_y_Flag == 4) {
        Speed_Set_y = 55;
    } else if (Speed_Set_y_Flag == 5) {
        Speed_Set_y = 60;
    } else if (Speed_Set_y_Flag == 6) {
        Speed_Set_y = 65;
    } else if (Speed_Set_y_Flag == 7) {
        Speed_Set_y = 70;
    }

    //横向速度
    if (Speed_Set_x_Flag == 1) {
        Speed_Set_x = 35;
    } else if (Speed_Set_x_Flag == 2) {
        Speed_Set_x = 40;
    } else if (Speed_Set_x_Flag == 3) {
        Speed_Set_x = 45;
    } else if (Speed_Set_x_Flag == 4) {
        Speed_Set_x = 50;
    } else if (Speed_Set_x_Flag == 5) {
        Speed_Set_x = 55;
    } else if (Speed_Set_x_Flag == 6) {
        Speed_Set_x = 60;
    }

    //环岛速度
    if (Speed_Set_c_Flag == 1) {
        Speed_Set_c = 45;
    } else if (Speed_Set_c_Flag == 2) {
        Speed_Set_c = 50;
    } else if (Speed_Set_c_Flag == 3) {
        Speed_Set_c = 55;
    } else if (Speed_Set_c_Flag == 4) {
        Speed_Set_c = 60;
    } else if (Speed_Set_c_Flag == 5) {
        Speed_Set_c = 65;
    } else if (Speed_Set_c_Flag == 6) {
        Speed_Set_c = 70;
    }
}

/*
 * @name: void Car_Rush(void);
 * @function: 三叉转车
 * @param: none
 * @return: none
 */
void Car_Rush_Turn(void)
{
    //速度挡位设置
    Speed_Flag_Set();

    //手动设定速度
//    Speed_Set_y = 55;   //正常速度
//    Speed_Set_x = 45;   //横向速度
//    Speed_Set_c = 55;   //环岛速度
    Speed_Set_in = 55;  //入环速度

//Angle_ElementBase_WritePID();
    if (Speed_Set_y == 40) {
        Start_Part = 45;
    } else if (Speed_Set_y == 45) {
        Start_Part = 40;
    } else if (Speed_Set_y == 50) {
        Start_Part = 38;
    } else if (Speed_Set_y == 55) { //4
        Start_Part = 36;
    } else if (Speed_Set_y == 60) {
        Start_Part = 34;
    } else if (Speed_Set_y == 65) {
        Start_Part = 33;
    } else if (Speed_Set_y == 70) {
        Start_Part = 31;
    } else {
        Start_Part = 37;
    }   //正常起始行
    if (flag_y_cross != 0) {
        if (Speed_Set_x == 35) {
            Start_Part = 49;
        } else if (Speed_Set_x == 40) {
            Start_Part = 49;
        } else if (Speed_Set_x == 45) {
            Start_Part = 47;
        } else if (Speed_Set_x == 50) { //4
            Start_Part = 47;
        } else if (Speed_Set_x == 55) {
            Start_Part = 47;
        } else if (Speed_Set_x == 60) {
            Start_Part = 45;
        } else {
            Start_Part = 48;
        }
    }   //横向起始行
    if (left_circle_flag == 3 || right_circle_flag == 3) {
        Start_Part = 45;    //对应Speed_Set_in
    }   //入环起始行
    if (left_circle_flag == 4 || right_circle_flag == 4) {
        if (Speed_Set_c == 45) {
            Start_Part = 40;
        } else if (Speed_Set_c == 50) {
            Start_Part = 38;
        } else if (Speed_Set_c == 55) {
            Start_Part = 36;
        } else if (Speed_Set_c == 60) { //40
            Start_Part = 34;
        } else if (Speed_Set_c == 65) {
            Start_Part = 33;
        } else if (Speed_Set_c == 70) {
            Start_Part = 31;
        } else {
            Start_Part = 37;
        }
    }   //环岛内起始行


    Gyro_Circle_Control();   //陀螺仪出入环检测


//    if (circle_flag != 0) {
//        for (uint8 i = 0; i < NUM; i++) {
//            if ((ELC_Slide[i][2] - ELC_Slide[i][1]) > (ELC_Slide[i][1]*0.8f))         ELC_Circle_Flag = 1;
//            else if ((ELC_Slide[i][1] - ELC_Slide[i][0]) > (ELC_Slide[i][0]*0.8f))    ELC_Circle_Flag = 1;
//        }
//        if (ELC_Circle_Flag != 1) {
//            ELC_Circle_Flag = 0;
//        }
//    }   //电磁判断环岛

//    if (sumInside >= 5250 && sumOutside >= 1000) {
    if (sumInside >= 5250) {
        ELC_Circle_Flag = 1;
    } else {
        ELC_Circle_Flag = 0;
    }   //电磁判断环岛

    ////////////////////////////////三叉控制////////////////////////////////
    if (flag_y_cross == 1 && (Fork_Road_Count == 1 || Fork_Road_Count == 4)) {
        //入三叉 (右转
        SG_Turn90_Right();  //舵机右转
        if (!Car_AdvanceX_Flag) {
            Car_Advance(45, 600, 2);             //前进一定距离
        }

        if (Car_AdvanceX_Flag) {
            if (!Car_Turn_Flag) {
                Car_Turn(60, 75, 1);            //车身左转
            }

            if (Car_Turn_Flag) {
                Gyro_Integral = 0;              //陀螺仪停止积分

                if (!Car_AdvanceY_Flag) {
                    Car_Translae(60, 1000, 2);  //平移一定距离
                }

                if (Car_AdvanceY_Flag) {
                    if (Gear_Flag) {
                        Speed_Set = Speed_Set_x + 5;
                    } else {
                        Speed_Set = Speed_Set_x;
                    }   //右三叉加速

                    if (flag_c) {
//                        if (AngleOutPut >  10)  AngleOutPut =  10;
//                        if (AngleOutPut < -10)  AngleOutPut = -10;      //识别到出三叉转角限幅

                        Car_Run(0, (Speed_Set + Gear), AngleOutPut);
                    } else {
                        Car_Run(3, (Speed_Set + Gear), AngleOutPut);    //三叉内循迹
                    }
                }
            }
        }
    } else if (flag_y_cross == 2 && (Fork_Road_Count == 1 || Fork_Road_Count == 4) && !Fork_Road_Step) {
        //出三叉前先清零
        Gyro_Integral = 0;
        Attitude_Angel_X = 0;
        Attitude_Yaw = Attitude_Angel_X2 = 0;
        CarTran_x = CarTran_y = 0;
        Attitude_Err = 0;
        Car_AdvanceX_Flag = Car_AdvanceY_Flag = Car_Turn_Flag = 0;
        Fork_Road_Step = 1;
    } else if (flag_y_cross == 2 && (Fork_Road_Count == 1 || Fork_Road_Count == 4) && Fork_Road_Step) {
        //出三叉
        SG_Turn_Front();            //舵机转正
        if (!Car_AdvanceY_Flag) {
            Car_Translae(50, 400, 2);   //向右平移一定距离
        }

        if (Car_AdvanceY_Flag) {
            /* 转车身 */
            if (!Car_Turn_Flag) {
                Car_Turn(60, 1000, 2);  //车身右转
            }
            /* 转方向 */
//            if (Fork_Road_Count == 1) {
//                SG_Turn_Back();
//                Car_Turn_Flag = 1;
//                Car_Direction = 2;
//            } else {
//                SG_Turn_Front();
//                Car_Turn_Flag = 1;
//                Car_Direction = 5;
//            }

            if (Car_Turn_Flag) {
                Gyro_Integral = 0;

                if (!Car_AdvanceX_Flag) {
                    Car_Advance(60, 1000, 1);   //前进一定距离
                }

                if (Car_AdvanceX_Flag) {
                    Fork_Road_Step = 0;
                    Fork_Road_Count++;                          //准备第入三叉另一边
                    if (Fork_Road_Count >= 5) {
                        Fork_Road_Count = 1;
                    }
                    Fork_Count++;
                    flag_y_cross = flag_y_cross_special = 0;    //清除三叉标志位
                }
            }
        }
    } else if (flag_y_cross == 1 && (Fork_Road_Count == 2 || Fork_Road_Count == 3)) {
        //第二次入三叉 (左转
        SG_Turn90_Left();   //舵机左转
        if (!Car_AdvanceX_Flag) {
            Car_Advance(5, 300, 1);             //前进一定距离
        }

        if (Car_AdvanceX_Flag) {
            if (!Car_Turn_Flag) {
                Car_Turn(60, 90, 2);            //车身右转
            }

            if (Car_Turn_Flag) {
                Gyro_Integral = 0;

                if (!Car_AdvanceY_Flag) {
                    Car_Translae(55, 1000, 1);  //平移一定距离
                }

                if (Car_AdvanceY_Flag) {
                    Speed_Set = Speed_Set_x;

                    if (flag_c) {
                        if (AngleOutPut >  10)  AngleOutPut =  10;
                        if (AngleOutPut < -10)  AngleOutPut = -10;      //识别到出三叉转角限幅

                        Car_Run(0, -(Speed_Set + Gear), AngleOutPut);
                    } else {
                        Car_Run(10, -(Speed_Set + Gear), AngleOutPut);
                    }

//                    Car_Run(0, -(Speed_Set + Gear), AngleOutPut);   //三叉内循迹
                }
            }
        }
    } else if (flag_y_cross == 2 && (Fork_Road_Count == 2 || Fork_Road_Count == 3) && !Fork_Road_Step) {
        //出三叉前先清零
        Gyro_Integral = 0;
        Attitude_Angel_X = 0;
        Attitude_Yaw = Attitude_Angel_X2 = 0;
        CarTran_x = CarTran_y = 0;
        Attitude_Err = 0;
        Car_AdvanceX_Flag = Car_AdvanceY_Flag = Car_Turn_Flag = 0;
        Fork_Road_Step = 1;
    } else if (flag_y_cross == 2 && (Fork_Road_Count == 2 || Fork_Road_Count == 3) && Fork_Road_Step) {
        //第二次出三叉
        SG_Turn_Front();            //舵机转正
        if (!Car_AdvanceY_Flag) {
            Car_Translae(45, 800, 1);   //向左平移一定距离
        }

        if (Car_AdvanceY_Flag) {
            /* 转车身 */
            if (!Car_Turn_Flag) {
                Car_Turn(60, 1000, 1);  //车身左转
            }
            /* 转方向 */
//            if (Fork_Road_Count == 2) {
//                SG_Turn_Front();
//                Car_Turn_Flag = 1;
//                Car_Direction = 3;
//            } else {
//                SG_Turn_Back();
//                Car_Turn_Flag = 1;
//                Car_Direction = 4;
//            }

            if (Car_Turn_Flag) {
                Gyro_Integral = 0;

                if (!Car_AdvanceX_Flag) {
                    Car_Advance(60, 1000, 1);   //前进一定距离
                }

                if (Car_AdvanceX_Flag) {
                    Fork_Road_Step = 0;
                    Fork_Road_Count++;                                  //准备第入三叉另一边
                    if (Fork_Road_Count >= 5) {
                        Fork_Road_Count = 1;
                    }
                    Fork_Count++;
                    flag_y_cross = flag_y_cross_special = 0;            //清除三叉标志位
                }
            }
        }
    } else if (left_circle_flag == 3 || right_circle_flag == 3) {
        ////////////////////////////////入环定速////////////////////////////////
        Car_Run_Protect(Speed_Set_in, -5, AngleOutPut, 0);
    } else if (left_circle_flag == 4 || right_circle_flag == 4) {
        ////////////////////////////////环内提速////////////////////////////////
        Car_Run_Protect(Speed_Set_c, 0, AngleOutPut, ELC_Protect_Flag);
    } else if (Gyro_Trans_Flag) {
        //姿态出环并偏移
        Car_Run_Protect(Speed_Set_c, -3, ((int16)(OutCircle_Inertial) - 2), ELC_Protect_Flag);  //用环内并偏移(左
    } else if (right_circle_flag == 5 || left_circle_flag == 5) {
        ////////////////////////////////姿态出环////////////////////////////////
//        Car_Run_Protect(Speed_CircleIn, 0, (int16)OutCircle_Inertial, ELC_Protect_Flag);        //用入环 - 动态速度
//        Car_Run_Protect(Speed_Set_y, 0, (int16)OutCircle_Inertial, ELC_Protect_Flag);           //用入环
        Car_Run_Protect(Speed_Set_c, 0, ((int16)(OutCircle_Inertial) - 2), ELC_Protect_Flag);  //用环内(左
//        Car_Run_Protect(60, 0, -19, 1);
    } else {
        if (!Ramp_Flag && circle_flag == 0 && stop_flag < 3 && Out_Garage) {
            Gyro_Repair_End_Flag = 0;                                   //陀螺仪入环标志位清零
            Gyro_Integral = 0;                                          //陀螺仪停止积分
            Attitude_Angel_X = 0;
            Attitude_Yaw = Attitude_Angel_X2 = 0;                       //陀螺仪积分清零
            CarTran_x = CarTran_y = 0;
            Attitude_Err = 0;
            Car_AdvanceX_Flag = Car_AdvanceY_Flag = Car_Turn_Flag = 0;  //前进平移转角标志位清零
        }   //不是三叉不是环岛 出库完成也不需要入库 清零

        //正常速度设置
        Speed_Set = Speed_Set_y;

        if (circle_flag != 0) {
            Gear = 0;
        }   //环岛关闭加速

        //坡道控制
//        Ramp_Control();

        Car_Run_Protect((Speed_Set + Gear), 0, AngleOutPut, ELC_Protect_Flag);
//        if (Car_Direction%2 != 0) {
//            Car_Run_Protect((Speed_Set + Gear), 0, AngleOutPut, ELC_Protect_Flag);      //正向
//        } else {
//            Car_Run_Protect(-(Speed_Set + Gear), 0, -AngleOutPut, ELC_Protect_Flag);    //反向
//        }
    }

    if (!Out_Garage) {
        //出库
        Car_Out_Garage(Out_Garage_Dir);
    } else {
        ELC_Protect_Flag = 1;
    }   //出库后再开启电磁保护

    //入库
//    if (stop_flag == 3) {
//        Gyro_Integral = 1;
//        Attitude_Err = ABS(Attitude_Yaw);
//        Car_Run_Protect(30, 0, AngleOutPut, 0);
//        if (Attitude_Err >= 600) {
//            Gyro_Integral = 0;
//            stop_flag = 4;
//        }
//    }
//    if (stop_flag == 4) {
//        Car_Advance(40, 2000, 1);
//        if (Car_AdvanceX_Flag == 1) {
////            Car_Run_Protect(0, 0, 0, 1);
//            Car_Stop(); //电磁停车保护比这个稳
//        }
//    }

    if (!HMI_Finish_Flag) {
        Car_State_Flag = 100;
        Car_Run(0, 0, 0);
        Gear = 0;
        Ramp_Flag = 0;
        Angle.Err_Sum = 0.0f;
        ELC_Circle_Flag = 0;
    }   //HMI 确认完毕前不发车

    ///////////////OKOKOK//////////////
}

/*
 * @name:void Car_Test(void)
 * @function:测试速度函数(带意外停车保护
 * @param:none
 * @return:none
 */
void Car_Test(void)
{
    HMI_Finish_Flag = 1;
//    Car_Run(30, 0, 0);
//    Car_Run(0, 0, 0);

    /* 测试陀螺仪环 */
    Speed_Set = 0;
    Car_Run_Protect(Speed_Set, 0, GyroOutPut, 1);
    //保命
    if (GyroOutPut > 80) {
        Car_Run_Protect(0, 0, 0, 1);
        Mortor1.PWM_Write = Mortor2.PWM_Write = Mortor3.PWM_Write = Mortor4.PWM_Write = 0;
    }

    /* 测试横向跑 */
//    SG_Turn90_Right();
//    AnglePID_Parameter_Write(Angle1_PID2_35_Parameter);
//    Car_Run(0, 40, AngleOutPut);

    /* 测试陀螺仪出环 */
//    Gyro_Circle_Control();
//    if (circle_flag == 0) {
//        Gyro_Integral = 0;  //陀螺仪停止积分
//        Attitude_Angel_X = Attitude_Yaw = Attitude_Err = 0; //陀螺仪积分清零
//        Gyro_Repair_End_Flag = 0;   //陀螺仪入环标志位清零
//    }   //不是环 清零
//    if (circle_flag != 0) {
//        Car_Run(0, 0, 0);
//    } else {
//        Car_Run(40, 0, AngleOutPut);
//    }
//    Car_Run(40, 0, AngleOutPut);

    /* 测试电磁环岛 */
//    for (uint8 i = 0; i < NUM; i++) {
//        if ((ELC_Slide[i][2] - ELC_Slide[i][1]) > (ELC_Slide[i][1]*0.8f))         ELC_Circle_Flag = 1;
//        else if ((ELC_Slide[i][1] - ELC_Slide[i][0]) > (ELC_Slide[i][0]*0.8f))    ELC_Circle_Flag = 1;
//    }
//    if (ELC_Circle_Flag != 1) {
//        ELC_Circle_Flag = 0;
//    }
//    if (ELC_Circle_Flag == 1) {
//        Car_Run(0, 0, 0);
//    } else {
//        Car_Run(25, 0, 0);
//    }

//    if (Improper_Row_Count > 0) {
//        Car_Run(0, 0, 0);
//    } else {
//        Car_Run(40, 0, AngleOutPut);
//    }

//    if (!Out_Garage) {
//        Car_Out_Garage(Out_Garage_Dir);
//    }   //出库
//    if (Out_Garage) {
//        Car_Run(0, 0, 0);
//    }
}

/*
 * @name:void Motor_Alert(uint8 Err)
 * @function:电机PID误差超过误差Err蜂鸣器警告
 * @param:none
 * @return:none
 */
void Motor_Alert(uint8 Err)
{
    if( (Mortor1_PID.Err_ABS > Err && ABS(Mortor1_PID.Now_Val) > 5) ||
        (Mortor2_PID.Err_ABS > Err && ABS(Mortor2_PID.Now_Val) > 5) ||
        (Mortor3_PID.Err_ABS > Err && ABS(Mortor3_PID.Now_Val) > 5) ||
        (Mortor4_PID.Err_ABS > Err && ABS(Mortor4_PID.Now_Val) > 5))
    {
        BEEP_ON;
    } else {
        BEEP_OFF;
    }
}

/*
 * @name: KEY_Change(uint8 Param, float Num)
 * @function: 按键调参
 * @param: Param选择调整哪个参数(P1,I2,D3),Num为一次增减的大小
 * @return: none
 * @notice: KEY1按下增大Num，KEY3按下减小Num，KEY2按下则依次选择是调哪个电机
 *          KEY_Change_Count是实际更改了参数的次数 可以让它显示在屏幕上
 * @notice: 不如直接调试用着方便(逃
 */
#pragma region KEY_Change
void KEY_Change(uint8 Param, float Num)
{
    unsigned char KEY_Change_Temp = 0;
    KEY_Change_Temp = KEY_Scan();

    if(KEY_Change_Temp == 1)
    {
        switch(Param) {
        case(1):
            switch(KEY_Change_Param) {
                case(1):
                    PID1_Parameter_M1[0] = PID1_Parameter_M1[0] + Num;
                    KEY_Change_Count++;
                break;
                case(2):
                    PID1_Parameter_M2[0] = PID1_Parameter_M2[0] + Num;
                    KEY_Change_Count++;
                break;
                case(3):
                    PID1_Parameter_M3[0] = PID1_Parameter_M3[0] + Num;
                    KEY_Change_Count++;
                break;
                case(4):
                    PID1_Parameter_M4[0] = PID1_Parameter_M4[0] + Num;
                    KEY_Change_Count++;
                break;
            }
            break;
        case(2):
            switch(KEY_Change_Param) {
                case(1):
                    PID1_Parameter_M1[1] = PID1_Parameter_M1[1] + Num;
                    KEY_Change_Count++;
                break;
                case(2):
                    PID1_Parameter_M2[1] = PID1_Parameter_M2[1] + Num;
                    KEY_Change_Count++;
                break;
                case(3):
                    PID1_Parameter_M3[1] = PID1_Parameter_M3[1] + Num;
                    KEY_Change_Count++;
                break;
                case(4):
                    PID1_Parameter_M4[1] = PID1_Parameter_M4[1] + Num;
                    KEY_Change_Count++;
                break;
            }
            break;
        case(3):
            switch(KEY_Change_Param) {
                case(1):
                    PID1_Parameter_M1[2] = PID1_Parameter_M1[2] + Num;
                    KEY_Change_Count++;
                break;
                case(2):
                    PID1_Parameter_M2[2] = PID1_Parameter_M2[2] + Num;
                    KEY_Change_Count++;
                break;
                case(3):
                    PID1_Parameter_M3[2] = PID1_Parameter_M3[2] + Num;
                    KEY_Change_Count++;
                break;
                case(4):
                    PID1_Parameter_M4[2] = PID1_Parameter_M4[2] + Num;
                    KEY_Change_Count++;
                break;
            }
            break;
        }
    }
    if(KEY_Change_Temp == 2) {
        KEY_Change_Param++;
        if(KEY_Change_Param >= 5)    KEY_Change_Param = 1;
    }
    if(KEY_Change_Temp == 3) {
        switch(Param) {
        case(1):
            switch(KEY_Change_Param) {
                case(1):
                    PID1_Parameter_M1[0] = PID1_Parameter_M1[0] - Num;
                    KEY_Change_Count--;
                break;
                case(2):
                    PID1_Parameter_M2[0] = PID1_Parameter_M2[0] - Num;
                    KEY_Change_Count--;
                break;
                case(3):
                    PID1_Parameter_M3[0] = PID1_Parameter_M3[0] - Num;
                    KEY_Change_Count--;
                break;
                case(4):
                    PID1_Parameter_M4[0] = PID1_Parameter_M4[0] - Num;
                    KEY_Change_Count--;
                break;
            }
            break;
        case(2):
           switch(KEY_Change_Param) {
                case(1):
                    PID1_Parameter_M1[1] = PID1_Parameter_M1[1] - Num;
                    KEY_Change_Count--;
                break;
                case(2):
                    PID1_Parameter_M2[1] = PID1_Parameter_M2[1] - Num;
                    KEY_Change_Count--;
                break;
                case(3):
                    PID1_Parameter_M3[1] = PID1_Parameter_M3[1] - Num;
                    KEY_Change_Count--;
                break;
                case(4):
                    PID1_Parameter_M4[1] = PID1_Parameter_M4[1] - Num;
                    KEY_Change_Count--;
                break;
            }
            break;
        case(3):
           switch(KEY_Change_Param) {
                case(1):
                    PID1_Parameter_M1[2] = PID1_Parameter_M1[2] - Num;
                    KEY_Change_Count--;
                break;
                case(2):
                    PID1_Parameter_M2[2] = PID1_Parameter_M2[2] - Num;
                    KEY_Change_Count--;
                break;
                case(3):
                    PID1_Parameter_M3[2] = PID1_Parameter_M3[2] - Num;
                    KEY_Change_Count--;
                break;
                case(4):
                    PID1_Parameter_M4[2] = PID1_Parameter_M4[2] - Num;
                    KEY_Change_Count--;
                break;
            }
            break;
        }
    }

    if(KEY_Change_Temp != 0) {

        MortorPID_Parameter_Write(PID1_Parameter_M1, PID1_Parameter_M2, PID1_Parameter_M3, PID1_Parameter_M4);
    }
}
#pragma endregion
