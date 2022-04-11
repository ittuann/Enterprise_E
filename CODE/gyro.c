/*
 *  gyro.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "gyro.h"
#include "SEEKFREE_ICM20602.h"

ICM20602 ICM_Offset;            //零漂值
ICM20602 ICM_State;             //原始返回值
ICM20602_Treated ICM_Treated;   //经过滑动处理的数据

#define AcceRatio   (4096.000f)
#define GyroRatio   (16.384f)
#define ACC_FILTER_NUM  5   // 加速度计滤波深度
#define GYRO_FILTER_NUM 3   // 陀螺仪滤波深度

uint8 ICM20602_Offset_Finished = 0; //陀螺仪零偏设置完成标志位
uint8 Offect_Count = 0;             //零偏采集的有效次数
uint8 Offect_Time = 0;              //零偏采集的轮数
//int32 offect_temp1, offect_temp2, offect_temp3, offect_temp4, offect_temp5;
int32 offect_temp6 = 0;       //零偏缓存
int32 data_temp6 = 0;

float   Gyro_Z_Last[3] = {0.000f};          //陀螺仪上次值 (0是本次1是上次

//int16   ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];        //滤波缓存数组
//int16   GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM];
int16   GYRO_Z_BUF[GYRO_FILTER_NUM];

float   Gyro_Z_FilterLast[3] = {0.000f};    //陀螺仪滤波后上次值 (0是本次1是上次
                                            //注意小数舍弃带来的误差

int16   ICM_TreatedGyroZ2 = 0;
int32   Attitude_Angel_X2 = 0;
double  Attitude_Angel_X = 0.0000;  //z轴积分值
int16   Attitude_Yaw = 0;           //z轴积分值归到3600
float   GyroZProtect = 0;

/*
 * @name: void Gyro_Init(void)
 * @function: Gyro_Init
 * @param:none
 * @return:none
 */
void Gyro_Init(void)
{
    uint8 i = 0;

    Gyro_Type_Init();

    icm20602_init_spi();    //初始化ICM20602

    Gyro_Type_Init();

    ICM20602_Offset();      //置零偏

    for (i = 0; i < 10; i++) {
//        delay_ms(2);            //与中断采集时间相同
//        Gyro_Data_Filter();
    }

    for (i = 0; i < 5; i++) {
//        systick_delay_ms(2);           //与中断采集时间相同
//        Gyro_Data_Filter();
//        Get_Attitude_Kalman();
    }
}

/*
 * @name: void Gyro_Type_Init(void)
 * @function: Gyro_Type_Init
 * @param:none
 * @return:none
 */
void Gyro_Type_Init(void)
{
    ICM_Offset.accdata.x = ICM_Offset.accdata.y = ICM_Offset.accdata.z = 0;
    ICM_Offset.gyro.x = ICM_Offset.gyro.y = ICM_Offset.gyro.z = 0;

    ICM_State.accdata.x = ICM_State.accdata.y = ICM_State.accdata.z = 0;
    ICM_State.gyro.x = ICM_State.gyro.y = ICM_State.gyro.z = 0;

    ICM_Treated.accdata.x = ICM_Treated.accdata.y = ICM_Treated.accdata.z = 0;
    ICM_Treated.gyro.x = ICM_Treated.gyro.y = ICM_Treated.gyro.z = 0;

    Attitude_Angel_X = 0.0;
    Attitude_Angel_X2 = 0;
    Attitude_Yaw = 0;
}

/*
 * @name: void ICM20602_Offset(void)
 * @function: ICM20602零偏
 * @param:none
 * @return:none
 * @notice: 零偏采样要与正常读值的中断时间相同
 */
