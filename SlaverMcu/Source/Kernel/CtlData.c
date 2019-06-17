/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlData.c
*
* DESCRIPTION:
*   data access module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/12/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlData.h"
#include "CtlDelivery.h"
#include "..\Driver\DrvFram.h"

//************global variables******************
TE_DELIVERY_STATE g_delivery_state;
TS_DELI_PARA g_sys_para_infu_info[TOTAL_DELIVERY_TIME_POINT_NUMBER];
T_U16 g_deliver_dose_value = 0;		//需输注的剂量
T_U16 g_deliver_dose_actual = 0;	//实际输注的剂量
TE_DOSE_THICKNESS g_sys_para_thickness = (TE_DOSE_THICKNESS)0;		//浓度
T_U16 g_sys_para_max_inf_dose = 0;	//单次最大剂量
T_U16 g_sys_para_actual_day_total = 0;	//实际输注的日总量
T_U16 g_sys_para_max_day_total = 0;		//日总量最大值
TE_BOOLEAN g_deliver_switch = E_FALSE;	//输注开关量
T_U16 g_dose_number = 0;			//剩余药量
T_U16 g_batt_number = 0;			//剩余电量
T_U16 g_min_alarm_dose = 0;				//最小剂量报警值
T_U16 g_min_alarm_battery = 0;			//最小电量报警值
T_U16 g_motor_position = 0;				//电机位置
T_U32 g_motor_monitor_pos_point = 0;	//电机位置存储地址指针
T_U32 g_motor_monitor_pos_counter = 0;	//电机位置监控计数器
TE_BOOLEAN g_occlusion_state = E_FALSE;	//电机堵塞状态
T_U16 g_motor_cali_circle = 0;			//电机校准毫米数
T_U16 g_deli_cali_rate = 0;				//输注校准比率
T_U16 g_deli_power = 0;					//输注力
T_U16 g_software_ver = 0;				//软件版本号
T_U16 g_hardware_ver = 0;				//硬件版本号
T_U32 g_serial_number = 0;				//产品序列号
T_U16 g_alarm_status = 0;			//报警状态位信息
TS_RECORD_POINT g_record_point = {0, 0, 0, 0};
TS_RECORD_COUNTER g_record_counter = {0, 0, 0, 0};
T_U16 g_password_private = 0;
T_U16 g_password_common = 0;
TE_BOOLEAN g_mcu_abnormal_reset_state = E_FALSE;	//MCU异常重启标识位
T_U32 g_fram_read_data_buff[20];
T_U32 g_fram_wr_data_buff[10];   //modified

/***********************************************
* Description:
*	Write the buffer data to the pointed adrress of FRAM.
*
* Argument:
*   fram_address: The pointed address of FRAM.
*   p_buf: Data buffer.
*   buf_len: Length of the data buffer.
*
* Return:
*	0->ok, 1->failure
************************************************/
TE_BOOLEAN WriteToFram(T_U32 fram_address,T_U32 *p_buf,T_U16 buf_len)
{       
	T_U32 *fram_point;
	fram_point=(T_U32 *)fram_address;
	T_U32 *p_readback = g_fram_read_data_buff;
	T_U16 index;
	/* Verify the data validity */
	FramWriteData(p_buf,fram_point, buf_len);
	FramReadData(p_readback,fram_point, buf_len);
	for(index = 0; index < buf_len; index++)
		if (p_buf[index] != p_readback[index])
			return E_FALSE;
	return E_TRUE;
}

/***********************************************
* Description:
*	Read the data from the pointed adrress of FRAM buffer into the buffer.
*
* Argument:
*   fram_address: The pointed address of FRAM.
*   p_buf: Data buffer.
*   buf_len: Length of the data buffer.
*
* Return:
*	0->ok, 1->failure
************************************************/
TE_BOOLEAN ReadFromFram(T_U32 fram_address,T_U32 * p_buf,T_U16 buf_len)
{       
	T_U32 *fram_point;
	fram_point=(T_U32 *)fram_address;
	T_U32 *p_readback = g_fram_read_data_buff;
	T_U16 index;
	/* Verify the data validity */
	FramReadData(p_buf,fram_point, buf_len);
	FramReadData( p_readback,fram_point, buf_len);
	for(index = 0; index < buf_len; index++)
		if (p_buf[index] != p_readback[index])
			return E_FALSE;
	return E_TRUE;
}

