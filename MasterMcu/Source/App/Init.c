/************************************************
* COPYRIGHT 2016.
*
* NAME: Init.c
*
* DESCRIPTION:
*   Initial module in the application layer.
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/14/2016
*
* END OF FILE HEADER
************************************************/
#include <string.h>
#include "Init.h"
//#include "AppuiScreen.h"
//#include "AppSysState.h"
//#include "AppEventHandle.h"
//#include "AppAlarm.h"
//#include "AppSetTimeDate.h"
//#include "AppDelivery.h"
//#include "..\Kernel\CtlAlarm.h"
#include "..\Kernel\CtlData.h"
#include "..\Kernel\CtlDelivery.h"
#include "..\Kernel\CtlDisplay.h"
#include "..\Kernel\CtlSpiComm.h"
#include "..\Kernel\CtlPower.h"
#include "..\Kernel\CtlInterrupt.h"
#include "..\Kernel\CtlKey.h"
#include "..\Kernel\CtlTimer.h"
#include "..\Driver\DrvRtc.h"
#include "..\Driver\DrvSpi.h"
#include "..\Driver\DrvTimer.h"
#include "..\Driver\DrvClk.h"
#include "..\Driver\DrvPort.h"
#include "..\Driver\DrvKey.h"
#include "..\Driver\DrvPower.h"
#include "..\Driver\DrvClk.h"
#include "..\Driver\DrvEncoder.h"
#include "..\Driver\DrvBeep.h"
#include "..\Driver\DrvKey.h"
#include "DrvUart.h"
#include "AppEngineerMode.h"
//#include "AppDelivery.h"
#include "version.h"
#include "CtlMotor.h"
#include "AppCommon.h"
#include "CtlDisplay.h"
#include "AppFactory.h"

void InitDriver()
{
	PortInit();
    ClockInit();
    DrvUart0Init();
    DrvUart1Init();    
	//DrvMotorInitial();
	DrvKeyInitial();
	DrvLcdInit();
	DrvInitPowerControl();
	DrvEncoderInitial();
	//AdcInit();
	//DrvSpixInitial(E_SPI_M_S_COM);
	DrvTimerInitial();
    DrvTimerEnable(E_TIMER_CHANNEL_A2);
    //DrvTimerEnable(E_TIMER_CHANNEL_A1);
}

void InitKernel()
{
	CtlPwrInitial();
	SpiCommInit();
	InterruptCtlInit();
    //CtlDeliveryInit();
	CtlKeyInit();
    CtlMotorInit();
    DisplayInit();
}

void InitApp()
{
	//AlarmInit();
    //AppEventHandleInit();
    //AppSysStateInit();
    AppEngineerModeInit();
    //AppCommonInit();
}

/*void HallTest(void)
{
    MOTOR_POWER_OFF;
    if(HALL_INPUT_STATE == 0)
        MOTOR_POWER_ON;
    if(HALL_INPUT_STATE == 0)
        CtlEventPost(E_ALARM_HALL_ERROR_ID,1);
}*/

// void BuzzTest(void)
// {
//     SetPlayOnce();
//   //DrvBuzzSetState(E_BUZZ_OFF);
//   //DrvBuzzSetState(E_BUZZ_ON);
//   //DrvBuzzSetState(E_BUZZ_OFF);
// }

// void FramTest(void)
// {
//     T_U32 wr_buff = 0xA55AA55A;
// 	T_U32 rd_buff = 0;
//     T_U32 *addr = (T_U32*)FRAM_START_ADDRESS;
//     T_U16 num[100] = {0};
//     T_U8 i = 0;
// 	T_U8 max_count = 0;

//     while((T_U32)addr < FRAM_END_ADDRESS){
//       FramReadData(&rd_buff,addr,1);
//       if(rd_buff == 0xffffffff){
//         if(i<100){
//           num[i] = ((T_U32)addr - FRAM_START_ADDRESS)/4;
//           i++;
//           max_count = i;
//         }else{
//           break;
//         }
//       }
//       addr++;
//     }
    
