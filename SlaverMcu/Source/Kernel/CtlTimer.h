/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlTimer.h
*
* DESCRIPTION:
*   define some function interfaces of timer module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/05/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLTIMER_H
#define CTLTIMER_H

#include "..\TypeDefine.h"
#include "CtlInterrupt.h"
#include "..\Driver\DrvClk.h"

/************************************************
* Declaration
************************************************/
#define AUTO_SLEEP_TIME_MS	1000
#define MCU_RESET_TIME_MS	30*1000		//MCU复位等待30s

#define MONITOR_SYSTEM_TIME_MS		100		//used to monitor normal system status
#define MONITOR_INFUSION_TIME_MS	20		//used to monitor infusion status
#define MONITOR_DEVICE_RUN_TIME_MS	1000	//used to monitor device run time status
#define MONITOR_REFRESH_SCREEN_MS	500
#define BATTERY_EXHAUSTION_TIME_MS	(3*60*1000)
#define TEXT_DISPLAY_FLASH_MS		1000
#define UPDATE_MOTOR_CURRENT		300		//自动定位时判断电机电流间隔
#define UPDATE_DOSE_DISPLAY_PERIOD	100		//刷新剂量数值时间间隔

#define TURN_OFF_BACKLIGHT_TIME_MS	10000	//等待10s后关闭背光
#define UPDATE_TRANS_CAL_PERIOD		1000	//每秒显示“.”
#define UPDATE_TIME_INFO_PERIOD		1000	//每秒检测时间信息

/* alarm play and show need delay  xx ms */
#define ALARM_PLAY_INTERVAL	10000
#define ALARM_SHOW_KEEP		5000

//define a ms-class delay
#define DelayMs(n) __delay_cycles((T_S32)(CPU_FREQ*(T_S64)n/1000.0))

typedef enum
{    
	/*id begin, add timer id after this line*/	 
	E_TIMER_COMMON_DELAY,			//common delay for app,
	//E_TIMER_SCAN_KEY,				//timer to scan keyboard to get stabilize state
	//E_TIMER_DELIVERING_MONITOR,		//timer to monitor deliver process
	E_TIMER_SYSTEM_MONITOR,			//used to monitor normal system status
	E_TIMER_EXTERNAL_SYS_MONITOR,	//used to monitor external system status
	E_TIMER_EXTERNAL_SYS_FAIL,
	E_TIMER_PULSE_SIGNAL_SEND,	    //used to send pulse signal	
	//E_TIMER_AUTO_SLEEP,				//used to set device sleeping after each deliver finished
	//E_TIMER_NO_OPERATION_SLEEP,		//used to set device sleeping while no operation happend during 30s
	//E_TIMER_UPDATE_MOTOR_FREQUENCY,	//used to update motor's frequency
	//E_TIMER_CHECK_BONUS,			//timer to check bonus after a certain seconds later 
									//when E_EVENT_ON_DELIVER_POINT was post
	//E_TIMER_BATTERY_EXHAUSTION,		//used to set a timer to power off
	//E_TIMER_SPI_SERVER_CHECK,		//used to check if received some data
	//E_TIMER_WAKEUP_MCU,				//used to wake up MCU when 10s before next deliver time
	//E_TIMER_DISPLAY_FLASH,			//used to control the time when some texts flashed
	//E_TIMER_ALARM_SHOW_KEEP,
	E_TIMER_ALARM_PLAY_TWICE,
	E_TIMER_ALARM_PLAY_BEEP_OFF,
	E_TIMER_ALARM_PLAY_VIBM_ON,
	E_TIMER_ALARM_PLAY_VIBM_OFF,
	E_TIMER_ALARM_PLAY_INTERVAL,
//		E_TIMER_UPDATE_TIME_INFO,		//used to update the time info at main UI
//		E_TIMER_UPDATE_DOSE_INFO,		//used to update the dose info at main UI and location UI
    E_TIMER_WAITING_MCU_RESET,		//used to waiting for the mcu reset
//		E_TIMER_REFRESH_SCREEN,			//used to refresh screen while lcd is active
//		E_TIMER_TURN_OFF_BACKLIGHT,		//used to turn off the backlight after 10s
//		E_TIMER_TRANS_CAL_UPDATE,		//used to display drops dynamically
//		E_TIMER_MOTOR_CURRENT_CHECK_DELAY,	//delay 200ms after motor run when get motor current
	E_TIMER_CMD_TIME_TOO_LONG, //receive the cmd time is too long, clear the cmd

    E_TIMER_ENGINEER_MODE_REPEAT_CMD, //used to delay to run engineer mode cmd
    E_TIMER_CHECK_IS_MEET_HALL, //used to check is meet hall
    E_TIMER_CHECK_IS_REACH_EXPECT_ENCODER, //used to check is reach expect encoder
	/*id end, do not add timer id after this line*/
    E_TIMER_COM_TEST,
	E_TIMER_MAX						//it is indicate the size of timer ids, it must be at the end 
}TE_TIMER_ID;

typedef void (*TF_TIMER_FUNC)(void);

//bind handle for timer id
void CtlTimerBindHandle(TE_TIMER_ID id, TF_TIMER_FUNC func);

/*bind handle for timer id, the timer id is relate with screen*/
/*So when exit screen, the id will auto stop*/
void CtlTimerBindScreenHandle(TE_TIMER_ID id, TF_TIMER_FUNC func);

//start a timer(previous timer will be clear),
//when milliseconds from now is pass, the handle function will be called.
//Note: the timer only run once,
//if you want to run again, please call it again at handle function.
//the timer is base on tick and tick is 5ms, so suggest milliseconds is Multiples of 5
void CtlTimerSet(TE_TIMER_ID id, T_S32 milliseconds);

//same as CtlTimerSet,but it runs repeat,
//the handle function will be called each milliseconds.
//if you want to stop, please call CtlTimerClear.
void CtlTimerSetRepeat(TE_TIMER_ID id, T_S32 milliseconds);

//stop the timer
void CtlTimerClear(TE_TIMER_ID id);

//check if the timer active
TE_BOOLEAN CtlTimerIsExist(TE_TIMER_ID id);

/*stop all screen timer, called when exit screen*/
void CtlTimerClearAllOfScreen(void);

//stop all timer
void CtlTimerClearAll(void);

//handle timer event
void CtlTimerHandle(void);

#endif /* CTLTIMER_H */

