#ifdef UNIT_TEST
#include <string.h>
#include <stdio.h>
#include "typedefine.h"
#include "CtlTime.h"
#include "CtlTimer.h"
#include "DrvRTC.h"
#include "CtlSpicomm.h"

/***********************************************/
void UTBeforeStart()
{

}
void test_low_power();
void test_internal_comm();
void test_display();
void test_LM3_4_Normal();

void test_rtc()
{
    T_U32 last_time;
    T_U32 time2;
    
    while(1)
    {
        last_time = CtlTimeSincePowerOn();
        Ds1302TimeRead();
        time2 = CtlTimeElapse(last_time);
        printf("rtc read waste time:%lu\n", time2);
        
    }
}

void UTRunSuite()
{
    //test_low_power();
    //test_LM3_4_Normal();
    //test_internal_comm();
    test_timer();
    //test_display();
    //test_rtc();
}

void UTStart(void)
{
    int ch;
    while(1)
    {
        SendSpiCommandToSlave(HEAD_SC, COMM_WD, 0);
        printf("unit test start\n");

        UTBeforeStart();
        UTRunSuite();

        printf("unit test end\n");

        printf("press \'a\' to test again or stop\n");
        ch = getchar();
        if (ch != 'a')
        {
            printf("---test end!---\n");
            //break;
        }
    }
}

/***********************************************/
#endif