//     addr = (T_U32*)FRAM_START_ADDRESS; //back to FRAM_START_ADDRESS
//     for(i=0; i < max_count; i++)
//         FramWriteData(&wr_buff,(addr + num[i]),1);

//     for(i=0; i < max_count; i++)
// 	{
//       FramReadData(&rd_buff, addr, 1);
//       if(rd_buff == wr_buff)
//         continue;
//       else
// 	  {
//         //AlarmSet(E_ALARM_FRAM_ERROR_ID);
//         break;
//       }
//     }

//     wr_buff=0xffffffff;
//     for(i=0; i< max_count; i++)
//       FramWriteData(&wr_buff,(addr + num[i]),1);
// }

// void BatteryPerformanceTest(T_U16 data1, T_U16 data2)
// {
//   static T_U16 batt_perf_vol[BATT_CHK_TIME]={0};
//   static T_U8  battery_volt_counter=0;
//   T_U8  i,cnt=0;
  
  
  
//   if(data1 == E_ADC_CHANNEL_BATT_PERF_VOL)
//   {         
//     if(battery_volt_counter<BATT_CHK_TIME)
//     {
//       batt_perf_vol[battery_volt_counter] = data2*3;
//       battery_volt_counter++;    
//     }
    
//     if(battery_volt_counter == BATT_CHK_TIME)
//     {
//       for(i=0; i<BATT_CHK_TIME; i++)
//       {
//         if(batt_perf_vol[i]<BATTERY_FUNCTION_CHK)
//           cnt++;
//       }

//       if(cnt >= BATT_CHK_TIME)
//         //AlarmSet(E_ALARM_BATTERY_EXHAUSTION_ID);

//       battery_volt_counter=0;
//       for(i=0; i<BATT_CHK_TIME; i++)
//       {
//         batt_perf_vol[i]=0;
//       }
      
//     }
//   }
// }
// void GetBatteryPerfVolt(void)
// {
//     T_U32 curr_time_in_sec=0;
//     static T_U8 last_time_date=0;
//     T_U8 i;
    
//     Ds1302TimeRead();
// 	curr_time_in_sec = ConvertTimeToSecond(&g_current_time);

//     if(g_current_time.date != last_time_date)
//     {
//       if(curr_time_in_sec >= 45000)          //time has passed 12:30
//       {
//         for(i=0; i<BATT_CHK_TIME; i++)
//         {
//           SendSpiCommandToSlave(HEAD_SQ,COMM_AD,E_ADC_CHANNEL_BATT_PERF_VOL);
//         }

//         Ds1302TimeRead();
//         last_time_date=g_current_time.date;
//       }
//     }
// }


// void SelfTest(void)
// {
//     //HallTest();
//     BuzzTest();
//     //LcdTest ();
//     FramTest();
//     GetBatteryPerfVolt();
// }

void SysInit(void)
{
	InitDriver();
    InitKernel();
	CtlTimerBindHandle(E_TIMER_SCAN_KEY, CtlKeyScan);
	CtlTimerSetRepeat(E_TIMER_SCAN_KEY, KEY_SCAN_MILLI_SECOND);
	InitApp();
	//SelfTest();
}

