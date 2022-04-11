/*
 *  encoder.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */
 
#include "MY_Bsp.h"
#include "define.h"
#include "encoder.h"

//encoder statement
EncoderTypeDef_t Encoder1, Encoder2, Encoder3, Encoder4;

//定义编码器的方向
#define    Encoder_1_DIR    1
#define    Encoder_2_DIR    0
#define    Encoder_3_DIR    0
#define    Encoder_4_DIR    1

//定义轮子数据
float Encoder_w = 1.000f;
float Encoder_Rate = 1.000f;
float Wheel_Perimeter  = 1.000f;    //轮子周长
float Wheel_x_Distance = 0.170f;    //x方向上车轮的距离     ↑y
float Wheel_y_Distance = 0.180f;    //y方向上车轮的距离     |-->x

//Define
int16   Mortor_RPM_Max =  200;
int16   Mortor_RPM_Min = -200;
int16   speed_read_tmp[MotNum] = {0};                   //用数组暂存电机转速
float   Enc_Filter[EncNum][2] = {0};                    //暂存速度值

int8    Mortor_MaxSpeedNum = 0, Mortor_MinSpeedNum = 0; //电机转速极值下标
int16   Mortor_MaxSpeed = 0, Mortor_MinSpeed = 0, Mortor_AveSpeed = 0;  //电机转速极值

int32   CarDis_x = 0, CarDis_y = 0, CarDis_a = 0;       //实际路程
float   CarSpeed_x = 0.000f, CarSpeed_y = 0.000f, CarSpeed_a = 0.000f;  //实际坐标轴转速
float   CarTran_x = 0.000f, CarTran_y = 0.00f;          //函数内路程累计(截断误差累计会很严重 最好用浮点数
float   CarRace_x = 0.000f, CarRace_y = 0.000f, CarRace_a = 0.000f; //实际速度

/*
 * @name:int16 Encoder_SLimit(int16 SpeedNew, int16 SpeedOld, int16 Grad)
 * @function:编码器速度采集梯度平滑，每次采集最大变化Grad
 * @param:input
 * @return:none
 */
int16 Encoder_SLimit(int16 SpeedNew, int16 SpeedOld, int16 Grad)
{
    int16 SpeedFinal = 0;

    if (ABS(SpeedNew - SpeedOld) > Grad) {
        if (SpeedNew > 0) {
            SpeedFinal = SpeedOld + Grad;
        } else if (SpeedNew < 0) {
            SpeedFinal = SpeedOld - Grad;
        }
    } else {
        SpeedFinal = SpeedNew;
    }

    return SpeedFinal;
}

/*
 * @name:int16 Encoder_Lowpass(uint8 num, int16 input)
 * @function: 编码器低通滤波
 * @param: num, input
 * @return: none
 */
int16 Encoder_Lowpass(uint8 num, int16 input)
{
    float lpf_factor = 0.550f;
    int16 final = 0;

    num -= 1;

    Enc_Filter[num][0] = (float)input * lpf_factor + Enc_Filter[num][1] * (1.000f - lpf_factor);

    if (isnanf(Enc_Filter[num][0]) || isinff(Enc_Filter[num][0])) {
        if (isnanf(Enc_Filter[num][1]) || isinff(Enc_Filter[num][1])) {
            Enc_Filter[num][0] = 0;
        } else {
            Enc_Filter[num][0] = Enc_Filter[num][1];
        }
    }

    Enc_Filter[num][1] = Enc_Filter[num][0];    //更新旧值

    final = (int16)(Enc_Filter[num][0]);
    return final;
}

/*
 * @name:void Encoder_Init(void);
 * @function:角度(绝对式)编码器初始化
 * @param:
 * @return:none
 */
void Encoder_Init(void)
{
    encoder_init_spi(ABS_ENCODER_SPI_PC1_PIN);
    encoder_init_spi(ABS_ENCODER_SPI_PC2_PIN);
    encoder_init_spi(ABS_ENCODER_SPI_PC3_PIN);
    encoder_init_spi(ABS_ENCODER_SPI_PC4_PIN);

    EncoderType_Init();

    CarTran_x = CarTran_y = 0.000f;
    CarDis_x = CarDis_y = CarDis_a = 0;
    CarSpeed_x = CarSpeed_y = CarSpeed_a = 0.000f;
    CarRace_x = CarRace_y = CarRace_a = 0.000f;
}

void Encoder_Parameter_Init(EncoderTypeDef_t *sptr)
{
    sptr->Encoder_Now   = 0;
    sptr->Encoder_Last  = 0;
}

void EncoderType_Init(void)
{
    Encoder_Parameter_Init(&Encoder1);
    Encoder_Parameter_Init(&Encoder2);
    Encoder_Parameter_Init(&Encoder3);
    Encoder_Parameter_Init(&Encoder4);
}

/*
 * @name:void Speed_Collect(void);
 * @function:编码器读值
 * @param:none
 * @return:none
 */
