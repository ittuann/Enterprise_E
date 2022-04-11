/*
 *  magent.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "define.h"
#include <math.h>
#include "magnet.h"

KFPTypeDef_t ELC_KFP0, ELC_KFP1, ELC_KFP2, ELC_KFP3;

uint16  elc_tmp[NUM][3] = {0};      //暂存原始采集值
uint16  ELC_Value[NUM] = {0};       //计算后平均电感值
uint16  ELC_Slide[NUM][NM] = {0};   //存储计算后平均电感值
uint16  ELC_KF[NUM] = {0};          //卡尔曼滤波电感值
uint16  ELC_Guiyi[NUM] = {0};       //电感归一值
float   ELC_1stLPF[NUM][2] = {0.000f}; //暂存一阶RC低通滤波值
float   ELC_GuiyiRate[NUM] = {1.00f, 1.00f, 1.00f, 1.00f};  //电感归一值倍率

uint16  MagneticField = 0;          //赛道整体强度
uint8   MaxELCNum = 0;              //最大值电感序号
uint16  MaxELC = 0;                 //最大电感值
uint16  MaxELC_Guiyi = 0, MaxELC_Guiyi_Old = 0;   //最大电感归一值

uint16 ad_min[NUM] = {128, 128, 128, 128};
uint16 ad_max[NUM] = {2048, 2048, 2048, 2048};    //归一化参考历史极值

/*
 * @name: void Magnet_ADC_Init(void);
 * @function: 电磁ADC初始化
 * @param: none
 * @return: none
 */
void Magnet_ADC_Init(void)
{
    uint8 i;

    adc_init(LLADC_PIN);
    adc_init(LADC_PIN);
    adc_init(RADC_PIN);
    adc_init(RRADC_PIN);
//    adc_init(Voltage_PIN);  //电池电压

    Magnet_KalmanFilter_Init(&ELC_KFP0);
    Magnet_KalmanFilter_Init(&ELC_KFP1);
    Magnet_KalmanFilter_Init(&ELC_KFP2);
    Magnet_KalmanFilter_Init(&ELC_KFP3);

    for (i = 0; i < 30; i++) {
//        systick_delay_ms(2);           //与中断采集时间相同
//        ELC_Get();
    }   //初始化电感时就采集满缓存数组

    for (i = 0; i < 5; i++) {
//        systick_delay_ms(6);           //与中断采集时间相同
//        ELC_Get();
//        ELC_Filter();
    }
}

/*
 * @name: void ELC_Original(void);
 * @function: 电感原始值获取
 * @param: none
 * @return: none
 */
void ELC_Original(void)
{
    uint8 i, j;
    uint16 ELC_Max = 4096, ELC_Min = 0;

    //存储旧值
    for (i = 0; i < NUM; i++) {
        elc_tmp[i][0] = elc_tmp[i][1];
    }

    for (j = 2; j > 0; j--) {
        //获取一次新值
        elc_tmp[0][j] = adc_convert(LLADC_PIN, ADC_12BIT) + 1;  //+1确保不出BUG
        elc_tmp[1][j] = adc_convert(LADC_PIN, ADC_12BIT) + 1;
        elc_tmp[2][j] = adc_convert(RADC_PIN, ADC_12BIT) + 1;
        elc_tmp[3][j] = adc_convert(RRADC_PIN, ADC_12BIT) + 1;

        //单次采集限幅
        for (i = 0; i < NUM; i++) {
            elc_tmp[i][j] = ((elc_tmp[i][j]) < (ELC_Min)) ? (ELC_Min) : ( ((elc_tmp[i][j]) > (ELC_Max)) ? (ELC_Max) : (elc_tmp[i][j]));
        }
    }
}

/*
 * @name: void ELC_Get(void);
 * @function: 电感获取值并简易滤波处理
 * @param: none
 * @return: none
 */
