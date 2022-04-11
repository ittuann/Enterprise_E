/*
 *  magnet.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef _myadc_h
#define _myadc_h

#include "MY_Bsp.h"

/*
 ||-----------------------------左-- --右------------------------------||
 ||---------------------------侧---------侧----------------------------||
 ||-------------------------第-------------第--------------------------||
 ||---左侧第1个电感--------2------------------2--------右侧第1个电感---||
 ||----------------------个---------------------个---------------------||
 ||--------------------电--------------------------电------------------||
 ||-----------------感--------------------------------感---------------||
 ||---LLADC0--------------LADC1--------------RADC2------------RRADC3---||
 */

#define LLADC_PIN       ADC_IN9_B1      //左侧第1个电感，垂直赛道
#define LADC_PIN        ADC_IN6_A6      //左侧第2个电感，与赛道夹角约30度
#define RADC_PIN        ADC_IN8_B0      //右侧第2个电感，与赛道夹角约30度
#define RRADC_PIN       ADC_IN4_A4      //右侧第1个电感，垂直赛道
//#define Voltage_PIN     ADC_IN9_B1      //电池电压

#define NUM     4       //电感个数
#define Times   10      //滤波深度
#define NM      3       //滤波结果保留个数

extern  uint16  elc_tmp[NUM][3];        //暂存原始采集值
extern  uint16  ELC_Value[NUM];         //计算后电感平均值
extern  uint16  ELC_Slide[NUM][NM];     //存储计算后平均电感值
extern  uint16  ELC_KF[NUM];            //卡尔曼滤波电感值
extern  uint16  ELC_Guiyi[NUM];         //电感归一值
//extern  float   Battery;              //电池电压

extern  uint16  MagneticField;          //赛道整体强度
extern  uint8   MaxELCNum;              //最大值电感序号
extern  uint16  MaxELC;                 //最大电感值
extern  uint16  MaxELC_Guiyi, MaxELC_Guiyi_Old; //电感最大值

typedef struct
{
    uint16  ELC_Input;              //输入值
    uint16  Priori_Estimation;      //先验估计
    uint16  Posterior_Estimation;   //后验估计
    int16   Priori_Convariance;     //先验方差
    int16   Posterior_Convariance;  //后验方差
    float   KalmanGain;             //卡尔曼增益
    uint16  Q;                      //过程噪声协方差
    uint16  R;                      //测量测量噪声协方差
}KFPTypeDef_t;   //用浮点数更好

extern KFPTypeDef_t ELC_KFP0, ELC_KFP1, ELC_KFP2, ELC_KFP3;

extern  void Magnet_ADC_Init(void);
extern  void ELC_Original(void);
extern  void ELC_Get(void);
extern  void ELC_Filter(void);
extern  void ELC_Analyse(void);
extern  void Max_Sample(void);
extern  void Magnet_KalmanFilter(KFPTypeDef_t *kfp);
extern  void Magnet_KalmanFilter_Init(KFPTypeDef_t *kfp);
extern  void Bubble_Sort(uint16 arr[], uint8 len);
extern  uint16 GetNormalizedADValue(uint16 ADinput, uint16 Min_val, uint16 Max_val, uint16 Normalize_MIN, uint16 Normalize_MAX);
extern  void InductorNormal(void);

void Guiyi(void);
void Bizhifa(void);

#endif /* CODE_myadc_H_ */
