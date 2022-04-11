/*
 *  define.c
 *
 *  Created on: 2021年1月12日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "define.h"

/**********************PID***********************/
//PIDi_Parameter_Mx[3] = {Kp, Ki, Kd};
//Matlab参数 0.2s
int16   PID1_Parameter_M1[3] = { 327 , 1 , 0 };
int16   PID1_Parameter_M2[3] = { 356 , 1 , 0 };
int16   PID1_Parameter_M3[3] = { 337 , 1 , 0 };
int16   PID1_Parameter_M4[3] = { 345 , 1 , 0 };
//横向PID
int16   PID2_Parameter_M1[3] = { 291 , 1 , 0 };
int16   PID2_Parameter_M2[3] = { 247 , 1 , 0 };
int16   PID2_Parameter_M3[3] = { 211 , 1 , 0 };
int16   PID2_Parameter_M4[3] = { 184 , 1 , 0 };

//int16   PID1_Parameter_M1[3] = { 436 , 47 , 23 };
//int16   PID1_Parameter_M1[3] = { 273 , 54 , 21 };
//int16   PID1_Parameter_M2[3] = { 273 , 54 , 21 };
//int16   PID1_Parameter_M3[3] = { 273 , 54 , 21 };
//int16   PID1_Parameter_M4[3] = { 273 , 54 , 21 };

//Anglei_PIDj_Parameter[4] = {Kp, Ki, Kd, Kl};
//Angle1角度环

///////////////////////////////////正常//////////////////////////////////////////
/* 正常 40 Start_Part,45 PartUsed,15 */
float   Angle1_PID1_40_Parameter[4] = { 0.605f , 0 , 0.235f , 77 };     //正常1 40
/* 正常 45 Start_Part,40 PartUsed,15 */
float   Angle1_PID1_45_Parameter[4] = { 0.505f , 0 , 0.115f , 77 };     //正常2 45
/* 正常 50 Start_Part,38 PartUsed,15 */
float   Angle1_PID1_50_Parameter[4] = { 0.515f , 0 , 0.085f , 77 };     //正常3 50
/* 正常 55 Start_Part,36 PartUsed,15 */
float   Angle1_PID1_55_Parameter[4] = { 0.465f , 0 , 0.075f , 77 };     //正常4 55
/* 正常 60 Start_Part,34 PartUsed,15 */
float   Angle1_PID1_60_Parameter[4] = { 0.435f , 0 , 0.055f , 77 };     //正常5 60
/* 正常 65 Start_Part,35 PartUsed,15 */
float   Angle1_PID1_65_Parameter[4] = { 0.415f , 0 , 0.045f , 77 };     //正常6 65
/* 正常 70 Start_Part,35 PartUsed,15 */
float   Angle1_PID1_70_Parameter[4] = { 0.415f , 0 , 0.045f , 77 };     //正常7 70
//速度不等于上述挡位时
float   Angle1_PID1_Parameter[4] = { 0.335f , 0 , 0.155 , 77 };         //正常

///////////////////////////////////横向//////////////////////////////////////////
/* 横向 35 Start_Part,49 PartUsed,15 */
float   Angle1_PID2_35_Parameter[4] = { 0.565f , 0 , 0.185f , 77 };     //横向1  35
/* 横向 40 Start_Part,49 PartUsed,15 */
float   Angle1_PID2_40_Parameter[4] = { 0.555f , 0 , 0.175f , 77 };     //横向2  40
/* 横向 45 Start_Part,49 PartUsed,15 */
float   Angle1_PID2_45_Parameter[4] = { 0.795f , 0 , 0.085f , 77 };     //横向3  45
/* 横向 50 Start_Part,49 PartUsed,15 */
float   Angle1_PID2_50_Parameter[4] = { 0.880f , 0 , 0.065f , 77 };     //横向4  50
/* 横向 55 Start_Part,49 PartUsed,15 */
float   Angle1_PID2_55_Parameter[4] = { 0.815f , 0 , 0.055f , 77 };     //横向5  55
/* 横向 60 Start_Part,49 PartUsed,15 */
float   Angle1_PID2_60_Parameter[4] = { 0.755f , 0 , 0.045f , 77 };     //横向6  60
//速度不等于上述挡位时
float   Angle1_PID2_Parameter[4] = { 0.340f , 0 , 0.145f , 77 };        //横向