/***********************************************
* Description:
*	save a certain system parameter to FRAM
* Argument:
*   type: data type
*   para: while type == E_SYSTEM_PARA_INFU_INFO, means index of 96 infusion points
*					value area: 1 ~ 96
*		  while type !=E_SYSTEM_PARA_INFU_INFO, should be 0
*
* Return:
*	0->ok,1->failure
************************************************/
TE_BOOLEAN SaveSysPara(TE_SYS_PARA type, T_U16 para)
{
	T_U32* save_addr;
	T_U32* save_data;
	T_U32* data_readback;
	T_U16 data_len = 1;
    
    T_U32 sdata=0;//add
    T_U32 rdata=0;
	save_data=&sdata;
	
	switch(type)
	{
		case E_SYSTEM_PARA_INFU_INFO:
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_INFU_START;
			save_addr += (para-1);   //  4*(para-1)
			*save_data = (T_U32)g_sys_para_infu_info[para-1].dose;
			break;
		case E_SYSTEM_PARA_THICKNESS:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_THICKNESS;
			*save_data = (T_U32)g_sys_para_thickness;
			break;
		case E_SYSTEM_PARA_MAX_INFU_DOSE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE;
			*save_data = (T_U32)g_sys_para_max_inf_dose;
			break;
		case E_SYSTEM_PARA_DAY_TOTAL:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DAY_TOTAL;
			*save_data = (T_U32)g_sys_para_actual_day_total;
			break;
		case E_SYSTEM_PARA_PASSWORD_COMMON:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_PASSWORD_DEF;
			*save_data = (T_U32)g_password_common;
			break;
		case E_SYSTEM_PARA_PASSWORD_PRIVATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_PASSWORD_PRI;
			*save_data = (T_U32)g_password_private;
			break;
		case E_SYSTEM_PARA_OCCLUSION_STATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MOTOR_STATUS;
			*save_data = (T_U32)g_occlusion_state;
			break;
		case E_SYSTEM_PARA_DELI_STATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_STATUS;
			*save_data = (T_U32)g_delivery_state;
			break;
		case E_SYSTEM_PARA_MOTOR_POSITION:			
			save_addr = (T_U32*)FRAM_ADDR_MOTOR_POS_REC_START;
			save_addr += g_motor_monitor_pos_point;
			*save_data = (T_U32)g_motor_position;
			break;
		case E_SYSTEM_PARA_HALL_CAL_DATA:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MOTOR_CAL;
			*save_data = (T_U32)g_motor_cali_circle;
			break;
		case E_SYSTEM_PARA_DELI_CAL_RATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_CAL_RAT;
			*save_data = (T_U32)g_deli_cali_rate;
			break;
		case E_SYSTEM_PARA_DELI_SWITCH_STATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_SWITCH;
			*save_data = (T_U32)g_deliver_switch;
			break;
		case E_SYSTEM_PARA_DELI_POWER:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_POWER;
			*save_data = (T_U32)g_deli_power;
			break;
		case E_SYSTEM_PARA_DOSE_ALARM_LIMIT:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DOSE_ALARM;
			*save_data = (T_U32)g_min_alarm_dose;
			break;
		case E_SYSTEM_PARA_BATT_ALARM_LIMIT:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_BATT_ALARM;
			*save_data = (T_U32)g_min_alarm_battery;
			break;
		case E_SYSTEM_PARA_RECORD_INDEX:
			switch(para)
			{
				case 0:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_DELI;
					*save_data = (T_U32)g_record_point.p_infusion_rec;
					break;
				case 1:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_DAY;
					*save_data = (T_U32)g_record_point.p_daytotal_rec;
					break;
				case 2:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_UNDELI;
					*save_data = (T_U32)g_record_point.p_stopinfu_rec;
					break;
				case 3:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_OPER;
					*save_data = (T_U32)g_record_point.p_operation_rec;
					break;
				default:
					break;
			}
			break;
		case E_SYSTEM_PARA_RECORD_COUNTER:
			switch(para)
			{
				case 0:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DER_COUNTER;
					*save_data = (T_U32)g_record_counter.de_rec_counter;
					break;
				case 1:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DTR_COUNTER;
					*save_data = (T_U32)g_record_counter.dt_rec_counter;
					break;
				case 2:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_SDR_COUNTER;
					*save_data = (T_U32)g_record_counter.sd_rec_counter;
					break;
				case 3:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OPR_COUNTER;
					*save_data = (T_U32)g_record_counter.op_rec_counter;
					break;
				default:
					break;
			}
			break;
		case E_SYSTEM_PARA_ALARM_FLAG:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_ALARM_INFO;
			*save_data = (T_U32)g_alarm_status;
			break;
		case E_SYSTEM_PARA_SIN_RST_FLAG:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MCU_RES_FLAG;
			*save_data = (T_U32)g_mcu_abnormal_reset_state;
			break;
		case E_SYSTEM_PARA_EXPECT_DELI_DOSE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_SHOULD;
			*save_data = (T_U32)g_deliver_dose_value;
			break;
		case E_SYSTEM_PARA_ACTUAL_DELI_DOSE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_IN_FACT;
			*save_data = (T_U32)g_deliver_dose_actual;
			break;
		case E_SYSTEM_PARA_SOFTWARE_VERSION:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_SW_VERSION;
			*save_data = (T_U32)g_software_ver;
			break;
		case E_SYSTEM_PARA_DOSE_LEFT:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DOSE_LEFT;
			*save_data = g_dose_number;
			break;
		case E_SYSTEM_PARA_MOTOR_POS_POINT:			
			save_addr = (T_U32*)FRAM_ADDR_MOTOR_POS_REC_POINT;
			*save_data = g_motor_monitor_pos_point;
			break;
		case E_SYSTEM_PARA_MOTOR_POS_COUNTER:			
			save_addr = (T_U32*)FRAM_ADDR_MOTOR_POS_CNT_START;
			save_addr += g_motor_monitor_pos_point;
			if (g_motor_monitor_pos_counter <= MOTOR_POS_COUNTER_MAX)
				*save_data = g_motor_monitor_pos_counter;
			else
			{
				save_addr ++;
				*save_data = 1;
			}
			break;
		default:
			break;
	}
        
	/* Verify the data validity */
	FramWriteData(save_data, save_addr, data_len);
    
    data_readback=&rdata;  //add
	FramReadData(data_readback, save_addr, data_len);
	if (*save_data != *data_readback)
		return E_FALSE;
	return E_TRUE;
}

