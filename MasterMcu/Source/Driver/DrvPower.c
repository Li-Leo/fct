/*******************************************************************************
**文件名:DrvPower.c
**
**功能:
**
**作者:HJH
**
**日期:2016-04-18
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/

#include "DrvPower.h"
#include "DrvLcd.h"
#include "DrvTimer.h"
#include "DrvSpi.h"
#include "DrvEncoder.h"
#include "DrvUart.h"
#include "Init.h"
/*******************************************************************************
**函数名:void DrvInitPowerControl(void)
**
**功能:电源初始化控制，具体操作为关闭运放电源,电池性能检测使能开关关闭，电池电压
**
** 检测使能开关关闭、光电开关使能关闭、
**
**作者：HJH
**
**日期：2016-04-18
**
*******************************************************************************/
void DrvInitPowerControl(void)
{
  MOTOR_POWER_OFF;//关闭电机电源
}
/*******************************************************************************
**函数名:void DrvPowerSleep(void)
**
**功能:系统供电进入睡眠模式，具体操作为关闭电机电源、所有使能开关关闭和MCU进入睡
**
**眠模式、支持按键和定时器唤醒
**
**作者：HJH
**
**日期：2016-04-18
**
*******************************************************************************/
void DrvPowerSleep(void)
{
  MOTOR_POWER_OFF;
}
/*******************************************************************************
**函数名:void DrvPowerStandby(void)
**
**功能:系统进入待机模式，具体操作为关闭电机电源及所有使能，只支持按键唤醒
**
**作者：HJH
**
**日期：2016-04-18
**
*******************************************************************************/
void DrvPowerStandby(void)
{
  MOTOR_POWER_OFF;
}
/*******************************************************************************
**函数名:void DrvPowerOn(void)
**
**功能:系统电源进入工作模式，具体操作为、单片机退出低功耗、电池电压
**
**检测使能
**
**作者：HJH
**
**日期：2016-04-18
**
*******************************************************************************/
void DrvPowerOn(void)
{
  MOTOR_POWER_ON;
}

/*
*********************************************
                low power
*********************************************
*/

void DrvMCULowPower(void)
{

  __bis_SR_register(LPM3_bits + GIE);      // LPM3
}

/*
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
*/
void DrvPortLowPower(void)
{   
    /*   P1    */   
    P1SEL1 = 0;
    P1SEL0 = 0;
    
    P1DIR = 0xff;
    P1DIR |= BIT1;        
    
    P1OUT = 0;
    P1OUT |= BIT1|BIT0;    
    
    P1IES = BIT1;
    P1IFG = 0;
    P1IE |= BIT1; 
    

    
    /*   P2    */    
#ifndef _DEBUG    
    P2SEL1 = 0;
    P2SEL0 = 0;
#endif
    P2DIR = 0xff; 
    P2DIR &= ~(BIT6|BIT7);
    P2OUT = 0;
    P2OUT |= (BIT6|BIT7);
    
    /*   P3    */ 
    P3SEL1 = 0;
    P3SEL0 = 0;    
    
    P3DIR = 0xFF;
    P3OUT = 0;
    P3OUT |= BIT7;

    /*   P4    */ 
    P4SEL1 = 0;
    P4SEL0 = 0;
    P4DIR = 0xFF;
    P4OUT = 0;

    /*   P5    */    
    P5SEL1 = 0;
    P5SEL0 = 0;    
    P5DIR = 0xFF;
    P5OUT = 0;

    /*   P9    */
    P9SEL1 = 0;
    P9SEL0 = 0;
    P9OUT = 0;
    P9DIR = 0xFF;

    /*   PJ    */
    PJSEL1 = 0;
    PJSEL0 = 0;
    PJDIR = 0xFFFF;
    PJOUT = 0;
    
    PM5CTL0 &= ~LOCKLPM5;
}

