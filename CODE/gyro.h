/*
 *  gyro.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef GYRO_H_
#define GYRO_H_

#include "MY_Bsp.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_IIC.h"

typedef struct
{
    int16   x;
    int16   y;
    int16   z;
}ICMDATA;

typedef struct
{
    ICMDATA accdata;
    ICMDATA gyro;
}ICM20602;

typedef struct
{
    float   x;
    float   y;
    float   z;
}ICMDATA_Treated;

typedef struct
{
  ICMDATA_Treated  accdata; //加速度计
  ICMDATA_Treated  gyro;    //角速度
}ICM20602_Treated;

extern ICM20602 ICM_Offset;                 //零漂值
extern ICM20602 ICM_State;                  //原始值
extern ICM20602_Treated ICM_Treated;        //经过滑动处理的数据

extern  uint8   ICM20602_Offset_Finished;   //陀螺仪零偏设置完成标志位
extern  uint8   Offect_Count;               //零偏采集的有效次数
extern  uint8   Offect_Time;                //零偏采集的轮数
extern  int32   offect_temp6;
extern  int32   data_temp6;

extern  int16   ICM_TreatedGyroZ2;
extern  float   Gyro_Z_Last[3];             //陀螺仪上次值 (0是本次1是上次

extern  int16   GYRO_Z_BUF[3];

extern  float   Gyro_Z_FilterLast[3];

extern  int32   Attitude_Angel_X2;
extern  float   GyroZProtect;
extern  int16   Attitude_Yaw;
extern  double  Attitude_Angel_X;

extern  void Gyro_Init(void);
extern  void Gyro_Type_Init(void);
extern  void ICM20602_Offset(void);
extern  void Gyro_Offset(void);
extern  void Gyro_Data_Filter(void);
extern  void Get_Attitude(void);
extern  void Get_Attitude_Kalman(void);
extern  void KalmanFilter(float ACC_Angle,float Gyro_y);
extern  void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

extern  void Gyro_Data_Filter2(void);
extern  void Get_Attitude2(void);

#endif /* CODE_GYRO_H_ */
