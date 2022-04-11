#ifndef _PHOTO_H_
#define _PHOTO_H_

#include "My_Bsp.h"
#include "math.h"

#define white 255
#define black 0

extern uint8 flag_a;
extern uint8 flag_b;
extern uint8 flag_c;
extern uint8 stop_flag;
extern uint8 stop_count;
//extern uint8 left_lose_near_num;             //近处左丢线
//extern uint8 right_lose_near_num;             //近处右丢线
//extern uint8 all_lose_near_num;               //近处全丢线
//
//extern uint8 left_lose_far_num;             //远处左丢线
//extern uint8 right_lose_far_num;             //远处右丢线
//extern uint8 all_lose_far_num;               //远处全丢线

extern uint8 right_circle_flag;
extern uint8 left_circle_flag;
extern uint8 projecting_point_row;
extern uint8 flag_test;
extern uint8 flag_y;
//extern uint8 flag_wider;
//extern int left_near_notwhite_row;
//extern int right_near_notwhite_row;
//extern uint8 flag_cross_switch;
//extern uint8 flag_circle_switch;
extern uint8 circle_flag;
extern uint8 flag_y_cross;
extern uint8 left_turning_point;
extern uint8 right_turning_point;
//extern uint8 flag_outbound;
//extern uint8 flag_put_in;
extern uint8 flag_y_cross_special;

extern uint8 flag_circle_elc;
extern uint8 flag_err_special;

//定义坐标结构体
typedef struct
{
    uint8 x;
    uint8 y;
} Site_t;

//定义矩形大小结构体
typedef struct
{
    uint8 W;       //宽
    uint8 H;       //高
} Size_t;

typedef enum {left=1,right=2}LeOrRi;   //八临域是左边还是右边

extern uint8 img[60][80];
extern uint8 left_black[60];
extern uint8 right_black[60];
extern uint8 middle_line[60];
extern uint8 img_real_width[60];
extern uint8 valid_row;
extern uint8 start_row;

/**********函数声明********/
void get_mid_line(int a);//图像处理函数
void find(int dir_num );//获取八临域下一个点坐标
int get_direction(Site_t A1,Site_t A2,LeOrRi a);//获取八临域起始位置方位
int find_valid_row( );//有效行识别
void set_valid_val();//判断左右边界是否有效
void set_midline();//设置中线，在寻找到左右边界后调用
void init_point();//数组初始化
void get_turning_point();//查询拐点


float  guai_pls(uint8 *array,int row_start,int row_end,char *valid );//计算赛道斜率
void lian_xian_zhi(int column1,int row1,int column2,int row2,uint8 *c);//连线

void  cross_check1();
void  new_deal_circle_right();
void  scan_circle_right();
void get_ring_point(uint8 a);
void deal_circle_right();


void midline_show();
void img_test();

#endif  //__PHOTO_H__

