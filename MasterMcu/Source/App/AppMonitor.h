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
#define M_DeliveryDetailPrint if(IsEnableOutput(E_MODULE_DELIVERY_DETAIL))printf
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


//参照旧产品的汇编代码中的定义并加上490的偏差
//unit: 0.001v
#define BATT_LEVEL_EMPTY	2550
#define BATT_LEVEL_LOW		2650
#define BATT_LEVEL_ONE		2752
#define BATT_LEVEL_TWO		2832
#define BATT_LEVEL_THREE	2976

#define DOSE_LEVEL_EMPTY	0
#define DOSE_LEVEL_LOW		150
#define DOSE_LEVEL_ONE		1000
#define DOSE_LEVEL_TWO		3000
 
#define PULSE_SIGNAL_HIGH	(P5OUT |= BIT0)   //心跳信号拉高
#define PULSE_SIGNAL_LOW	(P5OUT &= ~BIT0)  //心跳信号拉低
#define SCAN_PULSE_SIGNAL_STATUS	P1IN

#define MOTOR_CURRENT_ALARM_LIMIT	600	     //电机电流阻塞报警下限，待定
#define OCCLUSION_BONUS				5	     //电机堵塞报警丸剂量，待定
#define MOTOR_VOLTAGE_NORMAL_MIN	0x80	 //电机工作正常电压范围下限值
#define MOTOR_VOLTAGE_NORMAL_MAX	0xC0	 //电机工作正常电压范围上限值

#define DILI_POWER_MIN	1500	             //motor minimum deli_power

extern TE_BOOLEAN g_is_event_on_deli_posted ;
extern T_U16 g_mcu_reset_counter;
extern TE_BOOLEAN g_is_external_sys_resetting;
extern T_U16 g_curr_motor_current;

/************************************************
* Declaration
************************************************/
TE_DOSE_LEVEL DoseStatusLevel(T_U16 dose);
TE_BATTERY_LEVEL BattStatusLevel(T_SINT batt_vol);

//start monitor system normal status 
void SystemMonitorStart(TE_DELIVERY_STATE deli_state);

//stop monitor system normal status 
void SystemMonitorStop(void);

//start monitor infusion
void InfusionMonitorStart(void);

//stop monitor infusion
void InfusionMonitorStop(void);

//monitor enternal system 
void ExternSysMonitorStart();
void ExternSysMonitorStop();
TE_BOOLEAN ExternSysMonitorIsRuning();
TE_BOOLEAN MonitorCheckAlarm(void);


void PulseSignalSendStart();


//check system status
//static void MonitorCheckSystemStatus(void);

void MonitorOnGetMotorCurrent(T_U16 data);
void MonitorOnGetAdc(T_U16 data1, T_U16 data2);


//check infusion status
//static void MonitorCheckInfusionStatus(void);

//check battery level 
void MonitorCheckBatteryLevel(void);

//check battery charging status
void MonitorCheckDoseLevel(void);

//check whether the next deliver point is arrived.
void MonitorCheckOnDeliverPoint(void);

//check whether the deliver operation should be finished.
void MonitorCheckDeliverFinished(void);

//check whether LCD should be actived or not.
//void MonitorCheckLcdDisplay(void);

//check occlusion
//void MonitorCheckOcclusion(void);

//check deliver switch off
void MonitorCheckDeliSwitchOff(void);

//send pulse signal
void MonitorSendPulseSignal(void);

//check pulse signal
void MonitorCheckPulseSignal(void);

//check whether condition of sleeping is matched
TE_BOOLEAN IsMeetConditionOfSleep(void);

//Save System Parameters before sleep or shut down
void SaveSystemParameters();

//Set device to Sleeping mode
void ExcuteSleep();

//Wakeup device from Sleeping mode
void ExcuteWakeup();
#endif /* APP_MONITOR_H */