//	static void SignalFlagInitialAtFirstPowerOn()
//	{
//		T_U32 *fram_pt;
//	
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_MCU_RES_FLAG;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_SWITCH;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = (T_U32)E_TRUE;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_MOTOR_STATUS;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_DELI;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_DAY;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_UNDELI;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_OPER;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DER_COUNTER;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DTR_COUNTER;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_SDR_COUNTER;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_OPR_COUNTER;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_PASSWORD_DEF;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 222;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_ALARM_INFO;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 15;
//		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_THICKNESS;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = (T_U32)E_DOSE_THICKNESS_200;
//		fram_pt=(T_U32 *)FRAM_ADDR_MOTOR_POS_REC_POINT;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//		fram_pt=(T_U32 *)FRAM_ADDR_MOTOR_POS_CNT_START;
//		if(*fram_pt == 0xffffffff)
//			*fram_pt = 0;
//	}
void InitDataIfNotInit()
{
	T_U32 *fram_pt;

	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_FACTORY_FLAG;
	if(*fram_pt != 0x55AA55AA)
    {
        //工厂参数设置默认值,保留序列号
        memset((void*)FRAM_ADDR_SYS_PARA_FACTORY_START, 0, FRAM_ADDR_SYS_PARA_FACTORY_SIZE-8);
        memset((void*)FRAM_ADDR_MOTOR_POS_CNT_START, 0, FRAM_ADDR_MOTOR_POS_CNT_SIZE);
        //设置默认阻塞力,电机位置
        *(T_U32 *)FRAM_ADDR_SYS_PARA_OCCL_CAL_MIN = 10000;
		*(T_U32 *)FRAM_ADDR_SYS_PARA_OCCL_CAL_MAX = 10000;
        *(T_S32 *)FRAM_ADDR_SYS_PARA_HALL_TO_END_LEN = HALL_TO_END_MILIMETER;
        *(T_U32*)(FRAM_ADDR_MOTOR_POS_CNT_START) = 1; 
        *(T_U32 *)FRAM_ADDR_SYS_PARA_PASSWORD_DEF = 222;        
        *(T_U32*)(FRAM_ADDR_MOTOR_POS_CNT_START+4) = VolumeToEncoder(TOOLING_HALL_CAL_VOLUME-800);
        *(T_U32*)(FRAM_ADDR_MOTOR_POS_CNT_START+8) = VolumeToEncoder(TOOLING_HALL_CAL_VOLUME-800);
        *fram_pt = 0x55AA55AA; 

	}

//    fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_USER_FLAG;
//	if(*fram_pt != 0x55AA55AA)
//    {
//        //用户参数设置默认值,记录清空
//        memset((void*)FRAM_ADDR_SYS_PARA_USER_START, 0, FRAM_ADDR_SYS_PARA_USER_SIZE);
//        memset((void*)FRAM_START_ADDRESS, 0, (FRAM_END_ADDRESS-FRAM_START_ADDRESS+1));
//        *(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_SWITCH = E_TRUE;
//        *(T_U32 *)FRAM_ADDR_SYS_PARA_PASSWORD_DEF = 222;
//        *(T_U32 *)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE = 15;
//        *(T_U32 *)FRAM_ADDR_SYS_PARA_THICKNESS = E_DOSE_THICKNESS_200;
//        *fram_pt = 0x55AA55AA; 
//	}
}

// static void SignalFlagInitialAtFirstPowerOn()
// {
// 	T_U32 *fram_pt;

// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_MCU_RES_FLAG;
// 	if(*fram_pt > (T_U32)E_TRUE)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_SWITCH;
// 	if(*fram_pt > (T_U32)E_TRUE)
// 		*fram_pt = (T_U32)E_TRUE;
// //		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_MOTOR_STATUS;
// //		if(*fram_pt > (T_U32)E_TRUE)
// //			*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_DELI;
// 	if(*fram_pt  > (T_U32)USER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_DAY;
// 	if(*fram_pt > (T_U32)USER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_UNDELI;
// 	if(*fram_pt > (T_U32)USER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_REC_P_OPER;
// 	if(*fram_pt > (T_U32)OPER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DER_COUNTER;
// 	if(*fram_pt > (T_U32)USER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DTR_COUNTER;
// 	if(*fram_pt > (T_U32)USER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_SDR_COUNTER;
// 	if(*fram_pt > (T_U32)USER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_OPR_COUNTER;
// 	if(*fram_pt > (T_U32)OPER_HISTORY_RECORD)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_PASSWORD_DEF;
// 	if((*fram_pt > (T_U32)1000) || (*fram_pt == 0))
// 		*fram_pt = (T_U32)222;
// //		fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_ALARM_INFO;
// //		if(*fram_pt > (T_U32)E_ALARM_COUNT)
// //			*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE;
// 	if(*fram_pt > (T_U32)SINGLE_DELIVERY_DOSE_MAX || (*fram_pt == 0))
// 		*fram_pt = (T_U32)15;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_THICKNESS;
// 	if(*fram_pt > (T_U32)E_DOSE_THICKNESS_MAX)
// 		*fram_pt = (T_U32)E_DOSE_THICKNESS_200;
// //		fram_pt=(T_U32 *)FRAM_ADDR_MOTOR_POS_REC_POINT;
// //		if(*fram_pt > (T_U32)100)
// //			*fram_pt = 0;
// //		fram_pt=(T_U32 *)FRAM_ADDR_MOTOR_POS_CNT_START;
// //		if(*fram_pt > (T_U32)MOTOR_POS_COUNTER_MAX)
// //			*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_START_MOTOR_POS;
// 	if(*fram_pt == 0xffffffff)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_IS_RUNING;
// 	if(*fram_pt == 0xffffffff)
// 		*fram_pt = 0;
// 	fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_HALL_TO_END_LEN;
// 	if(*fram_pt == 0xffffffff)
// 		*fram_pt = HALL_TO_END_MILIMETER;    
// }

