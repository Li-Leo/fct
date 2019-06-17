#ifdef UNIT_TEST
#include "TypeDefine.h"
#include "DrvPower.h"
#include "DrvBeep.h"
#include "CtlTimer.h"
#include "DrvAdc.h"
#include "stdio.h"

void test_low_power()
{
    TS_ADC_INFO adc_value;
    //DrvLcdLowPower();
    //DelayMs(10);
    //    port_low();
    AdcInit();
    DrvBattVoltChkEn();
    DrvBattPerfChkEn();
    DrvAdcEnable();
    while(!g_adc_completed_flag);        //wait for convertion finished
    adc_value=AdcGetAverageValue();  
    printf("battery:%d,batt_perf:%d,s_vol_cal:%d,motor_vol:%d\n", 
        adc_value.batt_vol_channal_a12, adc_value.batt_perf_vol_channal_a13,
        adc_value.s_vol_cal_channal_a14, adc_value.motor_vol_channal_a15);
   
    DrvAdcDeinit();
    DelayMs(100);
    AdcInit();
    DrvBattVoltChkEn();
    DrvBattPerfChkEn();
    DrvAdcEnable();
    while(!g_adc_completed_flag);        //wait for convertion finished
    adc_value=AdcGetAverageValue();  
    printf("battery:%d,batt_perf:%d,s_vol_cal:%d,motor_vol:%d\n", 
    adc_value.batt_vol_channal_a12, adc_value.batt_perf_vol_channal_a13,
    adc_value.s_vol_cal_channal_a14, adc_value.motor_vol_channal_a15);
        
    while(1)
    {
        DrvLowPower();
        DrvVibMotorSetState(E_ZD_ON);
        DelayMs(3000);
        DrvVibMotorSetState(E_ZD_OFF);
        
        DrvBattVoltChkEn();
        DrvBattPerfChkEn();
        DrvAdcEnable();
        while(!g_adc_completed_flag);        //wait for convertion finished
        adc_value=AdcGetAverageValue();  
        printf("battery:%d,batt_perf:%d,s_vol_cal:%d,motor_vol:%d\n", 
        adc_value.batt_vol_channal_a12, adc_value.batt_perf_vol_channal_a13,
        adc_value.s_vol_cal_channal_a14, adc_value.motor_vol_channal_a15);
    }

}

#endif
