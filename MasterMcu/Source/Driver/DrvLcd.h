/*******************************************************************************
**文件名:DrvKey.h
**
**功能:
**
**作者:HJH
**
**日期:2016-04-28
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#ifndef DRV_LCD_
#define DRV_LCD_

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

#define LCD_BACK_LIGHT_ON       (P3OUT |= BIT0)
#define LCD_BACK_LIGHT_OFF      (P3OUT &= ~BIT0)

#define LCD_RD          (P3OUT |= BIT6)// 液晶屏读信号，对应液晶读写选择引脚R/W 
#define LCD_WR          (P3OUT &= ~BIT6)//液晶屏写信号，对应液晶读写选择引脚R/W

#define LCD_RST_ENA     (P3OUT &= ~BIT7)// 液晶复位，对应液晶复位引脚RES         
#define LCD_RST_DIS     (P3OUT |= BIT7)// 禁止液晶复位，对应液晶复位引脚RES

#define LCD_DATA_S      (P2OUT |= BIT2)//LCD读写操作的是数据，对应液晶屏引脚A0
#define LCD_CMD_S       (P2OUT &= ~BIT2)//LCD读写操作的是命令，对应液晶屏引脚A0

#define LCD_E_ENA       (P2OUT &= ~ BIT3)// 液晶屏读使能信号,对应液晶读写控制引脚E
#define LCD_E_DIS       (P2OUT |= BIT3)//液晶屏写使能信号，对应液晶读写控制引脚E

#define LCD_DATA        P9OUT//液晶数据输出口

#define  PAGE_MIN       0xb0//页起始地址(最小页地址)，为首页即第0页 
#define  PAGE_MAX       0xb8//页结束地址(最大页地址)，为末页即第8页，该页只有一行，
                            //实际操作中不使用该页 

#define ROW_MIN         0x40//行起始地址(最小行地址)，为首行即第0行
#define ROW_MAX         0x7f//行结束地址(最大行地址)，为末行即第63行

#define COLUMN_MIN      0x00//列起始地址(最小列地址)，为首列即第0列
#define COLUMN_MAX      0x80//列结束地址(最大列地址)，为末列即第128列




typedef enum //LCD写入内容类型定义
{
  E_LCD_INSTRUCTION,//命令
  E_LCD_DATA,//数据
  E_LCD_MAX,
} TE_LCD_CMD;

typedef enum//背光灯状态定义
{
  E_BACK_LIGHT_ON,//背光灯开启
  E_BACK_LIGHT_OFF,//背光灯关闭
  E_BACK_LIGHT_MAX,
} TE_BACK_LIGHT;

typedef enum//定义页码编号， 
{
  E_PAGE_ONE = 0,//第0页
  E_PAGE_TWO,//第1页
  E_PAGE_THREE,//第2页
  E_PAGE_FOUR,//第3页
  E_PAGE_MAX,
} TE_PAGE_NUM;

void DrvLcdInit(void);
void DrvLcdWriteByte(TE_LCD_CMD cmd,T_U8 data);
void DrvLcdDisp(void);
void DrvLcdClr(void);
void DrvLcdSetPos(TE_PAGE_NUM page_num,T_U8 colum_add);
void DrvLcdSetBackLight(TE_BACK_LIGHT bl_state);
void DrvLcdLowPower(void);

#endif
