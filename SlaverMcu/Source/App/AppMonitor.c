/************************************************
* COPYRIGHT 2016.
*
* NAME: AppMonitor.c
*
* DESCRIPTION:
*   monitor system status
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/03/2016
*
* END OF FILE HEADER
************************************************/
#include "AppMonitor.h"
//#include "AppDelivery.h"
//#include "AppAlarm.h"
#include "..\Kernel\CtlTimer.h"
#include "..\Kernel\CtlAdc.h"
#include "..\Kernel\CtlDelivery.h"
#include "..\Kernel\CtlEvent.h"
//#include "..\Kernel\CtlKey.h"
//#include "..\Kernel\CtlDisplay.h"
#include "..\Kernel\CtlPower.h"
#include "..\Kernel\CtlData.h"
#include "..\Kernel\CtlTime.h"
#include "..\Kernel\CtlSpiComm.h"
#include "..\Driver\DrvPower.h"
#include "AppAlarm.h"

#define PLUSE_SIGNAL_SEND_TIME_MS	250		//used to send pluse signal
#define MONITOR_EXTERNAL_SYS_TIME_MS	50		//used to monitor external sys
#define EXTERNAL_SYS_FAIL_TIME_MS	2000		//if at this time, not receive respond, reset it
#define EXTERNAL_SYS_ERROR_MAX_COUNT 4
/************************************************
* Variables
************************************************/
//record last battery and dose info
/*TE_BATTERY_LEVEL g_monitor_last_battery_level = E_BATTERY_LEVEL_MAX;
TE_DOSE_LEVEL g_monitor_last_dose_level = E_DOSE_LEVEL_MAX;
extern TE_DELIVERY_STATE g_delivery_state;
extern TE_BOOLEAN g_occlusion_state;
extern T_U16 g_deliver_dose_value;
extern T_U16 g_deliver_dose_actual;
extern TE_BOOLEAN g_is_time_to_sleep;
extern TE_BOOLEAN g_deliver_switch;
extern T_U16 g_dose_number;
extern T_U16 g_batt_number;
extern TE_DOSE_THICKNESS g_sys_para_thickness;
extern TE_BOOLEAN g_occlusion_state;
extern T_U16 g_sys_para_actual_day_total;
extern T_U16 g_sys_para_max_day_total;
extern TE_BOOLEAN g_is_no_operation;
extern TS_TIME_INFO g_current_time;
extern TS_TIME_INFO g_next_deliver_time;
extern TE_BOOLEAN g_is_screen_display;
extern TE_BOOLEAN g_is_delivery_dose_matched;*/
//T_U16 g_moniter_alarm_state;
T_U16 pulse_signal_false_counter = 0;
T_U16 g_mcu_reset_counter = 0;
TE_BOOLEAN g_is_external_sys_resetting = E_FALSE;
//TE_BOOLEAN g_is_event_on_deli_posted = E_FALSE;
//T_U32 last_date_number = 0;
TE_BOOLEAN send_pulse_signal_status = E_FALSE;
TE_BOOLEAN pulse_signal_last = E_FALSE;

//TE_BOOLEAN pul_sig = E_FALSE;

/************************************************
* Function 
************************************************/

/***********************************************
* Description:
*   check normal system status
* Argument:
*
* Return:
*
************************************************/
static void MonitorCheckSystemStatus(void)
{
	//MonitorCheckPulseSignal();
	//MonitorSendPulseSignal();
}

/***********************************************
* Description:
*   start monitor system normal status
* Argument:
*
* Return:
*
************************************************/
void SystemMonitorStart()
{
	//if already moniting, return
	if (CtlTimerIsExist(E_TIMER_SYSTEM_MONITOR))
	{
		CtlTimerClear(E_TIMER_SYSTEM_MONITOR);
	}
	
    //then start repeat timer to monitor
    CtlTimerBindHandle(E_TIMER_SYSTEM_MONITOR, MonitorCheckSystemStatus);
    CtlTimerSetRepeat(E_TIMER_SYSTEM_MONITOR, MONITOR_SYSTEM_TIME_MS);

    //check at once
    MonitorCheckSystemStatus();
}