void ELC_Get(void)
{
    uint8   i, j, k;
//    const  float   lpf_factor = 0.75855f;//一阶RC低通滤波参数
    static uint16  ELC_Times[NUM][Times] = {0};//暂存Times次滤波值
    uint16  ELC_Sort[NUM][Times] = {0}; //暂存排序值
    uint16  ELC_Cent[NUM] = {0};        //暂存中值滤波结果
    const  uint8 ELC_NM[NM] = {2, 3, 6}; //加权系数表
    uint8   sum_weight = 0;
    uint16  bubble_sort_temp = 0;       //暂存冒泡排序交换量
    uint8   lastSwapIndex = 0;          //冒泡排序最后一次交换的下标
    uint8   lastSwapIndexTemp = 0;

    ELC_Original(); //采集一次新值

    for (i = 0; i < NUM; i++)   //一次循环内算完一个电感
    {
        /* 存储新值 */
        ELC_Times[i][Times-1] = (elc_tmp[i][2] + elc_tmp[i][1] + elc_tmp[i][0]) / 3;

        /* 简单强制消抖 */
        if ((ABS(ELC_Times[i][Times-1] - ELC_Times[i][Times-2]) + ABS(ELC_Times[i][Times-2] - ELC_Times[i][Times-3])) < 5) {
            ELC_Times[i][Times-1] = ELC_Times[i][Times-2];
        }

        for (j = Times - 1; j > 0; j--) {
            ELC_Times[i][j-1] = ELC_Times[i][j];
        }   //更新旧值

        /* 存储待排序值 */
//        for (j = 0; j < times; j++) {
//            ELC_Sort[i][j] = ELC_Times[i][j];
//        }
        memcpy(ELC_Sort[i], ELC_Times[i], sizeof(ELC_Times[i]));

        /* 冒泡升序排序 */
        lastSwapIndexTemp = Times - 1;
        for (k = 0; k < Times - 1; k++) {
            lastSwapIndex = lastSwapIndexTemp;
            for (j = 0; j < lastSwapIndex; j++) {
                if (ELC_Sort[i][j] > ELC_Sort[i][j + 1]) {
                    bubble_sort_temp = ELC_Sort[i][j];
                    ELC_Sort[i][j] = ELC_Sort[i][j + 1];
                    ELC_Sort[i][j + 1] = bubble_sort_temp;
                    lastSwapIndexTemp = j;
                }
            }
            if (lastSwapIndexTemp == 0) {
                break;
            }
        }

        /* 中值滤波 */
        ELC_Cent[i] = 0;    //先清零
        for (k = 2; k < (Times - 2); k++) {
            ELC_Cent[i] += ELC_Sort[i][k];
        }
        ELC_Cent[i] = ELC_Cent[i] / (Times - 4);    //去除四个极值再计算均值

        /* 一阶RC低通滤波  */
//        ELC_1stLPF[i][1] = lpf_factor * ELC_Cent[i] + (1.000f - lpf_factor) * ELC_1stLPF[i][0]; //注意小数舍弃带来的误差
//        //可以优化为整数乘除法 或是用移位器和加法器来代替乘法
//        if (isnanf(ELC_1stLPF[i][1]) || isinff(ELC_1stLPF[i][1])) {
//            if (isnanf(ELC_1stLPF[i][0]) || isinff(ELC_1stLPF[i][0])) {
//                ELC_1stLPF[i][1] = ELC_1stLPF[i][0] = 0.000f;
//            } else {
//                ELC_1stLPF[i][1] = ELC_1stLPF[i][0];
//            }
//        }
//        ELC_1stLPF[i][0] = ELC_1stLPF[i][1];    //更新旧值

        /* 滑动递推加权均值滤波 */
//        ELC_Slide[i][NM - 1] = ELC_1stLPF[i][1];    //存入最新值
        ELC_Slide[i][NM - 1] = ELC_Cent[i];         //存入最新值
        ELC_Value[i] = 0;   //先清零
        sum_weight = 0;
        for (k = 0; k < NM; k++) {
            sum_weight += ELC_NM[k];
            ELC_Value[i] += ELC_Slide[i][k] * ELC_NM[k];
        }
        ELC_Value[i] = ELC_Value[i] / sum_weight;   //加权计算均值
        ELC_Slide[i][NM - 1] = ELC_Value[i];        //存入滤波后电感值
        for (k = NM - 1; k > 0; k--) {
            ELC_Slide[i][k-1] = ELC_Slide[i][k];
        }   //更新旧值
    }
}

/*
 * @name: void ELC_Filter(void);
 * @function: 电感滤波
 * @param: none
 * @return: none
 */
void ELC_Filter(void)
{
    /* 写入简易滤波后初始值 */
    ELC_KFP0.ELC_Input = ELC_Slide[0][1];
    ELC_KFP1.ELC_Input = ELC_Slide[1][1];
    ELC_KFP2.ELC_Input = ELC_Slide[2][1];
    ELC_KFP3.ELC_Input = ELC_Slide[3][1];

    /* 卡尔曼滤波 */
    Magnet_KalmanFilter(&ELC_KFP0);
    Magnet_KalmanFilter(&ELC_KFP1);
    Magnet_KalmanFilter(&ELC_KFP2);
    Magnet_KalmanFilter(&ELC_KFP3);

    /* 写入滤波后数值 */
    ELC_KF[0] = ELC_KFP0.Posterior_Estimation;
    ELC_KF[1] = ELC_KFP1.Posterior_Estimation;
    ELC_KF[2] = ELC_KFP2.Posterior_Estimation;
    ELC_KF[3] = ELC_KFP3.Posterior_Estimation;
}

