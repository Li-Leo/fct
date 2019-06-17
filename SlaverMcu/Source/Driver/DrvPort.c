
/************************************************
* COPYRIGHT 2016.
*
* NAME: DrvPort.c   ------- Slave MCU ----------
*
* DESCRIPTION:
*   data access module
*
* REVISION HISTORY:
*	Li 08/21/2016
*
* END OF FILE HEADER
************************************************/

#include"msp430fr5989.h"
#include"DrvPort.h"

/***********************************************
* Description:
*	Port Init
* Argument:
*
* Return:
*	
************************************************/
void PortInit(void)
{
  P1SEL1 =  0x00;         
  P1SEL0 = 0;
  P1DIR  =  0xff;
  P1OUT = 0;
  P1IE = 0;
  P1DIR  &= ~BIT1;    // P1.1 mcu pulse
  
  //P1IES |= BIT0|BIT1;    //P1.0£¬P1.1: fall edge interrupt
  //P1IE  |= BIT0|BIT1;    //P1.0£¬P1.1£ºinterrupt enable
/******************************************************************************/
  P2SEL1 |= BIT0|BIT1;     // P2.0,P2.1:UCA0TXD,UCA0RXD; P2.2-P2.7:I/O;
  P2SEL0 = 0x00;           //P2.2 beep
  P2DIR  = 0xff;    
  P2OUT &= 0x00;
  P2IE = 0;
/******************************************************************************/  
  P3SEL1 |= BIT4|BIT5;     //uart P3.4 P3.5;  P3.2:batt_v_che_en 
  P3SEL0 = 0x00;           //vibrator P3.6
  P3DIR  = 0xff;
  P3OUT  = 0x00; 
  P3IE = 0;
/******************************************************************************/  
  P4SEL1 = 0x00;           //P4.0:pwr_amp_en  P4.3:batt_che_en 
  P4SEL0 = 0x00;
  P4DIR  = 0xff; 
  P4OUT  = 0x00;
  P4IE = 0;
/******************************************************************************/  
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;
  P5DIR  = 0xff;
  P5OUT  = 0x00;
/******************************************************************************/  
  P9SEL1 |= BIT4|BIT5|BIT6|BIT7;  //P9.3-P9.7 AD input
  P9SEL0  = 0;
  P9DIR  |= BIT0|BIT1|BIT2;
  P9DIR  &= ~BIT3;
  P9OUT  &= ~(BIT0|BIT1|BIT2);
  //P9OUT  |= BIT3; 
/******************************************************************************/  
  PJSEL1 = 0x00;           //PJ.0-PJ.3: JTAG 
  PJSEL0 = 0x0f;
  PJDIR  = 0xff; 
  PJOUT  = 0x00;           //PJ.6 retreat; PJ.7 forward
}

/***************************** end of file ************************************/