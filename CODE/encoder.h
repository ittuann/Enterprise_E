/*
 *  encoder.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */
#ifndef CODE_INC_ENCODER_H_
#define CODE_INC_ENCODER_H_

#include "MY_Bsp.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"

#define ABS_ENCODER_SPI_PC1_PIN     A8
#define ABS_ENCODER_SPI_PC2_PIN     B12
#define ABS_ENCODER_SPI_PC3_PIN     C8
#define ABS_ENCODER_SPI_PC4_PIN     C9

#define EncNum 4    //编码器个数
#define MotNum 4    //电机个数

//Structure
typedef struct
{
    int16 Encoder_Now;
    int16 Encoder_Last;
} EncoderTypeDef_t;

extern  float Encoder_w;
extern  int16 Mortor_RPM_Max, Mortor_RPM_Min;

extern  int16   speed_read_tmp[MotNum];             //用数组暂存电机转速

extern  float   CarSpeed_x, CarSpeed_y, CarSpeed_a; //实际坐标轴转速
extern  int16   Mortor_MaxSpeed, Mortor_MinSpeed, Mortor_AveSpeed;    //电机转速极值
extern  int8    Mortor_MaxSpeedNum, Mortor_MinSpeedNum; //电机转速极值下标

extern  int32   CarDis_x, CarDis_y, CarDis_a;       //实际路程
extern  float   CarTran_x, CarTran_y;               //函数内路程累计(平移一定距离用
extern  float   CarRace_x, CarRace_y, CarRace_a;    //实际速度

//Parameter
extern EncoderTypeDef_t Encoder1, Encoder2, Encoder3, Encoder4;

//Function
extern  int16 Encoder_SLimit(int16 SpeedNew, int16 SpeedOld, int16 Grad);
extern  void Encoder_Parameter_Init(EncoderTypeDef_t *sptr);
extern  void Encoder_Init(void);
extern  void EncoderType_Init(void);
extern  void Speed_Collect(void);
extern  void Speed_Calcu(void);
extern  void Speed_Analyse(void);
extern  void Race_Calcu(void);
extern  void Speed_Dis(void);
extern  void FSS_Print_Encoder(void);

#endif /* CODE_myencoder_H_ */