/*
 * @name: void ELC_Analyse(void);
 * @function: ELC_Analyse
 * @param: none
 * @return: none
 */
void ELC_Analyse(void)
{
    Max_Sample();

    MagneticField = ELC_Guiyi[0] + ELC_Guiyi[1] + ELC_Guiyi[2] + ELC_Guiyi[3];  //赛道整体强度

//    if(MaxELC_Guiyi < 43)    MaxELC_Guiyi = MaxELC_Guiyi_Old;  //丢线时最大值取旧值
    //或丢线时保持上次摆角

    MaxELC_Guiyi_Old = MaxELC_Guiyi;
}

void Max_Sample(void)
{
    uint8 i;
//    uint16 ELC_Max = 4096, ELC_Min = 0;

    for (i = 0; i < NUM; i++) {
        if (ELC_Value[i] < ad_min[i]) {
            ad_min[i] = ELC_Value[i];     // 刷新最小值
//            ad_min[i] = (ad_min[i] < ELC_Min) ? ELC_Min : ((ad_min[i] > ELC_Max) ? ELC_Max : ad_min[i]); //限幅
        } else if (ELC_Value[i] > ad_max[i]) {
            ad_max[i] = ELC_Value[i];     // 刷新最大值
//            ad_max[i] = (ad_max[i] < ELC_Min) ? ELC_Min : ((ad_max[i] > ELC_Max) ? ELC_Max : ad_max[i]); //限幅
        }   //记录历史最大值和最小值

        if(ELC_Value[MaxELCNum] < ELC_Value[i] - 99) {
            MaxELCNum = i;
        }   //找出当前最强电感下标
    }

    MaxELC = ELC_Value[MaxELCNum];
    MaxELC_Guiyi = ELC_Guiyi[MaxELCNum];    //最强电感值
}

/*
 * @name: float Magnet_KalmanFilter(KFPTypeDef_t *kfp, uint16 input)
 * @function: Magnet_KalmanFilter
 * @param: none
 * @return: none
 */
void Magnet_KalmanFilter(KFPTypeDef_t *kfp)
{
    /* 时间更新(预测) */
    /* 更新先验协估计 */
    kfp->Priori_Estimation = kfp->Posterior_Estimation;
    if (kfp->Priori_Estimation != kfp->Priori_Estimation) {
        kfp->Priori_Estimation = 0;
    } // a is NaN. NaN不与任何值相等
    if (isnanf(kfp->Priori_Estimation) || isinff(kfp->Priori_Estimation)) {
        kfp->Priori_Estimation = 0;
    }

    /* 更新先验协方差 */
    kfp->Priori_Convariance = (int)(sqrt(kfp->Posterior_Convariance * kfp->Posterior_Convariance + kfp->Q * kfp->Q));
    if (kfp->Priori_Convariance != kfp->Priori_Convariance) {
        kfp->Priori_Convariance = 0;
    }

    /* 测量更新 */
    /* 计算卡尔曼增益 */
    kfp->KalmanGain = (float)sqrt((float)kfp->Priori_Convariance * kfp->Priori_Convariance / (kfp->Priori_Convariance * kfp->Priori_Convariance + kfp->R * kfp->R));
    if (kfp->KalmanGain != kfp->KalmanGain) {
        kfp->KalmanGain = 1.000f;
    }

    /* 更新最优值方程 */
    kfp->Posterior_Estimation = (int)(kfp->Priori_Estimation + (kfp->KalmanGain * (kfp->ELC_Input - kfp->Priori_Estimation)));
    if (kfp->Posterior_Estimation != kfp->Posterior_Estimation) {
        kfp->Posterior_Estimation = 0;
    }

    /* 更新协方差方程 */
    kfp->Posterior_Convariance = (int)(sqrt( (1.000f - kfp->KalmanGain) * kfp->Priori_Convariance * kfp->Priori_Convariance));
    if (kfp->Posterior_Convariance != kfp->Posterior_Convariance) {
        kfp->Posterior_Convariance = 0;
    }
}

/*
 * @name: void Magnet_KalmanFilter_Init(KFPTypeDef_t *kfp)
 * @function: Magnet_KalmanFilter_Init
 * @param: none
 * @return: none
 */