void Speed_Collect(void)
{
    /* 编码器读值 */
    Encoder1.Encoder_Now = encoder1_speed_spi(ABS_ENCODER_SPI_PC1_PIN);
    Encoder2.Encoder_Now = encoder2_speed_spi(ABS_ENCODER_SPI_PC2_PIN);
    Encoder3.Encoder_Now = encoder3_speed_spi(ABS_ENCODER_SPI_PC3_PIN);
    Encoder4.Encoder_Now = encoder4_speed_spi(ABS_ENCODER_SPI_PC4_PIN);

    /* 改变编码器读值方向 */
    #if(Encoder_1_DIR == 0)
        Encoder1.Encoder_Now =   Encoder1.Encoder_Now + 0;
    #elif(Encoder_1_DIR == 1)
        Encoder1.Encoder_Now = - Encoder1.Encoder_Now;
    #endif
    #if(Encoder_2_DIR == 0)
        Encoder2.Encoder_Now =   Encoder2.Encoder_Now + 0;
    #elif(Encoder_2_DIR == 1)
        Encoder2.encoder_tmp = - Encoder2.Encoder_Now;
    #endif
    #if(Encoder_3_DIR == 0)
        Encoder3.Encoder_Now =   Encoder3.Encoder_Now + 0;
    #elif(Encoder_3_DIR == 1)
        Encoder3.Encoder_Now = - Encoder3.Encoder_Now;
    #endif
    #if(Encoder_4_DIR == 0)
        Encoder4.Encoder_Now =   Encoder4.Encoder_Now + 0;
    #elif(Encoder_4_DIR == 1)
        Encoder4.Encoder_Now = - Encoder4.Encoder_Now;
    #endif

    /* 限幅 */
    Encoder1.Encoder_Now = RANGE(Encoder1.Encoder_Now, Mortor_RPM_Max, Mortor_RPM_Min);
    Encoder2.Encoder_Now = RANGE(Encoder2.Encoder_Now, Mortor_RPM_Max, Mortor_RPM_Min);
    Encoder3.Encoder_Now = RANGE(Encoder3.Encoder_Now, Mortor_RPM_Max, Mortor_RPM_Min);
    Encoder4.Encoder_Now = RANGE(Encoder4.Encoder_Now, Mortor_RPM_Max, Mortor_RPM_Min);  //初始化后电机概率疯转才加的

    /* 乘系数 */
//    Encoder1.Encoder_Now = (int16)(Encoder_Rate * Encoder1.Encoder_Now);
//    Encoder2.Encoder_Now = (int16)(Encoder_Rate * Encoder2.Encoder_Now);
//    Encoder3.Encoder_Now = (int16)(Encoder_Rate * Encoder3.Encoder_Now);
//    Encoder4.Encoder_Now = (int16)(Encoder_Rate * Encoder4.Encoder_Now);

    /* 编码器写入 */
    Mortor1.Speed_Read = Encoder_SLimit(Encoder1.Encoder_Now, Encoder1.Encoder_Last, 32);
    Mortor2.Speed_Read = Encoder_SLimit(Encoder2.Encoder_Now, Encoder2.Encoder_Last, 32);
    Mortor3.Speed_Read = Encoder_SLimit(Encoder3.Encoder_Now, Encoder3.Encoder_Last, 32);
    Mortor4.Speed_Read = Encoder_SLimit(Encoder4.Encoder_Now, Encoder4.Encoder_Last, 32);

    /* 编码器滤波 */
    Mortor1.Speed_Read = (Mortor1.Speed_Read * 6 + Mortor1.Speed_Last * 4) / 10;
    Mortor2.Speed_Read = (Mortor2.Speed_Read * 6 + Mortor2.Speed_Last * 4) / 10;
    Mortor3.Speed_Read = (Mortor3.Speed_Read * 5 + Mortor3.Speed_Last * 5) / 10;
    Mortor4.Speed_Read = (Mortor4.Speed_Read * 6 + Mortor4.Speed_Last * 4) / 10;
//    Mortor1.Speed_Read = Encoder_Lowpass(1, Mortor1.Speed_Read);
//    Mortor2.Speed_Read = Encoder_Lowpass(2, Mortor2.Speed_Read);
//    Mortor3.Speed_Read = Encoder_Lowpass(3, Mortor3.Speed_Read);
//    Mortor4.Speed_Read = Encoder_Lowpass(4, Mortor4.Speed_Read);

    /* 限幅 */
    Mortor1.Speed_Read = RANGE(Mortor1.Speed_Read, Mortor_RPM_Max, Mortor_RPM_Min);
    Mortor2.Speed_Read = RANGE(Mortor2.Speed_Read, Mortor_RPM_Max, Mortor_RPM_Min);
    Mortor3.Speed_Read = RANGE(Mortor3.Speed_Read, Mortor_RPM_Max, Mortor_RPM_Min);
    Mortor4.Speed_Read = RANGE(Mortor4.Speed_Read, Mortor_RPM_Max, Mortor_RPM_Min);  //初始化后电机概率疯转才加的

    /* 记录上次 */
    Encoder1.Encoder_Last = Encoder1.Encoder_Now;
    Encoder2.Encoder_Last = Encoder2.Encoder_Now;
    Encoder3.Encoder_Last = Encoder3.Encoder_Now;
    Encoder4.Encoder_Last = Encoder4.Encoder_Now;
    Mortor1.Speed_Last = Mortor1.Speed_Read;
    Mortor2.Speed_Last = Mortor2.Speed_Read;
    Mortor3.Speed_Last = Mortor3.Speed_Read;
    Mortor4.Speed_Last = Mortor4.Speed_Read;

    /* 速度分析 */
    Speed_Calcu();
    Speed_Analyse();
//    Speed_Dis();
//    Race_Calcu();

}