///////////////////////////////////环岛//////////////////////////////////////////
/* 环岛内 45 Start_Part,40 PartUsed,15 */
float   Angle1_PID3_45_Parameter[4] = { 0.475f , 0 , 0.165f , 77 };     //环岛内1 45
/* 环岛内 50 Start_Part,38 PartUsed,15 */
float   Angle1_PID3_50_Parameter[4] = { 0.505f , 0 , 0.085f , 77 };     //环岛内2 50
/* 环岛内 55 Start_Part,38 PartUsed,15 */
float   Angle1_PID3_55_Parameter[4] = { 0.495f , 0 , 0.085f , 77 };     //环岛内3 55
/* 环岛内 60 Start_Part,38 PartUsed,15 */
float   Angle1_PID3_60_Parameter[4] = { 0.485f , 0 , 0.065f , 77 };     //环岛内4 60
/* 环岛内 65 Start_Part,38 PartUsed,15 */
float   Angle1_PID3_65_Parameter[4] = { 0.455f , 0 , 0.055f , 77 };     //环岛内5 65
/* 环岛内 70 Start_Part,38 PartUsed,15 */
float   Angle1_PID3_70_Parameter[4] = { 0.455f , 0 , 0.055f , 77 };     //环岛内6 70
//环岛内速度不等于上述挡位时
float   Angle1_PID3_Parameter[4] = { 0.435f , 0 , 0.255f , 77 };        //环岛内
//入环 45
float   Angle1_PID4_45_Parameter[4] = { 0.385f , 0 , 0.185f , 77 };     //入环 45
//入环 50
float   Angle1_PID4_50_Parameter[4] = { 0.365f , 0 , 0.165f , 77 };     //入环 50
//入环 55
float   Angle1_PID4_55_Parameter[4] = { 0.525f , 0 , 0.115f , 77 };     //入环 55

///////////////////////////////////入库//////////////////////////////////////////
float   Angle1_PID6_Parameter[4] = { 0.485f , 0 , 0.285f , 77 };        //入库 40



///////////////////////////////////没用到//////////////////////////////////////////
//Angle2角加速度环
float   Angle2_PID1_Parameter[4] = { 80.0f , 6.0f , 2.0f , 1000};

//Gyro_PID_Parameter[3] = {Kp, Ki, Kd};
float   Gyro_PID_Parameter[4] = { 3.0f , 0 , 0 , 1000};

int16   Mortor_PWM_Max =  9500;
int16   Mortor_PWM_Min = -9500;

/**********************CONTROL***********************/

uint8   weight1[60]={                       //0为图像最顶行
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         1, 1, 3, 3, 5, 5,10,10,10,10,
        10,10,10,10,10,10,10,10,10,10,
        10,10,10,10,10,10,10,10,10,10,
        10,10,10,10,10, 5, 5, 5, 5, 5,
         3, 3, 3, 1, 1, 1, 0, 0, 0, 0,};    //基础    //注意斜率变化引起的跳变,要平滑

