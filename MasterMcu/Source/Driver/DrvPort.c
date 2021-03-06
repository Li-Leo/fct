/*******************************************************************************
**文件名:DrvPort.c
**
**功能:端口初始化、端口功能配置、端口方向配置
**
**作者:
**
**日期:2016-03-01
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#include "DrvPort.h"
/*******************************************************************************
**函数名:void PortInit()
**
**功能:端口初始化、端口功能选择、端口方向配置,适用于第一版电路
**
**作者：HJH
**
**日期：2016-03-01
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
/*
void PortInit(void)
{
  P1SELC = 0xff;//寄存器P1SEL1和P1SEL0寄存器内容同时改变
  P1SEL1 = 0x00;//P1.0-P1.3用作I/O口
  P1SEL0 = 0xf0;//P1.4-P1.7用作SPI口
  P1DIR = 0x70;//P1.4-P1.6用作输出口，P1.0-P1.3，P1.7用作输入口
  P1IES = 0x01;//P1.0的中断选择下降沿触发，用作光电脉冲计数输入
//------------------------------------------------------------------------------
  P2SELC = 0xff;//寄存器P2SEL1和P2SEL0寄存器内容同时改变
  P2SEL1 = 0x03;//P2.0,P2.1用作第二模块功能(UCA0TXD,UCA0RXD)
  P2SEL0 = 0x00;//P2.2-P2.7用作I/O口
  P2DIR = 0x1d;//P2.0,P2.2-P2.4用作输出;P2.1,P2.5-P2.7用作输入
//------------------------------------------------------------------------------  
  P3SELC = 0xff;//寄存器P3SEL1和P3SEL0寄存器内容同时改变
  P3SEL1 = 0x00;//P3.0-P3.7用作I/O口
  P3SEL0 = 0x00;
  P3DIR = 0xc7;//P3.0-P3.2,P3.6,P3.7用作输出;P3.3-P3.5用作输入
//------------------------------------------------------------------------------ 
  P4SELC = 0xff;//寄存器P3SEL1和P3SEL0寄存器内容同时改变
  P4SEL1 = 0x00;//P4.0-P4.3用作I/O口
  P4SEL0 = 0x00;
  P4DIR = 0x04;//P4.2用作输出;P4.0,P4.1,P4.3用作输入
//------------------------------------------------------------------------------
  P5SELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;//P5.0-P5.3用作I/O口
  P5DIR = 0x0e;//P5.1-P5.3用作输出;P5.0用作输入
//------------------------------------------------------------------------------ 
  P9SELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  P9SEL1 = 0x00;//P9.0-P9.7用作I/O口
  P9SEL0 = 0x00;
  P9DIR = 0xff;//P9.0-P9.7用作输出
//------------------------------------------------------------------------------ 
  PJSELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  PJSEL1 = 0x00;//PJ.0-PJ.3 用作JTAG功能
  PJSEL0 = 0x0f;
  PJDIR = 0x71;//PJ.0,PJ.5用作输出;PJ.1-PJ.3,PJ.4,PJ.6,PJ.7用作输入
}

*/
/*******************************************************************************
**函数名:void PortInit()
**
**功能:端口初始化、端口功能选择、端口方向配置,适用于第二版电路
**
**作者：HJH
**
**日期：2016-06-14
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void PortInit(void)
{
  P1SEL1 = 0x00;//P1.0-P1.3用作I/O口
  P1SEL0 = 0x00;//P1.4-P1.7用作I/O口
  P1DIR  = 0xff;//P1.4-P1.6用作输出口，P1.0-P1.3，P1.7用作输入口
  P1DIR  &= ~(BIT0|BIT1|BIT2);
  P1OUT =0;
  P1IES = 0x02;//P1.0，P1.1:下降沿中断
  P1IE = 0;    //关闭P1所有中断
//------------------------------------------------------------------------------
  P2SEL1 = 0x00;//P2.0,P2.1分别用作BSL程序下载的发送和接收引脚
  P2SEL0 = 0x00;//P2.0-P2.7用作I/O口
  P2DIR = 0x3c;//P2.2-P2.5用作输出;P2.0-P2.1,P2.6-P2.7用作输入
  P2IE = 0;    //关闭P2所有中断
//------------------------------------------------------------------------------  
  P3SELC = 0xff;//寄存器P3SEL1和P3SEL0寄存器内容同时改变
  P3SEL1 = 0x00;//P3.4和P3.5引脚用作CUA1TXD和UCA1RXD功能，其余引脚用作I/O口
  P3SEL0 = 0x00;
  P3DIR = 0xdf;//P3.5用作输入，其余引脚用作输出
  P3OUT =0;
  P3IE = 0;    //关闭P3所有中断
//------------------------------------------------------------------------------  
  P4SELC = 0xff;//寄存器P3SEL1和P3SEL0寄存器内容同时改变
  P4SEL1 = 0x00;//P4.0-P4.7用作I/O口
  P4SEL0 = 0x00;
  P4DIR = 0x0C;//P4.0-P4.7用作输出
  P4OUT &= ~(BIT2|BIT3);
  P4IE = 0;    //关闭P4所有中断
//------------------------------------------------------------------------------  
  P5SELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;//P5.0-P5.3用作I/O口
  P5DIR = 0x0f;//P5.0-P5.3用作输出
//------------------------------------------------------------------------------  
  P9SELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  P9SEL1 = 0x00;//P9.0-P9.7用作I/O口
  P9SEL0 = 0x00;
  P9DIR = 0xff;//P9.0-P9.7用作输出
//------------------------------------------------------------------------------  
  PJSELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  PJSEL1 = 0x00;//PJ.0-PJ.3 用作JTAG功能
  PJSEL0 = 0x0f;
  PJDIR = 0x71;//PJ.0,PJ.5用作输出;PJ.1-PJ.3,PJ.4,PJ.6,PJ.7用作输入
}
/*
void PortWakeup(void)
{
  P1SEL1 = 0x00; //P1.0-P1.3用作I/O口
  P1SEL0 = 0xf0; //P1.4-P1.7用作SPI口
  
  P1DIR  = 0;
  P1IFG  = 0;
  P1IE   = 0;
  
//------------------------------------------------------------------------------
  P2SEL1 = 0x00; //P2.0,P2.1分别用作BSL程序下载的发送和接收引脚
  P2SEL0 = 0x00; //P2.0-P2.7用作I/O口
  P2DIR  = 0;    //P2.2-P2.5用作输出;P2.0-P2.1,P2.6-P2.7用作输入
  P2DIR  |= 0x0f;
//------------------------------------------------------------------------------  

  P3SEL1 = 0x00;
  P3SEL0 = 0x00;
  P3DIR  = 0xff;//P3.5用作输入，其余引脚用作输出
  P3OUT  = 0;
//------------------------------------------------------------------------------  
  P4SEL1 = 0x00;//P4.0-P4.7用作I/O口
  P4SEL0 = 0x00;
  P4DIR = 0xff;//P4.0-P4.7用作输出
  P4OUT = 0;
//------------------------------------------------------------------------------  
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;//P5.0-P5.3用作I/O口
  P5DIR  = 0xff;//P5.0-P5.3用作输出
  P5OUT  = 0;
//------------------------------------------------------------------------------  

  P9SEL1 = 0x00;//P9.0-P9.7用作I/O口
  P9SEL0 = 0x00;
  P9DIR = 0xff;//P9.0-P9.7用作输出
  P9OUT = 0;
//------------------------------------------------------------------------------  

  PJSEL1 = 0x00;//PJ.0-PJ.3 用作JTAG功能
  PJSEL0 = 0x0f;
  PJDIR  = 0x71;//PJ.0,PJ.5用作输出;PJ.1-PJ.3,PJ.4,PJ.6,PJ.7用作输入
}
*/
/*******************************************************************************
**函数名:void PortLowPower()
**
**功能:对端口进行低功耗配置，主要包括关掉所有使能开关、未用端口配置等,适用于第一版电
**
**路
**
**作者：
**
**日期：2016-06-13
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
/*
void PortLowPower(void)
{
  P1SELC = 0xff;//寄存器P1SEL1和P1SEL0寄存器内容同时改变
  P1SEL1 = 0x00;//P1口用作通用I/O
  P1SEL0 = 0x00;//
  P1DIR = 0x08;//P1.3引脚(悬空)配置为输出，P1口其余引脚配置为高阻输入
//  P1IES = 0x01;//P1.0-P1.2的中断选择下降沿触发
//------------------------------------------------------------------------------
  P2SELC = 0xff;//寄存器P2SEL1和P2SEL0寄存器内容同时改变
  P2SEL1 = 0x00;//P2口用作通用I/O
  P2SEL0 = 0x00;//
  P2DIR = 0x10;//P2.4配置为输出，P2口其它引脚配置为高阻输入
  P2OUT &= ~BIT4;//P2.4输出低电平，关掉电机电源 
//------------------------------------------------------------------------------  
  P3SELC = 0xff;//寄存器P3SEL1和P3SEL0寄存器内容同时改变
  P3SEL1 = 0x00;//P3口用作通用I/O
  P3SEL0 = 0x00;
  P3DIR = 0x07;//P3.0-P3.2用作输出，P3口其它引脚用作高阻输入
  P3OUT &= ~(BIT0|BIT1|BIT2);//P3.0-P3.2输出低电平，关闭背光灯、
                            //蜂鸣器和振动电机
//------------------------------------------------------------------------------  
  P4SELC = 0xff;//寄存器P4SEL1和P4SEL0寄存器内容同时改变
  P4SEL1 = 0x00;//P4口用作通用I/O
  P4SEL0 = 0x00;
  P4DIR = 0x02;//P4.2输出，P4口其它引脚用作高阻输入
  P4OUT &= ~BIT2;//P4.2输出低电平，禁止光电开关使能
//------------------------------------------------------------------------------  
  P5SELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
  P5SEL1 = 0x00;//P5口用作通用I/O
  P5SEL0 = 0x00;//
  P5DIR = 0x00;//P5口用作高阻输入
//------------------------------------------------------------------------------  
  P9SELC = 0xff;//寄存器P9SEL1和P9SEL0寄存器内容同时改变
  P9SEL1 = 0x00;//P9口用作通用I/O
  P9SEL0 = 0x00;
  P9DIR = 0x00;//P9口用作高阻输入
//------------------------------------------------------------------------------  
  PJSELC = 0xff;//寄存器PJSEL1和PJSEL0寄存器内容同时改变
  PJSEL1 = 0x00;//PJ.0-PJ.3 用作JTAG功能
  PJSEL0 = 0x0f;
  PJDIR = 0x01;//PJ.0用作输出;PJ.1-PJ.7用作高阻输入
}
*/
/*******************************************************************************
**函数名:void PortLowPower()
**
**功能:对端口进行低功耗配置，主要包括关掉所有使能开关、未用端口配置等,适用于第二版电
**
**路
**
**作者：
**
**日期：2016-06-14
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
//	void PortLowPower(void)
//	{
//	  P1SELC = 0xff;//寄存器P1SEL1和P1SEL0寄存器内容同时改变
//	  P1SEL1 = 0x00;//P1口用作通用I/O
//	  P1SEL0 = 0x00;//
//	  P1DIR = 0x08;//P1.3引脚配置为输出，禁止对SLAVE MCU复位使能，P1口其余引脚配置为高
//	               //阻输入
//	  P1OUT = 0x00;
//	//------------------------------------------------------------------------------
//	  P2SELC = 0xff;//寄存器P2SEL1和P2SEL0寄存器内容同时改变
//	  P2SEL1 = 0x00;//P2口用作通用I/O
//	  P2SEL0 = 0x00;//
//	  P2DIR = 0x00;//P2口引脚配置为高阻输入
//	//------------------------------------------------------------------------------  
//	  P3SELC = 0xff;//寄存器P3SEL1和P3SEL0寄存器内容同时改变
//	  P3SEL1 = 0x00;//P3口用作通用I/O
//	  P3SEL0 = 0x00;
//	  P3DIR = 0x0f;//P3.0-P3.3用作输出，P3口其它引脚用作高阻输入
//	  P3OUT &= ~(BIT0|BIT1|BIT2);//P3.0-P3.2输出低电平，关闭背光灯、
//	                            //蜂鸣器和振动电机
//	//------------------------------------------------------------------------------  
//	  P4SELC = 0xff;//寄存器P4SEL1和P4SEL0寄存器内容同时改变
//	  P4SEL1 = 0x00;//P4口用作通用I/O
//	  P4SEL0 = 0x00;
//	  P4DIR = 0x03;//P4.1和P4.2输出，P4口其它引脚用作高阻输入
//	  P4OUT &= ~(BIT1|BIT2);//P4.1和P4.2输出低电平，分别禁止电机电源和光电开关使能
//	  
//	//------------------------------------------------------------------------------  
//	  P5SELC = 0xff;//寄存器P5SEL1和P5SEL0寄存器内容同时改变
//	  P5SEL1 = 0x00;//P5口用作通用I/O
//	  P5SEL0 = 0x00;//
//	  P5DIR = 0x00;//P5口用作高阻输入
//	 
//	//------------------------------------------------------------------------------  
//	  P9SELC = 0xff;//寄存器P9SEL1和P9SEL0寄存器内容同时改变
//	  P9SEL1 = 0x00;//P9口用作通用I/O
//	  P9SEL0 = 0x00;
//	  P9DIR = 0x00;//P9口用作高阻输入
//	//------------------------------------------------------------------------------  
//	  PJSELC = 0xff;//寄存器PJSEL1和PJSEL0寄存器内容同时改变
//	  PJSEL1 = 0x00;//PJ.0-PJ.3 用作JTAG功能
//	  PJSEL0 = 0x0f;
//	  PJDIR = 0x01;//PJ.0用作输出;PJ.1-PJ.7用作高阻输入
//	  
//	}

