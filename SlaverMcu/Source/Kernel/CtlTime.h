/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlTime.h
*
* DESCRIPTION:
*   define some function interfaces of time module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/05/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLTIME_H
#define CTLTIME_H

#include "..\TypeDefine.h"
#include "..\Driver\DrvTimer.h"

/************************************************
* Declaration
************************************************/
//returns true if the time a is after or equal time b
#define M_IsTimeAfterOrEq(a,b) ((T_S32)(a) - (T_S32)(b) >= 0)

//when 1 tick elapsed, call it, only called by the timer interrupt
void CtlTimeOnOneTick(void);

//calc the elapsed time from old_ms_time to now, unit: ms, old_ms_time:unit=ms
//set old_ms_time as 0 can get the elapsed time from power on.
T_U32 CtlTimeElapse(T_U32 old_ms_time);

//calc the elapsed time from old_ms_time to new_ms_time, unit: ms
T_U32 CtlTimeDiff(T_U32 new_ms_time, T_U32 old_ms_time);

//get working time since power on, unit: ms, it will recount when it reach MAX_T_U32 millisecond
T_U32 CtlTimeSincePowerOn(void);

//after tick occurred, is the tick handled
TE_BOOLEAN CtlTimeIsTickHandled(void);

//set the tick handled or not handled
void CtlTimeSetTickHandled(TE_BOOLEAN is_handled);

#endif /* CTL_TIME */

