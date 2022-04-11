/*
 *  anotc.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef _ANO_DT_H_
#define _ANO_DT_H_

//Include
#include <string.h>

#include "isr.h"
#include "MY_Bsp.h"

//Define
//数据拆分宏定义, 在发送大于1字节的数据类型时, 需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )   /* !< uint32_t 数据拆分 byte0 */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )   /* !< uint32_t 数据拆分 byte1 */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )   /* !< uint32_t 数据拆分 byte2 */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )   /* !< uint32_t 数据拆分 byte3 */

//Function
extern  void Wireless_Send(void);

#endif /* CODE_ANOTC_H_ */
