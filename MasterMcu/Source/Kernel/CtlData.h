/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlData.h
*
* DESCRIPTION:
*   data access module head file
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/12/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLDATA_H
#define CTLDATA_H

#include "..\TypeDefine.h"
#include "..\Driver\DrvFram.h"
#include "CtlDelivery.h"
#include "Appmonitor.h"

//主要参数: 0.5K
//define the address that all kinds of data should saved into.
//工厂参数
#define FRAM_ADDR_SYS_PARA_FACTORY_START	(FRAM_MAIN_PARAM_START_ADDRESS + 0)	//工厂参数开始位置
#define FRAM_ADDR_SYS_PARA_FACTORY_FLAG	(FRAM_ADDR_SYS_PARA_FACTORY_START + 0)	//工厂参数初始化flag
#define FRAM_ADDR_SYS_PARA_MOTOR_CAL	(FRAM_ADDR_SYS_PARA_FACTORY_START + 4)	//电机校准数据,从工装到光耦的距离，单位：encoder
//#define FRAM_ADDR_SYS_PARA_DELI_CAL_RAT	(FRAM_ADDR_SYS_PARA_FACTORY_START + 4)	//输注校准比例
//#define FRAM_ADDR_SYS_PARA_DOSE_ALARM	(FRAM_ADDR_SYS_PARA_FACTORY_START + 8)	//最小剂量报警值
//#define FRAM_ADDR_SYS_PARA_BATT_ALARM	(FRAM_ADDR_SYS_PARA_FACTORY_START + 12)	//最小电量报警值
#define FRAM_ADDR_SYS_PARA_PASSWORD_DEF	(FRAM_ADDR_SYS_PARA_FACTORY_START + 16)	//默认密码：固定值"222"
//#define FRAM_ADDR_SYS_PARA_PASSWORD_PRI	(FRAM_ADDR_SYS_PARA_FACTORY_START + 20)	//专用密码：序列号后三位
#define FRAM_ADDR_SYS_PARA_HALL_TO_END_LEN (FRAM_ADDR_SYS_PARA_FACTORY_START + 20) //hall到底的长度 ,unit: 0.001mm

#define FRAM_ADDR_SYS_PARA_OCCL_CAL_MIN	(FRAM_ADDR_SYS_PARA_FACTORY_START + 24)	//阻塞校准下限(输注力10)，Unit:0.01Hz
#define FRAM_ADDR_SYS_PARA_OCCL_SPEED_CAL_MIN	(FRAM_ADDR_SYS_PARA_FACTORY_START + 32)	//阻塞校准下限(encoder速度，输注力10)
#define FRAM_ADDR_SYS_PARA_OCCL_CAL_MAX	(FRAM_ADDR_SYS_PARA_FACTORY_START + 8)	//阻塞校准上限(输注力15)，Unit:0.01Hz
#define FRAM_ADDR_SYS_PARA_OCCL_SPEED_CAL_MAX	(FRAM_ADDR_SYS_PARA_FACTORY_START + 12)	//阻塞校准上限(encoder速度，输注力15)
//电池电压偏移量
#define FRAM_ADDR_SYS_PARA_BATTERY_VOLLTAGE_OFFSET (FRAM_ADDR_SYS_PARA_FACTORY_START + 28)

#define FRAM_ADDR_SYS_PARA_HW_VERSION	(FRAM_ADDR_SYS_PARA_FACTORY_START + 40)	//硬件版本号
#define FRAM_ADDR_SYS_PARA_SN	        (FRAM_ADDR_SYS_PARA_FACTORY_START + 44)	//产品序列号
#define FRAM_ADDR_SYS_PARA_FACTORY_SIZE	 48 //工厂参数大小


#define FRAM_ADDR_FCT_SEND	    (FRAM_ADDR_SYS_PARA_FACTORY_START + 92)
#define FRAM_ADDR_FCT_RECV	    (FRAM_ADDR_SYS_PARA_FACTORY_START + 96)


