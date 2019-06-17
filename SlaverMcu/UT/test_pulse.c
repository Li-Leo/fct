#ifdef UNIT_TEST
#include "TypeDefine.h"
#include "DrvPower.h"
#include "DrvBeep.h"
#include "CtlTimer.h"
//#include "DrvAdc.h"
#include "stdio.h"
#include "DrvTimer.h"
#include "CtlTime.h"


void test_pulse()
{
    TE_BOOLEAN pre_pulse = E_FALSE;
    TE_BOOLEAN pulse = E_FALSE;
    while(1)
    {
        pulse = ((P1IN & BIT1) != 0) ? E_TRUE : E_FALSE;

        if(pre_pulse != pulse)
        {
            printf("[%ld]pulse changed\n", CtlTimeSincePowerOn());
            pre_pulse = pulse;
        }
    }
}

#endif
