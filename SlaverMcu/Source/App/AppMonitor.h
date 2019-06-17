/************************************************
* COPYRIGHT 2016.
*
* NAME: AppMonitor.h
*
* DESCRIPTION:
*   monitor system's status
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/03/2016
*
* END OF FILE HEADER
************************************************/
#ifndef APP_MONITOR_H
#define APP_MONITOR_H

#include "..\TypeDefine.h"
#include "..\Kernel\CtlDelivery.h"

/************************************************
* Definition
************************************************/
#define M_McuMonitorPrint if(IsEnableOutput(E_MODULE_MCU_MONITOR))printf

typedef enum
{
	E_BATTERY_EMPTY,
	E_BATTERY_LOW,
	E_BATTERY_ONE,
	E_BATTERY_TWO,
	E_BATTERY_THREE, 
	E_BATTERY_FOUR,
	E_BATTERY_LEVEL_MAX,
} TE_BATTERY_LEVEL;

typedef enum
{
	E_DOSE_EMPTY,
	E_DOSE_LOW,
	E_DOSE_ONE,
	E_DOSE_TWO,
	E_DOSE_THREE,
	E_DOSE_LEVEL_MAX
} TE_DOSE_LEVEL;

#define BATT_LEVEL_EMPTY	200		//Temporary, the same below
#define BATT_LEVEL_LOW		500
#define BATT_LEVEL_ONE		1000
#define BATT_LEVEL_TWO		2000
#define BATT_LEVEL_THREE	3000

#define DOSE_LEVEL_EMPTY	0
#define DOSE_LEVEL_LOW		100
#define DOSE_LEVEL_ONE		1000
#define DOSE_LEVEL_TWO		3000
 
#define SCAN_PULSE_SIGNAL_STATUS	P1IN
#define PULSE_SIGNAL_HIGH	(P9OUT |= BIT0)		//心跳信号拉高
#define PULSE_SIGNAL_LOW	(P9OUT &= ~BIT0)	//心跳信号拉低

#define MOTOR_CURRENT_ALARM_LIMIT	600	//电机电流阻塞报警下限，待定
#define OCCLUSION_BONUS				5	//电机堵塞报警丸剂量，待定
#define MOTOR_VOLTAGE_NORMAL_MIN	0x80	//电机工作正常电压范围下限值
#define MOTOR_VOLTAGE_NORMAL_MAX	0xC0	//电机工作正常电压范围上限值

extern T_U16 g_mcu_reset_counter;
extern TE_BOOLEAN g_is_external_sys_resetting;
/************************************************
* Declaration
************************************************/

//start monitor system normal status 
void SystemMonitorStart(void);

//stop monitor system normal status 
void SystemMonitorStop(void);

//monitor enternal system 
void ExternSysMonitorStart();
void ExternSysMonitorStop();
TE_BOOLEAN ExternSysMonitorIsRuning();

void PulseSignalSendStart();


//check system status
//static void MonitorCheckSystemStatus(void);


//send pulse signal
void MonitorSendPulseSignal(void);

//check pulse signal
void MonitorCheckPulseSignal(void);

//Save System Parameters before sleep or shut down
void SaveSystemParameters();

//Set device to Sleeping mode
void ExcuteSleep();

//Wakeup device from Sleeping mode
void ExcuteWakeup();


#endif /* APP_MONITOR_H */