// void ParaInit()
// {
// 	T_U16 idx;
//     T_U32 fram_read_data;                //add fram data read buffer 
//     T_U32 fram_read_buffer[15]={0};      //add fram data read buffer
// 	T_U32 *p_data_buf = fram_read_buffer;

// 	//memset(p_data_buf, 0, 20 * sizeof(T_U32));
// 	//Load the first 12 system parameters.
// 	//ReadFromFram(FRAM_ADDR_SYS_PARA_DELI_STATUS, p_data_buf, 12);
// 	//g_delivery_state = (TE_DELIVERY_STATE)*p_data_buf ++;
//     //change delivery state as a fixed state
//     //g_delivery_state = E_DELI_STATE_STOP;
//     LoadSysPara(E_SYSTEM_PARA_SIN_RST_FLAG, 0);
//     if(g_mcu_abnormal_reset_state > E_TRUE)
//     {
//        g_mcu_abnormal_reset_state = E_FALSE;
//        SaveSysPara(E_SYSTEM_PARA_SIN_RST_FLAG, 0);
//     }
// 	//g_occlusion_state = (TE_BOOLEAN)*p_data_buf ++;
//     //change occlusion state as a fixed state
//     //g_occlusion_state = E_FALSE;
// 	//g_motor_monitor_pos_point = *p_data_buf ++;
//     LoadSysPara(E_SYSTEM_PARA_HALL_CAL_DATA, 0);
//     LoadSysPara(E_SYSTEM_PARA_HALL_TO_END_CAL_DATA, 0);
// 	//g_motor_cali_circle = (T_U16)*p_data_buf ++;
// 	//LoadSysPara(E_SYSTEM_PARA_DELI_CAL_RATE, 0);
// 	//g_deli_cali_rate = (T_U16)*p_data_buf ++;
// 	LoadSysPara(E_SYSTEM_PARA_DELI_SWITCH_STATE, 0);
//     if(g_deliver_switch > E_TRUE)
//     {
//        g_deliver_switch = E_TRUE;
//        SaveSysPara(E_SYSTEM_PARA_DELI_SWITCH_STATE, 0);
//     }
//     //g_deliver_switch = (TE_BOOLEAN)*p_data_buf ++;
//     LoadSysPara(E_SYSTEM_PARA_DELI_POWER, 0);
// 	//g_deli_power = (T_U16)*p_data_buf ++;
//     LoadSysPara(E_SYSTEM_PARA_DOSE_ALARM_LIMIT, 0);
// 	//g_min_alarm_dose = (T_U16)*p_data_buf ++;
//     LoadSysPara(E_SYSTEM_PARA_BATT_ALARM_LIMIT, 0);
// 	//g_min_alarm_battery = (T_U16)*p_data_buf ++;
//     LoadSysPara(E_SYSTEM_PARA_THICKNESS, 0);
// 	//g_sys_para_thickness = (TE_DOSE_THICKNESS)*p_data_buf ++;
//     LoadSysPara(E_SYSTEM_PARA_MAX_INFU_DOSE, 0);
// 	//g_sys_para_max_inf_dose = (T_U16)*p_data_buf ++;
	

