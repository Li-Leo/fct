/*******************************************************************************
**文件名:DrvRtc.h
**
**功能:定义在"rtc.c"文件中使用的变量、宏、数组等
**
**作者:HJH
**
**日期:2016-03-01
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#ifndef DRV_RTC_
#define DRV_RTC_

#include <msp430fr5989.h>
#include "..\TypeDefine.h"


//引脚定义
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
       

//DS1302地址定义
#define DS1302_SEC_ADD			0x81		//秒数据地址
#define DS1302_MIN_ADD			0x83		//分数据地址
#define DS1302_HOUR_ADD			0x85		//时数据地址
#define DS1302_DATE_ADD			0x87		//日数据地址
#define DS1302_MONTH_ADD		0x89		//月数据地址
#define DS1302_DAY_ADD			0x8b		//星期数据地址
#define DS1302_YEAR_ADD			0x8d		//年数据地址
#define DS1302_CONTROL_ADD		0x8e		//控制数据地址
#define DS1302_CHARGER_ADD		0x90 					 
#define DS1302_CLKBURST_ADD		0xbe

typedef enum//定义时间类型，年、月、日、时、分、秒
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

typedef enum//定义数据转换时的操作类型，即进行读转换还是写转换
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

