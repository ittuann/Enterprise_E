/*
 *  motor.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "MY_Bsp.h"
#include "motor.h"

//define
//定义PWM的方向
#define    PWM_1_DIR    0
#define    PWM_2_DIR    0
#define    PWM_3_DIR    1
#define    PWM_4_DIR    1

//PWM Statement
MortorTypedef_t Mortor1, Mortor2, Mortor3, Mortor4;

/*
 * @name: void Speed_Control_Init(void);
 * @function: 速度控制初始化
 * @param: none
 * @return: none
 */
void Speed_Control_Init(void)
{
    Motor_PWM_Init();
    MortorType_Init();
    PID_Init();

//    DIP_Speed_Control();
}

/*********************PWM**********************/

/*
 * @name: void Motor_PWM_Init(void);
 * @function: Motor_PWM_Init
 * @param: none
 * @return: none
 */
void Motor_PWM_Init(void)
{
    gpio_init(F1_PIN, GPO, 0, GPIO_PIN_CONFIG);
    gpio_init(F2_PIN, GPO, 0, GPIO_PIN_CONFIG);
    gpio_init(B1_PIN, GPO, 0, GPIO_PIN_CONFIG);
    gpio_init(B2_PIN, GPO, 0, GPIO_PIN_CONFIG);

    pwm_init(F1_PWM, 17000, 0);
    pwm_init(F2_PWM, 17000, 0);
    pwm_init(B1_PWM, 17000, 0);
    pwm_init(B2_PWM, 17000, 0);
}

/*
 * @name: void Mototr_Parameter_Init(MortorTypedef_t *sptr)
 * @function: 电机结构体初始化
 * @param: none
 * @return: none
 */
void Mototr_Parameter_Init(MortorTypedef_t *sptr)
{
    sptr->Speed_Idealx  = 0;
    sptr->Speed_Idealy  = 0;
    sptr->Speed_Idealr  = 0;
    sptr->Speed_Ideal   = 0;
    sptr->Speed_Read    = 0;
    sptr->PWM_Write     = 0;
    sptr->Acc           = 0;
    sptr->Distance      = 0;
}

/*
 * @name: void MortorType_Init(void);
 * @function: 电机初始化
 * @param: none
 * @return: none
 */
void MortorType_Init(void)
{
    Mototr_Parameter_Init(&Mortor1);
    Mototr_Parameter_Init(&Mortor2);
    Mototr_Parameter_Init(&Mortor3);
    Mototr_Parameter_Init(&Mortor4);

    Gear = 0;
}

/*
 * @name: void Mortor_Prevent_Madness(void);
 * @function: 防止电机疯转
 * @param: none
 * @return: none
 */
void Mortor_Prevent_Madness(void)
{
    Encoder1.Encoder_Now = Encoder2.Encoder_Now
    = Encoder3.Encoder_Now = Encoder4.Encoder_Now = 0;
    Encoder1.Encoder_Last = Encoder2.Encoder_Last
    = Encoder3.Encoder_Last = Encoder4.Encoder_Last = 0;

    Mortor1.Speed_Read = Mortor2.Speed_Read
    = Mortor3.Speed_Read = Mortor4.Speed_Read = 0;
    Mortor1.Speed_Last = Mortor2.Speed_Last
    = Mortor3.Speed_Last = Mortor4.Speed_Last = 0;
    Mortor1.Speed_Ideal = Mortor2.Speed_Ideal
    = Mortor3.Speed_Ideal = Mortor4.Speed_Ideal = 0;
    Mortor1.PWM_Write = Mortor2.PWM_Write
    = Mortor3.PWM_Write = Mortor4.PWM_Write = 0;
}

/*********************PID**********************/

/*
 * @name: void SpeedPID_Control(void);
 * @function: 电机PID控制
 * @param: none
 * @return: none
 */