void ICM20602_Offset(void)
{
    uint8   i = 0;
    uint8   Offect_Degree = 200;
    int8    GyroZ_Range = 16;

    while (!ICM20602_Offset_Finished) {
        if (Offect_Count == 0) {
            ICM_Treated.accdata.x = ICM_Treated.accdata.y = ICM_Treated.accdata.z = 0;
            ICM_Treated.gyro.x = ICM_Treated.gyro.y = ICM_Treated.gyro.z = 0;
            ICM_Offset.accdata.x = ICM_Offset.accdata.y = ICM_Offset.accdata.z = 0;
            ICM_Offset.gyro.x = ICM_Offset.gyro.y = ICM_Offset.gyro.z = 0;
//            offect_temp1 = offect_temp2 = offect_temp3
//            = offect_temp4 = offect_temp5 = 0;
            offect_temp6 = 0;
        }

        for (i = 0; i < MAX(ACC_FILTER_NUM, GYRO_FILTER_NUM); i++) {
            delay_ms(2);            //与中断采集时间相同
            Gyro_Data_Filter();
//            Gyro_Data_Filter2();
        }   //先填满缓存数组

        for (i = 0; i < Offect_Degree + 1; i++) {
            delay_ms(2);            //与中断采集时间相同
            Gyro_Data_Filter();
//            Gyro_Data_Filter2();

            if (ABS((int16)(ICM_Treated.gyro.z)) > GyroZ_Range || ABS((int16)(Gyro_Z_Last[1] - ICM_Treated.gyro.z)) > GyroZ_Range) {
                //如果当前值过大/两次采样差距过大, 则舍去本次
                ICM_Treated.accdata.x = ICM_Treated.accdata.y = ICM_Treated.accdata.z = 0;
                ICM_Treated.gyro.x = ICM_Treated.gyro.y = ICM_Treated.gyro.z = 0;
//                Offect_Degree++;
            } else {
                Offect_Count++;
            }

//            offect_temp1 += ICM_State.accdata.x;      //使用原始值
//            offect_temp2 += ICM_State.accdata.y;
//            offect_temp3 += ICM_State.accdata.z;
//            offect_temp4 += ICM_State.gyro.x;
//            offect_temp5 += ICM_State.gyro.y;
//            offect_temp6 += ICM_State.gyro.z;

//            offect_temp1 += ICM_Treated.accdata.x;    //使用滤波后值
//            offect_temp2 += ICM_Treated.accdata.y;
//            offect_temp3 += ICM_Treated.accdata.z;
//            offect_temp4 += ICM_Treated.gyro.x;
//            offect_temp5 += ICM_Treated.gyro.y;
            offect_temp6 += (int16)(ICM_Treated.gyro.z);
        }

//        ICM_Offset.accdata.x = offect_temp1 / Offect_Count;
//        ICM_Offset.accdata.y = offect_temp2 / Offect_Count;
//        ICM_Offset.accdata.z = offect_temp3 / Offect_Count;
//        ICM_Offset.gyro.x = offect_temp4 / Offect_Count;
//        ICM_Offset.gyro.y = offect_temp5 / Offect_Count;
        ICM_Offset.gyro.z = offect_temp6 / Offect_Count;

        Attitude_Yaw = Attitude_Angel_X = Attitude_Angel_X2 = 0;
        ICM20602_Offset_Finished = 1;
    }
}

/*
 * @name: void Gyro_Offset(void)
 * @function: 正常与上面合并写在初始化里就可以, 奇怪的问题导致只能分开写
 * @param:none
 * @return:none
 */
void Gyro_Offset(void)
{
    int8 GyroZ_Range = 16;

    if (Offect_Time < 50) {
//        ICM_Offset.accdata.x = offect_temp1 / Offect_Count;
//        ICM_Offset.accdata.y = offect_temp2 / Offect_Count;
//        ICM_Offset.accdata.z = offect_temp3 / Offect_Count;
//        ICM_Offset.gyro.x = offect_temp4 / Offect_Count;
//        ICM_Offset.gyro.y = offect_temp5 / Offect_Count;
        ICM_Offset.gyro.z = offect_temp6 / Offect_Count;

        if (ABS(ICM_Offset.gyro.z) > GyroZ_Range) {
            //零偏值过大则置零\重测
            ICM_Offset.accdata.x = ICM_Offset.accdata.y = ICM_Offset.accdata.z = 0;
            ICM_Offset.gyro.x = ICM_Offset.gyro.y = ICM_Offset.gyro.z = 0;
        }

        Offect_Time++;
    }

    if (ICM20602_Offset_Finished == 1) {
        ICM_Treated.accdata.x = ICM_Treated.accdata.y = ICM_Treated.accdata.z = 0;
        ICM_Treated.gyro.x = ICM_Treated.gyro.y = ICM_Treated.gyro.z = 0;
        Attitude_Yaw = Attitude_Angel_X = Attitude_Angel_X2 = 0;
    }   //中断里初始化没完成之前读之置零

    if (Offect_Time >= 50) {
        ICM20602_Offset_Finished = 2;
    }
}

