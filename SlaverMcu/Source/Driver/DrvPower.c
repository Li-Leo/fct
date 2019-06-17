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
#include "..\App\Init.h"
#include "DrvTimer.h"
#include "DrvAdc.h"
#include "DrvUart.h"
#include "common.h"
#include "DrvMotor.h"
#include "DrvEncoder.h"
#include "DrvHall.h"

T_U8 g_enter_low_power_state=0;        // enter low power mode flag
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
  //MOTOR_POWER_OFF;//�رյ����Դ
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
  //MOTOR_POWER_OFF;
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
  //MOTOR_POWER_OFF;
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
  //MOTOR_POWER_ON;
}

/*
*********************************************
                low power
*********************************************
*/

void DrvMCULowPower(void)
{
  __bis_SR_register(LPM3_bits + GIE);       // LPM3
}

void DrvPortLowPower(void)
{
  P1SEL1 =  0x00;          
  P1SEL0 =0;
  P1DIR = 0xFF;
  P1OUT = 0;
  P1OUT |= BIT0;
  P1IFG =  0;         //clear interrupt
  P1IE = 0;

  //P1.4��interrupt enable(used for wakeup by main MCU)
  P1DIR &= ~BIT4;
  P1IES &= ~BIT4;     //P1.4 interrupt (1 falling edge, 0 rising edge)
  P1IFG  =  0;        //clear interrupt  
  P1IE  |= BIT4;      

  P2SEL1 = 0;
  P2SEL0 = 0;   
  P2OUT = 0;
  P2DIR = 0xFF;
  P2IFG =  0;         //clear interrupt
  P2IE = 0;

  P3SEL1 = 0;
  P3SEL0 = 0; 
  P3OUT = 0;
  P3DIR = 0xFF;
  P3DIR  &= ~BIT5;     //P3.5  input
  P3IFG =  0;         //clear interrupt
  P3IE = 0;
  
  P4SEL1 = 0;
  P4SEL0 = 0;   
  P4OUT = 0;
  P4DIR = 0xFF;
  P4IFG =  0;         //clear interrupt
  P4IE = 0;  

  P5SEL1 = 0;
  P5SEL0 = 0; 
  P5DIR = 0xFF;
  P5OUT = 0;
  
  P9SEL1 = 0;
  P9SEL0 = 0;   
  P9OUT = 0;
  P9DIR = 0xFF;
  
  PJSEL1 = 0;
  PJSEL0 = 0;  
  PJOUT = 0; 
  PJDIR = 0xFFFF;  


  PM5CTL0 &= ~LOCKLPM5;
}

void LowPowerPort(void)
{
  P1SEL1 =  0x00;          
  P1SEL0 =0;
  P1DIR |= BIT4|BIT5|BIT6|BIT7;      // spi-->i/o
  P1OUT &= ~(BIT4|BIT5|BIT6|BIT7);
  P1OUT |= BIT0;                     // rpi_out 1

  P2SEL1 &= ~(BIT0|BIT1);            // uart-->i/o
  P2SEL0 = 0;   
  P2OUT &= ~(BIT0|BIT1);
  P2DIR = 0xFF;

  P3SEL1 &= ~(BIT4|BIT5);            // uart-->i/o
  P3DIR  &= ~BIT5;
  
  P3IES  &= ~BIT5;                   //P3.5 interrupt (1 falling edge, 0 rising edge)
  P3IFG  =  0;
  P3IE   |= BIT5;                    //P3.5��interrupt enable(used for wakeup by main MCU)
  
  P9SEL1 &= ~(BIT4|BIT5|BIT6|BIT7);  // AD-->i/o
  P9SEL0 = 0;   
  P9DIR  |= BIT4|BIT5|BIT6|BIT7;
  P9OUT  &= ~(BIT4|BIT5|BIT6|BIT7);

  
  PJSEL0 &= ~(BIT0|BIT1|BIT2|BIT3);  //JTAG-->i/o

  PM5CTL0 &= ~LOCKLPM5;
}

void DrvHallAmpEnable(void)
{
  P4OUT |= BIT0;
}
void DrvHallAmpDisable(void)
{
  P4OUT &= ~BIT0;
}

void DrvBattVoltageCheEnEnable(void)
{
  P3OUT |= BIT2;
}
void DrvBattVoltageCheEnDisable(void)
{
  P3OUT &= ~BIT2;
}

void DrvBattCheEnEnable(void)
{
  P4OUT |=  BIT3;
}
void DrvBattCheEnDisable(void)
{
  P4OUT &= ~BIT3;
}

void DrvBeepEnable(void)
{
  P2OUT |=  BIT2;
}
void DrvBeepDisable(void)
{
  P2OUT &= ~BIT2;
}

void DrvVibEnable(void)
{
  P3OUT |=  BIT6;
}
void DrvVibDisable(void)
{
  P3OUT &= ~BIT6;
}

//enable the power for hall and motor current
void DrvPowerEnableHallAndMotorCurrent()
{
    P4OUT |= BIT0;
}
//disable the power for hall and motor current
void DrvPowerDisableHallAndMotorCurrent()
{
    P4OUT &= ~BIT0;    
}

void DrvLowPower(void)
{ 
  M_CommonPrint("sleep\n");
  g_enter_low_power_state=1;                //set flag
  
  DrvTimerDisable(E_TIMER_CHANNEL_A2);
  DrvAdcDeinit();

  DrvPowerDisableHallAndMotorCurrent();
  DrvMotorCurrentDisable();
  DrvEncoderDisable();
  DrvHallDisable();

  M_Uart0DisableInterrupt();
  M_Uart1DisableInterrupt();
  
  DrvPortLowPower();
  DrvMCULowPower();

  InitDriver();        // re-initial driver layer
  M_CommonPrint("weekup\n");
}