//用户数据
#define FRAM_ADDR_SYS_PARA_USER_START	(FRAM_MAIN_PARAM_START_ADDRESS + 48)	//用户参数开始位置
#define FRAM_ADDR_SYS_PARA_USER_FLAG	(FRAM_ADDR_SYS_PARA_USER_START + 0)	//用户参数初始化flag
#define FRAM_ADDR_SYS_PARA_INFU_START	(FRAM_ADDR_SYS_PARA_USER_START + 4)	//96个输注量起始地址,下一个地址是388
//#define FRAM_ADDR_SYS_PARA_DAY_TOTAL	(FRAM_ADDR_SYS_PARA_USER_START + 388)	//日总量最大值
//#define FRAM_ADDR_SYS_PARA_DELI_STATUS	(FRAM_ADDR_SYS_PARA_USER_START + 388)		//输注状态  - 不用
#define FRAM_ADDR_SYS_PARA_MCU_RES_FLAG	(FRAM_ADDR_SYS_PARA_USER_START + 392)	//MCU异常重启标识位
//#define FRAM_ADDR_SYS_PARA_MOTOR_STATUS	(FRAM_ADDR_SYS_PARA_USER_START + 396)	//电机堵塞状态 - 不用
#define FRAM_ADDR_SYS_PARA_PRESET_DOSE	(FRAM_ADDR_SYS_PARA_USER_START + 400)	// user preset dose
#define FRAM_ADDR_SYS_PARA_DELI_START_MOTOR_POS	(FRAM_ADDR_SYS_PARA_USER_START + 404)	//本次输注开始位置
#define FRAM_ADDR_SYS_PARA_DELI_IS_RUNING	(FRAM_ADDR_SYS_PARA_USER_START + 408)	//是否在输注
#define FRAM_ADDR_SYS_PARA_DELI_SWITCH	(FRAM_ADDR_SYS_PARA_USER_START + 412)	//输注开关量
#define FRAM_ADDR_SYS_PARA_THICKNESS	(FRAM_ADDR_SYS_PARA_USER_START + 416)	//浓度
#define FRAM_ADDR_SYS_PARA_MAX_INF_DOSE	(FRAM_ADDR_SYS_PARA_USER_START + 420)	//单次最大输注量
#define FRAM_ADDR_SYS_PARA_MUTE_STATE	(FRAM_ADDR_SYS_PARA_USER_START + 424)	//静音状态位
#define FRAM_ADDR_SYS_PARA_OCCLUSION_ALARM_FLAG (FRAM_ADDR_SYS_PARA_USER_START + 428)
#define FRAM_ADDR_SYS_PARA_RUN_CYCLE_COUNT (FRAM_ADDR_SYS_PARA_USER_START + 432) //运行来回次数,测试用
#define FRAM_ADDR_SYS_PARA_USER_DELI_POWER	(FRAM_ADDR_SYS_PARA_USER_START + 436)	//用户设置输注力, 10~15

#define FRAM_ADDR_SYS_PARA_USER_SIZE    440 //用户参数大小

//电机位置, 需要24个字节
#define FRAM_ADDR_MOTOR_POS_CNT_START	(FRAM_MAIN_PARAM_START_ADDRESS + 488)	//电机位置计数器存储首地址
#define FRAM_ADDR_MOTOR_POS_CNT_SIZE    24


