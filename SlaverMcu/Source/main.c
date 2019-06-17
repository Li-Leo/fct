/********************************
********    Slaver MCU   ********
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
#include "CtlEvent.h"
#include "CtlTime.h"
#include "CtlTimer.h"
#include "CtlSpiComm.h"
#include "DrvHall.h"
#include "DrvUart.h"
#include "CtlComm.h"
#include "AppMonitor.h"
#include "AppEventHandle.h"
#include "CtlMotor.h"

extern TE_BOOLEAN g_motor_is_running;
extern T_U16 g_hz_value;
extern T_U8  g_motor_current_done;
//may be SYSRSTIV_BOR,SYSRSTIV_RSTNMI,SYSRSTIV_MPUSEGPIFG
T_U16 g_power_up_cause;

TE_BOOLEAN g_gpio_signal_status = E_TRUE;
void gpio_toggle()
{
    if(g_gpio_signal_status == E_FALSE)
    {
        //PJOUT &= ~(BIT0|BIT1|BIT2|BIT3);
        P3OUT &= ~(BIT4|BIT5);
        g_gpio_signal_status = E_TRUE;
    }
    else if(g_gpio_signal_status == E_TRUE)
    {
        //PJOUT |= (BIT0|BIT1|BIT2|BIT3);
        P3OUT |= (BIT4|BIT5);
        g_gpio_signal_status = E_FALSE;
    }
}

void main()
{
  WDTCTL = WDTPW + WDTHOLD;   //关看门狗
  PM5CTL0 &= ~LOCKLPM5; 
  //__enable_interrupt();
  __bis_SR_register(GIE);     //global interrupt enable
  g_power_up_cause = SYSRSTIV;
  Initial();
  M_Trace("Spump18 slaver start\n");
  M_Trace("power up cause:0x%X\n",g_power_up_cause);  
  
  //start timer to wait for MCU power on
  //CtlTimerBindHandle(E_TIMER_WAITING_MCU_RESET, AppTimerHandleAfterMasterMcuReset);
  //CtlTimerSet(E_TIMER_WAITING_MCU_RESET, MCU_RESET_TIME_MS);
  //PulseSignalSendStart();
  //P4OUT |= BIT0|BIT3;  //batt_che_en
  //P3OUT |= BIT2;       //batt_v_che_en

  //DrvMotorSetRun(E_MOVE_FORWARD);
  //CtlTimerBindHandle(E_TIMER_COMMON_DELAY, gpio_toggle);
  //CtlTimerSetRepeat(E_TIMER_COMMON_DELAY, 5);

  while(1)
  {
    CtlEventHandleAll();
    if (!CtlTimeIsTickHandled())
    {
      CtlTimeSetTickHandled(E_TRUE);
      CtlTimerHandle();
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
    
    if(g_motor_is_running == E_TRUE)
    {
      if((P1IN&BIT2) == 0)
         g_motor_is_meet_hall = E_TRUE;

      if(g_motor_current_done == 1){
        g_motor_current_done=0;
        SetSpiCommandToMaster(HEAD_FQ,COMM_MC,g_hz_value,1);
      }
      if(g_motor_is_reach_expect_encoder)
      {
        M_CommonPrint("finish encoder, stop\n");
        SetSpiCommandToMaster(HEAD_FC,COMM_MS,1,0);
        CtlMotorStop();
      }
      if(g_motor_is_need_check_hall && g_motor_is_meet_hall)
      {
        g_motor_is_need_check_hall = E_FALSE;
        SetSpiCommandToMaster(HEAD_FC,COMM_TH,0,0);//tell master have meet hall
      }
    }

    if((SCAN_PULSE_SIGNAL_STATUS & BIT1) != 0){
        PULSE_SIGNAL_HIGH;
    } else {
        PULSE_SIGNAL_LOW;
    }

    //if(CtlEventIsEmpty() && CtlTimeIsTickHandled())
    {
        //LPM1;
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




