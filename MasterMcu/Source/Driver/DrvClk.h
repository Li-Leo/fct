/*******************************************************************************
**�ļ���:DrvClk.h
**
**����:������"DrvClk.c"�ļ���ʹ�õĺꡢ�����ȡ�
**
**����:
**
**����:2016-03-17
**
**�޸�����:	Adjust the codes according to the Coding Standard.
**
**����:Kevin Shi
**
**����:04/21/2016
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

