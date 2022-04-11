/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_flash
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

#include "ch32v10x_misc.h"
#include "ch32v10x_flash.h"
#include "zf_flash.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      校验FLASH是否有数据
//  @param      sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        当前扇区页的编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @return                     返回1有数据，返回0没有数据，如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
//  @since      v1.0
//  Sample usage:               flash_check(FLASH_SECTION_00,FLASH_PAGE_0);
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num)
{
    uint16 temp_loop;
    uint32 flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_num+FLASH_PAGE_SIZE*page_num));     // 提取当前 Flash 地址

    for(temp_loop = 0; temp_loop < FLASH_PAGE_SIZE; temp_loop+=4)                                       // 循环读取 Flash 的值
    {
        if( (*(__IO u32*) (flash_addr+temp_loop)) != 0xffffffff )                                       // 如果不是 0xffffffff 那就是有值
            return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      擦除扇区
//  @param      sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        当前扇区页的编号     参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @return                     返回1有表示失败  返回0表示成功
//  @since      v1.0
//  Sample usage:               flash_erase_page(FLASH_SECTION_00,FLASH_PAGE_0);
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_erase_page (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32 flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_num+FLASH_PAGE_SIZE*page_num));     // 提取当前 Flash 地址

    FLASH_Unlock();                                                                                     // 解锁 Flash
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);                           // 清除操作标志
    gFlashStatus = FLASH_ErasePage(flash_addr);                                                         // 擦除
    FLASH_ClearFlag(FLASH_FLAG_EOP );                                                                   // 清楚操作标志
    FLASH_Lock();                                                                                       // 锁定 Flash
    if(gFlashStatus != FLASH_COMPLETE)                                                                  // 判断操作是否成功
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取一页
//  @param      sector_num      需要读取的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        需要读取的页编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @param      buf             需要读取的数据地址   传入的数组类型必须为uint32
//  @param      len             需要读取的数据长度   参数范围 1-256
//  @return                     返回1有表示失败  返回0表示成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flash_page_read (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num, uint32 *buf, uint16 len)
{
    uint16 temp_loop = 0;
    uint32 flash_addr = 0;
    flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_num+FLASH_PAGE_SIZE*page_num));            // 提取当前 Flash 地址
    //flash_addr = ((uint32_t)0x0800F000);

    for(temp_loop = 0; temp_loop < len; temp_loop++)                                                    // 根据指定长度读取
    {
        *buf++ = *(__IO uint32_t*)(flash_addr+temp_loop*4);                                             // 循环读取 Flash 的值
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编程一页
//  @param      sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        需要写入的页编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @param      buf             需要写入的数据地址   传入的数组类型必须为uint32
//  @param      len             需要写入的数据长度   参数范围 1-256
//  @return                     返回1有表示失败  返回0表示成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_page_program (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num, const uint32 *buf, uint16 len)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32 flash_addr = 0;
    flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_num+FLASH_PAGE_SIZE*page_num));            // 提取当前 Flash 地址

    if(flash_check(sector_num, page_num))                                                               // 判断是否有数据 这里是冗余的保护 防止有人没擦除就写入
        flash_erase_page(sector_num, page_num);                                                         // 擦除这一页

    FLASH_Unlock();                                                                                     // 解锁 Flash
    while(len--)                                                                                        // 根据长度
    {
        gFlashStatus = FLASH_ProgramWord(flash_addr, *buf++);                                           // 按字 32bit 写入数据
        if(gFlashStatus != FLASH_COMPLETE)                                                              // 反复确认操作是否成功
            return 1;
        flash_addr += 4;                                                                                // 地址自增
    }
    FLASH_Lock();                                                                                       // 锁定 Flash
    return 0;
}




























