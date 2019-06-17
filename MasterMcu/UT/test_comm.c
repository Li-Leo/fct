#ifdef UNIT_TEST
#include "DrvPower.h"
#include "ctlTimer.h"
#include "appuiscreen.h"
#include "ctlspicomm.h"
#include "stdio.h"

void test_internal_comm()
{
    T_U16 rv_result=0;
    
    P4OUT |= BIT3;
    //SendSpiCommandToSlave(HEAD_SC,COMM_TC,0);
    while(1)
    {
      
        //SendSpiCommandToSlave(HEAD_SC,COMM_AD,0);
        //DelayMs(500);  
        SendSpiCommandToSlave(HEAD_SQ,COMM_AD,E_ADC_CHANNEL_BATT_VOL);
        DelayMs(500);
        rv_result=(ReceiveSpiCommandFromSlave()).data;     
        printf("battery voltage[%d]\n", rv_result);
        DelayMs(500);
      /*
        SendSpiCommandToSlave(HEAD_SC,COMM_MC,4);
        DelayMs(500);
        rv_result=(ReceiveSpiCommandFromSlave()).data;
        printf("motor Hz[%d]\n", rv_result);*/
    }

}
#endif
