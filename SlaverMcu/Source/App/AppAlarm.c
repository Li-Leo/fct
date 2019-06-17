/************************************************
* COPYRIGHT 2016.
*
* NAME: AppAlarm.c
*
* DESCRIPTION:
*   alarm module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/20/2016
*
* END OF FILE HEADER
************************************************/
#include "AppAlarm.h"
#include "..\Kernel\CtlAlarm.h"
#include "..\Kernel\CtlTimer.h"
#include "..\Kernel\CtlEvent.h"
#include "..\Kernel\CtlTime.h"


/***********************************************
* Description:  play alarm beep and VibMotor.
*
* Argument:
*
* Return:
*
************************************************/
void AlarmPlay(TE_ALARM_TYPE type)
{
	assert(type < E_ALARM_TYPE_MAX);
	switch(type)
	{
		case E_ALARM_BEEP_ONE_WITH_VIBR:
    		CtlTimerBindHandle(E_TIMER_ALARM_PLAY_INTERVAL,SetPlayOnce);
			SetPlayOnce();
			break;
		case E_ALARM_BEEP_ONE_NO_VIBR:
    		CtlTimerBindHandle(E_TIMER_ALARM_PLAY_INTERVAL,SetPlayOnceNoVibration);
			SetPlayOnceNoVibration();
			break;
		case E_ALARM_BEEP_TWO_WITH_VIBR:
    		CtlTimerBindHandle(E_TIMER_ALARM_PLAY_INTERVAL,SetPlayTwice);
			SetPlayTwice();
			break;
		default:
    		//CtlTimerBindHandle(E_TIMER_ALARM_PLAY_INTERVAL,SetPlayNone);
			break;
	}
	CtlTimerSetRepeat(E_TIMER_ALARM_PLAY_INTERVAL, ALARM_PLAY_INTERVAL);
}

void AlarmStop()
{
    SetPlayNone();
    CtlTimerClear(E_TIMER_ALARM_PLAY_INTERVAL);
}


