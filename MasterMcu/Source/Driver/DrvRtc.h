/*******************************************************************************
**�ļ���:DrvRtc.h
**
**����:������"rtc.c"�ļ���ʹ�õı������ꡢ�����
**
**����:HJH
**
**����:2016-03-01
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
#ifndef DRV_RTC_
#define DRV_RTC_

#include <msp430fr5989.h>
#include "..\TypeDefine.h"


//���Ŷ���
#define DS_SCL  BIT1 
#define DS_SDA  BIT2  
#define DS_RST  BIT3  
#define DS_RST_IN P5DIR &= ~DS_RST
#define DS_RST_OUT P5DIR |= DS_RST
#define DS_RST0 P5OUT &= ~DS_RST
#define DS_RST1 P5OUT |= DS_RST
#define DS_SCL_IN P5DIR &= ~DS_SCL
#define DS_SCL_OUT P5DIR |= DS_SCL
#define DS_SCL0 P5OUT &= ~DS_SCL
#define DS_SCL1 P5OUT |= DS_SCL
#define DS_SDA_IN P5DIR &= ~DS_SDA
#define DS_SDA_OUT P5DIR |= DS_SDA
#define DS_SDA0 P5OUT &= ~DS_SDA
#define DS_SDA1 P5OUT |= DS_SDA
#define DS_SDA_BIT P5IN & DS_SDA    
       

//DS1302��ַ����
#define DS1302_SEC_ADD			0x81		//�����ݵ�ַ
#define DS1302_MIN_ADD			0x83		//�����ݵ�ַ
#define DS1302_HOUR_ADD			0x85		//ʱ���ݵ�ַ
#define DS1302_DATE_ADD			0x87		//�����ݵ�ַ
#define DS1302_MONTH_ADD		0x89		//�����ݵ�ַ
#define DS1302_DAY_ADD			0x8b		//�������ݵ�ַ
#define DS1302_YEAR_ADD			0x8d		//�����ݵ�ַ
#define DS1302_CONTROL_ADD		0x8e		//�������ݵ�ַ
#define DS1302_CHARGER_ADD		0x90 					 
#define DS1302_CLKBURST_ADD		0xbe

typedef enum//����ʱ�����ͣ��ꡢ�¡��ա�ʱ���֡���
{
  E_TIME_YEAR = 0,
  E_TIME_MONTH,
  E_TIME_DATE,
  E_TIME_HOUR,
  E_TIME_MINUTE,
  E_TIME_SECOND,
  E_TIME_DAY,
  E_TIME_MAX,
} TE_TIME_STYLE;

typedef enum//��������ת��ʱ�Ĳ������ͣ������ж�ת������дת��
{
  E_OPERATION_READ = 0,
  E_OPERATION_WRITE,
  E_OPERATION_MAX,
} TE_OPERATION_STYLE;

extern T_U8 g_write_time_buf[16];
extern T_U8 g_read_time_buf[8];
extern TS_TIME_INFO g_current_time;


void Ds1302Init(void);
void Write1Byte(T_U8 wdata);
T_U8 Read1Byte();
void WData(T_U8 addr, T_U8 wdata);
T_U8 RData(T_U8 addr);
void BurstWrite1302(T_U8 *ptr);
void BurstRead1302(T_U8 *ptr);
void BurstWriteRAM(T_U8 *ptr);
void BurstReadRAM(T_U8 *ptr);
void IsSetDS1302(TE_TIME_STYLE type,T_U32 data);
void SetDS1302(T_U8 *ptr);
void GetDS1302(void); 

void Ds1302TimeRead(void);


#endif

