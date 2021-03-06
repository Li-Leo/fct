/*******************************************************************************
**文件名:DrvTimer.h
**
**功能:定义在"DrvTimer.c"中使用的宏、枚举变量等以及对相关函数进行申明。
**
**作者:HJH
**
**日期:2016-04-18
**
**修改内容:	Adjust the codes according to the Coding Standard.
**
**作者:Kevin Shi
**
**日期:04/21/2016
**
*******************************************************************************/
#ifndef DRVTIMER_H
#define DRVTIMER_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

//the milliseconds per tick
#define MSECS_PER_TICK 5

typedef enum 
{
  E_TIMER_CHANNEL_A0,
  E_TIMER_CHANNEL_A1,
  E_TIMER_CHANNEL_A2,
  E_TIMER_CHANNEL_A3,
  E_TIMER_CHANNEL_B0,
  E_TIMER_CHANNEL_MAX,
} TE_TIMER_CHANNEL;

void DrvTimerInitial(void);
//void DrvTimerEnable(T_U16 timer_count,TE_TIMER_CHANNEL cha);
void DrvTimerEnable(TE_TIMER_CHANNEL cha);
void DrvTimerDisable(TE_TIMER_CHANNEL cha);
__interrupt void TimerA0Isr(void);
__interrupt void TimerA1Isr(void);
__interrupt void TimerA2Isr(void);
__interrupt void TimerA3Isr(void);
__interrupt void TimerB0Isr(void);

#endif	/* DRVTIMER_H */

