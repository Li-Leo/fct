
#include "AppFCT.h"

T_CHAR g_display_string[20]={0};
T_CHAR g_timer_timeout=0;
T_CHAR g_get_battery_voltage_timeout=0;

T_U16 g_battery_voltage=0;
TE_BOOLEAN g_battery_voltage_is_ready=E_FALSE;
TE_BOOLEAN g_motor_current_is_ready=E_FALSE;
TE_BOOLEAN g_is_use_continue_mode = E_TRUE;
TE_BOOLEAN g_is_get_mode_cmd= E_FALSE;
T_U16 g_case_num= 12;
T_U32 g_com_test_send_counter = 0;
T_U32 g_com_test_receive_counter = 0;
TE_BOOLEAN g_send_pulse_signal_status = E_TRUE;
TE_BOOLEAN g_pulse_signal_now = E_FALSE;
TE_BOOLEAN g_pulse_signal_last = E_FALSE;

void GetTimerTimeOut()
{
    g_timer_timeout=1;
}

void WaitKeyPressed(TE_KEY key)
{
    while(1)
    {
       CtlKeyScan();
       if(CtlKeyIsPressed(key) == 1)
       {
           DrvKeyPostClearState();
           return;
       }
    }
}

void FCTLcdDisplay(const T_CHAR* line1, const T_CHAR* line2)
{
    DisplayClearAll();
    DisplayFlushDirty();

    DisplayTextAtPos(0,0, line1);
    DisplayTextAtPos(0,16,line2);  
    DisplayFlushDirty();     
}

void FCTLcdDisplayCenter(const T_CHAR* line,T_U8 y_pos)
{
    DisplayClearAll();
    DisplayFlushDirty();

    DisplayTextAtXCenter(y_pos,line);
    DisplayFlushDirty();     
}


void BeepOneTime()
{
    DrvBuzzSetState(E_BUZZ_ON);
    DelayMs(30);
    DrvBuzzSetState(E_BUZZ_OFF);
}

void FCTTestShowVersion(T_CHAR* case_name)
{
    BeepOneTime();

    sprintf(g_display_string, "FCT Ver:%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, REVISION_VERSION);

    FCTLcdDisplay(g_display_string,"Press key 'main'");
    WaitKeyPressed(E_KEY_MAIN);
    BeepOneTime();
}

void FCTTestShowSN(T_CHAR* case_name)
{
    LoadSysPara(E_SYSTEM_PARA_SN,1);
    //g_serial_number=12345678;

    if(!g_is_use_continue_mode)
    {
        printf("serial number\n");
        printf("%ld\n",g_serial_number);
    }
    BeepOneTime();

    sprintf(g_display_string, "SN:%ld", g_serial_number);

    FCTLcdDisplay(g_display_string,"Press key '+'");
    WaitKeyPressed(E_KEY_PLUS);
    BeepOneTime();

}

void FCTTestKey(T_CHAR* case_name)
{
    if(!g_is_use_continue_mode)
    {
        printf("Key testing...\n");
        printf("Please press the keys\n");
    }
    BeepOneTime();
    FCTLcdDisplay(case_name,"Press key 'main'");
    WaitKeyPressed(E_KEY_MAIN);
    BeepOneTime();

    FCTLcdDisplay("Key Test:","Press key '+'");
    WaitKeyPressed(E_KEY_PLUS);
    BeepOneTime();

    FCTLcdDisplay("Key Test:","Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);
    BeepOneTime();
    if(!g_is_use_continue_mode)
    {
        FCTLcdDisplay("Key Test"," ");
        printf("Key test complete!\n");
    }
    
}

void FCTTestLcd(T_CHAR* case_name)
{
    BeepOneTime();
    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);
        TestLcd();
	    DelayMs(1000);
    }else{
        printf("Lcd testing...\n");
        printf("Please check the LCD\n");    
        TestLcd();
	    DelayMs(1000);
        FCTLcdDisplay("Lcd Test"," ");
        printf("Lcd test complete!\n");
    }
}