/***********************************************
* Description:
*	load a certain system parameter from FRAM
* Argument:
*   type: data type
*   para: while type == E_SYSTEM_PARA_INFU_INFO, means index of 96 infusion points
*					value area: 1 ~ 96
*		  while type !=E_SYSTEM_PARA_INFU_INFO, should be 0
*
* Return:
*	0->ok,1->failure
************************************************/
TE_BOOLEAN LoadSysPara(TE_SYS_PARA type, T_U16 para)
{
	T_U32* load_addr;
	T_U32* load_data;
	T_U32* data_readback;
	T_U16  data_len = 1;
    
    T_U32  tmp_load_data, tmp_data_readback; //add
	 
    switch(type)
	{
		case E_SYSTEM_PARA_INFU_INFO:
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_INFU_START;
			load_addr += (para-1);//4*(para-1)
			break;
		case E_SYSTEM_PARA_THICKNESS:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_THICKNESS;
			break;
		case E_SYSTEM_PARA_MAX_INFU_DOSE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE;
			break;
		case E_SYSTEM_PARA_DAY_TOTAL:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DAY_TOTAL;
			break;
		case E_SYSTEM_PARA_PASSWORD_PRIVATE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_PASSWORD_PRI;
			break;
		case E_SYSTEM_PARA_PASSWORD_COMMON:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_PASSWORD_DEF;
			break;
		case E_SYSTEM_PARA_OCCLUSION_STATE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MOTOR_STATUS;
			break;
		case E_SYSTEM_PARA_DELI_STATE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_STATUS;
			break;
		case E_SYSTEM_PARA_MOTOR_POSITION:			
			load_addr = (T_U32*)FRAM_ADDR_MOTOR_POS_REC_START;
			load_addr += g_motor_monitor_pos_point;
			break;
		case E_SYSTEM_PARA_HALL_CAL_DATA:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MOTOR_CAL;
			break;
		case E_SYSTEM_PARA_DELI_CAL_RATE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_CAL_RAT;
			break;
		case E_SYSTEM_PARA_DELI_SWITCH_STATE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_SWITCH;
			break;
		case E_SYSTEM_PARA_DELI_POWER:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_POWER;
			break;
		case E_SYSTEM_PARA_DOSE_ALARM_LIMIT:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DOSE_ALARM;
			break;
		case E_SYSTEM_PARA_BATT_ALARM_LIMIT:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_BATT_ALARM;
			break;
		case E_SYSTEM_PARA_RECORD_INDEX:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_DELI;
			load_addr += para;
			break;
		case E_SYSTEM_PARA_RECORD_COUNTER:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DER_COUNTER;
			load_addr += para;
			break;
		case E_SYSTEM_PARA_ALARM_FLAG:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_ALARM_INFO;
			break;
		case E_SYSTEM_PARA_SIN_RST_FLAG:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MCU_RES_FLAG;
			break;
		case E_SYSTEM_PARA_EXPECT_DELI_DOSE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_SHOULD;
			break;
		case E_SYSTEM_PARA_ACTUAL_DELI_DOSE:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_IN_FACT;
			break;
		case E_SYSTEM_PARA_SOFTWARE_VERSION:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_SW_VERSION;
			break;
		case E_SYSTEM_PARA_DOSE_LEFT:			
			load_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DOSE_LEFT;
			break;
		case E_SYSTEM_PARA_MOTOR_POS_POINT:			
			load_addr = (T_U32*)FRAM_ADDR_MOTOR_POS_REC_POINT;
			break;
		case E_SYSTEM_PARA_MOTOR_POS_COUNTER:			
			load_addr = (T_U32*)FRAM_ADDR_MOTOR_POS_CNT_START;
			load_addr += g_motor_monitor_pos_point;
			break;
		default:
			break;
	}
	/* Verify the data validity */
    load_data=&tmp_load_data;           //add
    data_readback=&tmp_data_readback;   //add
	
    FramReadData(load_data,load_addr , data_len);
	FramReadData(data_readback,load_addr , data_len);
	if (*load_data != *data_readback)
		return E_FALSE;
	switch(type)
	{
		case E_SYSTEM_PARA_INFU_INFO:
			g_sys_para_infu_info[para-1].dose = (T_U32)*load_data;
			break;
		case E_SYSTEM_PARA_THICKNESS:
			g_sys_para_thickness = (TE_DOSE_THICKNESS)*load_data;
			break;
		case E_SYSTEM_PARA_MAX_INFU_DOSE:
			g_sys_para_max_inf_dose = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_DAY_TOTAL:
			g_sys_para_actual_day_total = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_PASSWORD_PRIVATE:
			g_password_private = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_PASSWORD_COMMON:
			g_password_common = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_OCCLUSION_STATE:
			g_occlusion_state = (TE_BOOLEAN)*load_data;
			break;
		case E_SYSTEM_PARA_DELI_STATE:
			g_delivery_state = (TE_DELIVERY_STATE)*load_data;
			break;
		case E_SYSTEM_PARA_MOTOR_POSITION:
			g_motor_position = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_HALL_CAL_DATA:
			g_motor_cali_circle = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_DELI_CAL_RATE:
			g_deli_cali_rate = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_DELI_SWITCH_STATE:
			g_deliver_switch = (TE_BOOLEAN)*load_data;
			break;
		case E_SYSTEM_PARA_DELI_POWER:
			g_deli_power = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_DOSE_ALARM_LIMIT:
			g_min_alarm_dose = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_BATT_ALARM_LIMIT:
			g_min_alarm_battery = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_RECORD_INDEX:
			switch(para)
			{
				case 0:
					g_record_point.p_infusion_rec = (T_U16)*load_data;
					break;
				case 1:
					g_record_point.p_daytotal_rec = (T_U16)*load_data;
					break;
				case 2:
					g_record_point.p_stopinfu_rec = (T_U16)*load_data;
					break;
				case 3:
					g_record_point.p_operation_rec = (T_U16)*load_data;
					break;
				default:
					break;
			}
			break;
		case E_SYSTEM_PARA_RECORD_COUNTER:
			switch(para)
			{
				case 0:
					g_record_counter.de_rec_counter = (T_U16)*load_data;
					break;
				case 1:
					g_record_counter.dt_rec_counter = (T_U16)*load_data;
					break;
				case 2:
					g_record_counter.sd_rec_counter = (T_U16)*load_data;
					break;
				case 3:
					g_record_counter.op_rec_counter = (T_U16)*load_data;
					break;
				default:
					break;
			}
			break;
		case E_SYSTEM_PARA_ALARM_FLAG:
			g_alarm_status = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_SIN_RST_FLAG:
			g_mcu_abnormal_reset_state = (TE_BOOLEAN)*load_data;
			break;
		case E_SYSTEM_PARA_EXPECT_DELI_DOSE:
			g_deliver_dose_value = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_ACTUAL_DELI_DOSE:
			g_deliver_dose_actual = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_SOFTWARE_VERSION:
			g_software_ver = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_DOSE_LEFT:
			g_dose_number = (T_U16)*load_data;
			break;
		case E_SYSTEM_PARA_MOTOR_POS_POINT:
			g_motor_monitor_pos_point = *load_data;
			break;
		case E_SYSTEM_PARA_MOTOR_POS_COUNTER:
			g_motor_monitor_pos_counter = *load_data;
			break;
		default:
			break;
	}
	return E_TRUE;
}