/*
 * @name: void Gyro_Data_Filter(void)
 * @function: 数据滑动滤波
 * @param:none
 * @return:none
 */
void Gyro_Data_Filter(void)  // 陀螺仪采集数据并递推滤波
{
    uint8 i;
//    int32 temp[6] = {0};
//    int32 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0;
    data_temp6 = 0;


//    float lpf_factor = 0.300f;    //一阶RC低通滤波参数 代表新采样值在滤波结果中的权重
                                    //一般陀螺仪的低通滤波的经验值带宽是30-50Hz(截止频率fc
                                    //lpf_factor = 1.0 / (1.0 + (1.0 / 2Pi * T(s) * fc));
                                    //滤波系数越小, 滤波结果越平稳, 灵敏度越低
                                    //不能滤除频率高于采样频率的二分之一的干扰
//    float B[3] = {0.04613f, 0.09226f, 0.04613f};//二阶IIR滤波器参数
//    float A[3] = {1.0000f, -1.30729f, 0.49181f};
//                                                //用Matlab工具箱FilterDesign计算参数
//                                                //可以优化成int型或是用移位器和加法器来代替乘法

    //获取原始值
//    get_icm20602_accdata_spi();     //获取加速度计数据
    get_icm20602_gyro_spi();        //获取陀螺仪数据

    for (i = 0; i < ACC_FILTER_NUM - 1; i++) {
//        ACC_X_BUF[ACC_FILTER_NUM-1-i] = ACC_X_BUF[ACC_FILTER_NUM-2-i];
//        ACC_Y_BUF[ACC_FILTER_NUM-1-i] = ACC_Y_BUF[ACC_FILTER_NUM-2-i];
//        ACC_Z_BUF[ACC_FILTER_NUM-1-i] = ACC_Z_BUF[ACC_FILTER_NUM-2-i];
    }   //递推旧值
    for (i = 0; i < GYRO_FILTER_NUM - 1; i++) {
//        GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
//        GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
        GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
    }

//    ACC_X_BUF[0] = ICM_State.accdata.x; // 更新滑动窗口数组
//    ACC_Y_BUF[0] = ICM_State.accdata.y;
//    ACC_Z_BUF[0] = ICM_State.accdata.z;
//    GYRO_X_BUF[0] = ICM_State.gyro.x;
//    GYRO_Y_BUF[0] = ICM_State.gyro.y;
    GYRO_Z_BUF[0] = ICM_State.gyro.z;

    for(i = 0; i < ACC_FILTER_NUM; i++) {
//        temp1 += ACC_X_BUF[i];
//        temp2 += ACC_Y_BUF[i];
//        temp3 += ACC_Z_BUF[i];
    }
    for(i = 0; i < GYRO_FILTER_NUM; i++) {
//        temp4 += GYRO_X_BUF[i];
//        temp5 += GYRO_Y_BUF[i];
        data_temp6 += GYRO_Z_BUF[i];
    }

    if (ICM20602_Offset_Finished) {
        //转化为实际物理值
        //单位g/s
//        ICM_Treated.accdata.x = ((float)temp1 / ACC_FILTER_NUM - ICM_Offset.accdata.x) * 8.0f / AcceRatio;
//        ICM_Treated.accdata.y = ((float)temp2 / ACC_FILTER_NUM - ICM_Offset.accdata.y) * 8.0f / AcceRatio;
//        ICM_Treated.accdata.z = ((float)temp3 / ACC_FILTER_NUM - ICM_Offset.accdata.z) * 8.0f / AcceRatio;
        //陀螺仪角度转弧度
//        ICM_Treated.gyro.x = ((float)temp4 / GYRO_FILTER_NUM - ICM_Offset.gyro.x) * Pi / 180.0f  / GyroRatio;
//        ICM_Treated.gyro.y = ((float)temp5 / GYRO_FILTER_NUM - ICM_Offset.gyro.y) * Pi / 180.0f  / GyroRatio;
        GyroZProtect = 0.000f;
        Gyro_Z_Last[0] = Gyro_Z_FilterLast[0] = 0.000f;
        ICM_Treated.gyro.z = 0.000f;

        GyroZProtect = ((float)data_temp6 / 3.000f - (float)ICM_Offset.gyro.z) * Pi / 180.0f  / GyroRatio;
        Gyro_Z_Last[0] = ((float)data_temp6 / 3.000f - (float)ICM_Offset.gyro.z) * Pi / 180.0f  / GyroRatio;
//        Gyro_Z_FilterLast[0] = Gyro_Z_Last[0];
//        ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
        Gyro_Z_FilterLast[0] = ((float)data_temp6 / 3.000f - (float)ICM_Offset.gyro.z) * Pi / 180.0f  / GyroRatio;
        ICM_Treated.gyro.z = ((float)data_temp6 / 3.000f - (float)ICM_Offset.gyro.z) * Pi / 180.0f  / GyroRatio;
        /* 滤波 */
        //一阶RC低通滤波
//        Gyro_Z_FilterLast[0] = lpf_factor * Gyro_Z_Last[0] + (1.000f - lpf_factor) * Gyro_Z_FilterLast[1];
//        ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
        //二阶IIR滤波器
//        Gyro_Z_FilterLast[0] = (B[0] * Gyro_Z_Last[0] + B[1] * Gyro_Z_Last[1] + B[2] * Gyro_Z_Last[2]
//                                - A[1] * Gyro_Z_FilterLast[1] - A[2] * Gyro_Z_FilterLast[2]) / A[0];
//        ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
        if (isnanf(Gyro_Z_FilterLast[0]) || isinff(Gyro_Z_FilterLast[0])) {
            if (isnanf(Gyro_Z_FilterLast[1]) || isinff(Gyro_Z_FilterLast[1])) {
                if (isnanf(Gyro_Z_FilterLast[2]) || isinff(Gyro_Z_FilterLast[2])) {
                    if (isnanf(Gyro_Z_Last[0]) || isinff(Gyro_Z_Last[0])) {
                        if (isnanf(Gyro_Z_Last[1]) || isinff(Gyro_Z_Last[1])) {
                            if (isnanf(Gyro_Z_Last[2]) || isinff(Gyro_Z_Last[2])) {
                                if (isnanf(ICM_Treated.gyro.z) || isinff(ICM_Treated.gyro.z)) {
                                    if (isnanf(GyroZProtect) || isinff(GyroZProtect)) {
                                        get_icm20602_gyro_spi();    //再次测量陀螺仪数据
                                        GyroZProtect = ((float)ICM_State.gyro.z / 3.000f - (float)ICM_Offset.gyro.z) * Pi / 180.0f  / GyroRatio;
                                         Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1] = Gyro_Z_Last[2] = ICM_Treated.gyro.z = GyroZProtect;
                                        if (isnanf(GyroZProtect) || isinff(GyroZProtect)) {
                                            if (Mortor1.Speed_Idealr > 0) {
                                                Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1] = Gyro_Z_Last[2] = ICM_Treated.gyro.z = GyroZProtect = -12.0f;
                                            } else {
                                                Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1] = Gyro_Z_Last[2] = ICM_Treated.gyro.z = GyroZProtect = 12.0f;
                                            }
                                        }
                                    } else {
                                        Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1] = Gyro_Z_Last[2] = ICM_Treated.gyro.z = GyroZProtect;
                                    }
                                } else {
                                    Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1] = Gyro_Z_Last[2] = ICM_Treated.gyro.z;
                                }
                            } else {
                                Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1] = Gyro_Z_Last[2];
                                ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
                            }
                        } else {
                            Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0] = Gyro_Z_Last[1];
                            ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
                        }
                    } else {
                        Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2] = Gyro_Z_Last[0];
                        ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
                    }
                } else {
                    Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1] = Gyro_Z_FilterLast[2];
                    ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
                }
            } else {
                Gyro_Z_FilterLast[0] = Gyro_Z_FilterLast[1];
                ICM_Treated.gyro.z = Gyro_Z_FilterLast[0];
            }
        }
    } else {
//        ICM_Treated.accdata.x = temp1 / ACC_FILTER_NUM;     //采集用于置零偏值
//        ICM_Treated.accdata.y = temp2 / ACC_FILTER_NUM;
//        ICM_Treated.accdata.z = temp3 / ACC_FILTER_NUM;
//        ICM_Treated.gyro.x = temp4 / GYRO_FILTER_NUM;
//        ICM_Treated.gyro.y = temp5 / GYRO_FILTER_NUM;
        ICM_Treated.gyro.z = data_temp6 / GYRO_FILTER_NUM;
    }

    //更新上次值
    for (i = 0; i < 2; i++) {
        Gyro_Z_Last[i+1] = Gyro_Z_Last[i];
        Gyro_Z_FilterLast[i+1] = Gyro_Z_FilterLast[i];
    }
}

