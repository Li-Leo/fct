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
**用于用户数据存储的FRAM容量为45K,起始地址：0x10040；结束地址：0x1B440
*******************************************************************************/
#define FRAM_START_ADDRESS      0x10040//用户FRAM起始地址
#define FRAM_END_ADDRESS        0x1B440//用户FRAM结束地址

void FramReadData(T_U32 *data_point, T_U32 *fram_point, T_U16 data_num);
void FramWriteData(T_U32 *data_point,T_U32 *fram_point, T_U16 data_num);

#endif /* DRV_FRAM_H */