/***********************************************
* Description:
*	a function to sort the record data to data buffer.
* Argument:
*   rec_data: record data
*
* Return:
*	the pointer of sorted data buffer
************************************************/
T_U32* SortTheRecordData(TS_RECORD rec_data)
{
	//T_U32* p_data_buf;
	
    T_U32* p_index = g_fram_wr_data_buff;//add
	
    if (rec_data.rec_time.date)
	{
		*p_index = rec_data.rec_time.year;
		p_index++;
		*p_index = rec_data.rec_time.month;
		p_index++;
		*p_index = rec_data.rec_time.date;   // day-->date
		p_index++;
	}
	if (rec_data.rec_type == E_RECORD_INFUSION)
	{
		*p_index = rec_data.rec_time.hour;
		p_index++;
		*p_index = rec_data.rec_time.minute;
		p_index++;
		*p_index = rec_data.rec_time.second;
		p_index++;
		*p_index = rec_data.rec_para;
		p_index++;
	}
	else if (rec_data.rec_type == E_RECORD_DAYS_TOTAL)
	{
		*p_index = rec_data.rec_para;
		p_index++;
	}
	else if (rec_data.rec_type == E_RECORD_STOP_DELI)
	{
		*p_index = rec_data.rec_time.hour;
		p_index++;
		*p_index = rec_data.rec_time.minute;
		p_index++;
		*p_index = rec_data.rec_time.second;
		p_index++;
		*p_index = rec_data.rec_para;
		p_index++;
	}
	else if (rec_data.rec_type == E_RECORD_OPERATION)
	{
		*p_index = rec_data.rec_time.hour;
		p_index++;
		*p_index = rec_data.rec_time.minute;
		p_index++;
		*p_index = rec_data.rec_time.second;
		p_index++;
		*p_index = rec_data.rec_para;
		p_index++;
		*p_index = rec_data.rec_data;
		p_index++;
	}
	return g_fram_wr_data_buff;//modified
}