/*
 * @name: void Speed_Calcu(void);
 * @function: 计算小车实际转速
 * @param: none
 * @return: none
 */
void Speed_Calcu(void)
{
    /* 计算小车实际转速 */
    CarSpeed_x = (   Mortor1.Speed_Read + Mortor2.Speed_Read + Mortor3.Speed_Read + Mortor4.Speed_Read) / 4.0f;
    CarSpeed_y = (   Mortor1.Speed_Read - Mortor2.Speed_Read + Mortor3.Speed_Read - Mortor4.Speed_Read) / 4.0f;
    CarSpeed_a = (   Mortor1.Speed_Read - Mortor2.Speed_Read - Mortor3.Speed_Read + Mortor4.Speed_Read) / 4.0f;
}

/*
 * @name: void Speed_Analyse(void);
 * @function: 转速分析
 * @param: none
 * @return: none
 */
void Speed_Analyse(void)
{
    memset(speed_read_tmp, 0, sizeof(speed_read_tmp));

    /* 速度极值 */
    speed_read_tmp[0] = (int16)Mortor1.Speed_Read;
    speed_read_tmp[1] = (int16)Mortor2.Speed_Read;
    speed_read_tmp[2] = (int16)Mortor3.Speed_Read;
    speed_read_tmp[3] = (int16)Mortor4.Speed_Read;

    Mortor_AveSpeed = 0;
    for (uint8 i = 0; i < MotNum; i++) {
        if (speed_read_tmp[Mortor_MaxSpeedNum] < speed_read_tmp[i]) {
            Mortor_MaxSpeedNum = i;
        }   //找出转速最大轮
        if (speed_read_tmp[Mortor_MinSpeedNum] > speed_read_tmp[i]) {
            Mortor_MinSpeedNum = i;
        }   //找出转速最小轮

        Mortor_AveSpeed += speed_read_tmp[i];
    }

    Mortor_AveSpeed = Mortor_AveSpeed / MotNum;     //计算四个轮平均速度
    Mortor_MaxSpeed = speed_read_tmp[Mortor_MaxSpeedNum];
    Mortor_MinSpeed = speed_read_tmp[Mortor_MinSpeedNum];
}

/*
 * @name: void Speed_Dis(void);
 * @function: 计算小车运行距离
 * @param: none
 * @return: none
 */
void Speed_Dis(void)
{
    /* 计算轮子加速度 */
    Mortor1.Acc = (int16)(Mortor1.Speed_Read - Mortor1.Speed_Last);
    Mortor2.Acc = (int16)(Mortor2.Speed_Read - Mortor2.Speed_Last);
    Mortor3.Acc = (int16)(Mortor3.Speed_Read - Mortor3.Speed_Last);
    Mortor4.Acc = (int16)(Mortor4.Speed_Read - Mortor4.Speed_Last);

    /* 每个轮子经过路程 */
    Mortor1.Distance += (int16)Mortor1.Speed_Read;
    Mortor2.Distance += (int16)Mortor2.Speed_Read;
    Mortor3.Distance += (int16)Mortor3.Speed_Read;
    Mortor4.Distance += (int16)Mortor4.Speed_Read;

    /* 坐标轴方向经过路程 */
    CarDis_x += (int16)CarSpeed_x;
    CarDis_y += (int16)CarSpeed_y;
    CarDis_a += (int16)CarSpeed_a;
}

/*
 * @name: void Race_Calcu(void);
 * @function: 计算小车实际速度
 * @param: none
 * @return: none
 */
void Race_Calcu(void)
{
    //将平均每分钟转速(rpm)转换为平均每秒转速(rps)
    CarRace_x = CarSpeed_x / 60.0f;
    CarRace_y = CarSpeed_y / 60.0f;
    CarRace_a = CarSpeed_a / 60.0f;

    //将rps转换为m/s(rad/s)
    CarRace_x = CarRace_x * Wheel_Perimeter;
    CarRace_y = CarRace_y * Wheel_Perimeter;
    CarRace_a = CarRace_a * Wheel_Perimeter / ((Wheel_x_Distance / 2.0f) + (Wheel_y_Distance / 2.0f));
}

/*
 * @name: void FSS_Print_Encoder(void)
 * @function: FSS print encoder value
 * @param:none
 * @return:none
 */
void FSS_Print_Encoder(void)
{
    printf("Encoder 1 value is %d \n ", Mortor1.Speed_Read);
    printf("Encoder 2 value is %d \n ", Mortor2.Speed_Read);
    printf("Encoder 3 value is %d \n ", Mortor3.Speed_Read);
    printf("Encoder 4 value is %d \n ", Mortor4.Speed_Read);
}