void FCTTestBuzzer(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '-'");
        WaitKeyPressed(E_KEY_MINUS);
    }else{
        printf("Buzzer testing...\n");
        printf("Please check the buzzer\n");    
    }

    DrvBuzzSetState(E_BUZZ_ON);
    DelayMs(500);
    DrvBuzzSetState(E_BUZZ_OFF);
    
    if(!g_is_use_continue_mode)
    {
        FCTLcdDisplay("Buzzer Test"," ");
        printf("Buzzer test complete!\n");
    }
    
}

void FCTTestVibrator(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);
    }else{
        printf("Vibrator testing...\n");
        printf("Please check the vibrator\n");    
    }

    DrvVibMotorSetState( E_ZD_ON);
    DelayMs(500);
    DrvVibMotorSetState( E_ZD_OFF);
    
    if(!g_is_use_continue_mode)
    {
        FCTLcdDisplay("Vibrator Test"," ");
        printf("Vibrator test complete!\n");
    }
}

void test_com(void)
{
    SendSpiCommandToSlaveEx(HEAD_SC,COMM_TEST,0xa5a5,0x5a5a);
    g_com_test_send_counter++;
    sprintf(g_display_string, "T:%ld R:%ld", g_com_test_send_counter, g_com_test_receive_counter);
    DisplayFlushDirty();

    DisplayTextAtPos(0,0, g_display_string);
    DisplayTextAtPos(0,16,"Press '+' stop");  
}
void FCTTestCom(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '-'");
        WaitKeyPressed(E_KEY_MINUS);
    }else{
        FCTLcdDisplay("Comm Test"," ");
        printf("Comm testing...\n");
    }
    DisplayClearAll();

    g_com_test_send_counter = 0;
    g_com_test_receive_counter = 0;
    CtlTimerClearAll(ALL_TIMER);
    CtlTimerSetRepeat(E_TIMER_COMMON_DELAY, TEST_COM_INTERVAL_MS);
    CtlTimerBindHandle(E_TIMER_COMMON_DELAY, test_com);

    while(1)
    {  
        if (UartGetNewDataFlag(E_UART1))
        {
            UartClearNewDataFlag(E_UART1);
            CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
        }

        if (!CtlTimeIsTickHandled())
        {
            CtlTimeSetTickHandled(E_TRUE);
            CtlTimerHandle(ALL_TIMER);
        }

        CtlKeyScan();
        if(CtlKeyIsPressed(E_KEY_PLUS) == 1)
        {
            DrvKeyPostClearState();
            break;
        }

        if(g_com_test_receive_counter >= TEST_COM_EXPECT_COUNT)
            break;

    }

    CtlTimerClear(E_TIMER_COMMON_DELAY);
    sprintf(g_display_string, "T:%ld R:%ld", g_com_test_send_counter, g_com_test_receive_counter);

    *(T_U32 *)FRAM_ADDR_FCT_SEND = g_com_test_send_counter;
    *(T_U32 *)FRAM_ADDR_FCT_RECV = g_com_test_receive_counter;     

    FCTLcdDisplay(g_display_string,"Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);

    if(g_com_test_receive_counter >= g_com_test_send_counter - 1)
    {
        g_fct_case[E_FCT_TEST_CASE_COMM].result=E_FCT_RESULT_PASS;
        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);
        }else{
            printf("Comm test complete, pass!\n");
        } 
    }
    else
    {
        g_fct_case[E_FCT_TEST_CASE_COMM].result=E_FCT_RESULT_FAIL;
        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test FAIL!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);
        }else{
            printf("Comm test complete, fail!\n");
        } 
    }
    
}

void test_send_pulse(void)
{
    sprintf(g_display_string, "T:%ld R:%ld", g_com_test_send_counter, g_com_test_receive_counter);

    if(g_send_pulse_signal_status == E_FALSE)
	{
		PULSE_SIGNAL_LOW;
		g_send_pulse_signal_status = E_TRUE;
	}
	else if(g_send_pulse_signal_status == E_TRUE)
	{
		PULSE_SIGNAL_HIGH;
		g_send_pulse_signal_status = E_FALSE;
	}

    g_com_test_send_counter++;

    DisplayFlushDirty();
    DisplayTextAtPos(0,0, g_display_string);
    DisplayTextAtPos(0,16,"Press '+' stop");  
}