/***********************************************
* Description:
*	save one kind of record data
* Argument:
*   rec_data: record data
*
* Return:
*	0->ok,1->failure
************************************************/
TE_BOOLEAN SaveRec(TS_RECORD rec_data)
{
	T_U16 data_length = 0;
	T_U32* save_addr = 0;
	T_U32* p_data_buf;
	T_U32* p_readback = g_fram_read_data_buff;
	T_U16 index;
	switch(rec_data.rec_type)
	{
		case E_RECORD_INFUSION:
			data_length = LENGTH_OF_INFUSION_RECORD;
			save_addr = (T_U32*)FRAM_ADDR_INFUSION_RECORD_START;
			//rec_data.rec_id = g_record_point.p_infusion_rec;
			if(g_record_counter.de_rec_counter < USER_HISTORY_RECORD)
			{
				g_record_counter.de_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 0);
			}
			g_record_point.p_infusion_rec = rec_data.rec_id + 1;
			if (g_record_point.p_infusion_rec >= USER_HISTORY_RECORD)
				g_record_point.p_infusion_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 0);
			break;
		case E_RECORD_DAYS_TOTAL:			
			data_length = LENGTH_OF_DAYTOTAL_RECORD;
			save_addr = (T_U32*)FRAM_ADDR_DAYTOTAL_RECORD_START;
			//rec_data.rec_id = g_record_point.p_daytotal_rec;
			if(g_record_counter.dt_rec_counter < USER_HISTORY_RECORD)
			{
				g_record_counter.dt_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 1);
			}
			g_record_point.p_daytotal_rec = rec_data.rec_id + 1;
			if (g_record_point.p_daytotal_rec >= USER_HISTORY_RECORD)
				g_record_point.p_daytotal_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 1);
			break;
		case E_RECORD_STOP_DELI:			
			data_length = LENGTH_OF_STOPINFU_RECORD;
			save_addr = (T_U32*)FRAM_ADDR_STOPINFU_RECORD_START;
			//rec_data.rec_id = g_record_point.p_stopinfu_rec;
			if(g_record_counter.sd_rec_counter < USER_HISTORY_RECORD)
			{
				g_record_counter.sd_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 2);
			}
			g_record_point.p_stopinfu_rec = rec_data.rec_id + 1;
			if (g_record_point.p_stopinfu_rec >= USER_HISTORY_RECORD)
				g_record_point.p_stopinfu_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 2);
			break;
		case E_RECORD_OPERATION:			
			data_length = LENGTH_OF_OPERATE_RECORD;
			save_addr = (T_U32*)FRAM_ADDR_OPERATE_RECORD_START;
			//rec_data.rec_id = g_record_point.p_operation_rec;
			if(g_record_counter.op_rec_counter < OPER_HISTORY_RECORD)
			{
				g_record_counter.op_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 3);
			}
			g_record_point.p_operation_rec = rec_data.rec_id + 1;
			if (g_record_point.p_operation_rec >= USER_HISTORY_RECORD)
				g_record_point.p_operation_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 3);
			break;
		default:
			break;
	}
	save_addr += rec_data.rec_id * data_length;
	/* Get the sorted data buffer */
	p_data_buf = SortTheRecordData(rec_data);
	/* Verify the data validity */
    
	FramWriteData(p_data_buf,save_addr , data_length);
	FramReadData(p_readback, save_addr, data_length);
	for(index = 0; index < data_length; index++)
		if (p_data_buf[index] != p_readback[index])
			return E_FALSE;
    return E_TRUE;
}

