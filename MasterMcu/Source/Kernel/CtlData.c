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
#include "common.h"
#include "AppMonitor.h"
#include "CtlTime.h"
#include "string.h"
#include "DrvRTC.h"
#include "common.h"
#include "CtlSpicomm.h"
//#include "AppAlarm.h"
#include "AppFactory.h"

#define MOTOR_POS_REC_MAX 2
#define ONE_MOTOR_POS_REC_SIZE 12
typedef struct
{
    T_U32 next_sequence; //the unique number
    T_U32 previous_motor_position;
    T_U32 previous_save_time;
    T_S16 next_write_index;
}TS_MOTOR_POS_SAVE_MANAGER;

typedef struct
{
    T_S16 one_rec_size;
    T_S16 count;
    T_S16 max_count;
    T_S16 next_write_index;
    T_U16 save_address;
}TS_REC_INFOR;

typedef struct
{
    T_U8 year; //the base year is 2000, so real year is 2000+year
    T_U8 month;
    T_U8 day;
    T_U8 hour;
    T_U8 minute;
    T_U8 second;
	T_U8 para[4];			//记录参数
	T_U8 data[4];			//记录数据
	T_U16 crc16_value;
}TS_REC_SAVE;

//************global variables******************
//工厂模式数据（永久保存），变量不需要初始化，开机会从存储器读取
T_U32 g_motor_cali_circle;			//电机校准encoder数,从工装到光耦的距离，单位：encoder
T_S16 g_cali_hal_to_end_len;			//hall到底的长度 ,unit: 0.001mm
T_U16 g_occl_cal_min;				//阻塞校准下限(输注力10), unit: 0.01Hz
T_U16 g_occl_speed_cal_min;				//阻塞校准下限(输注力10), unit: encoder速度
T_U16 g_occl_cal_max;				//阻塞校准上限(输注力15), unit: 0.01Hz
T_U16 g_occl_speed_cal_max;				//阻塞校准上限(输注力15), unit: encoder速度
T_S16 g_battery_voltage_offset; //电池电压偏移量
T_U32 g_hardware_ver;				//硬件版本号
T_U32 g_serial_number;				//产品序列号
T_U16 g_password_common = 0;



//用户模式数据（永久保存），变量不需要初始化，开机会从存储器读取
TS_DELI_PARA g_sys_para_infu_info[TOTAL_DELIVERY_TIME_POINT_NUMBER]; // unit:0.1ug

TE_BOOLEAN g_deliver_switch = E_FALSE;	//输注开关量
TE_DOSE_THICKNESS g_sys_para_thickness = (TE_DOSE_THICKNESS)0;		//浓度, unit: ug/ml
T_U16 g_sys_para_max_inf_dose = 0;	//单次最大剂量, unit: 1ug
T_U8  g_sys_para_user_set_deli_power; //用户设置输注力, 10~15
TE_BOOLEAN g_is_mute_mode = E_FALSE;	//静音模式状态标识
T_U16 g_preset_dose_value = 0;		//preset dose 1ug

TS_RECORD_POINT g_record_point = {0, 0, 0, 0};
TS_RECORD_COUNTER g_record_counter = {0, 0, 0, 0};
T_U32 g_motor_position = 0;				//电机当前位置,单位是encoder数

T_U32 g_deliver_start_motor_pos = 0;		//输注开始时的马达位置,单位是encoder数
T_U16 g_deliver_is_running = 0;	//是否在输注中
TE_BOOLEAN g_mcu_abnormal_reset_state = E_FALSE;	//MCU异常重启标识位
TE_BOOLEAN g_occlusion_alarm_is_set = E_FALSE;	  //occlusion_alarm status


//临时数据（断电不保存）
T_U16 g_deliver_dose_value = 0;		//需输注的剂量, unit:0.1ug
T_U16 g_deliver_dose_actual = 0;	//实际输注的剂量, unit:0.1ug

//T_U16 g_sys_para_actual_day_total = 0;	//实际输注的日总量,  unit:0.1ug
T_U16 g_sys_para_max_day_total = 0;		//日总量最大值,  unit:0.1ug

//T_U16 g_dose_number = 0;			//剩余药量,  unit:0.1ug
T_SINT g_batt_number = 0;			//剩余电量, unit: 0.001v
TE_BATTERY_LEVEL g_last_batt_level= E_BATTERY_FOUR;			//剩余电量
//T_U16 g_min_alarm_dose = 0;				//最小剂量报警值
//T_U16 g_min_alarm_battery = 0;			//最小电量报警值

//static T_U32 g_previous_motor_position = 0;	//电机上次位置,单位是encoder数
//static T_U32 g_previous_save_time;
//T_U32 g_motor_monitor_pos_point = 0;	//电机位置存储地址指针
//T_U32 g_motor_monitor_pos_counter = 0;	//电机位置监控计数器
//TE_BOOLEAN g_occlusion_state = E_FALSE;	//电机堵塞状态

