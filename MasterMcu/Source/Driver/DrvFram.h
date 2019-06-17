/*******************************************************************************
**文件名:DrvFram.h
**
**功能:定影在“DrvFram.c”文件以及其它一些文件中使用的宏、变量等
**
**作者:
**
**日期:2016-03-01
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#ifndef DRV_FRAM_H
#define DRV_FRAM_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"
/*******************************************************************************
**用于用户主要数据存储的FRAM容量为0.5K, 放在information memory,下载程序时可以选择是否清空
**用于用户数据存储的FRAM容量为33K(其中参数: 1K,输注记录:4K, 日总量记录:4K, 停输记录:4K, 操作记录:20K),起始地址：0x4400；结束地址：0xC7FF
*******************************************************************************/
//512byte to save main param
#define FRAM_MAIN_PARAM_START_ADDRESS      0x1800//用户FRAM起始地址
#define FRAM_MAIN_PARAM_END_ADDRESS        0x19FF//用户FRAM结束地址

//33K to save other param and log
#define FRAM_START_ADDRESS      (T_U16)0x4400//用户FRAM起始地址
#define FRAM_END_ADDRESS        (T_U16)0xC7FF//用户FRAM结束地址

void FramReadData(T_U32 *data_point, T_U32 *fram_point, T_U16 data_num);
void FramWriteData(T_U32 *data_point,T_U32 *fram_point, T_U16 data_num);

#endif /* DRV_FRAM_H */
