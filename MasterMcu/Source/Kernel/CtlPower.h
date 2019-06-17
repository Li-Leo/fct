/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlPower.h
*
* DESCRIPTION:
*   power module head file
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLPOWER_H
#define CTLPOWER_H

#include <msp430fr5989.h>
#include "TypeDefine.h"
#include "..\Driver\DrvPower.h"

typedef enum 
{
	E_BATTERY_STATUS_ON,
	E_BATTERY_STATUS_SLEEP,
	E_BATTERY_STATUS_STANDBY,
	E_BATTERY_STATUS_MAX,
} TE_BATTERY_STATUS;

void CtlPwrInitial(void);
void SetPowerStatus(TE_BATTERY_STATUS status);
TE_BATTERY_STATUS GetPowerStatus(void);

#endif /* CTLPOWER_H */