T_U16 g_alarm_status = 0;			//报警状态位信息
T_U16 g_remind_alarm_status = 0;			//提示状态位信息

T_U16 g_password_private = 0;
TE_DELIVERY_STATE g_delivery_state = E_DELI_STATE_STOP;


T_U32 g_fram_read_data_buff[20];
T_U32 g_fram_wr_data_buff[10];   //modified

static TS_MOTOR_POS_SAVE_MANAGER g_motor_pos_manager;



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
	T_U32 fram_read_data_buff[20];
	fram_point=(T_U32 *)fram_address;
	T_U32 *p_readback = fram_read_data_buff;
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
	T_U32 fram_read_data_buff[20];
	fram_point=(T_U32 *)fram_address;
	T_U32 *p_readback = fram_read_data_buff;
	T_U16 index;
	/* Verify the data validity */
	FramReadData(p_buf,fram_point, buf_len);
	FramReadData( p_readback,fram_point, buf_len);
	for(index = 0; index < buf_len; index++)
		if (p_buf[index] != p_readback[index])
			return E_FALSE;
	return E_TRUE;
}

//TE_BOOLEAN SaveOneValue(T_U16 nvram_address, T_U32 save_value){
//	*(T_U32*)nvram_address = save_value;
//	return E_TRUE;
//}
//
//T_U32 ReadOneValue(T_U16 nvram_address){
//	return *(T_U32*)nvram_address;
//}


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
TE_BOOLEAN SaveOrLoadSysPara(TE_SYS_PARA type, T_U16 para, T_U32 *sys_para, TE_BOOLEAN is_save)
{
	T_U32* save_addr;
	T_U32* save_data = sys_para;  
    T_U32 sdata=0;//add

	if(save_data==NULL){
		save_data=&sdata;
	}
	
	switch(type)
	{
		case E_SYSTEM_PARA_INFU_INFO:
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_INFU_START;
			save_addr += (para-1);   //  4*(para-1)
			if(is_save){
				*save_data = (T_U32)g_sys_para_infu_info[para-1].dose;
			}else{
				g_sys_para_infu_info[para-1].dose = (T_U32)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_THICKNESS:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_THICKNESS;
			if(is_save){
				*save_data = (T_U32)g_sys_para_thickness;
			}else{
				g_sys_para_thickness = (TE_DOSE_THICKNESS)*save_addr;	
			}
			break;
		case E_SYSTEM_PARA_MAX_INFU_DOSE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE;
			if(is_save){
				*save_data = (T_U32)g_sys_para_max_inf_dose;
			}else{
				g_sys_para_max_inf_dose = (T_U16)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_USER_SET_DELI_POWER:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_USER_DELI_POWER;
			if(is_save){
				*save_data = (T_U32)g_sys_para_user_set_deli_power;
			}else{
				g_sys_para_user_set_deli_power = (T_U8)*save_addr;	
			}
			break;			
	    case E_SYSTEM_PARA_MUTE_STATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MUTE_STATE;				
			if(is_save){
				*save_data = (T_U32)g_is_mute_mode;
			}else{
				g_is_mute_mode = (TE_BOOLEAN)*save_addr;
			}
			break;
	    case E_SYSTEM_PAPA_PRESET_DOSE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_PRESET_DOSE;				
			if(is_save){
				*save_data = (T_U32)g_preset_dose_value;
			}else{
				g_preset_dose_value = (T_U16)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_PASSWORD_COMMON:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_PASSWORD_DEF;
			if(is_save){
				*save_data = (T_U32)g_password_common;
			}else{
				g_password_common = (T_U16)*save_addr;
			}
			break;

		case E_SYSTEM_PARA_HALL_CAL_DATA:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MOTOR_CAL;
            if(is_save){
            	*save_data = g_motor_cali_circle;
            }else{
            	g_motor_cali_circle = (T_U32)*save_addr;
            }
			break;
        case E_SYSTEM_PARA_HALL_TO_END_CAL_DATA:           
            save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_HALL_TO_END_LEN;
            if(is_save){
            	*save_data = g_cali_hal_to_end_len;
            }else{
            	g_cali_hal_to_end_len = (T_S16)*save_addr;
            }
            break;
		case E_SYSTEM_PARA_DELI_SWITCH_STATE:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_SWITCH;
			if(is_save){
				*save_data = (T_U32)g_deliver_switch;
			}else{
				g_deliver_switch = (TE_BOOLEAN)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_OCCL_CAL_MIN:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OCCL_CAL_MIN;
			if(is_save){
				*save_data = (T_U32)g_occl_cal_min;
			}else{
				g_occl_cal_min = (T_U16)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_OCCL_SPEED_CAL_MIN:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OCCL_SPEED_CAL_MIN;
			if(is_save){
				*save_data = (T_U32)g_occl_speed_cal_min;
			}else{
				g_occl_speed_cal_min = (T_U16)*save_addr;
			}
			break;		
		case E_SYSTEM_PARA_OCCL_CAL_MAX:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OCCL_CAL_MAX;
			if(is_save){
				*save_data = (T_U32)g_occl_cal_max;
			}else{
				g_occl_cal_max = (T_U16)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_OCCL_SPEED_CAL_MAX:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OCCL_SPEED_CAL_MAX;
			if(is_save){
				*save_data = (T_U32)g_occl_speed_cal_max;
			}else{
				g_occl_speed_cal_max = (T_U16)*save_addr;
			}
			break;			
//		case E_SYSTEM_PARA_OCCL_SPEED_CAL_MIN:			
//			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OCCL_SPEED_CAL_MIN;
//			*save_data = (T_U32)g_occl_speed_cal_min;
//			break;

		case E_SYSTEM_PARA_RECORD_INDEX:
			switch(para)
			{
				case 0:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_DELI;
					if(is_save){
						*save_data = (T_U32)g_record_point.p_infusion_rec;
					}else{
						g_record_point.p_infusion_rec = (T_U16)*save_addr;
					}
					break;
				case 1:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_DAY;
					if(is_save){
						*save_data = (T_U32)g_record_point.p_daytotal_rec;
					}else{
						g_record_point.p_daytotal_rec = (T_U16)*save_addr;
					}
					break;
				case 2:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_UNDELI;
					if(is_save){
						*save_data = (T_U32)g_record_point.p_stopinfu_rec;
					}else{
						g_record_point.p_stopinfu_rec = (T_U16)*save_addr;
					}
					break;
				case 3:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_REC_P_OPER;
					if(is_save){
						*save_data = (T_U32)g_record_point.p_operation_rec;
					}else{
						g_record_point.p_operation_rec = (T_U16)*save_addr;
					}
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
					if(is_save){
						*save_data = (T_U32)g_record_counter.de_rec_counter;
					}else{
						g_record_counter.de_rec_counter = (T_U16)*save_addr;
					}
					break;
				case 1:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DTR_COUNTER;
					if(is_save){
						*save_data = (T_U32)g_record_counter.dt_rec_counter;
					}else{
						g_record_counter.dt_rec_counter = (T_U16)*save_addr;
					}
					break;
				case 2:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_SDR_COUNTER;
					if(is_save){
						*save_data = (T_U32)g_record_counter.sd_rec_counter;
					}else{
						g_record_counter.sd_rec_counter = (T_U16)*save_addr;	
					}
					break;
				case 3:
					save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OPR_COUNTER;
					if(is_save){
						*save_data = (T_U32)g_record_counter.op_rec_counter;
					}else{
						g_record_counter.op_rec_counter = (T_U16)*save_addr;
					}
					break;
				default:
					break;
			}
			break;
		case E_SYSTEM_PARA_SIN_RST_FLAG:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_MCU_RES_FLAG;
			if(is_save){
				*save_data = (T_U32)g_mcu_abnormal_reset_state;
			}else{
				g_mcu_abnormal_reset_state = (TE_BOOLEAN)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_DELI_START_MOTOR_POS:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_START_MOTOR_POS;
			if(is_save){
				*save_data = (T_U32)g_deliver_start_motor_pos;
			}else{
				g_deliver_start_motor_pos= (T_U32)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_DELI_IS_RUNNING:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_DELI_IS_RUNING;
			if(is_save){
				*save_data = (T_U32)g_deliver_is_running;
			}else{
				g_deliver_is_running= (T_U16)*save_addr;
			}
			break;
		case E_SYSTEM_PARA_HW_VERSION:
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_HW_VERSION;
			if(!is_save)g_hardware_ver = (T_U32)*save_addr;
			break;
		case E_SYSTEM_PARA_SN:
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_SN;
			if(!is_save)g_serial_number= (T_U32)*save_addr;
			break;				
		case E_SYSTEM_PAPA_BATTERY_VOLLTAGE_OFFSET:			
			save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_BATTERY_VOLLTAGE_OFFSET;
			if(is_save){
				*save_data = (T_U32)g_battery_voltage_offset;
			}else{
				g_battery_voltage_offset = (T_SINT)*save_addr;
			}
			break;
        case E_SYSTEM_PAPA_OCCLUSION_ALARM_FLAG:         
            save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_OCCLUSION_ALARM_FLAG;
            if(is_save){
            	*save_data = (T_U32)g_occlusion_alarm_is_set;
            }else{
				g_occlusion_alarm_is_set = (TE_BOOLEAN)*save_addr;
            }
             break;
        case E_SYSTEM_PAPA_RUN_CYCLE_COUNT:         
            save_addr = (T_U32*)FRAM_ADDR_SYS_PARA_RUN_CYCLE_COUNT;
            break;		
		default:
			M_Assert(0);
			break;
	}

	if(is_save){
		T_U16 data_len = 1;
		T_U32 rdata=0;

		/* Verify the data validity */
		FramWriteData(save_data, save_addr, data_len);
		FramReadData(&rdata, save_addr, data_len);
		if (*save_data != rdata)
		{
	        //AlarmSet(E_ALARM_FRAM_ERROR_ID);
			return E_FALSE;
		}
	}else if(sys_para){//read data
		*sys_para = *save_addr;
	}
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
	return SaveOrLoadSysPara(type, para, 0, E_TRUE);
}

TE_BOOLEAN SaveSysParaEx(TE_SYS_PARA type, T_U16 para, T_U32 sys_value)
{
	return SaveOrLoadSysPara(type, para, &sys_value, E_TRUE);
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
	return SaveOrLoadSysPara(type, para, NULL, E_FALSE);
}
T_U32 LoadSysParaEx(TE_SYS_PARA type, T_U16 para)
{
	T_U32 ret;
	SaveOrLoadSysPara(type, para, &ret, E_FALSE);
	return ret;
}

TE_BOOLEAN DataWriteWithCheck(T_U32 fram_address,void * p_buf,T_U16 buf_len){
	T_U16 crc_value = crc16(p_buf, buf_len);
	T_U16 read_to_check;

	memcpy((T_U8*)fram_address, p_buf, buf_len);
	memcpy((T_U8*)fram_address+buf_len, &crc_value, 2);
	memcpy(&read_to_check, (T_U8*)(fram_address+buf_len), 2);
	if(crc_value == read_to_check){
		return E_TRUE;
	}else{
		return E_FALSE;
	}
	
}
TE_BOOLEAN DataReadWithCheck(T_U32 fram_address,void * p_buf,T_U16 buf_len){
	T_U16 crc_value;
	T_U16 read_to_check;

	memcpy(p_buf, (T_U8*)fram_address, buf_len);
	memcpy(&read_to_check, (T_U8*)(fram_address+buf_len), 2);
	crc_value = crc16(p_buf, buf_len);
	if(crc_value == read_to_check){
		return E_TRUE;
	}else{
		return E_FALSE;
	}
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
void SortTheRecordData(TS_RECORD *rec_data, TS_REC_SAVE *rec_save)
{
	//T_U32* p_data_buf;
	if(rec_data->rec_time.year>=BASE_YEAR)
    {
        rec_save->year = rec_data->rec_time.year - BASE_YEAR;
	}
    else
    {
        rec_save->year = 0 ;
    }
    rec_save->month 	= rec_data->rec_time.month;
    rec_save->day 		= rec_data->rec_time.date;
	rec_save->hour 		= rec_data->rec_time.hour;
	rec_save->minute 	= rec_data->rec_time.minute;
	rec_save->second 	= rec_data->rec_time.second;
    memcpy(rec_save->para, &rec_data->rec_para, 4);
	memcpy(rec_save->data, &rec_data->rec_data, 4);
}

TE_BOOLEAN SaveRec(TE_RECORD_TYPE type, T_U32 para, T_U32 data)
{
    TS_RECORD rec_data;

    Ds1302TimeRead();

    rec_data.rec_type = type;
    switch(type)
    {
        case E_RECORD_INFUSION:
            rec_data.rec_id = g_record_point.p_infusion_rec;
            break;
        case E_RECORD_DAYS_TOTAL:
            rec_data.rec_id = g_record_point.p_daytotal_rec;
            break;
        case E_RECORD_STOP_DELI:
            rec_data.rec_id = g_record_point.p_stopinfu_rec;
            break;
        case E_RECORD_OPERATION:
            rec_data.rec_id = g_record_point.p_operation_rec;
            break;
        default:
            M_Assert(0);
            break;
    }
    rec_data.rec_time = g_current_time;   
    rec_data.rec_para = para;
    rec_data.rec_data = data;

    return SaveRecEx(&rec_data);
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
TE_BOOLEAN SaveRecEx(TS_RECORD *rec_data)
{
	T_U16 data_length = 0;
	T_U8* save_addr = 0;
    TS_REC_SAVE rec_save;
    
	switch(rec_data->rec_type)
	{
		case E_RECORD_INFUSION:
			data_length = LENGTH_OF_INFUSION_RECORD;
			save_addr = (T_U8*)FRAM_ADDR_INFUSION_RECORD_START;
			//rec_data->rec_id = g_record_point.p_infusion_rec;
			if(g_record_counter.de_rec_counter < USER_HISTORY_RECORD)
			{
				g_record_counter.de_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 0);
			}
		
			g_record_point.p_infusion_rec = rec_data->rec_id + 1;
			if (g_record_point.p_infusion_rec >= USER_HISTORY_RECORD)
				g_record_point.p_infusion_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 0);
			break;
		case E_RECORD_DAYS_TOTAL:			
			data_length = LENGTH_OF_DAYTOTAL_RECORD;
			save_addr = (T_U8*)FRAM_ADDR_DAYTOTAL_RECORD_START;
			//rec_data->rec_id = g_record_point.p_daytotal_rec;
			if(g_record_counter.dt_rec_counter < USER_HISTORY_RECORD)
			{
				g_record_counter.dt_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 1);
			}

			g_record_point.p_daytotal_rec = rec_data->rec_id + 1;
			if (g_record_point.p_daytotal_rec >= USER_HISTORY_RECORD)
				g_record_point.p_daytotal_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 1);
			break;
		case E_RECORD_STOP_DELI:			
			data_length = LENGTH_OF_STOPINFU_RECORD;
			save_addr = (T_U8*)FRAM_ADDR_STOPINFU_RECORD_START;
			//rec_data->rec_id = g_record_point.p_stopinfu_rec;
			if(g_record_counter.sd_rec_counter < USER_HISTORY_RECORD)
			{
				g_record_counter.sd_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 2);
			}

			g_record_point.p_stopinfu_rec = rec_data->rec_id + 1;
			if (g_record_point.p_stopinfu_rec >= USER_HISTORY_RECORD)
				g_record_point.p_stopinfu_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 2);
			break;
		case E_RECORD_OPERATION:			
			data_length = LENGTH_OF_OPERATE_RECORD;
			save_addr = (T_U8*)FRAM_ADDR_OPERATE_RECORD_START;
			//rec_data->rec_id = g_record_point.p_operation_rec;
			if(g_record_counter.op_rec_counter < OPER_HISTORY_RECORD)
			{
				g_record_counter.op_rec_counter ++;
				SaveSysPara(E_SYSTEM_PARA_RECORD_COUNTER, 3);
			}

			g_record_point.p_operation_rec = rec_data->rec_id + 1;
			if (g_record_point.p_operation_rec >= OPER_HISTORY_RECORD)
				g_record_point.p_operation_rec = 0;
			SaveSysPara(E_SYSTEM_PARA_RECORD_INDEX, 3);
			break;
		default:
            M_Assert(0);
			break;
	}
	save_addr += rec_data->rec_id * data_length;
	/* Get the sorted data buffer */
	SortTheRecordData(rec_data, &rec_save);
	/* Verify the data validity */
    rec_save.crc16_value = crc16((T_U8*)&rec_save, data_length-2);
    memcpy(save_addr, (T_U8*)&rec_save, data_length);

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
void RestoreTheRecordData(TS_RECORD *rec_data, TS_REC_SAVE *rec_save)
{

	rec_data->rec_time.year = 	rec_save->year + BASE_YEAR;
	rec_data->rec_time.month = 	rec_save->month;
	rec_data->rec_time.date = 	rec_save->day;
	rec_data->rec_time.hour = 	rec_save->hour;
	rec_data->rec_time.minute = rec_save->minute;
	rec_data->rec_time.second = rec_save->second;
	memcpy(&rec_data->rec_para, rec_save->para, 4);
	memcpy(&rec_data->rec_data, rec_save->data, 4);

}


void RecGetInfo(TE_RECORD_TYPE type, TS_REC_INFOR* rec_info)
{
    switch(type)
    {
        case E_RECORD_INFUSION:
            rec_info->one_rec_size = LENGTH_OF_INFUSION_RECORD;
            rec_info->count = g_record_counter.de_rec_counter;
            rec_info->max_count = USER_HISTORY_RECORD;
            rec_info->next_write_index = g_record_point.p_infusion_rec;
            rec_info->save_address = FRAM_ADDR_INFUSION_RECORD_START;
            break;
        case E_RECORD_DAYS_TOTAL:
            rec_info->one_rec_size = LENGTH_OF_DAYTOTAL_RECORD;
            rec_info->count = g_record_counter.dt_rec_counter;
            rec_info->max_count = USER_HISTORY_RECORD;
            rec_info->next_write_index = g_record_point.p_daytotal_rec;
            rec_info->save_address = FRAM_ADDR_DAYTOTAL_RECORD_START;            
            break;
        case E_RECORD_STOP_DELI:
            rec_info->one_rec_size = LENGTH_OF_STOPINFU_RECORD;
            rec_info->count = g_record_counter.sd_rec_counter;
            rec_info->max_count = USER_HISTORY_RECORD;
            rec_info->next_write_index = g_record_point.p_stopinfu_rec;
            rec_info->save_address = FRAM_ADDR_STOPINFU_RECORD_START;  
            break;
        case E_RECORD_OPERATION:
            rec_info->one_rec_size = LENGTH_OF_OPERATE_RECORD;
            rec_info->count = g_record_counter.op_rec_counter;
            rec_info->max_count = OPER_HISTORY_RECORD;
            rec_info->next_write_index = g_record_point.p_operation_rec;
            rec_info->save_address = FRAM_ADDR_OPERATE_RECORD_START;
            break;
        default:
            M_Assert(0);
            break;
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
	T_U8* load_addr = 0;
	//T_U32* p_data_buf;
	//T_U32* p_readback = g_fram_read_data_buff;
	//T_U32  tmp[10]={0};
    TS_REC_SAVE rec_save;
	
	switch(rec_data->rec_type)
	{
		case E_RECORD_INFUSION:
			data_length = LENGTH_OF_INFUSION_RECORD;
			load_addr = (T_U8*)FRAM_ADDR_INFUSION_RECORD_START;
			break;
		case E_RECORD_DAYS_TOTAL:			
			data_length = LENGTH_OF_DAYTOTAL_RECORD;
			load_addr = (T_U8*)FRAM_ADDR_DAYTOTAL_RECORD_START;
			break;
		case E_RECORD_STOP_DELI:			
			data_length = LENGTH_OF_STOPINFU_RECORD;
			load_addr = (T_U8*)FRAM_ADDR_STOPINFU_RECORD_START;
			break;
		case E_RECORD_OPERATION:			
			data_length = LENGTH_OF_OPERATE_RECORD;
			load_addr = (T_U8*)FRAM_ADDR_OPERATE_RECORD_START;
			break;
		default:
			M_Assert(0);
			break;
	}
	load_addr += rec_data->rec_id * data_length;
    
    //p_data_buf=tmp;

	/* Verify the data validity */
    memcpy((T_U8*)&rec_save, load_addr, data_length);
    if(crc16((T_U8*)&rec_save, data_length-2) == rec_save.crc16_value)
    {
	    /* Restore data format  */
    	RestoreTheRecordData(rec_data, &rec_save);        
        return E_TRUE;
    }
    else
    {
        memset(&rec_data->rec_time, 0, sizeof(rec_data->rec_time));
        rec_data->rec_para = 0;
        rec_data->rec_data = 0;
        return E_FALSE;
    }
}

//the first is the oldest one
//return -1 is indicate the end
T_SINT RecFirstIndex(TE_RECORD_TYPE type)
{
    TS_REC_INFOR rec_info;
    T_SINT ret = -1;
    
    RecGetInfo(type, &rec_info);

    if(rec_info.count > 0)
    {
        ret = rec_info.next_write_index - rec_info.count;
        if(ret<0)
        {
            ret += rec_info.max_count;
        }
    }

    return ret;
}
T_SINT RecLastIndex(TE_RECORD_TYPE type)
{
    TS_REC_INFOR rec_info;
    T_SINT ret = -1;
    
    RecGetInfo(type, &rec_info);

    if(rec_info.count > 0)
    {
        ret = rec_info.next_write_index - 1;
        if(ret<0)
        {
            ret += rec_info.max_count;
        }
    }

    return ret;    
}
//the newer one
T_SINT RecNextIndex(TE_RECORD_TYPE type, T_SINT curr_index)
{
    TS_REC_INFOR rec_info;
    T_SINT ret = -1;
    
    RecGetInfo(type, &rec_info);

    if(rec_info.count > 0 && curr_index!=-1)
    {
        if(curr_index == RecLastIndex(type))
        {
            ret = -1;
        }
        else
        {
            ret = curr_index + 1;
            if(ret>=rec_info.max_count)
            {
                ret = 0;
            }
        }

    }

    return ret;   
}
//the older one
T_SINT RecPrevIndex(TE_RECORD_TYPE type, T_SINT curr_index)
{
    TS_REC_INFOR rec_info;
    T_SINT ret = -1;
    
    RecGetInfo(type, &rec_info);

    if(rec_info.count > 0 && curr_index!=-1)
    {
        if(curr_index == RecFirstIndex(type))
        {
            ret = -1;
        }
        else
        {
            ret = curr_index - 1;
            if(ret<0)
            {
                ret += rec_info.max_count;
            }
        }

    }

    return ret;      
}


//	void SaveAllDeliParaIntoOperRec()
//	{
//		TS_RECORD record;
//		TS_DELI_PARA valid_deli_para[16]={0}; // initialized to 0
//		T_U16 index = 0;
//		for(T_U16 idx = 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx++)
//		{
//			if (g_sys_para_infu_info[idx].dose != 0)
//			{
//				valid_deli_para[index].index = g_sys_para_infu_info[idx].index;
//				valid_deli_para[index].dose = g_sys_para_infu_info[idx].dose;
//				
//	            if(index<15)                  // range control
//	              index++;
//	            else
//	              break;
//			}
//		}
//	       
//	    //g_record_point.p_operation_rec=0;// initialized to 0, be cautious of out of storage range
//		
//	    record.rec_type = E_RECORD_OPERATION;
//		record.rec_id = g_record_point.p_operation_rec;
//		record.rec_time.year = ((valid_deli_para[0].index << 24) | (valid_deli_para[0].dose << 16) |
//							(valid_deli_para[1].index << 8) | valid_deli_para[1].dose);
//		record.rec_time.month = ((valid_deli_para[2].index << 24) | (valid_deli_para[2].dose << 16) |
//							(valid_deli_para[3].index << 8) | valid_deli_para[3].dose);
//		record.rec_time.date = ((valid_deli_para[4].index << 24) | (valid_deli_para[4].dose << 16) |
//							(valid_deli_para[5].index << 8) | valid_deli_para[5].dose);
//		record.rec_time.hour = ((valid_deli_para[6].index << 24) | (valid_deli_para[6].dose << 16) |
//							(valid_deli_para[7].index << 8) | valid_deli_para[7].dose);
//		record.rec_time.minute = ((valid_deli_para[8].index << 24) | (valid_deli_para[8].dose << 16) |
//							(valid_deli_para[9].index << 8) | valid_deli_para[9].dose);
//		record.rec_time.second = ((valid_deli_para[10].index << 24) | (valid_deli_para[10].dose << 16) |
//							(valid_deli_para[11].index << 8) | valid_deli_para[11].dose);
//		record.rec_para = ((valid_deli_para[12].index << 24) | (valid_deli_para[12].dose << 16) |
//							(valid_deli_para[13].index << 8) | valid_deli_para[13].dose);
//		record.rec_data = ((valid_deli_para[14].index << 24) | (valid_deli_para[14].dose << 16) |
//							(valid_deli_para[15].index << 8) | valid_deli_para[15].dose);
//		SaveRecEx(record);
//	}

//	void OnMotorPosSaveTimerout()
//	{
//	
//	}

//int and read the motor pos
void InitMotorPos()
{
    T_U32 motor_pos,motor_pos_back;
    T_U32 last_sequence_num=0;
    T_SINT pos_index = 0;
    T_U32 next_sequence_num;
    T_SINT i;
    T_U32 start_address;

    for(i=0; i<MOTOR_POS_REC_MAX; i++)
    {
        start_address = FRAM_ADDR_MOTOR_POS_CNT_START+i*ONE_MOTOR_POS_REC_SIZE;
        next_sequence_num = *(T_U32*)(start_address);
        motor_pos = *(T_U32*)(start_address+4);
        motor_pos_back = *(T_U32*)(start_address+8);  
        if(next_sequence_num==0xFFFFFFFF && motor_pos==0xFFFFFFFF && motor_pos_back==0xFFFFFFFF)
        {
            break;
        }
        else if(motor_pos != motor_pos_back)//check if the save is right
        {
            //not same, the save is wrong
            continue;
        }
        else if(last_sequence_num<next_sequence_num)
        {
            //this is the last, save it
            last_sequence_num = next_sequence_num;
            pos_index = i;
        }
        else
        {
            break;
        }
    }

    if(last_sequence_num == 0)
    {
        //not find any valid one
        g_motor_pos_manager.next_write_index = 0;
        g_motor_pos_manager.next_sequence = 1;
        g_motor_position = 0;
    }
    else
    {
        start_address = FRAM_ADDR_MOTOR_POS_CNT_START+pos_index*ONE_MOTOR_POS_REC_SIZE;
        g_motor_pos_manager.next_write_index = pos_index+1;
        if(g_motor_pos_manager.next_write_index >=MOTOR_POS_REC_MAX)
        {
           g_motor_pos_manager.next_write_index = 0; 
        }
        g_motor_pos_manager.next_sequence = last_sequence_num+1;
        g_motor_position = *(T_U32*)(start_address+4);
    }
    g_motor_pos_manager.previous_motor_position = g_motor_position;
    g_motor_pos_manager.previous_save_time = CtlTimeSincePowerOn();
    //CtlTimerBindHandle(E_TIMER_AUTO_SAVE_MOTOR_POS, OnMotorPosSaveTimerout);
    //CtlTimerSetRepeat(E_TIMER_AUTO_SAVE_MOTOR_POS, 100);
    
}

//save the motor pos
void SaveMotorPos(TE_BOOLEAN is_flash_cache)
{
    T_U32 start_address;

    if(g_motor_position == g_motor_pos_manager.previous_motor_position)
    {
        return;
    }

    if(!is_flash_cache)
    {
        if(CtlTimeElapse(g_motor_pos_manager.previous_save_time)<20)
        {
            return;
        }
    }
    g_motor_pos_manager.previous_motor_position = g_motor_position;
    g_motor_pos_manager.previous_save_time = CtlTimeSincePowerOn();

 
    start_address = FRAM_ADDR_MOTOR_POS_CNT_START +
        g_motor_pos_manager.next_write_index*ONE_MOTOR_POS_REC_SIZE;
    *(T_U32*)(start_address) = g_motor_pos_manager.next_sequence; 
    *(T_U32*)(start_address+4) = g_motor_pos_manager.previous_motor_position;    
    *(T_U32*)(start_address+8) = g_motor_pos_manager.previous_motor_position;
    //read to check if write ok
    if(*(T_U32*)(start_address) != g_motor_pos_manager.next_sequence ||
        *(T_U32*)(start_address+4) != g_motor_pos_manager.previous_motor_position ||
        *(T_U32*)(start_address+8) != g_motor_pos_manager.previous_motor_position 
        )
    {
        //AlarmSet(E_ALARM_FRAM_ERROR_ID);
    }
    
    g_motor_pos_manager.next_write_index++;
    if(g_motor_pos_manager.next_write_index >=MOTOR_POS_REC_MAX)
    {
       g_motor_pos_manager.next_write_index = 0; 
    }
    g_motor_pos_manager.next_sequence++;
}



T_U16 GetThicknessValue(TE_DOSE_THICKNESS thickness)
{
	T_U16 value;
	switch(thickness)
	{
		case E_DOSE_THICKNESS_100:
			value = DOSE_THICKNESS_100;
			break;
		case E_DOSE_THICKNESS_200:
			value = DOSE_THICKNESS_200;
			break;
		case E_DOSE_THICKNESS_500:
			value = DOSE_THICKNESS_500;
			break;
		default:
            value = DOSE_THICKNESS_200;
            M_Assert(0);
			break;
	}
	return value;
}

//convert dose to encoder counter, dose unit:0.1ug
T_U32 DoseToEncoder(TE_DOSE_THICKNESS thickness, T_U16 dose)
{
    float volume; //unit: ml
    T_U16 dose_thickness = GetThicknessValue(thickness);

    volume = dose/10.0/dose_thickness;
    return (T_U32)(M_VolumeToEncoder(volume));
}

//convert encoder to dose, return unit:0.1ug
T_U16 EncoderToDose(TE_DOSE_THICKNESS thickness, T_U32 encoder)
{
    float volume; //unit: ml
    T_U16 dose_thickness = GetThicknessValue(thickness);

    volume =M_EncoderToVolume(encoder);

    return (T_U16)(dose_thickness*volume*10);
}

//convert volume to dose, volume unit: 0.001ml=1mm^3, return unit:0.1ug
T_U16 VolumeToDose(TE_DOSE_THICKNESS thickness, T_U16 volume)
{
	T_U16 value = DOSE_THICKNESS_200;
	T_U16 temp;
	switch(thickness)
	{
		case E_DOSE_THICKNESS_100:
			value = DOSE_THICKNESS_100;
			break;
		case E_DOSE_THICKNESS_200:
			value = DOSE_THICKNESS_200;
			break;
		case E_DOSE_THICKNESS_500:
			value = DOSE_THICKNESS_500;
			break;
		default:
			M_Assert(0);
			break;
	}
	temp = (T_U32)volume * (T_U32)value / 100;
	return temp;    
}

//convert length to encode, length unit:0.001mm
T_U32 LengthToEncoder(T_U16 length)
{
    return (T_U32)MOTOR_SHIFTING_TO_CIRCLE*length/1000;
}

//convert encode to length , length unit:0.001mm
T_U16 EncoderToLength(T_U32 encoder)
{
    return encoder*1000/MOTOR_SHIFTING_TO_CIRCLE;
}


//convert volume to encoder counter, volume unit:0.001ml=1mm^3
T_U32 VolumeToEncoder(T_U16 volume)
{
    
    return (T_U32)M_VolumeToEncoder(volume/1000.0);
}

//convert encoder to volume, volume unit:0.001ml=1mm^3
T_U16 EncoderToVolume(T_U32 encoder)
{
    return (T_U16)(M_EncoderToVolume(encoder)*1000);
}


//the remaining dose, dose unit:0.1ug
T_U16 RemainDose()
{
    return EncoderToDose(g_sys_para_thickness, g_motor_position);
}

//the max motor pos, the unit is encoder
T_U32 MaxMotorPos()
{
    return VolumeToEncoder(TOOLING_HALL_CAL_VOLUME) + 
        g_motor_cali_circle + LengthToEncoder(g_cali_hal_to_end_len);
}

//获取阻塞报警Hz阈值
T_U16 GetOcclusionHzThreshold(){
	if (g_sys_para_user_set_deli_power == 10){		
		return g_occl_cal_min;
	}else if(g_sys_para_user_set_deli_power == 15){
		return g_occl_cal_max;
	}
	else{
		float step = 0;
		
	    if(g_occl_cal_max>g_occl_cal_min)
	    {
	        step = (g_occl_cal_max-g_occl_cal_min)/5.0;
	    }
		return (T_U16)(g_occl_cal_min+step*(g_sys_para_user_set_deli_power-10));
	}
}
//获取阻塞报警速度阈值
T_U16 GetOcclusionSpeedThreshold(){
	if (g_sys_para_user_set_deli_power == 10){		
		return g_occl_speed_cal_min;
	}else if(g_sys_para_user_set_deli_power == 15){
		return g_occl_speed_cal_max;
	}
	else{
		float step = 0;
		
	    if(g_occl_speed_cal_max<g_occl_speed_cal_min)
	    {
	        step = (g_occl_speed_cal_min-g_occl_speed_cal_max)/5.0;
	    }

		return (T_U16)(g_occl_speed_cal_max + step*(g_sys_para_user_set_deli_power-10));
	}
}
