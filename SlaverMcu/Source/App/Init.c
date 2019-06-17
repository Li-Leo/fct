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
#include "AppSysState.h"
#include "AppEventHandle.h"
//#include "AppAlarm.h"
#include "..\Kernel\CtlAlarm.h"
#include "..\Kernel\CtlData.h"
#include "..\Kernel\CtlDelivery.h"
//#include "..\Kernel\CtlDisplay.h"
#include "..\Kernel\CtlSpiComm.h"
#include "..\Kernel\CtlPower.h"
#include "..\Kernel\CtlInterrupt.h"
#include "..\Kernel\CtlEvent.h"
#include "..\Kernel\CtlTimer.h"
//#include "..\Driver\DrvRtc.h"
#include "..\Driver\DrvSpi.h"
#include "..\Driver\DrvTimer.h"
#include "..\Driver\DrvClk.h"
#include "..\Driver\DrvPort.h"
#include "..\Driver\DrvMotor.h"
//#include "..\Driver\DrvKey.h"
#include "..\Driver\DrvPower.h"
#include "..\Driver\DrvClk.h"
#include "..\Driver\DrvEncoder.h"
#include "..\Driver\DrvBeep.h"
#include "DrvHall.h"
#include "DrvUart.h"
#include "AppEngineerMode.h"
#include "CtlComm.h"

extern TE_DELIVERY_STATE g_delivery_state;
extern TS_TIME_INFO g_current_time;
extern TE_BOOLEAN g_mcu_abnormal_reset_state;
extern TE_BOOLEAN g_occlusion_state;
extern T_U16 g_motor_position;
extern T_U16 g_motor_cali_circle;
extern T_U16 g_deli_cali_rate;
extern TE_BOOLEAN g_deliver_switch;
extern T_U16 g_deli_power;
extern T_U16 g_min_alarm_dose;
extern T_U16 g_min_alarm_battery;
extern TE_DOSE_THICKNESS g_sys_para_thickness;
extern T_U16 g_sys_para_max_inf_dose;
extern TS_DELI_PARA g_sys_para_infu_info[TOTAL_DELIVERY_TIME_POINT_NUMBER];
extern T_U16 g_sys_para_max_day_total;
extern TS_RECORD_POINT g_record_point;
extern T_U16 g_deliver_dose_actual;
extern T_U16 g_deliver_dose_value;
extern T_U16 g_software_ver;
extern T_U16 g_hardware_ver;
extern T_U16 g_serial_number;
extern T_U32 g_fram_read_data_buff[20];
extern TS_ADC_INFO g_adc_value;

void InitDriver()
{
	ClockInit();
	PortInit();
    DrvUart0Init();
    DrvUart1Init();     
	DrvMotorInitial();
	DrvInitPowerControl();
	AdcInit();
	//DrvSpixInitial(E_SPI_M_S_COM);
	DrvTimerInitial();
	DrvTimerEnable(E_TIMER_CHANNEL_A2);
	DrvEncoderInitial();
	DrvHallInit();
    DrvMotorCurrentInit();
}

void InitKernel()
{
	//CtlPwrInitial();
	SpiCommInit();
	InterruptCtlInit();
	CtlDeliveryInit();
    CommInit();
}

void InitApp()
{
	//AlarmInit();
	AppEventHandleInit();
	AppSysStateInit();
    AppEngineerModeInit();
}

void HallTest(void)
{
    //MOTOR_POWER_OFF;
    //if(HALL_INPUT_STATE == 0)
        //MOTOR_POWER_ON;
    //if(HALL_INPUT_STATE == 0)
        //CtlEventPost(E_ALARM_HALL_ERROR_ID,1);
}

void FramTest(void)
{
    T_U32 wr_buff = 0xA55AA55A;
	T_U32 rd_buff = 0x00000000;
    T_U32 *addr = (T_U32*)FRAM_START_ADDRESS;
    T_U32 num[100] = {0};
    T_U8 i = 0;
	T_U8 max_count = 0;

    while(*addr < FRAM_END_ADDRESS){
        FramReadData(&rd_buff,addr,1);
        if(rd_buff == 0)
		{
            if(i<100)
			{
                num[i] = *addr - FRAM_START_ADDRESS;
                i++;
                max_count = i;
            }
			else
			{
                break;
            }
        }
        addr++;
    }

    for(i=0; i < max_count; i++)
        FramWriteData(&wr_buff,(addr + num[i]),1);

    for(i=0; i < max_count; i++){
        FramReadData(&rd_buff, addr, 1);
        if(rd_buff == wr_buff)
		{
            continue;
        }
		else
		{
            ;//AlarmSet(E_ALARM_FRAM_ERROR_ID);
            break;
        }
    }

    wr_buff=0;
    for(i=0; i< max_count; i++)
        FramWriteData(&wr_buff,(addr + num[i]),1);
}

void AdcTest(void)     // realized by slave mcu
{
    BATT_CHK_ENABLE;

    AdcGetAverageValue();    

    // should be channle A13
    if(g_adc_value.batt_perf_vol_channal_a13 < BATTERY_PERFORMANCE_CHK_LIMIT)  
         //CtlEventPost(E_EVENT_ALARM_BATTERY_ERROR,1);

    BATT_CHK_DISABLE;
}


