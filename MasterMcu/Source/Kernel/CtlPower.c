/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlPower.c
*
* DESCRIPTION:
*   power module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlPower.h"
#include "CtlEvent.h"

//************global variables******************
static TE_BATTERY_STATUS g_battery_status;

/***********************************************
*   description:
*       power initialization
*   parameters:
*       N/A
*   return:       
*       N/A
***********************************************/
void CtlPwrInitial(void)
{
 	DrvInitPowerControl();
}

/***********************************************
*   description:
*       set power status
*   parameters:
*       status: battery status
*   return:       
*       N/A
***********************************************/
//	void SetPowerStatus(TE_BATTERY_STATUS status)
//	{
//		switch(status)
//		{
//			case E_BATTERY_STATUS_ON:
//				//DrvPowerOn();
//				break;
//			case E_BATTERY_STATUS_SLEEP:
//				DrvPowerSleep();
//				CtlEventPost(E_EVENT_SYSTEM_SLEEP, 1);
//				break;
//			case E_BATTERY_STATUS_STANDBY:
//				DrvPowerStandby();
//				CtlEventPost(E_EVENT_ENTER_PAUSE_STATE, 1);
//				break;
//			default:
//				break;
//		}
//		g_battery_status = status;
//	}

/***********************************************
*   description:
*       get power shutdown
*   parameters:
*       N/A
*   return:       
*       battery status
***********************************************/
TE_BATTERY_STATUS GetPowerStatus(void)
{
	return g_battery_status;
}

