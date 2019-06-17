/********************************
********    Master MCU   ********
********************************/
/*******************************************************************************
**文件名:main.c
**
**功能:
**
**作者:
**
**日期:2016-03-01
**
*******************************************************************************/
#include <msp430fr5989.h>
#include "App\Init.h"
//#include "App\AppuiScreen.h"
#include "CtlKey.h"
#include "CtlTime.h"
#include "CtlTimer.h"
#include "..\Kernel\CtlDisplay.h"
#include "..\Kernel\CtlDelivery.h"
#include "..\Kernel\CtlData.h"
#include "..\Kernel\Fonts.h"
#include "..\Driver\DrvLcd.h"
#include "..\Driver\DrvPower.h"
#include "..\Driver\DrvRtc.h"
#include "CtlSpiComm.h"
#include "CtlEvent.h"
#include "DrvUart.h"
#include "CtlComm.h"
//#include "AppDelivery.h"
#include "intrinsics.h"
#include "CtlMotor.h"
//#include "AppMonitor.h"
//#include "AppAlarm.h"
//#include "CtlCommon.h"
#include "AppFCT.h"

//may be SYSRSTIV_BOR,SYSRSTIV_RSTNMI,SYSRSTIV_MPUSEGPIFG
T_U16 g_power_up_cause;

/*extern TS_RECORD_POINT g_record_point;
extern TS_TIME_INFO g_current_time;

void WriteTestRecords()
{
	TS_RECORD record;
	Ds1302TimeRead();
	T_U16 idx;
	record.rec_type = E_RECORD_INFUSION;
	record.rec_time = g_current_time;
	record.rec_para = 100;
	for(idx = 0; idx < 7; idx++)
	{
		record.rec_time.hour ++;
		if(record.rec_time.hour > 23)
		{
			record.rec_time.hour = 0;
			record.rec_time.date ++;
		}
		LoadSysPara(E_SYSTEM_PARA_RECORD_INDEX, 0);
		record.rec_id = g_record_point.p_infusion_rec;
		SaveRecEx(record);
	}

	record.rec_type = E_RECORD_DAYS_TOTAL;
	record.rec_time = g_current_time;
	record.rec_para = 1600;
	for(idx = 0; idx < 7; idx++)
	{
		record.rec_time.date ++;
		if(record.rec_time.date > 30)
		{
			record.rec_time.date = 1;
			record.rec_time.month ++;
		}
		LoadSysPara(E_SYSTEM_PARA_RECORD_INDEX, 1);
		record.rec_id = g_record_point.p_daytotal_rec;
		SaveRecEx(record);
	}
	
	record.rec_type = E_RECORD_STOP_DELI;
	record.rec_time = g_current_time;
	record.rec_para = 17;
	for(idx = 0; idx < 7; idx++)
	{
		record.rec_time.hour ++;
		if(record.rec_time.hour > 23)
		{
			record.rec_time.hour = 0;
			record.rec_time.date ++;
		}
		LoadSysPara(E_SYSTEM_PARA_RECORD_INDEX, 2);
		record.rec_id = g_record_point.p_stopinfu_rec;
		SaveRecEx(record);
	}
}*/

//check previous run is normal power off
// void CheckPreviousRunState()
// {
//     //check if abnormal reset
//     LoadSysPara(E_SYSTEM_PARA_SIN_RST_FLAG, 0);
// 	if (g_mcu_abnormal_reset_state == E_TRUE)
// 	{
//         M_CommonPrint("not safety power off last time\n");
// 		SaveRec(E_RECORD_OPERATION, 92, g_power_up_cause);
// 	}
// 	else
// 	{
// 		g_mcu_abnormal_reset_state = E_TRUE;
// 		SaveSysPara(E_SYSTEM_PARA_SIN_RST_FLAG, 0);
// 	}

//     LoadSysPara(E_SYSTEM_PAPA_OCCLUSION_ALARM_FLAG, 0);
//     if (g_occlusion_alarm_is_set== E_TRUE)
// 	{
// 		//AlarmSet(E_ALARM_OCCLUSION_ID);
// 		MonitorCheckAlarm();
// 	}

//     //check if is infusioning at last power on
//     LoadSysPara(E_SYSTEM_PARA_DELI_START_MOTOR_POS, 0);
//     LoadSysPara(E_SYSTEM_PARA_DELI_IS_RUNNING, 0);
// 	if (g_deliver_is_running)
// 	{
//         g_deliver_is_running = 0;
//         SaveSysPara(E_SYSTEM_PARA_DELI_IS_RUNNING, 0);    

//         if(g_motor_position<=g_deliver_start_motor_pos)
//         {
//             if(g_deliver_start_motor_pos>=g_motor_position)
//             {
//                 T_U32 encoder = g_deliver_start_motor_pos - g_motor_position;
     
//                 SaveRec(E_RECORD_INFUSION, EncoderToDose(g_sys_para_thickness, encoder), 0);
//             }
//         }
// 	}
	    
// }


__intrinsic int __low_level_init(void)
{

  WDTCTL = WDTPW + WDTHOLD;     //关看门狗 

   /*
  * Return value:
  *
  *  1 - Perform data segment initialization.
  *  0 - Skip data segment initialization.
  */

  return 1;
}

TE_BOOLEAN g_gpio_signal_status = E_TRUE;

