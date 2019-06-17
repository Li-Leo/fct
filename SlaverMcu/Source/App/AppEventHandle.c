/************************************************
* COPYRIGHT 2016.
*
* NAME: AppEventHandle.c
*
* DESCRIPTION:
*   handle all events processing
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/29/2016
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
//#include "AppuiScreen.h"
#include "..\kernel\CtlTimer.h"
//#include "..\kernel\CtlData.h"
//#include "..\Driver\DrvRtc.h"
//#include "..\Driver\DrvLcd.h"
#include "CtlMotor.h"
#include "CtlTime.h"
#include "stdio.h"

/************************************************
* Declaration
************************************************/

/************************************************
* Variable 
************************************************/
/*extern TE_DELIVERY_STATE g_delivery_state;
extern TE_BOOLEAN g_is_screen_display;
extern TE_BOOLEAN g_is_any_key_pressed;
extern TE_KEY g_key_current_pressed_key;
extern TS_TIME_INFO g_current_time;
extern TS_RECORD_POINT g_record_point;
extern TE_BACK_LIGHT g_back_light_status;
extern T_U16 g_deliver_dose_actual;
extern T_U16 g_dose_number;
TS_RECORD op_rec;*/
extern T_U16 g_mcu_reset_counter;
T_S16 g_mcu_reset_flag = 0;

/************************************************
* Function 
************************************************/

/***********************************************
* Description:
*   handle the actions after mcu reset
* Argument:
*
* Return:
*
************************************************/
void AppTimerHandleAfterMasterMcuReset(void)
{
    //printf("start %lums\n", CtlTimeSincePowerOn());
    //ExternSysMonitorStart();
}

/***********************************************
* Description:
*   handle the event when mcu's pulse signal failed: reset mcu
* Argument:
*   param:
*
* Return:
*
************************************************/
static void AppEventHanldeResetMasterMcu(T_U32 param)
{
	/*Ds1302TimeRead();
	op_rec.rec_time = g_current_time;
	op_rec.rec_id = g_record_point.p_operation_rec;
	op_rec.rec_para = 82;
	op_rec.rec_data = 0;*/
	
	//excute reset
    MCU_RESET_EXCUTE;
	
	//Add a operation recorder
	//SaveRec(op_rec);

    //stop motor
    if(g_motor_is_running)
    {
        CtlMotorStop();
    }

    //stop external sys monitor
    ExternSysMonitorStop();
    
    //start timer to wait for MCU reset    
    g_is_external_sys_resetting = E_TRUE;    
	g_mcu_reset_counter++;
    g_mcu_reset_flag = 1;

    //printf("stop1 %lums\n", CtlTimeSincePowerOn());
    //recover the port    
    DelayMs(5);
    MCU_RESET_RECOVER;

    //start timer to wait for MCU reset
    CtlTimerBindHandle(E_TIMER_WAITING_MCU_RESET, AppTimerHandleAfterMasterMcuReset);
    CtlTimerSet(E_TIMER_WAITING_MCU_RESET, MCU_RESET_TIME_MS);
    //printf("stop2 %lums\n", CtlTimeSincePowerOn());
}

/***********************************************
* Description:
*   do common thing while event null was pushed.
* Argument:
*   param:
*
* Return:
*
************************************************/
static void AppEventHanldeNull(T_U32 param)
{
    return;
}

/***********************************************
* Description:
*   register all the event to handle the fucntions
* Argument:
*
* Return:
*
************************************************/
void AppEventHandleInit(void)
{
	//op_rec.rec_type = E_RECORD_OPERATION;
    CtlEventBindHandle(E_EVENT_RESET_MCU, AppEventHanldeResetMasterMcu);
    CtlEventBindHandle(E_EVENT_NULL, AppEventHanldeNull);
}

