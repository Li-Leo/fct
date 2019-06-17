#ifdef UNIT_TEST
#include "DrvPower.h"
#include "ctlTimer.h"
#include "appuiscreen.h"
#include "CtlSpiComm.h"
#include "DrvTimer.h"
#include "AppDelivery.h"
#include "CtlMotor.h"
#include "DrvUart.h"
#include "stdio.h"

void port_low()
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
    P2SEL1 = 0;
    P2SEL0 = 0;
    
    P2DIR = 0xff; 
    //P2DIR &= ~(BIT6|BIT7);
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
}

extern TE_BOOLEAN g_is_in_aging_mode;

void test_LM3_4_Normal()
{
    SendSpiCommandToSlave(HEAD_SC,COMM_SS,0);
    DelayMs(3); //let the cmd send finish

    //clear port wakeup flag 
    DrvPortClearWakeupFlag();
    //enable wakeup timer
	DrvTimerEnable(E_TIMER_CHANNEL_A1);
    
    DrvLcdLowPower();
    //close system tick
    DrvTimerDisable(E_TIMER_CHANNEL_A2);
    CtlMotorStop();
    M_Uart0DisableInterrupt();//关闭PC串口中断
    M_Uart1DisableInterrupt();//关闭内部MCU通信串口中断
    DrvPortLowPower();

    do
    {
        printf("enter normal mode\n");
        while(!((SCAN_KEY_MAIN_STATUS & BIT1) ? E_FALSE : E_TRUE));
        DelayMs(1000);
        printf("enter LPM0 mode\n");  
        g_auto_wakeup_time.week = 0;
        DrvTimerEnable(E_TIMER_CHANNEL_A1);        
        g_auto_wakeup_timer_ms = 60000;
        __bis_SR_register(LPM0_bits + GIE);      // LPM0
        DrvTimerDisable(E_TIMER_CHANNEL_A1);

        DelayMs(1000);
        printf("enter LPM1 mode\n");    
        g_auto_wakeup_time.week = 0;
        DrvTimerEnable(E_TIMER_CHANNEL_A1);                
        g_auto_wakeup_timer_ms = 60000;
        __bis_SR_register(LPM1_bits + GIE);      // LPM1
        DrvTimerDisable(E_TIMER_CHANNEL_A1);
        
        DelayMs(1000);
        printf("enter LPM2 mode\n");        
        g_auto_wakeup_time.week = 0;
        DrvTimerEnable(E_TIMER_CHANNEL_A1);                
        g_auto_wakeup_timer_ms = 60000;
        __bis_SR_register(LPM2_bits + GIE);      // LPM2  
        DrvTimerDisable(E_TIMER_CHANNEL_A1);
        
        DelayMs(1000);
        printf("enter LPM3 mode\n");        
        g_auto_wakeup_time.week = 0;
        DrvTimerEnable(E_TIMER_CHANNEL_A1);        
        g_auto_wakeup_timer_ms = 60000;
        __bis_SR_register(LPM3_bits + GIE);      // LPM3
        DrvTimerDisable(E_TIMER_CHANNEL_A1);
        
        DelayMs(1000);
        printf("enter LPM4 mode\n");        
        g_auto_wakeup_time.week = 0;
        DrvTimerEnable(E_TIMER_CHANNEL_A1);        
        g_auto_wakeup_timer_ms = 60000;
        __bis_SR_register(LPM4_bits + GIE);      // LPM4
        DrvTimerDisable(E_TIMER_CHANNEL_A1);
        DelayMs(1000);
    }while(1);    
}


void test_low_power()
{
    //DrvLcdLowPower();
    //DelayMs(10);
    //    port_low();
    SendSpiCommandToSlave(HEAD_SC,COMM_SS,0);
    DelayMs(3); //let the cmd send finish
    g_auto_wakeup_timer_ms = MAX_T_U32;
    g_is_in_aging_mode = E_TRUE;
    SetNextDeliTimePoint();
    g_next_deliver_time.hour ++;
    g_is_in_aging_mode = E_FALSE;    
    
    DrvPowerEnterLowMode();

    PortInit();
    DrvLcdInit();

    DisplayUI(E_SCREEN_SHOW_LOGO);
    DisplayFlushDirty();

    P1IE &= ~BIT1;
    while(1);

}
#endif
