/*
 *  define.h
 *
 *  Created on: 2021年1月12日
 *      Author: LBQ
 */

#ifndef _DEFINE_H
#define _DEFINE_H

#include "My_Bsp.h"

#define PartUsed        15      //图像部分误差使用的行数
#define PartUsed_X      15      //横向    图像部分误差使用的行数

/**********************PID***********************/

extern  int16   PID1_Parameter_M1[3];
extern  int16   PID1_Parameter_M2[3];
extern  int16   PID1_Parameter_M3[3];
extern  int16   PID1_Parameter_M4[3];

extern  int16   PID2_Parameter_M1[3];
extern  int16   PID2_Parameter_M2[3];
extern  int16   PID2_Parameter_M3[3];
extern  int16   PID2_Parameter_M4[3];

extern  float   Angle1_PID1_40_Parameter[4];    //正常
extern  float   Angle1_PID1_45_Parameter[4];    //正常
extern  float   Angle1_PID1_50_Parameter[4];    //正常
extern  float   Angle1_PID1_55_Parameter[4];    //正常
extern  float   Angle1_PID1_60_Parameter[4];    //正常
extern  float   Angle1_PID1_65_Parameter[4];    //正常
extern  float   Angle1_PID1_70_Parameter[4];    //正常
extern  float   Angle1_PID1_Parameter[4];       //正常
extern  float   Angle1_PID2_35_Parameter[4];    //横向
extern  float   Angle1_PID2_40_Parameter[4];    //横向
extern  float   Angle1_PID2_45_Parameter[4];    //横向
extern  float   Angle1_PID2_50_Parameter[4];    //横向
extern  float   Angle1_PID2_55_Parameter[4];    //横向
extern  float   Angle1_PID2_60_Parameter[4];    //横向
extern  float   Angle1_PID2_Parameter[4];       //横向
extern  float   Angle1_PID3_45_Parameter[4];    //环岛
extern  float   Angle1_PID3_50_Parameter[4];    //环岛
extern  float   Angle1_PID3_55_Parameter[4];    //环岛
extern  float   Angle1_PID3_60_Parameter[4];    //环岛
extern  float   Angle1_PID3_65_Parameter[4];    //环岛
extern  float   Angle1_PID3_70_Parameter[4];    //环岛
extern  float   Angle1_PID3_Parameter[4];       //环岛
extern  float   Angle1_PID4_45_Parameter[4];    //入环
extern  float   Angle1_PID4_50_Parameter[4];    //入环
extern  float   Angle1_PID4_55_Parameter[4];    //入环

extern  float   Angle1_PID6_Parameter[4];       //入库

extern  float   Angle2_PID1_Parameter[4];

extern  float   Gyro_PID_Parameter[4];

extern  int16   Mortor_PWM_Max;
extern  int16   Mortor_PWM_Min;

/**********************CONTROL***********************/

extern  uint8 weight1[60];
/*****控制*****/

extern  int8    Speed_Set_y;            //正常期望速度
extern  int8    Speed_Set_x;            //横向速度
extern  int8    Speed_Set_c;            //环岛速度
extern  int8    Speed_Set_in;           //入环速度
extern  uint8   Speed_Set_y_Flag;       //正常期望速度 档位
extern  uint8   Speed_Set_x_Flag;       //横向速度 档位
extern  uint8   Speed_Set_c_Flag;       //环岛速度 档位
extern  uint8   ELC_Protect_Flag;       //电磁出赛道路保护标志位
extern  int8    Speed_Set;              //设定速度
extern  int16   Speed_CircleIn;         //入环速度
extern  uint8   middle_standard;        //摄像头中线中值
extern  uint8   Car_Direction;          //前进方向 1为正向2为反向
extern  float   AngleErr0, AngleErr1, AngleErr2;
extern  float   AngleErr;               //图像计算出的误差
extern  float   FusionErr;              //摄像头与电磁融合误差
extern  float   FusionErr_Last;         //上次磁融合误差
extern  int16   AngleOutPut;            //转角转速输出
extern  int16   Gyro_K;                 //陀螺仪放大系数
extern  int16   GyroOutPut;             //角加速度输出
//Angle_Part_Process()
extern  uint8   Start_Part;             //部分图像计算偏差开始行
extern  uint8   End_Part;               //部分图像计算偏差截至行
extern  float   Part_Err[PartUsed];     //图像部分误差
extern  uint8   Improper_Row[PartUsed]; //控制不能用的图像行
extern  uint8   Improper_Row_Count;     //控制不能用的图像总行数
//Angle_All_Process()
extern  float   midlinenow;             //全图像计算出的当前误差
extern  float   midline_f, midline_ff, midline_fff;
extern  float   Slope;                  //最小二乘法计算截距值(没啥比用)
extern  float   Regression_A, Regression_B;
//Gear
extern  int8    MidlineGear;            //Midline_Change加速
extern  int8    SlGear;                 //Speed_Change加速
extern  int8    SnGear;                 //Speed_Change_Near加速
extern  int8    DisGear;                //Dis_Control加速
extern  int8    VrGear;                 //ValidRow_Change加速
extern  int8    GearCount;              //平滑加速
extern  int8    Gear;                   //加速
extern  uint8   Fork_Count;             //三叉计数
extern  uint8   Fork_Gear_Flag;         //三叉驾驶标志位
extern  int8    FrontErr;               //前1/3中线误差(用于加减速
extern  uint16  Dis_Acc_Count;          //距离控制加速标
//Angle_Process()
extern  uint8   aim_line;               //打角行
extern  uint8   speed_line;             //电机取值行
extern  uint8   stright_speed_line;     //直道加速行
extern  uint8   circle_aim_line;        //圆环的打脚行
extern  uint8   prospect_row;           //前瞻位置
extern  uint8   aim_line1;
extern  uint8   speedline_err_abs;