//参数: 2K
#define FRAM_ADDR_SYS_PARA_REC_P_DELI	(FRAM_START_ADDRESS)	//输注记录索引号
#define FRAM_ADDR_SYS_PARA_REC_P_DAY	(FRAM_START_ADDRESS + 4)	//日总量记录索引号
#define FRAM_ADDR_SYS_PARA_REC_P_UNDELI	(FRAM_START_ADDRESS + 8)	//停输记录索引号
#define FRAM_ADDR_SYS_PARA_REC_P_OPER	(FRAM_START_ADDRESS + 12 )	//操作记录索引号
#define FRAM_ADDR_SYS_PARA_DER_COUNTER	(FRAM_START_ADDRESS + 16)	//输注记录计数器
#define FRAM_ADDR_SYS_PARA_DTR_COUNTER	(FRAM_START_ADDRESS + 20)	//日总量记录计数器
#define FRAM_ADDR_SYS_PARA_SDR_COUNTER	(FRAM_START_ADDRESS + 24)	//停输记录计数器
#define FRAM_ADDR_SYS_PARA_OPR_COUNTER	(FRAM_START_ADDRESS + 28)	//操作记录计数器

//输注记录:4K
#define FRAM_ADDR_INFUSION_RECORD_START	(FRAM_START_ADDRESS+4*1024)		//输注记录存储区首地址
//日总量记录:4K
#define FRAM_ADDR_DAYTOTAL_RECORD_START	(FRAM_ADDR_INFUSION_RECORD_START+4*1024)		//日总量记录存储区首地址
//停输记录:4K
#define FRAM_ADDR_STOPINFU_RECORD_START	(FRAM_ADDR_DAYTOTAL_RECORD_START+4*1024)		//停输记录存储区首地址
//操作记录:20K
#define FRAM_ADDR_OPERATE_RECORD_START	(FRAM_ADDR_STOPINFU_RECORD_START+4*1024)		//操作记录存储区首地址

//define the data length of each kind of history record
#define LENGTH_OF_INFUSION_RECORD	16
#define LENGTH_OF_DAYTOTAL_RECORD	16
#define LENGTH_OF_STOPINFU_RECORD	16
#define LENGTH_OF_OPERATE_RECORD	16

#define TOTAL_DELIVERY_TIME_POINT_NUMBER	96
#define USER_HISTORY_RECORD	128
#define OPER_HISTORY_RECORD	1000


#define MOTOR_POS_COUNTER_MAX	0xA0000

//default value
#define DEFAULT_USER_SET_DELI_POWER 10
#define MIN_USER_SET_DELI_POWER 10
#define MAX_USER_SET_DELI_POWER 15

//工厂模式数据（永久保存）
extern T_U32 g_motor_cali_circle ;			//电机校准encoder数,从工装到光耦的距离，单位：encoder
extern T_S16 g_cali_hal_to_end_len;			//hall到底的长度 ,unit: 0.001mm
extern T_U16 g_occl_cal_min;				//阻塞校准下限(输注力10), unit: 0.01Hz
extern T_U16 g_occl_speed_cal_min;				//阻塞校准下限(输注力10), unit: encoder速度
extern T_U16 g_occl_cal_max;				//阻塞校准上限(输注力15), unit: 0.01Hz
extern T_U16 g_occl_speed_cal_max;				//阻塞校准上限(输注力15), unit: encoder速度
extern T_S16 g_battery_voltage_offset; //电池电压偏移量
extern T_U32 g_hardware_ver;				//硬件版本号
extern T_U32 g_serial_number;				//产品序列号
extern T_U16 g_password_common;


//用户模式数据（永久保存）
extern TS_DELI_PARA g_sys_para_infu_info[TOTAL_DELIVERY_TIME_POINT_NUMBER]; //unit: 0.1ug
extern TE_BOOLEAN g_deliver_switch ;	//输注开关量
extern TE_DOSE_THICKNESS g_sys_para_thickness;		//浓度, unit: ug/ml
extern T_U16 g_sys_para_max_inf_dose;	//单次最大剂量, unit: 1ug
extern T_U8  g_sys_para_user_set_deli_power; //用户设置输注力, 10~15
//extern T_U16 g_sys_para_actual_day_total;	//实际输注的日总量,  unit:0.1ug
extern TE_BOOLEAN g_is_mute_mode;	//静音模式状态标识
extern T_U16 g_preset_dose_value;