TE_BOOLEAN ReadPulseSignalLevel()
{
    TE_BOOLEAN pul_sig = E_FALSE;
    
	pul_sig = ((SCAN_PULSE_SIGNAL_STATUS & BIT2) != 0) ? E_TRUE : E_FALSE;
	return pul_sig;
}

void FCTTestPulse(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '-'");
        WaitKeyPressed(E_KEY_MINUS);
    }else{
        FCTLcdDisplay("Pulse Test"," ");
        printf("Pulse testing...\n");
    }
    DisplayClearAll();

    g_com_test_send_counter = 0;
    g_com_test_receive_counter = 0;
    CtlTimerClearAll(ALL_TIMER);
    CtlTimerSetRepeat(E_TIMER_COMMON_DELAY, TEST_PULSE_INTERVAL_MS);
    CtlTimerBindHandle(E_TIMER_COMMON_DELAY, test_send_pulse);

    while(1)
    {  
        g_pulse_signal_now = ReadPulseSignalLevel();


        if (!CtlTimeIsTickHandled())
        {
            CtlTimeSetTickHandled(E_TRUE);
            CtlTimerHandle(ALL_TIMER);
        }

        if (g_pulse_signal_now != g_pulse_signal_last){
            g_pulse_signal_last = g_pulse_signal_now;
            g_com_test_receive_counter++;
        }

        CtlKeyScan();
        if(CtlKeyIsPressed(E_KEY_PLUS) == 1)
        {
            DrvKeyPostClearState();
            break;
        }

        if(g_com_test_receive_counter >= TEST_COM_EXPECT_COUNT)
            break;

    }
    CtlTimerClear(E_TIMER_COMMON_DELAY);
    sprintf(g_display_string, "T:%ld R:%ld", g_com_test_send_counter, g_com_test_receive_counter);

    *(T_U32 *)FRAM_ADDR_FCT_SEND_PULSE = g_com_test_send_counter;
    *(T_U32 *)FRAM_ADDR_FCT_RECV_PULSE = g_com_test_receive_counter; 

    FCTLcdDisplay(g_display_string,"Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);

    if(g_com_test_receive_counter >= g_com_test_send_counter - 1)
    {
        g_fct_case[E_FCT_TEST_CASE_PULSE].result=E_FCT_RESULT_PASS;
        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);
        }else{
            printf("Pulse test complete, pass!\n");
        } 
    }
    else
    {
        g_fct_case[E_FCT_TEST_CASE_PULSE].result=E_FCT_RESULT_FAIL;
        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test FAIL!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);
        }else{
            printf("Pulse test complete, fail!\n");
        } 
    }
    
}

void FCTTestMotor(T_CHAR* case_name)
{
    BeepOneTime();

    if(!g_is_use_continue_mode)
    {
        printf("Motor testing...\n");
        printf("Please check the motor\n");
        printf("Press key \'+\'-->forward, \'-\'-->backward, \'main\'-->stop\n");
    }
    
    FCTLcdDisplay(case_name,"Press key 'main'");
    WaitKeyPressed(E_KEY_MAIN);

    FCTLcdDisplay("motor-->forward","Press key '+'");
    WaitKeyPressed(E_KEY_PLUS);
    AppStartRun(E_MOVE_FORWARD,5000);

    FCTLcdDisplay("motor-->backward","Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);
    AppStartRun(E_MOVE_BACKWARD,5000);

    if(g_is_use_continue_mode){
        FCTLcdDisplay("motor-->stop","Press key 'main");
        WaitKeyPressed(E_KEY_MAIN);
        AppStopRun();
    }else{
        FCTLcdDisplay("motor-->stop","Press key 'main");
        WaitKeyPressed(E_KEY_MAIN);
        FCTLcdDisplay("Motor Test"," ");
        AppStopRun();
        printf("Motor test complete!\n");
    }
}

void FCTTestHall(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);
        //FCTLcdDisplay("Magnetic near","and away of HALL");
    }else{
        FCTLcdDisplay("Hall Test"," ");
        printf("Hall testing...\n");
        printf("Let the magnetic near HALL sensor\n");
    }

    g_timer_timeout=0;
    g_is_already_meet_hall=E_FALSE;
    CtlTimerClearAll(ALL_TIMER);
    CtlTimerSet(E_TIMER_COMMON_DELAY, 1000);
    CtlTimerBindHandle(E_TIMER_COMMON_DELAY, GetTimerTimeOut);
    P4OUT &= ~BIT3;  //motor power off
    ExcuteHallLocationCal();
    while(1)
    {  
        if (UartGetNewDataFlag(E_UART1))
        {
            UartClearNewDataFlag(E_UART1);
            CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
        }

        if(!g_is_already_meet_hall)
        {
            CtlTimerHandle(ALL_TIMER);
            if(g_timer_timeout)
            {
                AppStopRun();
                break;
            }
        }
        else
        {
            AppStopRun();
            break;
        }
    }

    if(g_is_already_meet_hall)
    {
        g_fct_case[E_FCT_TEST_CASE_HALL].result=E_FCT_RESULT_PASS;
        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '-'");
            WaitKeyPressed(E_KEY_MINUS);
        }else{
            printf("Hall test complete!\n");
        } 
    }
    else
    {
        g_fct_case[E_FCT_TEST_CASE_HALL].result=E_FCT_RESULT_FAIL;
        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test FAIL!","Press key '-'");
            WaitKeyPressed(E_KEY_MINUS);
        }else{
            printf("Hall test complete!\n");
        } 
    }
    
}

