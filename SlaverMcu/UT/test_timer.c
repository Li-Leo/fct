#ifdef UNIT_TEST
#include "TypeDefine.h"
#include "DrvPower.h"
#include "DrvBeep.h"
#include "CtlTimer.h"
//#include "DrvAdc.h"
#include "stdio.h"
#include "DrvTimer.h"
#include "CtlTime.h"

void test_systick()
{
    DrvTimerDisable(E_TIMER_CHANNEL_A2);
    DrvTimerEnable(E_TIMER_CHANNEL_A2);
    while(1)
    {
        T_U32 curr_time_ms  =CtlTimeSincePowerOn();
        DelayMs(5000);
        //__delay_cycles((T_S32)1000);
        T_U32 elapse_time = CtlTimeElapse(curr_time_ms);
        printf("should pass 5s, actual pass %ldms\n", elapse_time);
    }

}

//	void test_wakeup_timer()
//	{
//	    g_auto_wakeup_timer_ms = 0;
//	    DrvTimerDisable(E_TIMER_CHANNEL_A1);
//	    DrvTimerEnable(E_TIMER_CHANNEL_A1);
//	    DelayMs(5000);
//	    printf("should pass 5s, actual pass %ldms\n", g_timer_pass_count);
//	    DrvTimerDisable(E_TIMER_CHANNEL_A1);
//	}

void test_timer()
{
    test_systick();
    //test_wakeup_timer();
}

#endif