void SpeedPID_Control(void)
{
    #if (Speed_Flag == 1)
        /* 理想速度写入 */
        Mortor1_PID.EX_Val = Mortor1.Speed_Ideal;
        Mortor2_PID.EX_Val = Mortor2.Speed_Ideal;
        Mortor3_PID.EX_Val = Mortor3.Speed_Ideal;
        Mortor4_PID.EX_Val = Mortor4.Speed_Ideal;
        /* 当前速度写入 */
        Mortor1_PID.Now_Val = Mortor1.Speed_Read;
        Mortor2_PID.Now_Val = Mortor2.Speed_Read;
        Mortor3_PID.Now_Val = Mortor3.Speed_Read;
        Mortor4_PID.Now_Val = Mortor4.Speed_Read;
        /* PID */
//        if (Mortor1.Speed_Idealy == 0) {
//            MortorPID_Parameter_Write(PID1_Parameter_M1, PID1_Parameter_M2, PID1_Parameter_M3, PID1_Parameter_M4);
//        } else {
//            MortorPID_Parameter_Write(PID2_Parameter_M1, PID2_Parameter_M2, PID2_Parameter_M3, PID2_Parameter_M4);
//        }
//        Incremental_PID(&Mortor1_PID);
//        Incremental_PID(&Mortor2_PID);
//        Incremental_PID(&Mortor3_PID);
//        Incremental_PID(&Mortor4_PID);
        Motor_Realize_PID(&Mortor1_PID, 1);
        Motor_Realize_PID(&Mortor2_PID, 2);
        Motor_Realize_PID(&Mortor3_PID, 3);
        Motor_Realize_PID(&Mortor4_PID, 4);
        /* PWM输出 */
        Mortor1.PWM_Write = (int16)Mortor1_PID.Output;
        Mortor2.PWM_Write = (int16)Mortor2_PID.Output;
        Mortor3.PWM_Write = (int16)Mortor3_PID.Output;
        Mortor4.PWM_Write = (int16)Mortor4_PID.Output;
    #elif (Speed_Flag == 2)
        /* PWM输出 */
        Mortor1.PWM_Write = Mortor1.Speed_Ideal;
        Mortor2.PWM_Write = Mortor2.Speed_Ideal;
        Mortor3.PWM_Write = Mortor3.Speed_Ideal;
        Mortor4.PWM_Write = Mortor4.Speed_Ideal;
    #endif

        /* PWM方向 */
    #if(PWM_1_DIR == 1)
        Mortor1.PWM_Write =   Mortor1.PWM_Write + 0;
    #elif (PWM_1_DIR == 0)
        Mortor1.PWM_Write = - Mortor1.PWM_Write;
    #endif

    #if(PWM_2_DIR == 1)
        Mortor2.PWM_Write =   Mortor2.PWM_Write + 0;
    #elif (PWM_2_DIR == 0)
        Mortor2.PWM_Write = - Mortor2.PWM_Write;
    #endif

    #if(PWM_3_DIR == 1)
        Mortor3.PWM_Write =   Mortor3.PWM_Write + 0;
    #elif (PWM_3_DIR == 0)
        Mortor3.PWM_Write = - Mortor3.PWM_Write;
    #endif

    #if(PWM_4_DIR == 1)
        Mortor4.PWM_Write =   Mortor4.PWM_Write + 0;
    #elif (PWM_4_DIR == 0)
        Mortor4.PWM_Write = - Mortor4.PWM_Write;
    #endif

        /* 写入PWM */
        Motor_PWM_Write();
}

/*
 *  @name: void Motor_PWM_Write(void);
 *  @function: PWM Output
 *  @brief: PWM占空比设置
 *  @param: none
 */
