/*******************************************************************************
**�ļ���:DrvTimer.h
**
**����:������"DrvTimer.c"��ʹ�õĺꡢö�ٱ������Լ�����غ�������������
**
**����:HJH
**
**����:2016-04-18
**
**�޸�����:	Adjust the codes according to the Coding Standard.
**
**����:Kevin Shi
**
**����:04/21/2016
**
*******************************************************************************/
#ifndef DRVTIMER_H
#define DRVTIMER_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

//the milliseconds per tick
#define MSECS_PER_TICK 5          // 5ms per tick

typedef enum 
{
  E_TIMER_CHANNEL_A0,
  E_TIMER_CHANNEL_A1,
  E_TIMER_CHANNEL_A2,
  E_TIMER_CHANNEL_A3,
  E_TIMER_CHANNEL_B0,
  E_TIMER_CHANNEL_MAX,
} TE_TIMER_CHANNEL;

extern T_S32 g_timer_pass_count ;
extern T_S32 g_auto_wakeup_timer_ms;
//only use the week, hour, minute, second
extern TS_TIME_INFO g_auto_wakeup_time;

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