void Magnet_KalmanFilter_Init(KFPTypeDef_t *kfp)
{
    kfp->ELC_Input = 0;
    kfp->Posterior_Convariance = 0;
    kfp->Posterior_Estimation = 0;
    kfp->Priori_Convariance = 0;
    kfp->Priori_Estimation = 0;
    kfp->KalmanGain = 0.000f;
    kfp->Q = 4;     //越小积分越慢, 跟踪加速度计越慢越平滑
    kfp->R = 750;   //越小跟踪加速度计越快
}

/*
 * @name: void Bubble_Sort(uint16 arr[]);
 * @function: 改进冒泡排序升序
 * @param:  arr
 * @return: none
 */
void Bubble_Sort(uint16 arr[], uint8 len)
{
    int8 i,j;
    uint16  temp = 0;
    uint8   lastSwapIndex = 0;  // 最后一次交换的下标
    uint8   lastSwapIndexTemp = 0;

    lastSwapIndexTemp = len - 1;
    for (i = 0; i < len - 1; i++) {
        lastSwapIndex = lastSwapIndexTemp;
        for (j = 0; j < lastSwapIndex; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                lastSwapIndexTemp = j;
            }
//            if (arr[j] > arr[j + 1]) {
//                arr[j + 1] ^= arr[j];
//                arr[j] ^= arr[j + 1];
//                arr[j + 1] ^= arr[j];
//            }
        }
        if (lastSwapIndexTemp == 0) {
            break;
        }
    }
}

/*
 * @name: InductorNormal
 * @function: 传感器电压归一化
 * @param:  ADinput
 * @return: none
 * @use: InductorNormal(uint16 ADinput[])
 */
void InductorNormal(void)
{
    uint8   i = 0;

    for (i = 0; i < NUM; i++) {
        ELC_Guiyi[i] = (ELC_Value[i] - ad_min[i]) * 1000 / (ad_max[i] - ad_min[i]);     // 各偏移量归一化到1--1000以内
        ELC_Guiyi[i] = (uint16)(ELC_Guiyi[i] * ELC_GuiyiRate[i]);

        if(ELC_Guiyi[i] < 1)    ELC_Guiyi[i] = 1;
        if(ELC_Guiyi[i] > 1000) ELC_Guiyi[i] = 1000; //限幅
    }
}

/*
 * @name: GetNormalizedADValue
 * @function: 另一种电磁压归一化
 * @param:  ADinput                      传感器的值
 *          Min_val、Max_val             传感器的最小，最大值
 *          Normalize_MIN、Normalize_MAX 归一后的最小、最大值
 * @return: none
 * @use: getNormalizedADValue(adc_convert(), 0, 4000, 0, 1000);
 */
uint16 GetNormalizedADValue(uint16 ADinput, uint16 Min_val, uint16 Max_val, uint16 Normalize_MIN, uint16 Normalize_MAX)
{
  uint16 final = 0;

  if (ADinput < Min_val) {
      Min_val = ADinput;
  } //防止出负数

  final = (ADinput - Min_val) * (Normalize_MAX - Normalize_MIN) / (Max_val - Min_val) + Normalize_MIN;  //归一值

  if(final < Normalize_MIN) final = Normalize_MIN;
  if(final > Normalize_MAX) final = Normalize_MAX;  //限幅

  return final;
}

/*
 * @name: void Guiyi(void)
 * @function: Guiyi
 * @param: none
 * @return: none
 */
void Guiyi(void)
{
    ELC_Guiyi[0] = GetNormalizedADValue((uint16)(ELC_Value[0] * ELC_GuiyiRate[0]), 1, 4000, 1, 1000);
    ELC_Guiyi[1] = GetNormalizedADValue((uint16)(ELC_Value[1] * ELC_GuiyiRate[1]), 1, 4000, 1, 1000);
    ELC_Guiyi[2] = GetNormalizedADValue((uint16)(ELC_Value[2] * ELC_GuiyiRate[2]), 1, 4000, 1, 1000);
    ELC_Guiyi[3] = GetNormalizedADValue((uint16)(ELC_Value[3] * ELC_GuiyiRate[3]), 1, 4000, 1, 1000);

//    InductorNormal();

//    if (circle_flag != 0) ELC_ClaerGuiyi_Flag = 1;
//    if (circle_flag == 0 && ELC_ClaerGuiyi_Flag == 1) ELC_ClaerGuiyi_Flag = 2;
//    if (ELC_ClaerGuiyi_Flag == 2) {
//        for (uint8 i = 0; i < NUM; i++) {
//            ad_min[i] = 128;
//            ad_max[i] = 2048;
//        }
//        ELC_ClaerGuiyi_Flag = 0;
//    }   //跑完环岛后将归一化基准值清回初始值
}
