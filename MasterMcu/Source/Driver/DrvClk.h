/*******************************************************************************
**文件名:DrvClk.h
**
**功能:定义在"DrvClk.c"文件中使用的宏、变量等。
**
**作者:
**
**日期:2016-03-17
**
**修改内容:	Adjust the codes according to the Coding Standard.
**
**作者:Kevin Shi
**
**日期:04/21/2016
**
*******************************************************************************/
#ifndef DRVCLK_H
#define DRVCLK_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

#define CPU_FREQ ((T_S32)8000000)
//#define USE_EXTERNAL_CLOCK_FOR_LOW_POWER

void ClockInit(void);

#endif	/* DRVCLK_H */