/***********************************************
* Description:
*	a function to restore the record data from data buffer.
* Argument:
*   rec_data: record data
*   data_buf: sorted data buffer
*   data_len: length of data buffer
*
* Return:
*	N/A
************************************************/
void RestoreTheRecordData(TS_RECORD *rec_data, T_U32* data_buf, T_U16 data_len)
{
	if (data_len)
	{
		rec_data->rec_time.year = *data_buf++;
		rec_data->rec_time.month = *data_buf++;
		rec_data->rec_time.date = *data_buf++;
	}
	if (rec_data->rec_type == E_RECORD_INFUSION)
	{
		rec_data->rec_time.hour = *data_buf++;
		rec_data->rec_time.minute = *data_buf++;
		rec_data->rec_time.second = *data_buf++;
		rec_data->rec_para = *data_buf++;
	}
	else if (rec_data->rec_type == E_RECORD_DAYS_TOTAL)
	{
		rec_data->rec_para = *data_buf++;
	}
	else if (rec_data->rec_type == E_RECORD_STOP_DELI)
	{
		rec_data->rec_time.hour = *data_buf++;
		rec_data->rec_time.minute = *data_buf++;
		rec_data->rec_time.second = *data_buf++;
		rec_data->rec_para = *data_buf++;
	}
	else if (rec_data->rec_type == E_RECORD_OPERATION)
	{
		rec_data->rec_time.hour = *data_buf++;
		rec_data->rec_time.minute = *data_buf++;
		rec_data->rec_time.second = *data_buf++;
		rec_data->rec_para = *data_buf++;
		rec_data->rec_data = *data_buf++;
	}
}