//ELC
extern  int16   sumOutside, subOutside; //电磁外侧和 外侧差
extern  int16   sumInside, subInside;   //电磁内测和 内测差
extern  float   ELCErr;                 //电磁误差
//Others
extern  uint8   Beep_Flag;              //蜂鸣器开关标志位
extern  uint8   Ramp_Flag;              //坡道检测标志位
extern  uint8   Ramp_TimeCount;         //坡道检测时间
extern  uint8   Gyro_Start_Flag;        //开启陀螺仪标志位(一直开
extern  uint8   Gyro_Integral;          //陀螺仪开始积分标志位
extern  uint8   ELC_ClaerGuiyi_Flag;    //电磁归一化基准值清回标志位
extern  int16   Attitude_Err;           //陀螺仪转角误差
extern  uint8   Gyro_Repair_End_Flag;   //陀螺仪出环标志位
extern  uint8   Gyro_StartIdOutCir_Flag;//陀螺仪给出标志位开始识别出环
extern  uint8   Gyro_TimeOutCir_Flag;   //陀螺仪该出环了标志位
extern  uint8   Gyro_Trans_Flag;        //出环平移标志位
extern  uint8   Gyro_OutCircle_Flag;    //陀螺仪出环标志位
extern  uint8   ELC_Circle_Flag;        //电磁判断环岛标志位
extern  uint8   Car_Protect_Flag;       //停车保护标志位
extern  uint8   Car_Protect;            //停车保护状态
extern  uint8   ELC_Protect_Count;      //电磁出赛道路计数
extern  uint8   ELC_Protect_State;      //电磁出赛道路保护标志位
extern  float   OutCircle_Inertial;     //环岛内平均打角
extern  int16   OutCircle_InertialCount;//环岛内平均打角计数
extern  int16   OutCircle_Time;         //出环岛后的时间

/*****标志位*****/
extern  uint8   Photo_Finsh_Flag;       //摄像头处理完成标志位
extern  uint8   Car_State_Flag;         //当前状态
extern  uint8   Fork_Road_Count;        //第几次进入三叉的一边标志位
extern  uint8   Fork_Road_Step;
//Others
extern  uint8   Out_Garage;             //出库标志位
extern  uint8   Out_Garage_Dir;         //出库方向
extern  uint8   Gear_Flag;              //加速全局标志位
extern  uint8   Car_AdvanceX_Flag;      //前进一段距离标志位
extern  uint8   Car_AdvanceY_Flag;      //前进一段距离标志位
extern  uint8   Car_Turn_Flag;          //转向一定角度标志位
//Useless


/*****其他*****/
extern  uint16  System_Time;            //系统运行时间
extern  uint32  Tim_Count;              //记录运行时间
extern  uint8   PID_Count;              //PID中断计数
extern  uint8   Collection_Count;       //采集中断计数
extern  uint8   Screen_Slow_Start;      //计时1s后开启屏幕标志位
extern  int8    KEY_Change_Count;       //按键调参
extern  uint8   KEY_Change_Param;       //按键调参选择电机
extern  uint16  SechErr[41];            //Sech(Err)误差0-40对应到0-6, 并放大10000倍

/**********************OTHER***********************/

#endif  /* CODE_DEFINE_H_ */