void Gyro_Data_Filter2(void)
{
    get_icm20602_gyro_spi();        //获取陀螺仪数据

    if (ICM20602_Offset_Finished) {
        ICM_TreatedGyroZ2 = (ICM_State.gyro.z - ICM_Offset.gyro.z);
    } else {
        ICM_Treated.gyro.z = ICM_State.gyro.z;
    }
}

/*
 * @name:void Get_Attitude(void)
 * @function: 直接积分
 * @param: none
 * @return: none
 */
void Get_Attitude(void)
{
    float Gyro_DeadZone = 0.065f;
    float Gyro_Rate = 1.1557f;

    if ((fabsf(Gyro_Z_FilterLast[0]) - Gyro_DeadZone) > 0.0f) {
        Attitude_Angel_X += Gyro_Z_FilterLast[0];
//        Attitude_Angel_X += ICM_Treated.gyro.z;
        Attitude_Yaw = (int16)(Gyro_Rate * Attitude_Angel_X);
    }

//    if (Attitude_Yaw > 4000) {
//        Attitude_Yaw = 0;
//        Attitude_Angel_X = 0;
//        Gyro_Z_FilterLast[0] = 0;
//    }   //防止溢出
}

void Get_Attitude2(void)
{
    int16 Gyro_DeadZone = 9;
    float Gyro_Rate = 0.012245f;

    if ((ABS(ICM_TreatedGyroZ2) - Gyro_DeadZone) > 0) {
        Attitude_Angel_X2 += ICM_TreatedGyroZ2;
        Attitude_Yaw = (int16)(Gyro_Rate * (float)Attitude_Angel_X2);
    }
}

