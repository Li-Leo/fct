/*******************************************************************************
**�ļ���:CtlSpiComm.h  (Slave MCU)
**
**����:�ں˲�-����оƬͨ��ģ�顣����"CtlSpiComm.c�ļ���ʹ�õĺꡢö�ٱ�����"
**
**����:Kevin Shi
**
**����:2016-04-28
**
*******************************************************************************/
#ifndef	DRVSPICOMM_H
#define	DRVSPICOMM_H

#include <msp430fr5989.h>
#include "TypeDefine.h"
#include "DrvSpi.h"
#include "DrvFram.h"
#include "DrvMotor.h"
#include "CtlData.h"
#include "CtlDelivery.h"
#include "CtlAdc.h"


/* Head master receive from slave */
//	#define HEAD_SP 0xa501
//	#define HEAD_ST 0xa502
#define HEAD_SC 0xa503
#define HEAD_SQ 0xa504

/* Head slave feedback to master */
//	#define HEAD_FP 0x4650
//	#define HEAD_FT 0x4654
#define HEAD_FC 0x4643
#define HEAD_FQ 0x4651


/***  Send Parameter  ****/
//	#define COMM_MD 0x4D44
//	#define COMM_DD 0x4444
//	#define COMM_DT 0x4454
//	#define COMM_SM 0x534D
//	#define COMM_SC 0x5343
//	#define COMM_OC 0x4F43
//	#define COMM_DS 0x4453
//	#define COMM_DA 0x4441
//	#define COMM_BA 0x4241
//	#define COMM_DP 0x4450
//	#define COMM_SI 0x5349
//	#define COMM_CM 0x434D
//	#define COMM_CR 0x4352
//	#define COMM_CC 0x4343
//	
//	#define PARA_CM_MAIN_HOLD	0x05
//	#define PARA_CM_MAIN_PRESS	0x02
//	#define PARA_CM_PLUS_PRESS	0x03
//	#define PARA_CM_MINUS_PRESS	0x04
//	
//	#define OC_MAIN_PRESS	    0x01
//	#define OC_PLUS_PRESS	    0x02
//	#define OC_MINUS_PRESS	    0x03
//	#define AL_GET_CURR_FREQ    0x04

/***  Send time  ****/
//	#define COMM_YR 0x5952
//	#define COMM_MH 0x4D48
//	#define COMM_DY 0x4459
//	#define COMM_HR 0x4852
//	#define COMM_ME 0x4D45
//	#define COMM_SD 0x5344

/*** Send Command ***/
#define COMM_MR 0x4D52 //motor run
#define COMM_MS 0x4D53 //motor stop
//#define COMM_AL 0x414C  
//#define COMM_TC	0x5443
#define COMM_TH 0x5448  //to Hall
#define COMM_WD 0x5744  //watchdog

//#define COMM_LD 0x4C44
#define COMM_SS 0x5353 //sleep
//#define COMM_SW 0x5357
//#define COMM_DS 0x4453
#define COMM_PU 0x5055  //power up


/*** Send Query   ***/
#define COMM_AD 0x4144  //AD
#define COMM_MC 0x4D43 //motor current

