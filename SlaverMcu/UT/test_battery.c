#ifdef UNIT_TEST
#include "TypeDefine.h"
#include "DrvPower.h"
#include "DrvBeep.h"
#include "CtlTimer.h"
#include "DrvAdc.h"
#include "stdio.h"
#include "CtlAdc.h"


void test_battery()
{
    TS_ADC_INFO adc_value;


    while(1)
    {       
        DrvBattVoltChkEn();
        DrvBattPerfChkEn();
        DrvAdcEnable();
        while(!g_adc_completed_flag);        //wait for convertion finished
        DrvAdcDisable();
        adc_value=AdcGetAverageValue();  
        printf("battery:%d,batt_perf:%d,s_vol_cal:%d,motor_vol:%d\n", 
        adc_value.batt_vol_channal_a12, adc_value.batt_perf_vol_channal_a13,
        adc_value.s_vol_cal_channal_a14, adc_value.motor_vol_channal_a15);
        printf("battery:%.3fv,batt_perf:%.3fv,s_vol_cal:%.3fv,motor_vol:%.3fv\n", 
        M_GetVoltageFromAdc(adc_value.batt_vol_channal_a12)*2/1000.0,
        M_GetVoltageFromAdc(adc_value.batt_perf_vol_channal_a13)/1000.0,
        M_GetVoltageFromAdc(adc_value.s_vol_cal_channal_a14)/1000.0,
        M_GetVoltageFromAdc(adc_value.motor_vol_channal_a15))/1000.0;
        DelayMs(2000);
    }

}

#endif
