/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlAlarm.c
*
* DESCRIPTION:
*   alarm module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/11/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlAlarm.h"
#include "CtlTimer.h"
#include "..\Driver\DrvBeep.h"

static void TimerHandleStopBeep(void)
{
	DrvBuzzSetState(E_BUZZ_OFF);
	CtlTimerClear(E_TIMER_ALARM_PLAY_BEEP_OFF);
}

static void TimerHandleStopVibrate(void)
{
	DrvVibMotorSetState(E_ZD_OFF);
	CtlTimerClear(E_TIMER_ALARM_PLAY_VIBM_OFF);
}

static void TimerHandleStartVibrate(void)
{
	DrvVibMotorSetState(E_ZD_ON);
	CtlTimerClear(E_TIMER_ALARM_PLAY_VIBM_ON);
}

/***********************************************
* Description:
*   Set buzzing vibrating NOT play.
* Argument:
*   N/A
*   
* Return:
*   N/A
************************************************/
void SetPlayNone()
{
	DrvBuzzSetState(E_BUZZ_OFF);
	DrvVibMotorSetState(E_ZD_OFF);
    CtlTimerClear(E_TIMER_ALARM_PLAY_BEEP_OFF);
    CtlTimerClear(E_TIMER_ALARM_PLAY_VIBM_OFF);
    CtlTimerClear(E_TIMER_ALARM_PLAY_VIBM_ON);
    CtlTimerClear(E_TIMER_ALARM_PLAY_TWICE);
}

/***********************************************
* Description:
*   Set buzzing vibrating play once in turn.
* Argument:
*   N/A
*   
* Return:
*   N/A
************************************************/
/*void SetPlayOnce()
{
	//T_U16 play_ms = 400;
	//T_U16 break_ms = 100;
	DrvBuzzSetState(E_BUZZ_OFF);
	DrvVibMotorSetState(E_ZD_OFF);
	
	DrvBuzzSetState(E_BUZZ_ON);
	DelayMs(PLAY_MS);
	DrvBuzzSetState(E_BUZZ_OFF);
	DelayMs(WAIT_MS);
	
	DrvVibMotorSetState(E_ZD_ON);
	DelayMs(PLAY_MS);
	DrvVibMotorSetState(E_ZD_OFF);
}*/

void SetPlayOnce()
{
	DrvBuzzSetState(E_BUZZ_OFF);
	DrvVibMotorSetState(E_ZD_OFF);
	
	CtlTimerBindHandle(E_TIMER_ALARM_PLAY_BEEP_OFF, TimerHandleStopBeep);
	CtlTimerBindHandle(E_TIMER_ALARM_PLAY_VIBM_ON, TimerHandleStartVibrate);
	CtlTimerBindHandle(E_TIMER_ALARM_PLAY_VIBM_OFF, TimerHandleStopVibrate);
	DrvBuzzSetState(E_BUZZ_ON);
	CtlTimerSet(E_TIMER_ALARM_PLAY_BEEP_OFF, PLAY_MS);
	CtlTimerSet(E_TIMER_ALARM_PLAY_VIBM_ON, PLAY_MS + WAIT_MS);
	CtlTimerSet(E_TIMER_ALARM_PLAY_VIBM_OFF, PLAY_MS + WAIT_MS + PLAY_MS);
	CtlTimerClear(E_TIMER_ALARM_PLAY_TWICE);
}

/***********************************************
* Description:
*   Set buzzing play once in turn without vibrating.
* Argument:
*   N/A
*   
* Return:
*   N/A
************************************************/
/*void SetPlayOnceNoVibration()
{
	//T_U16 play_ms = 400;
	//T_U16 break_ms = 100;
	DrvBuzzSetState(E_BUZZ_OFF);
	DrvVibMotorSetState(E_ZD_OFF);
	
	DrvBuzzSetState(E_BUZZ_ON);
	DelayMs(PLAY_MS);
	DrvBuzzSetState(E_BUZZ_OFF);
	DelayMs(WAIT_MS);
}*/

void SetPlayOnceNoVibration()
{
	DrvBuzzSetState(E_BUZZ_OFF);
	DrvVibMotorSetState(E_ZD_OFF);
	
	CtlTimerBindHandle(E_TIMER_ALARM_PLAY_BEEP_OFF, TimerHandleStopBeep);
	DrvBuzzSetState(E_BUZZ_ON);
	CtlTimerSet(E_TIMER_ALARM_PLAY_BEEP_OFF, PLAY_MS);
}


/***********************************************
* Description:
*   Set buzzing vibrating play twice in turn.
* Argument:
*   N/A
*
* Return:
*   N/A
************************************************/
/*void SetPlayTwice()
{
	//T_U16 play_ms = 400;
	//T_U16 break_ms = 100;
	DrvBuzzSetState(E_BUZZ_OFF);
	DrvVibMotorSetState(E_ZD_OFF);
	
	DrvBuzzSetState(E_BUZZ_ON);
	DelayMs(PLAY_MS);
	DrvBuzzSetState(E_BUZZ_OFF);
	DelayMs(WAIT_MS);
	
	DrvVibMotorSetState(E_ZD_ON);
	DelayMs(PLAY_MS);
	DrvVibMotorSetState(E_ZD_OFF);
	DelayMs(WAIT_MS); 
	
	DrvBuzzSetState(E_BUZZ_ON);
	DelayMs(PLAY_MS);
	DrvBuzzSetState(E_BUZZ_OFF);
	DelayMs(WAIT_MS);
	
	DrvVibMotorSetState(E_ZD_ON);
	DelayMs(PLAY_MS);
	DrvVibMotorSetState(E_ZD_OFF);
}*/

void SetPlayTwice()
{
	CtlTimerBindHandle(E_TIMER_ALARM_PLAY_TWICE, SetPlayOnce);
	SetPlayOnce();
	CtlTimerSet(E_TIMER_ALARM_PLAY_TWICE, 2 * (PLAY_MS + WAIT_MS));	
}