// 	//Load the dose values of 96 delivery point.
//     //memset(p_data_buf, 0, 20 * sizeof(T_U32));
// 	for(idx = 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx ++)
// 	{
// 		ReadFromFram((FRAM_ADDR_SYS_PARA_INFU_START + 4 * idx), &fram_read_data, 1);
// 		g_sys_para_infu_info[idx].index = idx;
// 		g_sys_para_infu_info[idx].dose = fram_read_data;
// 	}
// //-------------

// 	//Load other 16 system parameters.
//     p_data_buf = fram_read_buffer;     //pointer address assign
//     memset(p_data_buf, 0, 4 * sizeof(T_U32));
// 	ReadFromFram(FRAM_ADDR_SYS_PARA_REC_P_DELI, p_data_buf, 4);
// 	g_record_point.p_infusion_rec = (T_U16)*p_data_buf++;
// 	g_record_point.p_daytotal_rec = (T_U16)*p_data_buf++;
// 	g_record_point.p_stopinfu_rec = (T_U16)*p_data_buf++;
// 	g_record_point.p_operation_rec = (T_U16)*p_data_buf++;
//     LoadSysPara(E_SYSTEM_PARA_DELI_START_MOTOR_POS,0);
//     //g_deliver_start_motor_pos = (T_U16)*p_data_buf++;
//     LoadSysPara(E_SYSTEM_PARA_DELI_IS_RUNNING,0);
//     //g_deliver_is_running = (T_U16)*p_data_buf++;
//     //g_software_ver = INTERNAL_VERSION;
//     //g_software_ver = (T_U16)*p_data_buf++;

// 	/****** Test Codes ******/
// 	//g_sys_para_max_inf_dose = 15;
// 	//g_password_common = 222;
// 	/****** Test Codes ******/

// 	//Load hareware version and serial number.
// 	//g_hardware_ver = GetHardwareVersion();
// 	//g_serial_number = GetSerialNumber();

//     LoadSysPara(E_SYSTEM_PARA_PASSWORD_COMMON,0);
// 	//ReadFromFram(FRAM_ADDR_SYS_PARA_PASSWORD_DEF, p_data_buf, 1);
// 	//g_password_common = (T_U16)*p_data_buf++;

// 	//g_password_private = GetPasswordPrivate();

//     p_data_buf = fram_read_buffer;     //pointer address assign
// 	ReadFromFram(FRAM_ADDR_SYS_PARA_DER_COUNTER, p_data_buf, 4);
// 	//g_alarm_status = (T_U16)*p_data_buf++;
//     //change alarm status as a fixed status
//     //g_alarm_status = E_ALARM_NONE;
// 	g_record_counter.de_rec_counter = (T_U16)*p_data_buf++;
// 	g_record_counter.dt_rec_counter = (T_U16)*p_data_buf++;
// 	g_record_counter.sd_rec_counter = (T_U16)*p_data_buf++;
// 	g_record_counter.op_rec_counter = (T_U16)*p_data_buf++;

//     InitMotorPos();
//     LoadSysPara(E_SYSTEM_PAPA_BATTERY_VOLLTAGE_OFFSET, 0);
//     LoadSysPara(E_SYSTEM_PARA_MOTOR_SPEED, 0);
// 	//LoadSysPara(E_SYSTEM_PARA_MOTOR_POSITION, 0);
// //		for(idx = 0; idx < 4; idx ++)
// //			LoadSysPara(E_SYSTEM_PARA_MOTOR_POS_COUNTER, idx);