/*****控制*****/
int8    Speed_Set_y = 0;            //正常期望速度
int8    Speed_Set_x = 0;            //横向速度
int8    Speed_Set_c = 0;            //环岛速度
int8    Speed_Set_in = 0;           //入环速度
uint8   Speed_Set_y_Flag = 4;       //正常期望速度 档位
uint8   Speed_Set_x_Flag = 4;       //横向速度 档位
uint8   Speed_Set_c_Flag = 4;       //环岛速度 档位
uint8   ELC_Protect_Flag = 0;       //电磁出赛道路保护标志位
int8    Speed_Set = 0;              //设定速度
int16   Speed_CircleIn = 0;         //入环速度
uint8   middle_standard = 39;       //摄像头中值
uint8   Car_Direction = 1;          //前进方向 奇数为正向, 偶数为反向
float   AngleErr0 = 0.000f, AngleErr1 = 0.000f, AngleErr2 = 0.000f;
float   AngleErr = 0.000f;          //图像计算出的误差
int16   AngleOutPut = 0;            //转角转速输出
float   FusionErr = 0.000f;         //摄像头与电磁融合误差
float   FusionErr_Last = 0.000f;    //上次磁融合误差
int16   Gyro_K = 2;                 //陀螺仪放大系数
int16   GyroOutPut = 0;             //角加速度输出
//Angle_Part_Process()
uint8   Start_Part = 45;             //部分图像计算偏差起始行
uint8   End_Part = 30;               //部分图像计算偏差截至行
float   Part_Err[PartUsed] = {0.000f};  //图像部分误差 0是用的最底行
uint8   Improper_Row[PartUsed] = {0};   //控制不能用的图像行
uint8   Improper_Row_Count = 0;     //控制不能用的图像总行数
//Angle_All_Process()
float   midlinenow = 0.000f;        //全图像计算出的当前误差
float   midline_f = 0.000f, midline_ff = 0.000f, midline_fff = 0.000f;
float   Slope = 0.000f;             //最小二乘法计算截距值(没啥比用
float   Regression_A = 0.000f, Regression_B = 0.000f;
//Gear
int8    MidlineGear = 0;            //Midline_Change加速
int8    SlGear = 0;                 //Speed_Change加速
int8    SnGear = 0;                 //Speed_Change_Near加速
int8    DisGear = 0;                //Dis_Control加速
int8    VrGear = 0;                 //ValidRow_Change加速
int8    GearCount = 0;              //平滑加速
int8    Gear = 0;                   //加速
uint8   Fork_Count = 0;             //三叉计数
uint8   Fork_Gear_Flag = 0;         //三叉驾驶标志位
int8    FrontErr = 0;               //前1/3中线误差(用于加减速
uint16  Dis_Acc_Count = 0;          //距离控制加速标
//Angle_Process()
uint8   aim_line = 27;              //打角行 原27
uint8   speed_line = 20;            //电机取值行 原20
uint8   stright_speed_line = 25;    //直道加速行 原25
uint8   circle_aim_line = 29;       //圆环的打脚行
uint8   prospect_row = 0;           //前瞻位置
uint8   aim_line1 = 0;
uint8   speedline_err_abs = 0;
//ELC
int16   sumOutside = 0, subOutside = 0; //电磁外侧和 外侧差
int16   sumInside = 0, subInside = 0;   //电磁内测和 内测差
float   ELCErr = 0.000f;            //电磁误差
//Others
uint8   Beep_Flag = 0;              //蜂鸣器开关标志位
uint8   Ramp_Flag = 0;              //坡道检测标志位
uint8   Ramp_TimeCount = 0;         //坡道检测时间
uint8   Gyro_Start_Flag = 1;        //开启陀螺仪标志位(一直开
uint8   Gyro_Integral = 0;          //陀螺仪开始积分标志位
uint8   ELC_ClaerGuiyi_Flag = 0;    //电磁归一化基准值清回标志位
int16   Attitude_Err = 0;           //陀螺仪转角误差
uint8   Gyro_Repair_End_Flag = 0;   //陀螺仪停止入环补线标志位
uint8   Gyro_StartIdOutCir_Flag = 0;//陀螺仪给出标志位开始识别出环
uint8   Gyro_TimeOutCir_Flag = 0;   //陀螺仪该出环了标志位
uint8   Gyro_Trans_Flag = 0;        //出环平移标志位
uint8   Gyro_OutCircle_Flag = 0;    //陀螺仪出环标志位
uint8   ELC_Circle_Flag = 0;        //电磁判断环岛标志位
uint8   Car_Protect_Flag = 0;       //停车保护标志位
uint8   Car_Protect = 0;            //停车保护状态
uint8   ELC_Protect_Count = 0;      //电磁出赛道路计数
uint8   ELC_Protect_State = 0;      //电磁出赛道路计数
float   OutCircle_Inertial = 0;     //环岛内平均打角
int16   OutCircle_InertialCount = 0;//环岛内平均打角计数
int16   OutCircle_Time = 0;         //出环岛后的时间


/*****标志位*****/
uint8   Photo_Finsh_Flag = 0;       //摄像头处理完成标志位
uint8   Car_State_Flag = 0;         //当前状态
uint8   Fork_Road_Count = 1;        //第几次进入三叉的一边标志位
uint8   Fork_Road_Step = 0;
//Others
uint8   Out_Garage = 1;             //出库标志位
uint8   Out_Garage_Dir = left;      //出库方向
//uint8   Out_Garage_Dir = right;     //出库方向
uint8   Gear_Flag = 0;              //加速全局标志位
uint8   Car_AdvanceX_Flag = 0;      //前进一段距离标志位
uint8   Car_AdvanceY_Flag = 0;      //平移一段距离标志位
uint8   Car_Turn_Flag = 0;          //转向一定角度标志位
//Useless


/*****其他*****/
uint16  System_Time = 0;            //系统运行时间
uint32  Tim_Count = 0;              //记录运行时间
uint8   PID_Count = 0;              //PID中断计数
uint8   Collection_Count = 0;       //采集中断计数
uint8   Screen_Slow_Start = 0;      //计时1s后开启屏幕标志位
int8    KEY_Change_Count = 0;       //按键调参
uint8   KEY_Change_Param = 1;       //按键调参选择电机

/**********************OTHER***********************/

uint16  SechErr[41] = {
    10000, 9888, 9566, 9066, 8435, 7723, 6977, 6235, 5522, 4858, 4250,
    3704, 3218, 2789, 2412, 2084, 1799, 1552, 1338, 1153, 993,
    855, 736, 634, 546, 470, 404, 348, 299, 258, 222,
    191, 164, 141, 121, 104,  90,  77,  66,  57,  49};  //Sech(Err)误差0-40对应到0-6, 并放大10000倍