void FCTTestEncoder(T_CHAR* case_name)
{
    TE_BOOLEAN encoder_high=E_FALSE, encoder_low=E_FALSE;

    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);
    }else{
        FCTLcdDisplay("Encoder Test"," ");
        printf("Encoder testing...\n");
    } 

    P1SEL1 &= ~BIT0;
    P1SEL0 &= ~BIT0;    // P1.0 rpi_out
    P1DIR  &= ~BIT0;    // set P1.0 input

    P4OUT |= BIT2;      //encoder on
    DelayMs(30);
    if((P1IN&BIT0) == 0)
       encoder_low=E_TRUE;
    
    P4OUT &= ~BIT2;
    DelayMs(30);
    if((P1IN&BIT0) == 1)
       encoder_high=E_TRUE;

    if(encoder_low==E_TRUE && encoder_high==E_TRUE)
    {
        g_fct_case[E_FCT_TEST_CASE_ENCODER].result=E_FCT_RESULT_PASS;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '-'");
            WaitKeyPressed(E_KEY_MINUS);
        }else{
            printf("Encoder test complete, pass!\n");
        } 
    }
    else
    {
        g_fct_case[E_FCT_TEST_CASE_ENCODER].result=E_FCT_RESULT_FAIL;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test FAIL!","Press key '-'");
            WaitKeyPressed(E_KEY_MINUS);
        }else{
            printf("Encoder test complete, fail!\n");
        } 
    }
}
/*
void FCTTestMotorCurrent(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key 'Main'");
        WaitKeyPressed(E_KEY_MAIN);
    }else{
        FCTLcdDisplay("Motor Current","Test");
        printf("Motor current testing...\n");
    }

    g_motor_current_is_ready=E_FALSE;
    g_curr_motor_current=0;
    g_timer_timeout=0;
    
    CtlTimerClearAll(ALL_TIMER);
    CtlTimerSet(E_TIMER_COMMON_DELAY, 1000);
    CtlTimerBindHandle(E_TIMER_COMMON_DELAY, GetTimerTimeOut);
    AppStartRun(E_MOVE_FORWARD,5000);
    DelayMs(500);

    while(1)
    {  
        if (UartGetNewDataFlag(E_UART1))
        {
            UartClearNewDataFlag(E_UART1);
            CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
        }

        if(!g_motor_current_is_ready)
        {
            CtlTimerHandle(ALL_TIMER);
            if(g_timer_timeout)
            {
                AppStopRun();
                break;
            }
        }
        else
           break;
    }
    sprintf(g_display_string, "MotorHz:%d.%dHz",g_curr_motor_current/100,g_curr_motor_current%100);
    AppStopRun();

    if(g_is_use_continue_mode)
    {
        FCTLcdDisplay(g_display_string,"Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);
    }
    else
    {
        printf("%s\n",g_display_string);
    }

    //if(g_curr_motor_current>2700)
    {
        g_fct_case[E_FCT_TEST_CASE_MOTOR_CURRENT].result=E_FCT_RESULT_PASS;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '-'");
            WaitKeyPressed(E_KEY_MINUS);        
        }else{
            printf("Motor current test complete!\n");
        } 
    }
//    else
//    {
//        g_fct_case[E_FCT_TEST_CASE_MOTOR_CURRENT].result=E_FCT_RESULT_FAIL;
//        if(g_is_use_continue_mode){
//            FCTLcdDisplay("Test FAIL!","Press key '-'");
//            WaitKeyPressed(E_KEY_MINUS);        
//        }else{
//            printf("Motor current test complete!\n");
//        }
//    }
}
*/

