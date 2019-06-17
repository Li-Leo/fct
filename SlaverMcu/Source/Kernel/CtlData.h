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

//define the address that all kinds of data should saved into.
#define FRAM_ADDR_SYS_PARA_DELI_STATUS	FRAM_START_ADDRESS			//��ע״̬
#define FRAM_ADDR_SYS_PARA_MCU_RES_FLAG	(FRAM_START_ADDRESS + 4)	//MCU�쳣������ʶλ
#define FRAM_ADDR_SYS_PARA_MOTOR_STATUS	(FRAM_START_ADDRESS + 8)	//�������״̬
#define FRAM_ADDR_MOTOR_POS_REC_POINT	(FRAM_START_ADDRESS + 12)	//���λ�ô洢��ַָ��
#define FRAM_ADDR_SYS_PARA_MOTOR_CAL	(FRAM_START_ADDRESS + 16)	//���У׼����
#define FRAM_ADDR_SYS_PARA_DELI_CAL_RAT	(FRAM_START_ADDRESS + 20)	//��עУ׼����
#define FRAM_ADDR_SYS_PARA_DELI_SWITCH	(FRAM_START_ADDRESS + 24)	//��ע������
#define FRAM_ADDR_SYS_PARA_DELI_POWER	(FRAM_START_ADDRESS + 28)	//��ע��
#define FRAM_ADDR_SYS_PARA_DOSE_ALARM	(FRAM_START_ADDRESS + 32)	//��С��������ֵ
#define FRAM_ADDR_SYS_PARA_BATT_ALARM	(FRAM_START_ADDRESS + 36)	//��С��������ֵ
#define FRAM_ADDR_SYS_PARA_THICKNESS	(FRAM_START_ADDRESS + 40)	//Ũ��
#define FRAM_ADDR_SYS_PARA_MAX_INF_DOSE	(FRAM_START_ADDRESS + 44)	//���������ע��
#define FRAM_ADDR_SYS_PARA_INFU_START	(FRAM_START_ADDRESS + 48)	//96����ע����ʼ��ַ
#define FRAM_ADDR_SYS_PARA_DAY_TOTAL	(FRAM_START_ADDRESS + 432)	//���������ֵ
#define FRAM_ADDR_SYS_PARA_REC_P_DELI	(FRAM_START_ADDRESS + 436)	//��ע��¼������
#define FRAM_ADDR_SYS_PARA_REC_P_DAY	(FRAM_START_ADDRESS + 440)	//��������¼������
#define FRAM_ADDR_SYS_PARA_REC_P_UNDELI	(FRAM_START_ADDRESS + 444)	//ͣ���¼������
#define FRAM_ADDR_SYS_PARA_REC_P_OPER	(FRAM_START_ADDRESS + 448)	//������¼������
#define FRAM_ADDR_SYS_PARA_DELI_SHOULD	(FRAM_START_ADDRESS + 452)	//����Ӧ��ע����
#define FRAM_ADDR_SYS_PARA_DELI_IN_FACT	(FRAM_START_ADDRESS + 456)	//����ʵ����ע����
#define FRAM_ADDR_SYS_PARA_SW_VERSION	(FRAM_START_ADDRESS + 460)	//����汾��
#define FRAM_ADDR_SYS_PARA_HW_VERSION	(FRAM_START_ADDRESS + 464)	//Ӳ���汾��
#define FRAM_ADDR_SYS_PARA_PASSWORD_DEF	(FRAM_START_ADDRESS + 468)	//Ĭ�����룺�̶�ֵ"222"
#define FRAM_ADDR_SYS_PARA_PASSWORD_PRI	(FRAM_START_ADDRESS + 472)	//ר�����룺���кź���λ
#define FRAM_ADDR_SYS_PARA_ALARM_INFO	(FRAM_START_ADDRESS + 476)	//����״̬��ʾλ
#define FRAM_ADDR_SYS_PARA_DER_COUNTER	(FRAM_START_ADDRESS + 480)	//��ע��¼������
#define FRAM_ADDR_SYS_PARA_DTR_COUNTER	(FRAM_START_ADDRESS + 484)	//��������¼������
#define FRAM_ADDR_SYS_PARA_SDR_COUNTER	(FRAM_START_ADDRESS + 488)	//ͣ���¼������
#define FRAM_ADDR_SYS_PARA_OPR_COUNTER	(FRAM_START_ADDRESS + 492)	//������¼������
#define FRAM_ADDR_SYS_PARA_DOSE_LEFT	(FRAM_START_ADDRESS + 496)	//ʣ�����
#define FRAM_ADDR_MOTOR_POS_REC_START	(FRAM_START_ADDRESS + 500)	//���λ�ô洢�׵�ַ
#define FRAM_ADDR_MOTOR_POS_CNT_START	(FRAM_START_ADDRESS + 900)	//���λ�ü������洢�׵�ַ

#define FRAM_ADDR_INFUSION_RECORD_START	0x11240		//��ע��¼�洢���׵�ַ
#define FRAM_ADDR_DAYTOTAL_RECORD_START	0x13240		//��������¼�洢���׵�ַ
#define FRAM_ADDR_STOPINFU_RECORD_START	0x15240		//ͣ���¼�洢���׵�ַ
#define FRAM_ADDR_OPERATE_RECORD_START	0x17240		//������¼�洢���׵�ַ

//define the data length of each kind of history record
#define LENGTH_OF_INFUSION_RECORD	7
#define LENGTH_OF_DAYTOTAL_RECORD	4
#define LENGTH_OF_STOPINFU_RECORD	7
#define LENGTH_OF_OPERATE_RECORD	8

#define TOTAL_DELIVERY_TIME_POINT_NUMBER	96
#define USER_HISTORY_RECORD	128
#define OPER_HISTORY_RECORD	1000

#define MOTOR_POS_COUNTER_MAX	0xA0000
#define FRAM_ADDR_FCT_RECV      0x1800

TE_BOOLEAN WriteToFram(T_U32 fram_address,T_U32 * p_buf,T_U16 buf_len);
TE_BOOLEAN ReadFromFram(T_U32 fram_address,T_U32 * p_buf,T_U16 buf_len);
TE_BOOLEAN SaveSysPara(TE_SYS_PARA type, T_U16 para);
TE_BOOLEAN LoadSysPara(TE_SYS_PARA type, T_U16 para);
T_U32* SortTheRecordData(TS_RECORD rec_data);
void RestoreTheRecordData(TS_RECORD *rec_data, T_U32* data_buf, T_U16 data_len);
TE_BOOLEAN SaveRec(TS_RECORD rec_data);
TE_BOOLEAN LoadRec(TS_RECORD *rec_data);
void SaveAllDeliParaIntoOperRec(void);

#endif /* CTLDATA_H */

