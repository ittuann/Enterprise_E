/*
 *  screen.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "MY_Bsp.h"
#include "SEEKFREE_18TFT.h"

extern  uint8   HMI_Finish_Flag;        //HMI结束标志位
extern  uint8 Screen_Close_Flag;        //按键关闭屏幕标志位

extern  void Screen_Show_Menu(uint8 startmod);
extern  void HMI_Display(uint8 startmod);
extern  void Screen_Show_Menu_SpeedBase(uint8 startmod, uint8 Speed);
extern  void Screen_Show_Temp(void);

extern  void Screen_Show_Debug(void);
extern  void Screen_Show_Camera(void);
extern  void Screen_Show_Motor(void);
extern  void Screen_Show_Gyro(void);
extern  void Screen_Show_ELC(void);

extern  void Screen_Show_Car_State(uint16 x,uint16 y);

//-----------------------------------user--------------------------------------------//

//void Image_DrawXLine(int16 x, int16 length_y, uint16 color, uint16 size);
//void Image_DrawYLine(int16 length_x, int16 y, uint16 color, uint16 size);
//void Screen_Drawline(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint16 color);
//void Image_Show_CentreRectangle(uint16 x,uint16 y,uint16 color,uint16 length,uint16 wide);
//void Image_Show_CornerRectangle(uint16 x_1,uint16 y_1,uint16 x_2,uint16 y_2,uint16 color);
//void Image_Show_X(uint16 x,uint16 y,uint16 color,uint16 size);
//void Screen_Smile(void);

#endif /* CODE_myscreen_H_ */
