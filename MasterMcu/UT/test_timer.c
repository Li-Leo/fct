#ifdef UNIT_TEST
#include "TypeDefine.h"
#include "DrvPower.h"
#include "DrvBeep.h"
#include "CtlTimer.h"
//#include "DrvAdc.h"
#include "stdio.h"
#include "DrvTimer.h"
#include "CtlTime.h"
#include "CtlSpicomm.h"
#include "DrvRTC.h"
#include "DrvPower.h"
#include "Msp430fr5989.h"
#include "AppDelivery.h"

void test_systick()
{
    TS_TIME_INFO time1,time2;
    
    printf("start test systick timer\n");
    Ds1302TimeRead(); 
    printf("curr time: %02d:%02d:%02d\n", 
                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second);  
    time1 = g_current_time;
    DrvTimerDisable(E_TIMER_CHANNEL_A2);
    DrvTimerEnable(E_TIMER_CHANNEL_A2);
    T_U32 curr_time_ms  =CtlTimeSincePowerOn();
    DelayMs(60000);
    T_U32 elapse_time = CtlTimeElapse(curr_time_ms);
    Ds1302TimeRead(); 
    printf("curr time: %02d:%02d:%02d\n", 
                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second);
    time2 = g_current_time;
    printf("should pass 60s, actual pass %ldms, %lds\n", elapse_time, DiffDateTimeInOneDay(&time2, &time1));


}

void test_wakeup_timer()
{
    TS_TIME_INFO time1,time2;
    
    printf("start test wakeup timer\n");
    Ds1302TimeRead(); 
    printf("curr time: %02d:%02d:%02d\n", 
                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
    time1 = g_current_time;
    g_auto_wakeup_time.week = 0;
    DrvTimerEnable(E_TIMER_CHANNEL_A1);
    g_auto_wakeup_timer_ms = 60000;
    DelayMs(60000);
    DrvTimerDisable(E_TIMER_CHANNEL_A1);
    Ds1302TimeRead(); 
    printf("curr time: %02d:%02d:%02d\n", 
                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
    time2 = g_current_time;
    printf("should pass 60s, actual pass %ldms, %lds\n", g_timer_pass_count, DiffDateTimeInOneDay(&time2, &time1));
 
}

void test_wakeup_timer_at_sleep()
{
    TS_TIME_INFO time1,time2;
    
    printf("start test wakeup timer when sleep\n");
    DrvTimerDisable(E_TIMER_CHANNEL_A2);
    Ds1302TimeRead(); 
    printf("curr time: %02d:%02d:%02d\n", 
                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
    time1 = g_current_time;
    g_auto_wakeup_time.week = 0;
    DrvTimerEnable(E_TIMER_CHANNEL_A1);
    g_auto_wakeup_timer_ms = 60000;
    //DelayMs(60000);
    __bis_SR_register(LPM3_bits + GIE);      // LPM3
    DrvTimerDisable(E_TIMER_CHANNEL_A1);
    Ds1302TimeRead(); 
    printf("curr time: %02d:%02d:%02d\n", 
                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
    time2 = g_current_time;
    printf("should pass 60s, actual pass %ldms, %lds\n", g_timer_pass_count, DiffDateTimeInOneDay(&time2, &time1));
}

void test_timer()
{
    test_systick();
    test_wakeup_timer();
    test_wakeup_timer_at_sleep();
}

#endif
