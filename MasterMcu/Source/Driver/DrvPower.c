/*******************************************************************************
**�ļ���:DrvPower.c
**
**����:
**
**����:HJH
**
**����:2016-04-18
**
**�޸�����:
**
**����:
**
**����:
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
**������:void DrvInitPowerControl(void)
**
**����:��Դ��ʼ�����ƣ��������Ϊ�ر��˷ŵ�Դ,������ܼ��ʹ�ܿ��عرգ���ص�ѹ
**
** ���ʹ�ܿ��عرա���翪��ʹ�ܹرա�
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
**
*******************************************************************************/
void DrvInitPowerControl(void)
{
  MOTOR_POWER_OFF;//�رյ����Դ
}
/*******************************************************************************
**������:void DrvPowerSleep(void)
**
**����:ϵͳ�������˯��ģʽ���������Ϊ�رյ����Դ������ʹ�ܿ��عرպ�MCU����˯
**
**��ģʽ��֧�ְ����Ͷ�ʱ������
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
**
*******************************************************************************/
void DrvPowerSleep(void)
{
  MOTOR_POWER_OFF;
}
/*******************************************************************************
**������:void DrvPowerStandby(void)
**
**����:ϵͳ�������ģʽ���������Ϊ�رյ����Դ������ʹ�ܣ�ֻ֧�ְ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
**
*******************************************************************************/
void DrvPowerStandby(void)
{
  MOTOR_POWER_OFF;
}
/*******************************************************************************
**������:void DrvPowerOn(void)
**
**����:ϵͳ��Դ���빤��ģʽ���������Ϊ����Ƭ���˳��͹��ġ���ص�ѹ
**
**���ʹ��
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
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
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xae);//�ر���ʾ
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa5);//������ʾ���
  _NOP();
  _NOP();
  _NOP();
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x28);//�رյ�Դ
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
    //DrvSpiDisable(E_SPI_M_S_COM);//�ر�SPI
    MOTOR_POWER_OFF;//�رյ����Դ
    DrvEncoderDisable();//�رջ���������
    DrvBeepDisable();//�رշ�����
    DrvVibDisable();//�ر���
    DrvRPIDisable();//�رչ���
    M_Uart0DisableInterrupt();//�ر�PC�����ж�
    M_Uart1DisableInterrupt();//�ر��ڲ�MCUͨ�Ŵ����ж�

#if 1
    //��ȫ���ر�,���͹���
  	DrvPortLowPower();
#else
    //���󲿷ֲ����ر�,��򵥹ر���ȹ��ĸ�һ��
    //��Main key�ж�,���ڻ���mcu
    P1IFG = 0;
    P1IE |= BIT1;   

    //�ر�RPI������, ��Ϊ���RPI
    P1SEL1 &= ~BIT0;
    P1SEL0 &= ~BIT0;    // P1.0 change to IO 
    P1DIR |= BIT0; 
    P1OUT |= BIT0; // rpi_out 1

    //disable RTC
    P1OUT &= ~BIT3;

    //����KEY+, KEY-Ϊ�����
    P2DIR |= BIT6|BIT7;
    P2OUT |= (BIT6|BIT7);

	//������RTCͨ�Žӿ�ȫ������
    P5OUT &= ~(BIT0|BIT1|BIT2|BIT3);  

    //����JTAG�Ƚ�Ϊ���
	/*   PJ    */
    PJSEL1 = 0;
    PJSEL0 = 0;
    PJDIR = 0xFFFF;
    PJOUT = 0;
#endif

    //��MCU����͹���                   
    DrvMCULowPower();
    
    
}
void DrvPowerExitLowMode(void)  /* setting system exit low power mode */
{
    InitDriver();
    //disable wakeup timer
    DrvTimerDisable(E_TIMER_CHANNEL_A1);
    
}


