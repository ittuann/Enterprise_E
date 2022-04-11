/*
 *  tfminiplus.c
 *
 *  Created on: 2021年1月10日
 *      Author: LBQ
 */

#include "My_Bsp.h"
#include "tfminiplus.h"

// Statement
#define TFMINI_DATA_Len     9
#define TFMINT_DATA_HEAD    (0x59)

uint16_t TFmin_Dist = 0;    // 距离信息 单位 cm
uint16_t TFmin_Strength = 0;// 信号强度 小于 100 或等于 65535 时, Dist 的测量值被认为不可信输出置0
uint16_t TFmin_Temp = 0;    // 表征芯片内部温度值 摄氏度 = Temp / 8 - 256

uint8_t TFminiPlus_flag = 0;                    // 接收数据标志位 1-成功;2-帧头错误;3-SC校验错误

uint8_t TFminiPlus_buf[TFMINI_DATA_Len] = {0};  // 暂存数据帧
uint8_t TFminiPlus_temp = 0;                    // 暂存当前字节
uint8_t TFminiPlus_num = 0;                     // 暂存本字节数据编码
uint8_t TFminiPlus_cmd[4] = {0};                // 暂存命令帧


/*
 * @name:       void TFminiPlus_Proc (uint8_t *buf, uint8_t len)
 * @function:   TFminiPlus_Proc
 * @param:      none
 * @return:     none
 * @notion:     Standard Data Format(89BYTE)
 */
void TFminiPlus_Proc (uint8_t *buf, uint8_t len)
{
    uint8_t i = 0;
    uint8_t checksum = 0;

    if (TFMINI_DATA_Len == len)                                         // 验证接收的数据帧长度与协议规定长度相同
    {
        if (TFMINT_DATA_HEAD == buf[0] && TFMINT_DATA_HEAD == buf[1])   // 校验帧头
        {
            for (i = 0; i < (TFMINI_DATA_Len - 1); i++) {
                checksum += buf[i];                                     // 从帧头开始到数据区结束, 做sumcheck校验
            }

            if (checksum == buf[TFMINI_DATA_Len - 1]) {                 // 校验Checksum
                // 数据为小端模式, 低字节保存在数据帧的低地址中
                TFmin_Dist = buf[2] | (buf[3] << 8);
                TFmin_Strength = buf[4] | (buf[5] << 8);
                TFmin_Temp = buf[6] | (buf[7] << 8);
                TFminiPlus_flag = 1;
            } else {
                TFminiPlus_flag = 3;
            }
        } else {
            TFminiPlus_flag = 2;
        }
    }
}

/*
 * @name:       void TFminiPlus_Proc2(uint8_t date)
 * @function:   TFminiPlus_Proc2
 * @param:      none
 * @return:     none
 * @example:    TFminiPlus_Proc2(TFminiPlus_temp);
 * @notion:     单字节接收方式
 */
void TFminiPlus_Proc2(uint8_t data)
{
    uint8_t i = 0;
    uint8_t checksum = 0;

    if (TFminiPlus_num == 0 && data == TFMINT_DATA_HEAD) {                      // 校验并记录帧头
        TFminiPlus_buf[0] = TFMINT_DATA_HEAD;
        TFminiPlus_num = 1;
    } else if (TFminiPlus_num == 1 && data == TFMINT_DATA_HEAD) {               // 校验并记录帧头
        TFminiPlus_buf[1] = TFMINT_DATA_HEAD;
        TFminiPlus_num = 2;
    } else if (TFminiPlus_num >= 2 && TFminiPlus_num <= (TFMINI_DATA_Len - 2)) {
        TFminiPlus_buf[TFminiPlus_num] = data;
        TFminiPlus_num++;
    } else if (TFminiPlus_num == (TFMINI_DATA_Len - 1)) {
        for (i = 0; i < (TFMINI_DATA_Len - 1); i++) {
            checksum += TFminiPlus_buf[i];                                      // 从帧头开始到数据区结束, 做checksum校验
        }

        if (checksum == data) {                                                 // 校验Checksum
            //数据为小端模式, 低字节在前保存在数据帧的低地址中
            TFmin_Dist = TFminiPlus_buf[2] | (TFminiPlus_buf[3] << 8);
            TFmin_Strength = TFminiPlus_buf[4] | (TFminiPlus_buf[5] << 8);
            TFmin_Temp = TFminiPlus_buf[6] | (TFminiPlus_buf[7] << 8);
            TFminiPlus_flag = 1;
        } else {
            TFminiPlus_flag = 3;
        }

        TFminiPlus_num = 0;
    } else {
        TFminiPlus_flag = 2;
        TFminiPlus_num = 0;
    }
}

/*
 * @name:       void TFminiPlus_GetOnce(void)
 * @function:   TFminiPlus_GetOnce
 * @param:      none
 * @return:     数据帧
 * @notion:     将输出帧率设置为0后, 才可使用单次触发指令
 */
void TFminiPlus_GetOnce(void)
{
    memset(TFminiPlus_cmd, 0, sizeof(TFminiPlus_cmd));

    TFminiPlus_cmd[0] = 0x5A;    //指令帧帧头
    TFminiPlus_cmd[1] = 0x04;    //指令帧总长度
    TFminiPlus_cmd[2] = 0x04;    //ID
    TFminiPlus_cmd[3] = 0x62;    //Data

    uart_putbuff(UART_3, TFminiPlus_cmd, sizeof(TFminiPlus_cmd)); //串口发送指令
}