void SelfTest(void)
{
    HallTest();
    //BuzzTest();
    //LcdTest ();
    FramTest();
    AdcTest();
}

void SysInit(void)
{
	InitDriver();
	InitKernel();
	InitApp();
	//SelfTest();
}

void ParaInit()
{
	T_U16 idx;
    T_U32 fram_read_data;                //add fram data read buffer 
    T_U32 fram_read_buffer[15]={0};      //add fram data read buffer
	T_U32 *p_data_buf = fram_read_buffer;
	
	//memset(p_data_buf, 0, 20 * sizeof(T_U32));
	//Load the first 12 system parameters.
	ReadFromFram(FRAM_ADDR_SYS_PARA_DELI_STATUS, p_data_buf, 12);
	g_delivery_state = (TE_DELIVERY_STATE)*p_data_buf ++;
	g_mcu_abnormal_reset_state = (TE_BOOLEAN)*p_data_buf ++;
	g_occlusion_state = (TE_BOOLEAN)*p_data_buf ++;
	g_motor_position = *p_data_buf ++;
	g_motor_cali_circle = *p_data_buf ++;
	g_deli_cali_rate = *p_data_buf ++;
	g_deliver_switch = (TE_BOOLEAN)*p_data_buf ++;
	g_deli_power = *p_data_buf ++;
	g_min_alarm_dose = *p_data_buf ++;
	g_min_alarm_battery = *p_data_buf ++;
	g_sys_para_thickness = (TE_DOSE_THICKNESS)*p_data_buf ++;
	g_sys_para_max_inf_dose = *p_data_buf ++;
/*
	//Load the dose values of 96 delivery point.
    memset(p_data_buf, 0, 20 * sizeof(T_U32));
	for(idx = 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx ++)
	{
		ReadFromFram((FRAM_ADDR_SYS_PARA_INFU_START + 4 * idx), p_data_buf, 1);
		g_sys_para_infu_info[idx].index = idx;
		g_sys_para_infu_info[idx].dose = *p_data_buf ++;
	}

	//Load other 12 system parameters.

*/

	//Load the dose values of 96 delivery point.
    //memset(p_data_buf, 0, 20 * sizeof(T_U32));
	for(idx = 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx ++)
	{
		ReadFromFram((FRAM_ADDR_SYS_PARA_INFU_START + 4 * idx), &fram_read_data, 1);
		g_sys_para_infu_info[idx].index = idx;
		g_sys_para_infu_info[idx].dose = fram_read_data;
	}
//-------------

    p_data_buf = fram_read_buffer;     //pointer address assign
    memset(p_data_buf, 0, 15 * sizeof(T_U32));
	ReadFromFram(FRAM_ADDR_SYS_PARA_DAY_TOTAL, p_data_buf, 8);
    g_sys_para_max_day_total=*p_data_buf++;
	g_record_point.p_infusion_rec = *p_data_buf++;
	g_record_point.p_daytotal_rec = *p_data_buf++;
	g_record_point.p_stopinfu_rec = *p_data_buf++;
	g_record_point.p_operation_rec = *p_data_buf++;
    g_deliver_dose_value=*p_data_buf++;
    g_deliver_dose_actual=*p_data_buf++;
    g_software_ver=*p_data_buf++;

	//Load hareware serial number.
	//g_hardware_ver = GetSerialNumber();

	/*if (g_mcu_abnormal_reset_state == E_TRUE)
	{
		TS_RECORD rec;
		rec.rec_type = E_RECORD_OPERATION;
		rec.rec_data = 0;
		Ds1302TimeRead();
		rec.rec_time = g_current_time;
		rec.rec_id = g_record_point.p_operation_rec;
		rec.rec_para = 92;
		SaveRec(rec);
		if ((g_record_point.p_operation_rec)++ >= OPER_HISTORY_RECORD)
			g_record_point.p_operation_rec = 0;
	}

	if (g_deliver_dose_actual != g_deliver_dose_value)
	{
		TS_RECORD rec;
		rec.rec_type = E_RECORD_INFUSION;
		Ds1302TimeRead();
		rec.rec_time = g_current_time;
		rec.rec_para = g_deliver_dose_actual;
		rec.rec_id = g_record_point.p_infusion_rec;
		SaveRec(rec);
		if ((g_record_point.p_infusion_rec)++ >= USER_HISTORY_RECORD)
			g_record_point.p_infusion_rec = 0;
	}*/

	switch(g_delivery_state)
	{
		case E_DELI_STATE_STOP:
			CtlEventPost(E_EVENT_ENTER_STOP_STATE, 1);
			break;
		case E_DELI_STATE_RUNNING:
			CtlEventPost(E_EVENT_ENTER_RUN_STATE, 1);
			break;
		case E_DELI_STATE_PAUSE:
			CtlEventPost(E_EVENT_ENTER_PAUSE_STATE, 1);
			break;
		default:
			CtlEventPost(E_EVENT_ENTER_STOP_STATE, 1);
			break;
	}
}

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
	//ParaInit();
}