void gpio_toggle()
{
    if(g_gpio_signal_status == E_FALSE)
    {
        PJOUT &= ~(BIT0|BIT1|BIT2|BIT3);
        P3OUT &= ~(BIT4|BIT5);
        g_gpio_signal_status = E_TRUE;
    }
    else if(g_gpio_signal_status == E_TRUE)
    {
        PJOUT |= (BIT0|BIT1|BIT2|BIT3);
        P3OUT |= (BIT4|BIT5);
        g_gpio_signal_status = E_FALSE;
    }
}
void main()
{
  WDTCTL = WDTPW + WDTHOLD;     //关看门狗
  PM5CTL0 &= ~LOCKLPM5; 
  __enable_interrupt();
  g_power_up_cause = SYSRSTIV;
  //P1IE &= ~BIT1;

  //g_batt_number = 3000;
  Initial();
  //g_motor_position = 152000;
  //SaveMotorPos(1);
  //M_Trace("Spump18 master start\n");
  //M_Trace("power up cause:0x%X\n",g_power_up_cause);  

  //WriteTestRecords();

  //CtlSetCurrentMode(E_SYSTEM_MODE_NORMAL);
  //ShowStartupPage();
  //SetNextDeliTimePoint();
  
  //CheckPreviousRunState();
  //CtlTimerSet(E_TIMER_NO_OPERATION_SLEEP, NO_OPERATION_SLEEP_MS);	//NO_OPERATION_SLEEP_MS
  
  // wake up slave MCU(rising edge)
  P1OUT |= BIT4;    
  P1OUT &= ~BIT4;
  
  //CtlTimerBindHandle(E_TIMER_COMMON_DELAY, gpio_toggle);
  //CtlTimerSetRepeat(E_TIMER_COMMON_DELAY, 5);

  //DisplayUI(E_SCREEN_HOME);

  //CtlEventPost(E_EVENT_ENTER_STOP_STATE, 1);
  //PulseSignalSendStart();
  //monitor slave
  //ExternSysMonitorStart();
    while(1)
    {
      CtlEventHandleAll();
      if (!CtlTimeIsTickHandled())
      {
        CtlTimeSetTickHandled(E_TRUE);
        CtlTimerHandle(ALL_TIMER);
      }
  
      //handle new received data from usart
      if (UartGetNewDataFlag(E_UART0))
      {
          UartClearNewDataFlag(E_UART0);
          CommHandleNewReceiveData(E_COMM_PC_UART);
      }
      if (UartGetNewDataFlag(E_UART1))
      {
          UartClearNewDataFlag(E_UART1);
          CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
      }
 
    }
    


  FCTTestStart();

  //tell slaver power up
  SendSpiCommandToSlave(HEAD_SC, COMM_PU, 0);

#if 0  //when debug, you can enable it to let amcu not monitor
  //let slave not monitor me
  SendSpiCommandToSlave(HEAD_SC, COMM_WD, 0);
  //stop monitor slave
  ExternSysMonitorStop();
#endif

  while(1)
  {
    CtlEventHandleAll();
    if (!CtlTimeIsTickHandled())
    {
      CtlTimeSetTickHandled(E_TRUE);
      CtlTimerHandle(ALL_TIMER);
    }

    if(g_encoder_is_monitor && g_encoder_need_stop_flag==1)
    {
        if(!CtlMotorIsStoping() && CtlMotorIsRunning())
        {
            CtlMotorStopEx(E_MOTOR_STOP, g_motor_stop_cause, 0);
        }
        g_encoder_is_monitor = 0; 
    }

    if(DisplayIsDirty())
    {
        DisplayFlushDirty();
    }
    
    //handle new received data from usart
    if (UartGetNewDataFlag(E_UART0))
    {
        UartClearNewDataFlag(E_UART0);
        CommHandleNewReceiveData(E_COMM_PC_UART);
    }
    if (UartGetNewDataFlag(E_UART1))
    {
        UartClearNewDataFlag(E_UART1);
        CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
    }

    if(CtlEventIsEmpty() && !DisplayIsDirty() &&
       CtlTimeIsTickHandled() )
    {   
        __bis_SR_register(LPM1_bits + GIE);      // LPM1, add GIE for debugger
    }
  }
  
}

#pragma vector = SYSNMI_VECTOR//非屏蔽中断
__interrupt void SYSNMI_ISR(void)
{
    switch (__even_in_range(SYSSNIV, SYSSNIV_CBDIFG)) {
    case SYSSNIV_NONE:       break;
    case SYSSNIV_SVS:        break;
    case SYSSNIV_UBDIFG:     break;
    case SYSSNIV_ACCTEIFG:   break;
    case SYSSNIV_MPUSEGPIFG: break;
    case SYSSNIV_MPUSEGIIFG: break;
    case SYSSNIV_MPUSEG1IFG:         // segment 1
    case SYSSNIV_MPUSEG2IFG:                // segment 2
    case SYSSNIV_MPUSEG3IFG:          // segment 3

        //write can not write area
        printf("Fault error: violation!\n");
        
        // Clear violation interrupt flag
        //MPU_clearInterruptFlag(__MSP430_BASEADDRESS_MPU__, MPUSEG2IFG);

        M_Assert(0);
        while (1);
        break;
    case SYSSNIV_VMAIFG:     break;
    case SYSSNIV_JMBINIFG:   break;
    case SYSSNIV_JMBOUTIFG:  break;
    case SYSSNIV_CBDIFG:     break;
    default:                 break;
    }
}

