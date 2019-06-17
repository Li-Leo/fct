/*********************************************************************************
 * $Header: TypeDefine.h
 * Descripton:
 *		This file is the common used head file of the whole project.
 *		It defines the data type, data structure and enum type variables.
 * Original Author: Kevin Shi
 ********************************************************************************/
#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

typedef signed char		T_S8;
typedef unsigned char	T_U8;
typedef signed short	T_S16;
typedef unsigned short	T_U16;
typedef signed long		T_S32;
typedef unsigned long	T_U32;
typedef signed long		T_SWord;
typedef unsigned long	T_UWord;
typedef signed long long int	T_S64;
typedef unsigned long long int	T_U64;
typedef signed int T_SINT;
typedef unsigned int T_UINT;
typedef char T_CHAR;


#define MAX_T_S8    0x7F
#define MIN_T_S8    (-128)
#define MAX_T_S16   0x7FFF
#define MIN_T_S16   (-32768)
#define MAX_T_S32   0x7FFFFFFF
#define MIN_T_S32   (-2147483647L - 1)
#define MAX_T_S64   0x7FFFFFFFFFFFFFFF
#define MIN_T_S64   (-9223372036854775807LL - 1)

#define MAX_T_U8    0xFF
#define MIN_T_U8    0
#define MAX_T_U16   0xFFFF
#define MIN_T_U16   0
#define MAX_T_U32   0xFFFFFFFF
#define MIN_T_U32   0
#define MAX_T_U64   0xFFFFFFFFFFFFFFFF
#define MIN_T_U64   0

/* Define the stop-infusion-codes here. */
#define STOP_INFU_CODE_OCCL	16	//阻塞引起的停输，停输代码16
#define STOP_INFU_CODE_MOFF	17	//输注开关为"关"引起的停输，停输代码17
#define STOP_INFU_CODE_DOSE	18	//药尽引起的停输，停输代码18

/* Define the boolean type here. */
typedef enum
{
	E_FALSE,
	E_TRUE, 
}TE_BOOLEAN;

/* Define the structure of real-time type here. */
typedef struct
{
	T_U32	year;
	T_U32	month;
    T_U32	date;
	T_U32	day;
	T_U32	hour; 
	T_U32	minute;
	T_U32	second;
} TS_TIME_INFO;

/* Define the type of history records here. */
typedef enum
{
	E_RECORD_INFUSION,		//输注记录
	E_RECORD_DAYS_TOTAL,	//日总量记录
	E_RECORD_STOP_DELI,		//停输记录
	E_RECORD_OPERATION,		//操作记录
	E_RECORD_MAX,
} TE_RECORD_TYPE;

/* Define the structure of records type here. */
typedef struct
{
	TE_RECORD_TYPE	rec_type;	//历史记录类型
	T_U8	rec_id;				//记录id号
	TS_TIME_INFO	rec_time;	//记录时间
	T_U32	rec_para;			//记录参数
	T_U32	rec_data;			//记录数据
} TS_RECORD;

/* Define the structure of records' point here. */
typedef struct
{
	T_U16	p_infusion_rec;
	T_U16	p_daytotal_rec;
	T_U16	p_stopinfu_rec;
	T_U16	p_operation_rec;
} TS_RECORD_POINT;

/* Define the structure of records' counter here. */
typedef struct
{
	T_U16	de_rec_counter;
	T_U16	dt_rec_counter;
	T_U16	sd_rec_counter;
	T_U16	op_rec_counter;
} TS_RECORD_COUNTER;

/* Define the structure of infusion infomation type here. */
typedef struct
{
	T_U16	hour;
	T_U16	minute;
	T_U16	dose;
} TS_INFUSION_INFO;

/* Define the type of systerm parameters here. */
typedef enum
{
	E_SYSTEM_PARA_INFU_INFO,
	E_SYSTEM_PARA_THICKNESS,
	E_SYSTEM_PARA_MAX_INFU_DOSE,
	E_SYSTEM_PARA_DAY_TOTAL,
	E_SYSTEM_PARA_PASSWORD_PRIVATE,
	E_SYSTEM_PARA_PASSWORD_COMMON,
	E_SYSTEM_PARA_OCCLUSION_STATE,
	E_SYSTEM_PARA_DELI_STATE,
	E_SYSTEM_PARA_MOTOR_POSITION,
	E_SYSTEM_PARA_HALL_CAL_DATA,
	E_SYSTEM_PARA_DELI_CAL_RATE,
	E_SYSTEM_PARA_DELI_SWITCH_STATE,
	E_SYSTEM_PARA_DELI_POWER,
	E_SYSTEM_PARA_DOSE_ALARM_LIMIT,
	E_SYSTEM_PARA_BATT_ALARM_LIMIT,
	E_SYSTEM_PARA_RECORD_INDEX,
	E_SYSTEM_PARA_RECORD_COUNTER,
	E_SYSTEM_PARA_ALARM_FLAG,
	E_SYSTEM_PARA_SIN_RST_FLAG,
	E_SYSTEM_PARA_EXPECT_DELI_DOSE,
	E_SYSTEM_PARA_ACTUAL_DELI_DOSE,
	E_SYSTEM_PARA_SOFTWARE_VERSION,
	E_SYSTEM_PARA_DOSE_LEFT,
	E_SYSTEM_PARA_MOTOR_POS_POINT,
	E_SYSTEM_PARA_MOTOR_POS_COUNTER,
	E_SYSTEM_PARA_MAX,
} TE_SYS_PARA;

/* Define the structure of delivery parameters here. */
typedef struct
{
	T_U32	index;	//from 0 to 95. 96 time points.
	T_U32	dose;	//unit: ug
} TS_DELI_PARA;

/* Define the structure of AD datas here. */
typedef struct
{
	T_U16	batt_vol_channal_a12;
	T_U16	batt_perf_vol_channal_a13;
	T_U16	s_vol_cal_channal_a14;
	T_U16	motor_vol_channal_a15;
} TS_ADC_INFO;

#endif	/* TYPEDEFINE_H */

