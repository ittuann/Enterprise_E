
#include "My_Bsp.h"
#include "define.h"
#include <math.h>
#include "camera.h"

//0为图像最顶行

/*
 * @name: uint8 Get_Valid_Row (void);
 * @function: 获取有效行
 * @param: none
 * @return: Valid_Row
 */
uint8 Get_Valid_Row(void)
{
    uint8 valid_row = 0;

    return valid_row;
}

/*
 * @name:void Bin_Image_Filter (void);
 * @function:滤波过滤噪点
 * @param:none
 * @return:none
 * @notice:三面被围的数据将被修改为同一数值
 */
void Bin_Image_Filter (void)
{

}

//Sobel算子边缘检测
uint8 imgbuff_three[MT9V03X_H][MT9V03X_W];
void MT9V032_extraction(uint8 imgbuff_extration[MT9V03X_H][MT9V03X_W],uint8 imgbuff_two[MT9V03X_H][MT9V03X_W])
{

}

#define GrayScale 256
//大津法计算阈值 最大类间方差
uint8 OTSU_threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
    uint8 threshold = 0;

    return threshold;
}
