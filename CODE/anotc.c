/*
 *  anotc.h
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "anotc.h"

#define UserDataLen 8

uint8_t waveform[6 + UserDataLen] = {0};     // 数据帧缓存

/*
 * @name:       void Wireless_Send(void)
 * @function:   Wireless_Send
 * @param:      none
 * @return:     none
 * @notion:     V7
 * @notion:     逐飞无线串口一次发送数据最好不要超过30字节
 */
void Wireless_Send(void)
{
//    uint8_t waveform[6 + UserDataLen] = {0};    // 数据帧缓存
    uint8_t _cnt = 0;

    memset(waveform, 0, sizeof(waveform));

    waveform[_cnt++] = 0xAA;        // 帧头
    waveform[_cnt++] = 0xFF;        // 目标地址
    waveform[_cnt++] = 0xF1;        // 功能码ID
//    waveform[_cnt++] = sizeof(waveform) - 6;  // 有效数据长度
//    waveform[_cnt++] = UserDataLen;
    waveform[_cnt++] = 0;

    int16_t UserData_1 = (int16_t)(Mortor1.Speed_Read);
    int16_t UserData_2 = (int16_t)(Mortor2.Speed_Read);
    int16_t UserData_3 = (int16_t)(Mortor3.Speed_Read);
    int16_t UserData_4 = (int16_t)(Mortor4.Speed_Read);

//    int32_t UserData_1 = (int32_t)(Gyro_Z_FilterLast[0] * 1000.0f);

    //数据区使用小端模式, 低字节在前。
//    waveform[_cnt++] = int8;
//    waveform[_cnt++] = BYTE0(int16);
//    waveform[_cnt++] = BYTE1(int16);
//    waveform[_cnt++] = BYTE0(int32);
//    waveform[_cnt++] = BYTE1(int32);
//    waveform[_cnt++] = BYTE2(int32);
//    waveform[_cnt++] = BYTE3(int32);
//    waveform[_cnt++] = BYTE0(float * 100.000f);
//    waveform[_cnt++] = BYTE1(float * 100.000f);
//    waveform[_cnt++] = BYTE2(float * 100.000f);
//    waveform[_cnt++] = BYTE3(float * 100.000f);

    waveform[_cnt++] = BYTE0(UserData_1);   // 数据内容
    waveform[_cnt++] = BYTE1(UserData_1);
//    waveform[_cnt++] = BYTE2(UserData_1);
//    waveform[_cnt++] = BYTE3(UserData_1);

    waveform[_cnt++] = BYTE0(UserData_2);
    waveform[_cnt++] = BYTE1(UserData_2);

    waveform[_cnt++] = BYTE0(UserData_3);
    waveform[_cnt++] = BYTE1(UserData_3);

    waveform[_cnt++] = BYTE0(UserData_4);
    waveform[_cnt++] = BYTE1(UserData_4);

    waveform[3] = _cnt - 4; // 写入有效数据字节数

    uint8_t sumcheck = 0;   // 和校验SC
    uint8_t addcheck = 0;   // 附加校验AC
    for(uint8_t i = 0; i < waveform[3] + 4; i++) {
      sumcheck += waveform[i];      // 从帧头开始, 一直到 data 区结束, 对每一字节进行累加操作, 只取低8位
      addcheck += sumcheck;         // 计算和校验时, 每进行一字节的加法运算, 同时进行一次 sumcheck 的累加操作, 只取低8位
    }
    waveform[_cnt++] = sumcheck;
    waveform[_cnt++] = addcheck;

    seekfree_wireless_send_buff(waveform, sizeof(waveform));    //串口发送数据
}