// 	//ReadFromFram(FRAM_ADDR_SYS_PARA_DOSE_LEFT, p_data_buf, 1);
//     //g_dose_number = (T_U16)*p_data_buf++;
//     //use motor pos to get dose num
//     //g_dose_number = VolumeToDose(g_sys_para_thickness, g_motor_position);


// //		switch(g_delivery_state)
// //		{
// //			case E_DELI_STATE_STOP:
// //				CtlEventPost(E_EVENT_ENTER_STOP_STATE, 1);
// //				break;
// //			case E_DELI_STATE_RUNNING:
// //				CtlEventPost(E_EVENT_ENTER_RUN_STATE, 1);
// //				break;
// //			case E_DELI_STATE_PAUSE:
// //				CtlEventPost(E_EVENT_ENTER_PAUSE_STATE, 1);
// //				break;
// //			default:
// //				CtlEventPost(E_EVENT_ENTER_STOP_STATE, 1);
// //				break;
// //		}
// }

void TestLcd()
{
	T_U16 idx;
	for(idx = COL_NUMBER_MIN; idx <= COL_NUMBER_MAX; idx ++)
	{
		DrawLineColumn(idx, ROW_NUMBER_MIN, ROW_NUMBER_MAX);
        DisplayFlushDirty();
		DelayMs(10);	//This number should be tested and reset.
	}
	DrvLcdSetBackLight(E_BACK_LIGHT_ON);
	DelayMs(2000);
	DrvLcdSetBackLight(E_BACK_LIGHT_OFF);
}

// void ShowLogoAndVer()
// {
// 	//DisplayUI(E_SCREEN_SHOW_LOGO);
// }

// void ShowRemindingPage()
// {
// 	//DisplayUI(E_SCREEN_REMINDING);
// }

// void ShowStartupPage()
// {
//     T_SINT i;
    
// 	TestLcd();
// 	DelayMs(1000);
// 	ShowLogoAndVer();    
//     DisplayFlushDirty();
// 	DelayMs(5000);
// 	ShowRemindingPage();
//     DisplayFlushDirty();
// 	DelayMs(5000);

//     //play two beep
//     for(i=0; i<2; i++)
//     {
//         DrvBuzzSetState(E_BUZZ_ON);
//         DelayMs(PLAY_MS);
//         DrvBuzzSetState(E_BUZZ_OFF);

//         if(i == 0)
//         {
//             DelayMs(WAIT_MS);
//             DelayMs(PLAY_MS + WAIT_MS);
//         }
//     }
// }

//	void RecordInitToZeroAtFirstPowerOn()
//	{
//		T_U16 i;
//		T_U32 *fram_pt;
//	
//		fram_pt=(T_U32 *)(FRAM_ADDR_SYS_PARA_INFU_START);
//		if(*fram_pt == 0xffffffff)
//		{
//			*fram_pt=0;
//			fram_pt=(T_U32 *)FRAM_ADDR_SYS_PARA_INFU_START;
//			for(i=0; i<(FRAM_ADDR_SYS_PARA_DAY_TOTAL-FRAM_ADDR_SYS_PARA_INFU_START)/4; i++)
//			{
//				*(fram_pt++)=0;
//			}
//		}
//	
//		fram_pt=(T_U32 *)(FRAM_ADDR_INFUSION_RECORD_START);
//		if(*fram_pt == 0xffffffff)
//		{
//			*fram_pt=0;
//			fram_pt=(T_U32 *)FRAM_ADDR_INFUSION_RECORD_START;
//			for(i=0; i<(FRAM_END_ADDRESS-FRAM_ADDR_INFUSION_RECORD_START+1)/4; i++)
//			{
//				*(fram_pt++)=0;
//			}
//		}
//	}

#ifdef UNIT_TEST
extern void UTStart(void);
#endif

void Initial()
{
    CommonInit();
	SysInit();
#ifdef UNIT_TEST
    UTStart();
#endif
    InitDataIfNotInit();
	//SignalFlagInitialAtFirstPowerOn();
    //RecordInitToZeroAtFirstPowerOn();
	//ParaInit();
}

