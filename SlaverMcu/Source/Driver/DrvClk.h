/*******************************************************************************
**文件名:DrvClk.h
**
**功能:定义在"DrvClk.c"文件中使用的宏、变量等。
**
**作者:
**
**日期:2016-03-17
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#ifndef DRV_CLK_
#define DRV_CLK_

#include<msp430fr5989.h>
#include"TypeDefine.h"
#define CPU_FREQ ((T_S64)4000000)

void ClockInit(void);

#endif