void Motor_PWM_Write(void)
{
    /* 限幅 */
    Mortor1.PWM_Write = LIMIT(Mortor1.PWM_Write, Mortor_PWM_Min, Mortor_PWM_Max);
    Mortor2.PWM_Write = LIMIT(Mortor2.PWM_Write, Mortor_PWM_Min, Mortor_PWM_Max);
    Mortor3.PWM_Write = LIMIT(Mortor3.PWM_Write, Mortor_PWM_Min, Mortor_PWM_Max);
    Mortor4.PWM_Write = LIMIT(Mortor4.PWM_Write, Mortor_PWM_Min, Mortor_PWM_Max);

    /* Motor NO.1 */
     if(Mortor1.PWM_Write >= 0) {
        gpio_set(F1_PIN, 0);
        pwm_duty(F1_PWM, Mortor1.PWM_Write);
     } else {
         gpio_set(F1_PIN, 1);
         pwm_duty(F1_PWM, -Mortor1.PWM_Write);
     }

     /* Motor NO.2 */
     if(Mortor2.PWM_Write >= 0) {
         gpio_set(F2_PIN, 0);
         pwm_duty(F2_PWM, Mortor2.PWM_Write);
     } else {
         gpio_set(F2_PIN, 1);
         pwm_duty(F2_PWM, -Mortor2.PWM_Write);
     }

     /* Motor NO.3 */
     if(Mortor3.PWM_Write >= 0) {
         gpio_set(B1_PIN, 0);
         pwm_duty(B1_PWM, Mortor3.PWM_Write);
     } else {
         gpio_set(B1_PIN, 1);
         pwm_duty(B1_PWM, -Mortor3.PWM_Write);
     }

     /* Motor NO.4 */
     if(Mortor4.PWM_Write >= 0) {
        gpio_set(B2_PIN, 0);
        pwm_duty(B2_PWM, Mortor4.PWM_Write);
     } else {
         gpio_set(B2_PIN, 1);
         pwm_duty(B2_PWM, -Mortor4.PWM_Write);
     }
}


/*********************Car**********************/
/*
 * @name:void Car_Run(short x,short y,short rotate);
 * @function:速度融合
 * @param:
 *     y:y轴速度 正向前 负向后
 *     x:x轴速度 正向右 负向左
 *     rotate:角度 正向右 负向左
 *     rotate:旋转轴速度 |正:逆时针,角度加   |负:顺时针,角度减
 * @return:
 */
void Car_Run(int16 x, int16 y, int16 rotate)
{
    Mortor1.Speed_Idealx = x;
    Mortor2.Speed_Idealx = x;
    Mortor3.Speed_Idealx = x;
    Mortor4.Speed_Idealx = x;

    Mortor1.Speed_Idealy = y;
    Mortor2.Speed_Idealy = y;
    Mortor3.Speed_Idealy = y;
    Mortor4.Speed_Idealy = y;

//    Mortor1.Speed_Idealr = rotate * Encoder_w;
//    Mortor2.Speed_Idealr = rotate * Encoder_w;
//    Mortor3.Speed_Idealr = rotate * Encoder_w;
//    Mortor4.Speed_Idealr = rotate * Encoder_w;
    Mortor1.Speed_Idealr = rotate;
    Mortor2.Speed_Idealr = rotate;
    Mortor3.Speed_Idealr = rotate;
    Mortor4.Speed_Idealr = rotate;

    Mortor1.Speed_Ideal = (Mortor1.Speed_Idealx + Mortor1.Speed_Idealy + Mortor1.Speed_Idealr); //前左轮
    Mortor2.Speed_Ideal = (Mortor2.Speed_Idealx - Mortor2.Speed_Idealy - Mortor2.Speed_Idealr); //前右轮
    Mortor3.Speed_Ideal = (Mortor3.Speed_Idealx + Mortor3.Speed_Idealy - Mortor3.Speed_Idealr); //后左轮
    Mortor4.Speed_Ideal = (Mortor4.Speed_Idealx - Mortor4.Speed_Idealy + Mortor4.Speed_Idealr); //后右轮
}

/*
 * @name:void FSS_Print_PWM(void)
 * @function:FSS print PWM value
 * @param:none
 * @return:none
 */
void FSS_Print_PWM(void)
{
    printf("PWM1 Write is %d \n", Mortor1.PWM_Write);
    printf("PWM2 Write is %d \n", Mortor2.PWM_Write);
    printf("PWM3 Write is %d \n", Mortor3.PWM_Write);
    printf("PWM4 Write is %d \n", Mortor4.PWM_Write);
}
