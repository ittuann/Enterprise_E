/*
 *  My_Bsp.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef CODE_MY_BSP_H_
#define CODE_MY_BSP_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//library h
#include "headfile.h"

//user library h
#include "anotc.h"      //匿名上位机
#include "tfminiplus.h" //TfMiniPlus激光雷达
//#include "SEEKFREE_7725.h"

//user h
#include "define.h"     //变量定义
#include "magnet.h"     //电感
#include "pid.h"        //PID
#include "control.h"    //控制
#include "encoder.h"    //编码器
#include "motor.h"      //电机
#include "screen.h"     //屏幕
#include "gyro.h"       //陀螺仪
#include "sg.h"         //舵机
#include "key.h"        //按键
#include "beep.h"       //蜂鸣器和LED
#include "camera.h"     //摄像头二值化
#include "photo.h"      //摄像头处理

#define Pi  (3.14159265f)
#define ONE_PI  (3.14159265f)
#define TWO_PI  (2.000f * 3.14159265f)
#define ANGLE_UNIT  (TWO_PI / 10.0f)
#define EPS (1e-6)

#define mian main   // :D

//Define
#define ABS(x)  (((x) > 0) ? (x) : (-(x)))                                                  //宏定义实现返回绝对值(x里不能有自加自减的语句，否则变量出错)
#define LIMIT(x, min, max)  (((x) < (min)) ? (min) : ( ((x) > (max)) ? (max) : (x)))        //限幅
#define RANGE(x, max, min)  (((x) <= (min)) ? (min) : ( ((x) >= (max)) ? (max) : (x)))      //确保x的范围为min~max
#define MIN(x, y)   (((x) < (y)) ? (x) : (y))                                               //取小值
#define MAX(x, y)   (((x) > (y)) ? (x) : (y))                                               //取大值
#define SIGN(x) (((x) > 0) ? 1 :-1) //((int32)(((x)>0?1:-1)*ceil(ABS((x)))))                //取符号
#define INT(x)  ((int)(x))   //((int32)(((x)>0?1:-1)*ceil(ABS((x)))))                       //强制整型
#define SWAP(x, y)   do{(x) ^= (y); (y) ^= (x); (x) ^= (y);} while(0)                       //交换 x, y 的值
#define ARR_SIZE(a) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )                                  //返回数组元素的个数
#define OFFSET(type, member)    ((uint32)(&(((type *)0)->member)))                          //获取结构体某成员偏移


//Function
unsigned char My_Bsp_Init(void);
void delay_ms(uint64 ms);
void delay_us(uint64 us);
void My_swap(int *x, int *y);
float My_sqrt(float number);
float InvSqrt(float x);
int My_pow(int x, int y);
//float mx_sin(float rad);
float My_sin(float rad);
float My_cos(float rad);
char Get_State(char x);



// 交换16位数据的2字节顺序
#if 1                   //两种方法, 第一种是CMSIS库自带
#define SWAP16(data)    __REVSH(data)
#else
#define SWAP16(data)    (uint32)((((uint16)(data) & (0xFF<<0 ))<<8)      \
                          | (((uint32)(data) & (0xFF<<8))>>8)            \
                        )
#endif


// 交换32位数据的4字节顺序
#if 1                    //两种方法, 第一种是CMSIS库自带
#define SWAP32(data)    __REV(data)
#else
#define SWAP32(data)    (uint32)((((uint32)(data) & (0xFFu<<0 ))<<24)      \
                          | (((uint32)(data) & (0xFFu<<8 ))<<8)            \
                          | (((uint32)(data) & (0xFFu<<16))>>8)            \
                          | (((uint32)(data) & (0xFFu<<24))>>24)           \
                        )
#endif


// 打印宏定义展开结果
#define _MKSTR(str)  #str
#define MKSTR(str)  _MKSTR(str)                       // 把宏定义展开结果转换为字符串
#define DEFINE_PRINTF(def)  printf(MKSTR(def)"\n")    // 串口打印宏定义展开结果


#endif