extern TS_RECORD_POINT g_record_point; //日志索引
extern TS_RECORD_COUNTER g_record_counter; //日志数量
extern T_U32 g_motor_position ;				//电机当前位置,单位是encoder数

extern T_U32 g_deliver_start_motor_pos;		//输注开始时的马达位置,单位是encoder数
extern T_U16 g_deliver_is_running;	//是否在输注中
extern TE_BOOLEAN g_mcu_abnormal_reset_state;	//MCU异常重启标识位
extern TE_BOOLEAN g_occlusion_alarm_is_set;	  //occlusion_alarm status



//临时数据（断电不保存）
extern TE_BATTERY_LEVEL g_last_batt_level;//剩余电量
extern T_SINT g_batt_number;			//剩余电量, unit: 0.001v

extern T_U16 g_alarm_status;			//报警状态位信息
extern T_U16 g_remind_alarm_status;			//提示状态位信息
//extern T_U16 g_software_ver;				//软件版本号

extern T_U16 g_deliver_dose_value ;		//需输注的剂量, unit:0.1ug
extern T_U16 g_deliver_dose_actual;	//实际输注的剂量, unit:0.1ug

extern T_U16 g_password_private;
// record code
typedef enum
{
    //cause of infusion stop code
    E_CAUSE_OF_STOP_OCCLUSION=16,
    E_CAUSE_OF_STOP_SWITCH_OFF=17,
    E_CAUSE_OF_STOP_NO_DOSE_LEFT=18,
    E_CAUSE_OF_STOP_NO_BATTERY_LEFT=19,
    E_CAUSE_OF_STOP_EXCEED_DAYTOTAL=20,

    //operation record code
    E_OPERATION_USER_SET_DELI_POINT=21,
    E_OPERATION_USER_SET_MANUAL_DOSE=22,
    E_OPERATION_USER_SET_AUTO_DOSE=23,
    E_OPERATION_USER_SET_THICKNESS=31,
    E_OPERATION_USER_SET_ONCE_MAX_DOSE=32,
    E_OPERATION_USER_SET_DELI_SWITCH=33,
    E_OPERATION_USER_SET_DELI_POWER=39,
    E_OPERATION_USER_SET_YEAR=61,
    E_OPERATION_USER_SET_MONTH=62,
    E_OPERATION_USER_SET_DAY=63,
    E_OPERATION_USER_SET_HOUR=64,
    E_OPERATION_USER_SET_MINUTE=65,
    E_OPERATION_USER_SET_AUTO_LOCATION=66,
    E_OPERATION_USER_SET_MANUAL_LOCATION=67,
    E_OPERATION_SYSTEM_POWER_UP=68,
    E_OPERATION_USER_CHANGE_BATTERY=69,
    E_OPERATION_RESERVED=70,
    E_OPERATION_FACTORY_MODE_SWITCH_CALI=71,
    E_OPERATION_FACTORY_MODE_OCCLUSION_CALI=72, 
    E_OPERATION_FACTORY_MODE_AGING_SET=73,
    E_OPERATION_FACTORY_MODE_RESET_MACHINE=74,
    E_OPERATION_USER_SET_MUTE=75,
    E_OPERATION_FACTORY_MODE_MAX_PRESSURE=76,
    E_OPERATION_FACTORY_MODE_MIN_PRESSURE=77,
    E_OPERATION_MATER_RESTART=81,
    E_OPERATION_SLAVE_RESTART=82,
    E_OPERATION_ALERT_LOW_DOSE=83,
    E_OPERATION_ALERT_EXCEED_DAYTOTAL=84,
    E_OPERATION_ALARM_LOW_BATTERY=85,
    E_OPERATION_ALARM_LOW_DOSE=86,
    E_OPERATION_ALARM_BATTERY_EMPTY=87,
    E_OPERATION_ALARM_DOSE_EMPTY=88,
    E_OPERATION_ALARM_OCCLUSION=89,
    E_OPERATION_ALARM_DELI_SWITCH_OFF=90,
    E_OPERATION_ALARM_SYSTEM_FAULT=91,
    E_OPERATION_ALARM_SYSTEM_ABNORMAL_RESTART=92,

    E_OPERATION_ERROR_COUNT=100,
    E_OPERATION_PARA=101,

    E_MAX_RECORD_CODE
}TE_RECORD_CODE;

