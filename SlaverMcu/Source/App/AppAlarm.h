/************************************************
* COPYRIGHT 2016.
*
* NAME: AppAlarm.h
*
* DESCRIPTION:
*   alarm module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/20/2016
*
* END OF FILE HEADER
************************************************/

#ifndef APP_ALARM_H
#define APP_ALARM_H

#include "..\TypeDefine.h"
#include "..\Common.h"
#include "..\Kernel\CtlEvent.h"

/* alarm times */
typedef enum
{
	E_ALARM_BEEP_ONE_WITH_VIBR,
	E_ALARM_BEEP_ONE_NO_VIBR,
	E_ALARM_BEEP_TWO_WITH_VIBR,
	E_ALARM_TYPE_MAX    
} TE_ALARM_TYPE;

/* -------------external functions ------------- */
void AlarmPlay(TE_ALARM_TYPE type);

void AlarmStop();

#endif /* APP_ALARM_H */