/*
 * @name: void KalmanFilter(float ACC_Angle,float Gyro_y);
 * @function: 卡尔曼滤波
 * @param:none
 * @return:none
 * @notice: 参数可以不用改
 */
#define Peried 3/500.0f     //卡尔曼积分周期
#define Gyro_Q 3.0f         //过程噪声协方差2.0       越小积分越慢, 跟踪加速度计越慢越平滑
#define Gyro_R 1500.0f      //测量噪声协方差5000.0    越小跟踪加速度计越快
float Gyro_KalmanGain = 1.000f;    //卡尔曼增益

void KalmanFilter(float ACC_Angle,float Gyro_y)
{
    //卡尔曼滤波局部参量
    static float Priori_Estimation = 0.000f;     //先验估计
    static float Posterior_Estimation = 0.000f;  //后验估计
    static float Priori_Convariance = 0.000f;    //先验方差
    static float Posterior_Convariance = 0.000f; //后验方差

    //卡尔曼滤波
    //1.时间更新(预测) : X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k)
    //预测状态方程。目的由 系统状态变量k-1时刻的最优值 和 系统输入 计算出k时刻的 系统预测值。
    //X(k|k-1)是利用K-1时刻预测的当前状态结果; X(k-1|k-1)是K-1时刻最优值(即输出值)
    //A是作用在X(k|k-1)状态下的变换矩阵，它是算法对状态变量进行预测的依据。当X为一维数据时，A的值是1
    //Bk是作用在控制量上的变换矩阵，在大多数实际情况下并没有控制增益; uk是当前状态的控制增益，一般没有这个变量，可以设为0
    Priori_Estimation = Posterior_Estimation - Gyro_y * Peried;     //先验估计，积分获得角度
    if (Priori_Estimation != Priori_Estimation) {
        Priori_Estimation = 0.000f;
    }

    //2.更新先验协方差  : P(k|k-1) = A(k,k-1)*P(k-1|k-1)*A(k,k-1)'+Q(k)
    //预测协方差方程。，目的根据 k-1时刻的系统协方差 预测 k时刻系统协方差。
    //P(k|k-1)是k时刻系统协方差矩阵; P(k-1|k-1)是k-1时刻系统协方差矩阵
    //Q(k)是系统过程噪声的协方差。协方差矩阵只要确定了一开始的P0，后面都可以递推出来，而且初始协方差矩阵P只要不是为0，它的取值对滤波效果影响很小，都能很快收敛
    Priori_Convariance = (float)sqrt( Posterior_Convariance * Posterior_Convariance + Gyro_Q * Gyro_Q );
    if (Priori_Convariance != Priori_Convariance) {
        Priori_Convariance = 0.000f;
    }

    //  卡尔曼后验估计：测量更新
    // 1.计算卡尔曼增益  : K(k) = P(k|k-1)*H(k)' / (H(k)*P(k|k-1)*H(k)' + R(k))
    //目的根据（k时刻） 协方差矩阵的预测值 计算 卡尔曼增益
    //K(k)是卡尔曼增益，是滤波的中间结果
    //H(k)是对象的预测矩阵。当 Pk|k-1 为一个一维矩阵时，Hk 是1
    //R(k)是对象测量噪声的协方差矩阵，它是一个数值，作为已知条件输入滤波器。注意，这个值过大过小都会使滤波效果变差， 且Rk取值越小收敛越快，所以可以通过实验手段寻找合适的R，值再利用它进行真实的滤波。
    Gyro_KalmanGain = (float)sqrt( Priori_Convariance * Priori_Convariance / ( Priori_Convariance * Priori_Convariance + Gyro_R * Gyro_R ) );
    if (Gyro_KalmanGain != Gyro_KalmanGain) {
        Gyro_KalmanGain = 1.000f;
    }

    //2.测量更新(校正): X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1))
    //更新最优值方程（卡尔曼滤波的输出）。目的根据 状态变量的预测值 和 系统测量值 计算出 k时刻状态变量的最优值
    //X(k|k)是k时刻状态变量最优估计值
    //Z(k)是对象的测量值
    Posterior_Estimation  = Priori_Estimation + Gyro_KalmanGain * (ACC_Angle - Priori_Estimation );
    if (Posterior_Estimation != Posterior_Estimation) {
        Posterior_Estimation = 0.000f;
    }

    // 3.更新后验协方差  : P(k|k) =（I-K(k)*H(k)）*P(k|k-1)
    //更新协方差方程。目的为了求 k时刻的协方差矩阵。（为得到k+1时刻的卡尔曼输出值做准备）
    //P(k|k)为k时刻协方差矩阵； I为单位矩阵
    //当 Pk|k-1 为一个一维矩阵时，Hk 是1
    Posterior_Convariance = (float)sqrt(( 1.000f - Gyro_KalmanGain ) * Priori_Convariance * Priori_Convariance );
    if (Posterior_Convariance != Posterior_Convariance) {
        Posterior_Convariance = 0.000f;
    }

    //得到最终角度
    Attitude_Angel_X = Posterior_Estimation;
}

