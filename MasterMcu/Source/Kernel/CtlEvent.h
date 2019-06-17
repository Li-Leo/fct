/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlEvent.h
*
* DESCRIPTION:
*   define some function interfaces of event module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 04/28/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLEVENT_H
#define CTLEVENT_H

#include "..\TypeDefine.h"
#include "CtlInterrupt.h"
/************************************************
* Declaration
************************************************/
//all event
typedef enum 
{	
	E_EVENT_NULL=0, //must at first, use only internal
	/*id begin, add id after this line*/   
	E_EVENT_KEY_PRESSED,
	E_EVENT_KEY_RELEASED,
	E_EVENT_KEY_HELD_2S,
	E_EVENT_KEY_HELD,
	E_EVENT_KEY_ANY_KEY_PRESSED,
	E_EVENT_KEY_ANY_KEY_RELEASED,
	//when one key pressed more than 30s, the event will exist, the param is the key
	E_EVENT_KEY_STUCK, 
	//enter to stop state
	E_EVENT_ENTER_STOP_STATE,
	//enter to pause state 
	E_EVENT_ENTER_PAUSE_STATE,
	//enter to run state
	E_EVENT_ENTER_RUN_STATE,
	//enter to system sleep
	E_EVENT_SYSTEM_SLEEP,
	/* alarm event	*/
	//E_EVENT_REMIND_DOSE_LEFT_LESS_THAN_INFU_VOL,
	//E_EVENT_REMIND_DAY_TOTAL_OVERRUN,
	//E_EVENT_REMIND_DAY_TOTAL_OUT_OF_RANGE,
	E_EVENT_ALARM_BATT_LOW,
	E_EVENT_ALARM_DOSE_LOW,
	E_EVENT_ALARM_BATT_EMPTY,
	E_EVENT_ALARM_DOSE_EMPTY,
	E_EVENT_ALARM_OCCLUSION,
	E_EVENT_ALARM_DELI_SWITCH_OFF,
	//E_EVENT_ALARM_PULSE_SIGNAL_FALSE,
	E_EVENT_ALARM_SYSTEM_ERROR,
	//E_EVENT_ALARM_CLEAR,
	/* automatic location finished */
	E_EVENT_AUTO_LOCATION_FINISHED,
	/* manual location finished */
	E_EVENT_MANUAL_LOCATION_FINISHED,
	E_EVENT_ON_DELIVER_POINT,
	//when on deliver finished, this event is post
	E_EVENT_DELIVER_FINISHED,
	//when no operation time period arrive 30s,this event is post
	E_EVENT_NO_OPERATION,
	//when mcu's pulse signal failed, reset mcu
	E_EVENT_RESET_MCU,
	//EVENT_LCD_DISPLAY_OFF,
	E_EVENT_LCD_DISPLAY_ON,

    E_EVENT_BATTERY_LEVEL_CHANGED,

    E_EVENT_MOTOR_STOP, //param: the stop cause
    E_EVENT_MOTOR_STOPPING, //motor is  stoping
    E_EVENT_SYSTEM_MODE_CHANGED, //system mode changed, para: system mode
    E_EVENT_ALARM_NOTIFY, //param: the alarm
    E_EVENT_ALARM_CLEAR, //param: the alarm 
    E_EVENT_ALARM_NONE, //no alarm
    
	/*id end, do not add id after this line*/
	E_EVENT_MAX  //must at end,
} TE_EVENT;

//event struct
typedef struct 
{
	TE_EVENT event;
    T_U32 param;
} TS_EVENT;

typedef void (*TF_EVENT_FUNC)(T_U32 param);

//get event, if no event, will return E_EVENT_NULL
void CtlEventGet(TS_EVENT *event_obj);

//handle event
void CtlEventHandle(TS_EVENT *event_obj);

//handle all events until no event
void CtlEventHandleAll();

//post event then return immediately
void CtlEventPost(TE_EVENT event, T_U32 param);

//bind handle function for event
void CtlEventBindHandle(TE_EVENT event, TF_EVENT_FUNC func);

//clear one event's handle
void CtlEventClearHandle(TE_EVENT event);

//clear all events' handle
void CtlEventClearAllHandle(void);

//is exist event
TE_BOOLEAN CtlEventIsEmpty(void);

#endif /* CTLENENT_H */