void LowPowerPort(void)
{
  P1SEL1 = 0x00;          
  P1SEL0 =0;
  P1DIR |= BIT4|BIT5|BIT6|BIT7;      // spi-->i/o
  P1OUT &= ~(BIT4|BIT5|BIT6|BIT7);
  P1OUT |= BIT0;                     // rpi_out 1
  
  P1DIR &= ~BIT1;        
  P1IES = BIT1;
  P1IFG = 0;
  P1IE |= BIT1;                      //Main key: wakeup mcu

  P2SEL1 &= ~(BIT0|BIT1);            // uart-->i/o
  P2SEL0 = 0;   
  P2OUT &= ~(BIT0|BIT1);
  P2DIR = 0xFF;

  P3SEL1 &= ~(BIT4|BIT5);            // uart-->i/o
  
  PJSEL0 &= ~(BIT0|BIT1|BIT2|BIT3);  // JTAG-->i/o

  PM5CTL0 &= ~LOCKLPM5;
}

void DrvBeepEnable(void)
{
  P3OUT |= BIT1;
}
void DrvBeepDisable(void)
{
  P3OUT &= ~BIT1;
}

void DrvVibEnable(void)
{
  P3OUT |= BIT2;
}
void DrvVibDisable(void)
{
  P3OUT &= ~BIT2;
}

void DrvBackLightEnable(void)
{
  P3OUT |= BIT0;
}
void DrvBackLightDisable(void)
{
  P3OUT &= ~BIT0;
}

void DrvPwrMotorEnable(void)
{
  P4OUT |= BIT3;
}
void DrvPwrMotorDisable(void)
{
  P4OUT &= ~BIT3;
}

void DrvRPIEnable(void)
{
  P4OUT |= BIT2;
}
void DrvRPIDisable(void)
{
  P4OUT &= ~BIT2;
}

void DrvPowerEnterLowMode(void) /* setting system into low power mode */
{
    //clear port wakeup flag 
    DrvPortClearWakeupFlag();
    //enable wakeup timer
	DrvTimerEnable(E_TIMER_CHANNEL_A1);
    
    DrvLcdLowPower();
    //close system tick
    DrvTimerDisable(E_TIMER_CHANNEL_A2);
    //DrvSpiDisable(E_SPI_M_S_COM);//关闭SPI
    MOTOR_POWER_OFF;//关闭电机电源
    DrvEncoderDisable();//关闭霍尔编码器
    DrvBeepDisable();//关闭蜂鸣器
    DrvVibDisable();//关闭振动
    DrvRPIDisable();//关闭光耦
    M_Uart0DisableInterrupt();//关闭PC串口中断
    M_Uart1DisableInterrupt();//关闭内部MCU通信串口中断

#if 1
    //简单全部关闭,更低功耗
  	DrvPortLowPower();
#else
    //将大部分部件关闭,与简单关闭相比功耗高一点
    //打开Main key中断,用于唤醒mcu
    P1IFG = 0;
    P1IE |= BIT1;   

    //关闭RPI捕获功能, 设为输出RPI
    P1SEL1 &= ~BIT0;
    P1SEL0 &= ~BIT0;    // P1.0 change to IO 
    P1DIR |= BIT0; 
    P1OUT |= BIT0; // rpi_out 1

    //disable RTC
    P1OUT &= ~BIT3;

    //设置KEY+, KEY-为输出高
    P2DIR |= BIT6|BIT7;
    P2OUT |= (BIT6|BIT7);

	//心跳和RTC通信接口全部拉低
    P5OUT &= ~(BIT0|BIT1|BIT2|BIT3);  

    //设置JTAG等脚为输出
	/*   PJ    */
    PJSEL1 = 0;
    PJSEL0 = 0;
    PJDIR = 0xFFFF;
    PJOUT = 0;
#endif

    //让MCU进入低功耗                   
    DrvMCULowPower();
    
    
}
void DrvPowerExitLowMode(void)  /* setting system exit low power mode */
{
    InitDriver();
    //disable wakeup timer
    DrvTimerDisable(E_TIMER_CHANNEL_A1);
    
}


