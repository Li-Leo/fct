/*******************************************************************************
**文件名:DrvLcd.c
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
#include "DrvLcd.h"

TE_BACK_LIGHT g_back_light_status = E_BACK_LIGHT_OFF;

/*******************************************************************************
**函数名:void DrvLcdInit(void)
**
**功能:液晶屏初始化，对液晶屏驱动电压和扫描方向等进行设置
**
**作者：HJH
**
**日期：2016-04-28
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdInit(void)
{
  T_U16 i;
  LCD_RST_ENA;//液晶屏复位
  for(i = 0;i < 10;i++)//产生一定时间延时
  {
    ;
  }
  LCD_RST_DIS;//禁止液晶屏复位
  for(i = 0;i < 5000;i++)
  {
    ;
  }
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa3);//液晶驱动电压偏压比(bias ratio)选择1/7 0xa3
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa0);//扫描方向设置：normal(0xa0),reverse(0xa1)
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa0);//垂直显示方向设置：normal(0xc0),reverse(0xc8)
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x24);//设置Regulation Ratio(V5)为5.0  adjust range:0x20-0x27
  
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x81);//设置EV参数 adjust brightness of LCD
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x15);//range: 0x00-0x3f
  
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x2f);//设置Power Control设置
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xaf);//显示开启
  DrvLcdClr();//清屏
  }

/*******************************************************************************
**函数名:void DrvLcdDisp(void)
**
**功能:点亮屏即满屏显示，点亮所有像素点
**
**作者：HJH
**
**日期：2016-04-28
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdDisp(void)
{
  T_U16 j,k,m,n;
   for(j = PAGE_MIN; j < PAGE_MAX;j++)
  {
    DrvLcdWriteByte(E_LCD_INSTRUCTION,j);
    for(k = COLUMN_MIN;k < COLUMN_MAX;k++)
    {
      m = k&0x0f;
      n = k&0xf0;
      n= (n>>4);
      n = n|0x10;
      DrvLcdWriteByte(E_LCD_INSTRUCTION,n);//设置起始列为第一列
      DrvLcdWriteByte(E_LCD_INSTRUCTION,m);//
      DrvLcdWriteByte(E_LCD_DATA,0xff);
    }
  }  
}

/*******************************************************************************
**函数名:void DrvLcdClr(void)
**
**功能:液晶屏清屏操作，把屏所有像素点关闭
**
**作者：HJH
**
**日期：2016-04-28
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdClr(void)
{
  T_U16 j,k,m,n;
   for(j = PAGE_MIN; j < PAGE_MAX;j++)
  {
    DrvLcdWriteByte(E_LCD_INSTRUCTION,j);//写入页地址
    for(k = COLUMN_MIN;k < COLUMN_MAX;k++)
    {
      m = k&0x0f;
      n = k&0xf0;
      n= (n>>4);
      n = n|0x10;
      DrvLcdWriteByte(E_LCD_INSTRUCTION,n);//写入列地址
      DrvLcdWriteByte(E_LCD_INSTRUCTION,m);
      DrvLcdWriteByte(E_LCD_DATA,0x00);
    }
  } 
}

/*******************************************************************************
**函数名:void DrvLcdSetPos(TE_PAGE_NUM page_num,T_U8 colum_add)
**
**功能:设置液晶屏显示坐标,其中pos_x表示页地址，pos_y表示列地址
**
**作者：HJH
**
**日期：2016-04-28
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdSetPos(TE_PAGE_NUM page_num,T_U8 colum_add)
{
  T_U16 k,m,n;//k：存放页地址；m：存放列地址第4位；n：存放列地址高四位
/******************************************************************************/  
  k = page_num&0x0f;//根据屏datasheet将page_num高4位清零
  k = k|0xb0;//计算出可以直接写入寄存器的页地址
/******************************************************************************/  
  m = colum_add&0x0f;//根据ST7565的datasheet求取列地址低四位
  n = colum_add&0xf0;//根据ST7565的datasheet求取列地址高四位
  n= (n>>4);
  n = n|0x10;
/******************************************************************************/    
  DrvLcdWriteByte(E_LCD_INSTRUCTION,k);//写入页地址
  DrvLcdWriteByte(E_LCD_INSTRUCTION,n);//写入列地址
  DrvLcdWriteByte(E_LCD_INSTRUCTION,m);
}

/*******************************************************************************
**函数名:void DrvLcdWriteByte(TE_LCD_CMD cmd,T_U8 data)
**
**功能:向液晶屏写入一个字节内容，该内容可以为数据或者是命令码
**
**作者：HJH
**
**日期：2016-04-28
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdWriteByte(TE_LCD_CMD cmd,T_U8 data)
{
  T_U8 i;
  LCD_DATA = data;//将要写的数据写入输出端口寄存器
  switch(cmd)
  {
    case E_LCD_INSTRUCTION:
      {
        LCD_CMD_S;//指出写入的内容为命令
      }
      break;
  case E_LCD_DATA:
    {
      LCD_DATA_S;//指出写入的内容为命令
    }
    break;
  default:
    ;
    break;
  }
  LCD_WR;//LCD写操作
  LCD_E_DIS;//禁止数据锁存
  for(i = 0;i < 10;i++)
  {
    ;
  }
  LCD_E_ENA;//使能数据锁存
  for(i = 0;i < 10;i++)
  {
    ;
  }
  LCD_E_DIS;//禁止数据锁存
}

/*******************************************************************************
**函数名:void DrvLcdSetBackLight(T_U8 bl_state)
**
**功能:液晶屏背光灯打开和关闭控制
**
**作者：HJH
**
**日期：2016-04-11
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdSetBackLight(TE_BACK_LIGHT bl_state)
{
  if(bl_state == E_BACK_LIGHT_ON)
  {
    LCD_BACK_LIGHT_ON;
  }
  else
  {
    LCD_BACK_LIGHT_OFF;
  }
  g_back_light_status = bl_state;
}

/*******************************************************************************
**函数名:void DrvLcdLowPower()
**
**功能:通过关闭背光灯以及液晶屏内部电源，使液晶屏进入低功耗状态
**
**作者：HJH
**
**日期：2016-05-20
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvLcdLowPower(void)
{
  LCD_BACK_LIGHT_OFF; 
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xae);//关闭显示
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa5);//所有显示点打开
  _NOP();
  _NOP();
  _NOP();
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x28);//关闭电源
}