/***********************************************
* Description:
*	load one kind of record data
* Argument:
*   rec_data: record data
*
* Return:
*	0->ok,1->failure
************************************************/
TE_BOOLEAN LoadRec(TS_RECORD *rec_data)
{
	T_U16 data_length = 0;
	T_U32* load_addr = 0;
	T_U32* p_data_buf;
	T_U32* p_readback = g_fram_read_data_buff;
	T_U32  tmp[10]={0};
	
	switch(rec_data->rec_type)
	{
		case E_RECORD_INFUSION:
			data_length = LENGTH_OF_INFUSION_RECORD;
			load_addr = (T_U32*)FRAM_ADDR_INFUSION_RECORD_START;
			break;
		case E_RECORD_DAYS_TOTAL:			
			data_length = LENGTH_OF_DAYTOTAL_RECORD;
			load_addr = (T_U32*)FRAM_ADDR_DAYTOTAL_RECORD_START;
			break;
		case E_RECORD_STOP_DELI:			
			data_length = LENGTH_OF_STOPINFU_RECORD;
			load_addr = (T_U32*)FRAM_ADDR_STOPINFU_RECORD_START;
			break;
		case E_RECORD_OPERATION:			
			data_length = LENGTH_OF_OPERATE_RECORD;
			load_addr = (T_U32*)FRAM_ADDR_OPERATE_RECORD_START;
			break;
		default:
			break;
	}
	load_addr += rec_data->rec_id * data_length;
    
    p_data_buf=tmp;

	/* Verify the data validity */
	FramReadData(p_data_buf, load_addr, data_length);
	FramReadData(p_readback, load_addr, data_length);
	for(int index = 0; index < data_length; index++)
		if (p_data_buf[index] != p_readback[index])
			return E_FALSE;
		
	/* Restore data format  */
	RestoreTheRecordData(rec_data, p_data_buf, data_length);
	
	return E_TRUE;
}

void SaveAllDeliParaIntoOperRec()
{
	TS_RECORD record;
	TS_DELI_PARA valid_deli_para[16]={0}; // initialized to 0
	T_U16 index = 0;
	for(T_U16 idx = 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx++)
	{
		if (g_sys_para_infu_info[idx].dose != 0)
		{
			valid_deli_para[index].index = g_sys_para_infu_info[idx].index;
			valid_deli_para[index].dose = g_sys_para_infu_info[idx].dose;
			
            if(index<15)                  // range control
              index++;
            else
              break;
		}
	}
       
    //g_record_point.p_operation_rec=0;// initialized to 0, be cautious of out of storage range
	
    record.rec_type = E_RECORD_OPERATION;
	record.rec_id = g_record_point.p_operation_rec;
	record.rec_time.year = ((valid_deli_para[0].index << 24) | (valid_deli_para[0].dose << 16) |
						(valid_deli_para[1].index << 8) | valid_deli_para[1].dose);
	record.rec_time.month = ((valid_deli_para[2].index << 24) | (valid_deli_para[2].dose << 16) |
						(valid_deli_para[3].index << 8) | valid_deli_para[3].dose);
	record.rec_time.date = ((valid_deli_para[4].index << 24) | (valid_deli_para[4].dose << 16) |
						(valid_deli_para[5].index << 8) | valid_deli_para[5].dose);
	record.rec_time.hour = ((valid_deli_para[6].index << 24) | (valid_deli_para[6].dose << 16) |
						(valid_deli_para[7].index << 8) | valid_deli_para[7].dose);
	record.rec_time.minute = ((valid_deli_para[8].index << 24) | (valid_deli_para[8].dose << 16) |
						(valid_deli_para[9].index << 8) | valid_deli_para[9].dose);
	record.rec_time.second = ((valid_deli_para[10].index << 24) | (valid_deli_para[10].dose << 16) |
						(valid_deli_para[11].index << 8) | valid_deli_para[11].dose);
	record.rec_para = ((valid_deli_para[12].index << 24) | (valid_deli_para[12].dose << 16) |
						(valid_deli_para[13].index << 8) | valid_deli_para[13].dose);
	record.rec_data = ((valid_deli_para[14].index << 24) | (valid_deli_para[14].dose << 16) |
						(valid_deli_para[15].index << 8) | valid_deli_para[15].dose);
	SaveRec(record);
}

