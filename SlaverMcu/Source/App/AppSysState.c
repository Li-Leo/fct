/************************************************
* COPYRIGHT 2016.
*
* NAME: AppSysState.c
*
* DESCRIPTION:
*   process the shift of system status
*
* REVISION HISTORY:
*	Created by Kevin Shi at 07/06/2016
*
* END OF FILE HEADER
************************************************/
#include "AppEventHandle.h"
//#include "..\kernel\CtlCommon.h"
#include "..\kernel\CtlEvent.h"
#include "..\kernel\CtlDelivery.h"
//#include "AppAlarm.h"
#include "AppMonitor.h"
//#include "AppDelivery.h"
#include "..\kernel\CtlTimer.h"
#include "..\kernel\CtlData.h"

/************************************************
* Declaration
************************************************/

/************************************************
* Variable 
************************************************/
//extern TE_BOOLEAN g_is_event_on_deli_posted;

/************************************************
* Function 
************************************************/

/***********************************************
* Description:
*   do common thing and switch screen when enter stop state
* Argument:
*   param:
*
* Return:
*
************************************************/
static void AppStateHanldeOnEnterStop(T_U32 param)
{
    //set the state
    CtlSetDeliveryState(E_DELI_STATE_STOP);
    SaveSysPara(E_SYSTEM_PARA_DELI_STATE, 0);
    //g_is_event_on_deli_posted = E_FALSE;

    //run system and device runtime monitor
    SystemMonitorStart();
	//DeviceRunTimeMonitorStart();

    //stop infusion monitor
    //InfusionMonitorStop();

    //clear infusion alarms
    //AppClearInfusionAlarms();

    //CtlTimerSet(E_TIMER_AUTO_SLEEP, AUTO_SLEEP_TIME_MS);
}

/***********************************************
* Description:
*   do common thing and switch screen when enter pause state
* Argument:
*   param: 
*
* Return:
*
************************************************/
static void AppStateHanldeOnEnterPause(T_U32 param)
{
    //set the state
    CtlSetDeliveryState(E_DELI_STATE_PAUSE);
    SaveSysPara(E_SYSTEM_PARA_DELI_STATE, 0);

    //run system and device runtime monitor
    //SystemMonitorStart();
	//DeviceRunTimeMonitorStart();

    //stop infusion monitor
    //InfusionMonitorStop();
}

/***********************************************
* Description:
*   do common thing and switch screen when enter run state
* Argument:
*   param:
*
* Return:
*
************************************************/
static void AppStateHanldeOnEnterRun(T_U32 param)
{
    //set the state
    CtlSetDeliveryState(E_DELI_STATE_RUNNING);
    SaveSysPara(E_SYSTEM_PARA_DELI_STATE, 0);
	
	//run system and device runtime monitor
    //SystemMonitorStart();
    //InfusionMonitorStart();

    //stop device runtime monitor
    //DeviceRunTimeMonitorStop();

}

/***********************************************
* Description:
*   do common thing and switch screen when enter sleep state
* Argument:
*   param:
*
* Return:
*
************************************************/
static void AppStateHanldeOnEnterSleep(T_U32 param)
{
    //set the state
    CtlSetDeliveryState(E_DELI_STATE_POWEROFF);
    //SaveSysPara(E_SYSTEM_PARA_DELI_STATE, 0);
	
    //TS_TIME_INFO wakeup_time = GetNextWakeTime();

	//stop all monitors
	//SystemMonitorStop();
	//InfusionMonitorStop();
	//DeviceRunTimeMonitorStop();
    //SetNextWakeTime(wakeup_time);

	//excute sleep
    ExcuteSleep();
}

/***********************************************
* Description:
*   register the state event to handle the state's change
* Argument:
*
* Return:
*
************************************************/
void AppSysStateInit(void)
{
    CtlEventBindHandle(E_EVENT_ENTER_STOP_STATE, AppStateHanldeOnEnterStop);
    CtlEventBindHandle(E_EVENT_ENTER_PAUSE_STATE, AppStateHanldeOnEnterPause);
    CtlEventBindHandle(E_EVENT_ENTER_RUN_STATE, AppStateHanldeOnEnterRun);
    CtlEventBindHandle(E_EVENT_SYSTEM_SLEEP, AppStateHanldeOnEnterSleep);
}