/***********************************************
* Description:
*   stop monitor system normal status
* Argument:
*
* Return:
*
************************************************/
void SystemMonitorStop(void)
{
    CtlTimerClear(E_TIMER_SYSTEM_MONITOR);
}

void SaveSystemParameters()
{
	
}

void ExcuteSleep()
{
    //SaveSystemParameters();
    DrvLowPower();
    //ExcuteWakeup();
}


static TE_BOOLEAN ReadPulseSignalLevel()
{
    return ((SCAN_PULSE_SIGNAL_STATUS & BIT1) != 0) ? E_TRUE : E_FALSE;
}

static void ExternSysFail()
{
    M_CommonPrint("mcu not active, reset it!\n");
    CtlEventPost(E_EVENT_RESET_MCU,1);

    if (g_mcu_reset_counter >= EXTERNAL_SYS_ERROR_MAX_COUNT)
    {
        M_CommonPrint("reset too much, alarm!\n");
		AlarmPlay(E_ALARM_BEEP_TWO_WITH_VIBR);
        ExternSysMonitorStop();
    }

}

/***********************************************
* Description:
*   check pulse signal
* Argument:
*
* Return:
*
************************************************/
void MonitorCheckPulseSignal(void)
{
    TE_BOOLEAN pulse_signal_now = E_FALSE;
    
	pulse_signal_now = ReadPulseSignalLevel();
	
	if(pulse_signal_now != pulse_signal_last)
	{        
        pulse_signal_last = pulse_signal_now;
        CtlTimerSet(E_TIMER_EXTERNAL_SYS_FAIL, EXTERNAL_SYS_FAIL_TIME_MS);
        g_mcu_reset_counter = 0;
	}
}

//monitor enternal system 
void ExternSysMonitorStart()
{
    //if already moniting, return
	if (CtlTimerIsExist(E_TIMER_EXTERNAL_SYS_MONITOR))
	{
		return;
	}
    M_CommonPrint("start monitor mcu\n");
    pulse_signal_last = ReadPulseSignalLevel();
    g_is_external_sys_resetting = E_FALSE;  
    
    //then start repeat timer to monitor
    CtlTimerBindHandle(E_TIMER_EXTERNAL_SYS_MONITOR, MonitorCheckPulseSignal);
    CtlTimerSetRepeat(E_TIMER_EXTERNAL_SYS_MONITOR, MONITOR_EXTERNAL_SYS_TIME_MS);   
    CtlTimerBindHandle(E_TIMER_EXTERNAL_SYS_FAIL, ExternSysFail);
    CtlTimerSet(E_TIMER_EXTERNAL_SYS_FAIL, EXTERNAL_SYS_FAIL_TIME_MS);   
}
void ExternSysMonitorStop()
{
    M_CommonPrint("stop monitor mcu\n");
    CtlTimerClear(E_TIMER_WAITING_MCU_RESET);
    CtlTimerClear(E_TIMER_EXTERNAL_SYS_MONITOR);
    CtlTimerClear(E_TIMER_EXTERNAL_SYS_FAIL);
}

TE_BOOLEAN ExternSysMonitorIsRuning()
{
    if(CtlTimerIsExist(E_TIMER_EXTERNAL_SYS_MONITOR))
    {
        return E_TRUE;
    }
    else
    {
        return E_FALSE;
    }
}


/***********************************************
* Description:
*   send pulse signal
* Argument:
*
* Return:
*
************************************************/
void MonitorSendPulseSignal(void)
{
    //printf("curr %lums\n", CtlTimeSincePowerOn());
	if(send_pulse_signal_status == E_FALSE)
	{
		PULSE_SIGNAL_LOW;
		send_pulse_signal_status = E_TRUE;
	}
	else if(send_pulse_signal_status == E_TRUE)
	{
		PULSE_SIGNAL_HIGH;
		send_pulse_signal_status = E_FALSE;
	}
}


void PulseSignalSendStart()
{
    MonitorSendPulseSignal();
    
    //then start repeat timer to monitor
    CtlTimerBindHandle(E_TIMER_PULSE_SIGNAL_SEND, MonitorSendPulseSignal);
    CtlTimerSetRepeat(E_TIMER_PULSE_SIGNAL_SEND, PLUSE_SIGNAL_SEND_TIME_MS);    
}


