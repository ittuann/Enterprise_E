#include "My_Bsp.h"
#include "math.h"

uint8 start_row=50;
/**********鎵嚎鍩烘湰鍙橀噺***********/
int dir2next_5[9]={3,2,1,4,99,0,5,6,7};
int site_a[2]={2,2};
Site_t left_site[180]={0},right_site[180]={0};

//璧涢亾鐪熷疄瀹藉害
uint8 img_real_width[60]={0};
uint8 all_white_row=0;  //鍏ㄧ櫧琛�
uint8 normal_row=0;

//鎻愬墠娴嬪ソ鐨勮禌閬撳搴�
//uint8 img_real_width2[60]=
//{
//  0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
//  8 ,9 ,10,12,10,13,14,16,17,19,
//  20,22,23,25,26,28,31,31,32,34,
//  35,36,38,41,43,44,45,46,47,48,
//  49,51,54,56,57,58,60,61,63,64,
//  65,67,68,70,71,73,74,75,75,76
//};  //鏋楀ぇ鍏ゼ璧涢亾

//uint8 img_real_width2[60]=
//{
//    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,// 0-9
//    14,16,17,18,19,20,22,24,26,26,//10-19
//    28,30,31,33,35,37,37,39,41,42,//20-29
//    44,46,47,49,50,51,53,55,56,58,//30-39
//    59,61,62,64,65,67,68,70,72,73,//40-49
//    75,76,77,78,79,79,79,79,79,79 //50-59
//};  //宸ュぇ璧涢亾

//uint8 img_real_width2[60]=
//{
//    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,// 0-9
//    9 ,10,12,13,15,17,18,19,21,22,//10-19
//    24,25,26,27,29,31,32,33,35,37,//20-29
//    38,39,41,43,44,46,48,48,50,52,//30-39
//    53,54,56,58,59,60,62,62,64,66,//40-49
//    68,70,72,72,74,75,76,77,78,79 //50-59
//};  //鏋楀ぇ鍏ゼ璧涢亾

uint8 img_real_width2[60]=
{
    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,// 0-9
    16,16,17,18,20,21,23,24,26,28,//10-19
    28,30,32,32,34,35,37,38,39,41,//20-29
    42,43,45,46,47,49,51,51,53,55,//30-39
    56,58,59,60,62,63,64,66,67,68,//40-49
    70,71,72,74,75,76,78,79,79,79 //50-59
};  //鏋楀ぇ鍥涜疆鏂板叓妤艰禌閬�

uint8 img[60][80];
/****************************鎵嚎鍙橀噺*************************/
char  left_valid[60] = {0},right_valid[60] = {0};
uint8 left_black[60] = {0};
uint8 right_black[60] = {0};
uint8 middle_line[60] = {0};
uint8 valid_row=0;

uint8 start_point_left=39;
uint8 start_point_right=39;
/************鎷愮偣*************/
uint8 left_turning_point=59; uint8 right_turning_point = 59;  //鎷愮偣

/************鐗规畩琛ョ嚎鏍囧織浣�************/
uint8 flag_midline_special =0;  // 2涓哄嚭鐜矝鐗规畩琛ョ嚎鏍囧織浣�

/***********鍋滆溅绾�***********/
uint8 stop_flag=0;
//uint8 stop_flag=1;
uint8 stop_count=0;

/***********涓夊弶*************/
uint8 flag_y_cross=0;   //杞悜
uint8 flag_y_cross_special=0;
uint8 flag_c=0; //璇嗗埆鍒�
uint8 flag_y_count=0;   //涓夊弶璁℃暟

/***********鐜矝鏍囧織浣�*************/
uint8 flag_circle_elc=0;
int16 circle_pulse_num=0;
//circle_flag 1宸︾幆宀� 2 鍙崇幆宀�
///1 鎽勫儚澶磋瘑鍒埌鐜矝 2 鎽勫儚澶寸洿琛屽悜鐜矝 3杩涚幆琛ョ嚎 4鐜唴姝ｅ父琛岃繘 5鍑虹幆鐗规畩澶勭悊 6鍑虹幆宀涚洿琛� 7 鍑虹幆鎴愬姛锛屾竻绌烘爣蹇椾綅
uint8 circle_flag=0,left_circle_flag=0,right_circle_flag=0;
///1 鎽勫儚澶磋瘑鍒埌鐜矝 2 鎽勫儚澶寸洿琛屽悜鐜矝 3杩涚幆琛ョ嚎 4鐜唴姝ｅ父琛岃繘 5鍑虹幆鐗规畩澶勭悊 6鍑虹幆宀涚洿琛� 7 鍑虹幆鎴愬姛锛屾竻绌烘爣蹇椾綅
//uint8 temp_left_line[59]={0},temp_right_line[59]={0};

//uint8 flag_a=0,flag_b=0; //鎽勫儚澶磋瘑鍒幆宀涙爣蹇椾綅

///*********鍧￠亾妫�娴�***************/
//uint8 Ramp_Flag=0;
//uint8 Ramp_Error[10]={0};
//int32 ramp_pulse_num=0;

void get_mid_line(int a)
{

}

void midline_show()
{

}