TE_BOOLEAN WriteToFram(T_U32 fram_address,T_U32 * p_buf,T_U16 buf_len);
TE_BOOLEAN ReadFromFram(T_U32 fram_address,T_U32 * p_buf,T_U16 buf_len);
TE_BOOLEAN SaveSysPara(TE_SYS_PARA type, T_U16 para);
TE_BOOLEAN SaveSysParaEx(TE_SYS_PARA type, T_U16 para, T_U32 sys_value);
TE_BOOLEAN LoadSysPara(TE_SYS_PARA type, T_U16 para);
T_U32 LoadSysParaEx(TE_SYS_PARA type, T_U16 para);
TE_BOOLEAN DataWriteWithCheck(T_U32 fram_address,void * p_buf,T_U16 buf_len);
TE_BOOLEAN DataReadWithCheck(T_U32 fram_address,void * p_buf,T_U16 buf_len);
//T_U32* SortTheRecordData(TS_RECORD rec_data);
//void RestoreTheRecordData(TS_RECORD *rec_data, T_U32* data_buf, T_U16 data_len);
TE_BOOLEAN SaveRec(TE_RECORD_TYPE type, T_U32 para, T_U32 data);
TE_BOOLEAN SaveRecEx(TS_RECORD *rec_data);
TE_BOOLEAN LoadRec(TS_RECORD *rec_data);
//the first is the oldest one
//return -1 is indicate the end
T_SINT RecFirstIndex(TE_RECORD_TYPE type);
T_SINT RecLastIndex(TE_RECORD_TYPE type);
//the newer one
T_SINT RecNextIndex(TE_RECORD_TYPE type, T_SINT curr_index);
//the older one
T_SINT RecPrevIndex(TE_RECORD_TYPE type, T_SINT curr_index);


void SaveAllDeliParaIntoOperRec(void);
//int and read the motor pos
void InitMotorPos();
//save the motor pos
void SaveMotorPos(TE_BOOLEAN is_flash_cache);


T_U16 GetThicknessValue(TE_DOSE_THICKNESS thickness);

//convert dose to encoder counter, dose unit:0.1ug
T_U32 DoseToEncoder(TE_DOSE_THICKNESS thickness, T_U16 dose);

//convert encoder to dose, return unit:0.1ug
T_U16 EncoderToDose(TE_DOSE_THICKNESS thickness, T_U32 encoder);

//convert volume to dose, volume unit: 0.001ml=1mm^3, return unit:0.1ug
T_U16 VolumeToDose(TE_DOSE_THICKNESS thickness, T_U16 volume);

//convert length to encode, length unit:0.001mm
T_U32 LengthToEncoder(T_U16 length);

//convert encode to length , length unit:0.001mm
T_U16 EncoderToLength(T_U32 encoder);


//convert volume to encoder counter, volume unit:0.001ml=1mm^3
T_U32 VolumeToEncoder(T_U16 volume);

//convert encoder to volume, volume unit:0.001ml=1mm^3
T_U16 EncoderToVolume(T_U32 encoder);


//the remaining dose,dose unit:0.1ug
T_U16 RemainDose();

//the max motor pos, the unit is encoder
T_U32 MaxMotorPos();

//获取阻塞报警Hz阈值
T_U16 GetOcclusionHzThreshold();
//获取阻塞报警速度阈值
T_U16 GetOcclusionSpeedThreshold();
#endif /* CTLDATA_H */