//	#define COMM_AS 0x4153
//	#define COMM_AF 0x4146
//	#define COMM_MP 0x4D50
//	#define COMM_FM 0x464D
//	#define COMM_RS 0x5253
//	
//	#define DATA_RM 0x524D
//	#define DATA_SS 0x5353
//	#define DATA_FM 0x464D
//	#define DATA_RS 0x5253
//	
//	#define DATA_OK 0x4F4B
//	
//	/* List all kinds of SPI head info here. */
//	typedef enum
//	{
//		E_SPI_HEAD_TYPE_SEND_PARA,		//"SP"
//		E_SPI_HEAD_TYPE_SEND_TIME,		//"ST"
//		E_SPI_HEAD_TYPE_SEND_CONTROL,	//"SC"
//		E_SPI_HEAD_TYPE_SEND_QUERY,		//"SQ"
//		E_SPI_HEAD_TYPE_MAX,
//	} TE_SPI_HEAD_TYPE;
//	
//	/* List all kinds of segment in "SP" head type. */
//	typedef enum
//	{
//		E_SPI_HEAD_SP_SEGMENT_MOTOR_DECTION,	//"MD"
//		E_SPI_HEAD_SP_SEGMENT_DELIVERING_DOSE,	//"DD"
//		E_SPI_HEAD_SP_SEGMENT_THICKNESS,		//"TH"
//		E_SPI_HEAD_SP_SEGMENT_DAY_TOTAL_DOSE,	//"DT"
//		E_SPI_HEAD_SP_SEGMENT_SINGLE_MAX_DOSE,	//"SM"
//		E_SPI_HEAD_SP_SEGMENT_STOP_DELI_CODE,	//"IC"
//		E_SPI_HEAD_SP_SEGMENT_OPERATION_CODE,	//"OC"
//		E_SPI_HEAD_SP_SEGMENT_DELIVERY_STATUS,	//"DS"
//		E_SPI_HEAD_SP_SEGMENT_MACHINE_STATUS,	//"MS"
//		E_SPI_HEAD_SP_SEGMENT_DOSE_MIN_ALARM,	//"DA"
//		E_SPI_HEAD_SP_SEGMENT_BATT_MIN_ALARM,	//"BA"
//		E_SPI_HEAD_SP_SEGMENT_DELIVERY_POWER,	//"DP"
//		E_SPI_HEAD_SP_SEGMENT_SAVE_REC_INDEX,	//"SI"
//		E_SPI_HEAD_SP_SEGMENT_CALI_MILLIMETER,	//"CM"
//		E_SPI_HEAD_SP_SEGMENT_CALI_CIRCLE,		//"CC"
//		E_SPI_HEAD_SP_SEGMENT_MAX,
//	} TE_SPI_HEAD_SS_SEGMENT;
//	
//	/* List all kinds of segment in "ST" head type. */
//	typedef enum
//	{
//		E_SPI_HEAD_ST_SEGMENT_YEAR,				//"YR"
//		E_SPI_HEAD_ST_SEGMENT_MONTH,			//"MH"
//		E_SPI_HEAD_ST_SEGMENT_DAY,				//"DY"
//		E_SPI_HEAD_ST_SEGMENT_HOUR,				//"HR"
//		E_SPI_HEAD_ST_SEGMENT_MINUTE,			//"ME"
//		E_SPI_HEAD_ST_SEGMENT_SECOND,			//"SD"
//		E_SPI_HEAD_ST_SEGMENT_MAX,
//	} TE_SPI_HEAD_ST_SEGMENT;
//	
//	/* List all kinds of segment in "SC" head type. */
//	typedef enum
//	{
//		E_SPI_HEAD_SC_SEGMENT_MOTOR_RUN,	//"MD"
//		E_SPI_HEAD_SC_SEGMENT_MOTOR_STOP,	//"MS"
//		E_SPI_HEAD_SC_SEGMENT_SAVE_DATA,	//"SD"
//		E_SPI_HEAD_SC_SEGMENT_LOAD_DATA,	//"LD"
//		E_SPI_HEAD_SC_SEGMENT_SET_SLEEP,	//"SS"
//		E_SPI_HEAD_SC_SEGMENT_SET_WAKEUP,	//"SW"
//		E_SPI_HEAD_SC_SEGMENT_SELF_CALI,	//"SE"
//		E_SPI_HEAD_SC_SEGMENT_SET_INIT,		//"SI"
//		E_SPI_HEAD_SC_SEGMENT_PULSE_SIGNAL,	//"PS"
//		E_SPI_HEAD_SC_SEGMENT_AD_SAMPLING,	//"AD"
//		E_SPI_HEAD_SC_SEGMENT_MAX,
//	} TE_SPI_HEAD_SC_SEGMENT;
//	
//	/* List all kinds of segment in "SQ" head type. */
//	typedef enum
//	{
//		E_SPI_HEAD_SQ_SEGMENT_DELIVERY_STATUS,	//"DS"
//		E_SPI_HEAD_SQ_SEGMENT_PULSE_SIGNAL,		//"PS"
//		E_SPI_HEAD_SQ_SEGMENT_AD_SAMPLING,		//"AD"
//		E_SPI_HEAD_SQ_SEGMENT_ALARM_STATUS,		//"AS"
//		E_SPI_HEAD_SQ_SEGMENT_ABNORMAL_FLAG,	//"AF"
//		E_SPI_HEAD_SQ_SEGMENT_MOTOR_POSITION,	//"MP"
//		E_SPI_HEAD_SQ_SEGMENT_MAX,
//	} TE_SPI_HEAD_SQ_SEGMENT;
//	
//	/* List all kinds of Save/Load data type. */
//	typedef enum
//	{
//		E_SPI_SAVE_LOAD_TYPE_DELIVERING_DOSE,	//"DD"
//		E_SPI_SAVE_LOAD_TYPE_THICKNESS,			//"TH"
//		E_SPI_SAVE_LOAD_TYPE_DAY_TOTAL_DOSE,	//"DT"
//		E_SPI_SAVE_LOAD_TYPE_SINGLE_MAX_DOSE,	//"SM"
//		E_SPI_SAVE_LOAD_TYPE_TIME,				//"TI"
//		E_SPI_SAVE_LOAD_TYPE_DELIVERY_RECORD,	//"DR"
//		E_SPI_SAVE_LOAD_TYPE_DAY_TOTAL_RECORD,	//"TR"
//		E_SPI_SAVE_LOAD_TYPE_STOP_DELI_RECORD,	//"SR"
//		E_SPI_SAVE_LOAD_TYPE_OPERATION_RECORD,	//"OR"
//		E_SPI_SAVE_LOAD_TYPE_RECORD_POINTER,	//"RP"
//		E_SPI_SAVE_LOAD_TYPE_DELIVERY_STATUS,	//"DS"
//		E_SPI_SAVE_LOAD_TYPE_MACHINE_STATUS,	//"MS"
//		E_SPI_SAVE_LOAD_TYPE_DOSE_NUMBER,		//"DN"
//		E_SPI_SAVE_LOAD_TYPE_BATT_NUMBER,		//"BN"
//		E_SPI_SAVE_LOAD_TYPE_DELIVERY_POWER,	//"DP"
//		E_SPI_SAVE_LOAD_TYPE_CALI_MILLIMETER,	//"CM"
//		E_SPI_SAVE_LOAD_TYPE_CALI_CIRCLE,		//"CC"
//		E_SPI_SAVE_LOAD_TYPE_MAX,
//	} TE_SPI_SAVE_LOAD_TYPE;

/* Define the structure of SPI data packet here. */
typedef struct
{
	T_U8	sop1;
    T_U8    sop2;
	T_U16	command;
	T_U16	data;
    T_U16	data2;
	T_U16	crc_code;
} TS_SPI_COMMAND;

typedef struct
{
  TS_SPI_COMMAND spi_command;
  TE_BOOLEAN flag;
} TS_SPI_READBACK;


void SpiCommInit(void);
TS_SPI_COMMAND ReceiveSpiCommandFromMaster(T_U8 *one_packet_buff);

void AnalysisCommmandFromMaster(TS_SPI_COMMAND spi_rx_command);
void SetSpiCommandToMaster(T_U16 head, T_U16 command, T_U16 data, T_U16 data2);

T_U16 crc16( T_U8* data_p, T_U8 length);
#endif /* DRVSPICOMM_H */