void FCTTestADC(T_CHAR* case_name)
{
    T_SINT offset=0;

    BeepOneTime();
    if(g_is_use_continue_mode){ 
        FCTLcdDisplay(case_name,"Press key 'Main'");
        WaitKeyPressed(E_KEY_MAIN);
    }else{
        FCTLcdDisplay("ADC test"," ");
        printf("ADC testing...\n");
    }  

    g_timer_timeout=0;
    CtlTimerClearAll(ALL_TIMER);
    CtlTimerSet(E_TIMER_COMMON_DELAY, 1000);
    CtlTimerBindHandle(E_TIMER_COMMON_DELAY, GetTimerTimeOut);
    UartClearNewDataFlag(E_UART0);

    while(1)
    {      
        SendSpiCommandToSlave(HEAD_SQ,COMM_AD,E_ADC_CHANNEL_BATT_VOL);
        if (UartGetNewDataFlag(E_UART1))
        {
            UartClearNewDataFlag(E_UART1);
            CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
        }
        
        if(!g_battery_voltage_is_ready)
        {
            CtlTimerHandle(ALL_TIMER);
            if(g_timer_timeout)
            {
                break;
            }
        }
        else
           break;
    }

    sprintf(g_display_string, "Volts: %d mV",g_battery_voltage);

    if(g_is_use_continue_mode)
    {
        FCTLcdDisplay(g_display_string,"Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);
    }
    else
    {
        printf("%s\n",g_display_string);
    }

    sprintf(g_display_string, "Offset: %d mV",3000-g_battery_voltage);

    if(g_is_use_continue_mode)
    {
        FCTLcdDisplay(g_display_string,"Press key '-'");
        WaitKeyPressed(E_KEY_MINUS);
    }
    else
    {
        printf("%s\n",g_display_string);
    }    

    if(g_battery_voltage > 2400)
    {
        offset = 3000 - g_battery_voltage;
        g_battery_voltage_offset = offset;
        SaveSysPara(E_SYSTEM_PAPA_BATTERY_VOLLTAGE_OFFSET, 0);
        g_fct_case[E_FCT_TEST_CASE_ADC].result=E_FCT_RESULT_PASS;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);        
        }else{
            printf("ADC test complete!\n");
        }     
    }
    else
    {
        g_battery_voltage_offset = 0;
        SaveSysPara(E_SYSTEM_PAPA_BATTERY_VOLLTAGE_OFFSET, 0);
        g_fct_case[E_FCT_TEST_CASE_ADC].result=E_FCT_RESULT_FAIL;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test FAIL!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);        
        }else{
            printf("ADC test complete!\n");
        }  
    }
}

void FCTTestRTC(T_CHAR* case_name)
{
    T_U8 time[8]={0,0,0,1,1,7,0x17,0};    //set time 2017.1.1 00:00:00
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key 'main'");
        WaitKeyPressed(E_KEY_MAIN);

        FCTLcdDisplay("SetTime:00:00:00","Press key '+'");
        WaitKeyPressed(E_KEY_PLUS);

        FCTLcdDisplayCenter("Testing ...",8);
    }else{
        FCTLcdDisplay("RTC test"," ");
        printf("Set time:00:00:00\n");
        printf("RTC testing...\n");
    }

    BurstWrite1302(time);
    DelayMs(2100);

    Ds1302TimeRead();

    sprintf(g_display_string, "GetTime:0%d:0%d:0%d", (T_U8)g_current_time.hour,
           (T_U8)g_current_time.minute,(T_U8)g_current_time.second);

    if(g_is_use_continue_mode)
    {
        FCTLcdDisplay(g_display_string,"Press key '-'");
        WaitKeyPressed(E_KEY_MINUS);
    }
    else
    {
        printf("%s\n",g_display_string);
    }

    if(g_current_time.second >= 2)
    {
        g_fct_case[E_FCT_TEST_CASE_RTC].result=E_FCT_RESULT_PASS;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test PASS!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);        
        }else{
            printf("RTC test complete!\n");
        }  
    }
    else
    {
        g_fct_case[E_FCT_TEST_CASE_RTC].result=E_FCT_RESULT_FAIL;

        if(g_is_use_continue_mode){
            FCTLcdDisplay("Test FAIL!","Press key '+'");
            WaitKeyPressed(E_KEY_PLUS);        
        }else{
            printf("RTC test complete!\n");
        }          

    }
}