void Get_Attitude_Kalman(void)
{
    KalmanFilter(ICM_Treated.accdata.x*90.0f/8.0f, ICM_Treated.gyro.x/Pi/180.0f);
}

//===============================四元素===============================//
#define GKp 1.6f //10.0f                    // proportional gain governs rate of convergence to accelerometer/magnetometer
#define GKi 0.001f//1.2f // //0.008f        // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                        // half the sample period 采样周期的一半
float Gq0 = 1, Gq1 = 0, Gq2 = 0, Gq3 = 0;   // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;      // scaled integral error
/*
 * 函数名：IMUupdate
 * 描述  ：四元素解算欧拉角
 * 输入  ：陀螺仪 加速度计
 * 输出  ：无
 * 调用  ：内部调用
 */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    // 先把这些用得到的值算好
    float q0q0 = Gq0*Gq0;
    float q0q1 = Gq0*Gq1;
    float q0q2 = Gq0*Gq2;
    float q1q1 = Gq1*Gq1;
    float q1q3 = Gq1*Gq3;
    float q2q2 = Gq2*Gq2;
    float q2q3 = Gq2*Gq3;
    float q3q3 = Gq3*Gq3;

    if (ax*ay*az == 0)
    {
        return;
    }

    norm = sqrt(ax*ax + ay*ay + az*az); // acc数据归一化
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity and flux (v and w)    估计重力方向和流量/变迁
    vx = 2*(q1q3 - q0q2);                                   // 四元素中xyz的表示
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;

    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) ;      // 向量外积在相减得到差分就是误差
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;

    exInt = exInt + ex * GKi;    // 对误差进行积分
    eyInt = eyInt + ey * GKi;
    ezInt = ezInt + ez * GKi;

    // adjusted gyroscope measurements
    gx = gx + GKp*ex + exInt;    // 将误差PI后补偿到陀螺仪，即补偿零点漂移
    gy = gy + GKp*ey + eyInt;
    gz = gz + GKp*ez + ezInt;    // 这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

    // integrate quaternion rate and normalise  // 四元素的微分方程
    Gq0 = Gq0 + (-Gq1*gx - Gq2*gy - Gq3*gz)*halfT;
    Gq1 = Gq1 + (Gq0*gx + Gq2*gz - Gq3*gy)*halfT;
    Gq2 = Gq2 + (Gq0*gy - Gq1*gz + Gq3*gx)*halfT;
    Gq3 = Gq3 + (Gq0*gz + Gq1*gy - Gq2*gx)*halfT;

    // normalise quaternion
    norm = sqrt(Gq0*Gq0 + Gq1*Gq1 + Gq2*Gq2 + Gq3*Gq3);
    Gq0 = Gq0 / norm;
    Gq1 = Gq1 / norm;
    Gq2 = Gq2 / norm;
    Gq3 = Gq3 / norm;

