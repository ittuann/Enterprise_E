
#ifndef _camera_h
#define _camera_h

#include "My_Bsp.h"
#include "define.h"
#include <math.h>

//宏定义

//#include "SEEKFREE_MT9V03X.h"

//函数声明

void Bin_Image_Filter(void);//过滤噪点

#define MT9V03X_W 80
#define MT9V03X_H 60
//void MT9V032_extraction(uint8 imgbuff_extration[MT9V03X_H][MT9V03X_W],uint8 imgbuff_two[MT9V03X_H][MT9V03X_W]);

uint8 OTSU_threshold(uint8 *image, uint16 col, uint16 row);



#endif

