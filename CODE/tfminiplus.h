/*
 *  tfminiplus.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef _TFMINI_PLUS_H_
#define _TFMINI_PLUS_H_

// Includes
#include "My_Bsp.h"
#include <string.h>

// Statement
extern  uint16_t    TFmin_Dist;     //距离信息
extern  uint16_t    TFmin_Strength; //信号强度
extern  uint16_t    TFmin_Temp;     //芯片内部温度值

extern  uint8_t     TFminiPlus_flag;//接收数据标志位

extern  uint8_t     TFminiPlus_temp;//暂存当前字节

// Function
extern  void TFminiPlus_Proc (uint8_t *buf, uint8_t len);
extern  void TFminiPlus_Proc2(uint8_t data);
extern  void TFminiPlus_GetOnce(void);

#endif /* CODE_TFMINI_PLUS_ */