//    Attitude_Angel_Y = asin(-2*Gq1*Gq3 + 2*Gq0*Gq2) * 57.3; // pitch
//    Attitude_Angel_X = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)*57.3; // roll
//    Attitude_Angel_Z = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1)*57.3; // yaw
}

//一阶互补滤波
//三轴的加速度值没有累积误差, 且通过算tan()可以得到倾角， 但是它包含的噪声太多， 不能直接使用（因为待测物运动时会产生加速度，电机运行时振动会产生加速度等）
//陀螺仪对外界振动影响小, 精度高, 通过对角速度积分可以得到倾角，但是会产生累积误差。所以不能单独使用 MPU6050 的加速度计或陀螺仪来得到倾角，需要互补
//一阶互补算法的思想就是给加速度和陀螺仪不同的权值，把它们结合到一起，进行修正

float K1 = 0.100f;  //对加速度计取值的权重
float dt = 0.001f;  //dt的取值为滤波器采样时间

//angle_ax = atan(ICM_Treated.accdata.x/ICM_Treated.accdata.z)*57.3;     //加速度得到的角度
//gy = (float)ICM_Treated.gyro.y/7510.0;       //陀螺仪得到的角速度
//Angle_Pitch = yijiehubu(angle_ax,gy);

float yijiehubu(float angle_m, float gyro_m)    //采集后计算的角度和角加速度
{
    float angle = 0.000f;
    angle = K1 * angle_m + (1-K1) * (angle + gyro_m * dt);
    return angle;   //可以计算pitch(俯仰)角(x), roll(滚转)角(y)
}