void FCTTestSleepCurrent(T_CHAR* case_name)
{
    BeepOneTime();

    if(g_is_use_continue_mode){
        FCTLcdDisplay(case_name,"Press key 'Main'");
        WaitKeyPressed(E_KEY_MAIN);
    }else{
        printf("Sleep current testing...\n");
        printf("Please test the sleep current\n");    
    }
    FCTLcdDisplay("'main' wakeup","Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);

    ExcuteSleep();

    //exit low power mode
    DrvPowerExitLowMode();
    // wake up slave MCU(rising edge)
    P1OUT |= BIT4;    
    P1OUT &= ~BIT4;
}

void FCTTestNormalCurrent(T_CHAR* case_name)
{
    Initial();
    SendSpiCommandToSlave(HEAD_SC,COMM_SS,0);
    DelayMs(3); //let the cmd send finish
    BeepOneTime();

    FCTLcdDisplay(case_name,"Press key '+'");
    //WaitKeyPressed(E_KEY_PLUS);
    while(1)
    {
       LPM1;
       CtlKeyScan();
       if(CtlKeyIsPressed(E_KEY_PLUS) == 1)
       {
           DrvKeyPostClearState();
           break;
       }
    }
    //FCTLcdDisplay("if done test","Press key '-'");
    //WaitKeyPressed(E_KEY_MINUS);
}

void FCTTestBacklightOnCurrent(T_CHAR* case_name)
{
    Initial();
    SendSpiCommandToSlave(HEAD_SC,COMM_SS,0);
    DelayMs(3); //let the cmd send finish
    BeepOneTime();
    DrvLcdSetBackLight(E_BACK_LIGHT_ON);

    FCTLcdDisplay(case_name,"Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);

    FCTLcdDisplay("if done test","Press key '+'");
    WaitKeyPressed(E_KEY_PLUS);
    DrvLcdSetBackLight(E_BACK_LIGHT_OFF);
} 
void FCTTestMotorOnCurrent(T_CHAR* case_name)
{
    BeepOneTime();
    P1OUT |= BIT4;    
    P1OUT &= ~BIT4;
    FCTLcdDisplay(case_name,"Press key '-'");
    WaitKeyPressed(E_KEY_MINUS);
    AppStartRun(E_MOVE_FORWARD,5000);

    FCTLcdDisplay("if done test","Press key '+'");
    WaitKeyPressed(E_KEY_PLUS);
    AppStopRun();
}

void TestSpump(T_CHAR* name,T_U16 dose)
{
    T_U32 expect_circle;
    T_U8  is_need_refresh=0;
    FCTLcdDisplay(name,"+go   -next");

    while(1){
        CtlEventHandleAll();
        if (!CtlTimeIsTickHandled())
        {
            CtlTimeSetTickHandled(E_TRUE);
            CtlTimerHandle(ALL_TIMER);
        }

        if(CtlKeyIsPressed(E_KEY_PLUS)){
            expect_circle = DoseToEncoder(E_DOSE_THICKNESS_100, dose);
            CtlDeliveryStart(expect_circle, dose, E_DOSE_THICKNESS_100);
            FCTLcdDisplayCenter("Testing...",8);
            is_need_refresh=1;
        }else if(CtlKeyIsPressed(E_KEY_MINUS)){
            break;
        }
        if(!CtlMotorIsRunning()&& is_need_refresh){
            is_need_refresh=0;
            FCTLcdDisplay(name,"+go   -next");
        };
            
    }
}

T_U32 g_expect_circle;
T_U8 g_dose;
T_U8 g_delivery_counter;

void start_delivery()
{
    T_U32 encoder;
    g_delivery_counter ++;
    
    if(g_expect_circle > g_delivery_deviation_encoder){
      encoder=g_expect_circle-g_delivery_deviation_encoder;

    }
    
    if(g_delivery_counter >= 12){
        g_expect_circle += DoseToEncoder(E_DOSE_THICKNESS_100, g_dose)%12;
        if(g_expect_circle > g_delivery_deviation_encoder)
            encoder=g_expect_circle-g_delivery_deviation_encoder;
    }
    CtlDeliveryStart(encoder, g_dose, E_DOSE_THICKNESS_100);
}

void TestApump(T_CHAR* name,T_U16 dose)
{
    FCTLcdDisplay(name,"+go   -next");

    g_dose=dose;
    while(1){
        CtlEventHandleAll();
        if (!CtlTimeIsTickHandled())
        {
            CtlTimeSetTickHandled(E_TRUE);
            CtlTimerHandle(ALL_TIMER);
        }

        if(CtlKeyIsPressed(E_KEY_PLUS)){
            CtlTimerBindHandle(E_TIMER_COMMON_DELAY_FCT, start_delivery);
            if(g_dose <100)
                CtlTimerSetRepeat(E_TIMER_COMMON_DELAY_FCT, 1000);
            else
                CtlTimerSetRepeat(E_TIMER_COMMON_DELAY_FCT, 2500);
            g_delivery_counter=0;
            g_delivery_deviation_encoder=0;
            g_delivery_actural_encoder=0;
            g_delivery_expect_encoder=DoseToEncoder(E_DOSE_THICKNESS_100, g_dose);
            g_expect_circle = g_delivery_expect_encoder/12;
            FCTLcdDisplayCenter("Testing...",8);

        }else if(CtlKeyIsPressed(E_KEY_MINUS)){
            break;
        }

        if(g_delivery_counter >= 12){
            CtlTimerClear(E_TIMER_COMMON_DELAY_FCT);
            g_delivery_counter=0;
            FCTLcdDisplay(name,"+go   -next");
        }
            
    }
}


void FCTTestPrecision(T_CHAR* case_name)
{
    BeepOneTime();
    FCTLcdDisplay(case_name,"Press key 'Main'");
    WaitKeyPressed(E_KEY_MAIN);

    TestApump("0.5U/h test",5);   // 0.5U/h
    TestApump("1U/h test",10);    // 1U/h
    TestApump("5U/h test",50);    // 5U/h
    TestApump("10U/h test",100);  // 10U/h
    TestApump("24U/h test",240);  // 24U/h

    TestSpump("1ug test",10);     //1ug
    TestSpump("10ug test",100);
    TestSpump("30ug test",300);
}


static TS_FCT_CASE g_fct_case[E_FCT_TEST_CASE_MAX] =
{
    {FCTTestShowVersion, "","fct version"},
    {FCTTestShowSN, "","SN"},
    {FCTTestKey, "1.Keys Test","Keys"},
    {FCTTestLcd, "2.LCD Test","LCD"},
    {FCTTestBuzzer, "3.Buzzer Test","Buzzer"},
    {FCTTestVibrator, "4.Vibrator Test","Vibrator"},
    {FCTTestCom, "5.Com Test","Com"},
    {FCTTestPulse, "6.Pulse Test","Pulse"},
    {FCTTestMotor, "7.Motor Test","Motor"},
    {FCTTestHall, "8.Hall Test","Hall"},
    {FCTTestEncoder, "9.Encoder Test","Encoder"},
    //{FCTTestMotorCurrent, "10.MotorCurrent","Motor Current"},
    {FCTTestADC, "10.ADC Test","ADC"},
    {FCTTestRTC, "11.RTC Test","RTC"},
    {FCTTestSleepCurrent, "12.Sleep mA","SC"},
    {FCTTestNormalCurrent, "13.Normal mA","NC"},
    {FCTTestBacklightOnCurrent, "14.BL on mA","NC"},
    {FCTTestMotorOnCurrent, "15.Motor on mA","WC"},
    {FCTTestPrecision,"16.PrecisionTest",""},
}; 
void FCTTestStart()
{
    T_U16 i,j,k=0;
    TE_BOOLEAN is_all_pass = E_TRUE;

#ifdef _DEBUG
    printf("FCT start!\n");
    printf("Input \'c\' to select continue mode, \'m\' to select menu mode:\n");
    FCTLcdDisplayCenter("Input command...",8);

    while(1)
    {
        if (UartGetNewDataFlag(E_UART0)) {
            UartClearNewDataFlag(E_UART0);
            CommHandleNewReceiveData(E_COMM_PC_UART);
        }
        
        if(g_is_get_mode_cmd ) break;
    }
    printf("\n");
#endif

a:
    if(g_is_use_continue_mode) {

        FCTLcdDisplayCenter("FCT Start!",8);
        DelayMs(1000);

        for (i = 0; i < E_FCT_TEST_CASE_MAX; i++) {//E_FCT_TEST_CASE_PRECISION_TEST
            //suppose the case is ok before test
            g_fct_case[i].result = E_FCT_RESULT_PASS;
            //run the test
            g_fct_case[i].fun(g_fct_case[i].case_name);
        }

        for (i = 0; i < E_FCT_TEST_CASE_MAX; i++) {
            if(g_fct_case[i].result == E_FCT_RESULT_FAIL) {
                is_all_pass = E_FALSE;
                break;
            }
        }

        if (is_all_pass == E_TRUE) {
            BeepOneTime();
            FCTLcdDisplayCenter("FCT OK!",8); 
            while(1);
        } else {
            BeepOneTime();
            DisplayClearAll();
            DisplayFlushDirty();
            for (i = 0,j = 0; i < E_FCT_TEST_CASE_MAX; i++) {
                if (g_fct_case[i].result == E_FCT_RESULT_FAIL) {
                    sprintf(g_display_string, "%d",i-1);
                    if (i < 10) {
                        DisplayTextAtPos(j*12,16,g_display_string);
                        k++;
                    } else {
                        DisplayTextAtPos(k*12+20*(j-k),16,g_display_string);
                    }
                    j++;
                }
            }
            DisplayTextAtXCenter(0,"FCT FAIL!");
            DisplayFlushDirty();
            while(1);
        }    
    } else {
        printf("Please select case by input the num:\n");
        printf("0: Complete Test\n");

        for (i=0; i<E_FCT_TEST_CASE_MAX; i++)
        {
            printf("%d: %s\n", i+1, g_fct_case[i].print_name);
        }
        printf("\n");

        for (i=E_FCT_TEST_CASE_HALL; i<E_FCT_TEST_CASE_MAX; i++)
        {
            g_fct_case[i].result = E_FCT_RESULT_FAIL;
        }

        while(1)
        {
            if (UartGetNewDataFlag(E_UART0))
            {
                UartClearNewDataFlag(E_UART0);
                CommHandleNewReceiveData(E_COMM_PC_UART);
            }
            if (g_case_num == 0)
            {
                //test complete
                break;
            }
            else if (g_case_num>0 && g_case_num<=E_FCT_TEST_CASE_MAX)
            {
                g_case_num--;          
                g_fct_case[g_case_num].fun(g_fct_case[g_case_num].case_name);
                g_case_num=12;
                printf("\n");
            }
            else if (g_case_num == 13)
            {
                g_case_num=11;
                goto a;
            }

        }

        printf("\nFCT test result:\n");
        for (i=E_FCT_TEST_CASE_HALL; i<E_FCT_TEST_CASE_MAX; i++)
        {
            //check if all case is ok 
            if(g_fct_case[i].result == E_FCT_RESULT_FAIL)
            {
                printf("%s FAIL!\n", g_fct_case[i].print_name);
            }
            else if(g_fct_case[i].result == E_FCT_RESULT_PASS)
            {
                printf("%s PASS!\n", g_fct_case[i].print_name);
            }
        }
 
        FCTLcdDisplayCenter("FCT Complete!",8);
        printf("\nFCT Complete!\n");
        printf("Turn off the circuit!\n");
        while(1);
    }
}

