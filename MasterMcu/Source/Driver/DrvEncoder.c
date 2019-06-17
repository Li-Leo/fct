/*******************************************************************************
**文件名:DrvEncoder.c
**
**功能:
**
**作者:HJH
**
**日期:2016-04-11
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#include "DrvEncoder.h"
#include "DrvPower.h"
#include "..\App\AppMonitor.h"
#include "..\App\AppDelivery.h"
#include "..\Kernel\CtlData.h"
#include "..\Kernel\CtlSpiComm.h"
#include "ctlmotor.h"
#include "CtlData.h"
#include "stdio.h"

#define ENCODER_ENABLE          (P4OUT |= BIT2)//编码器使能
#define ENCODER_DISABLE         (P4OUT &= ~BIT2)//禁止编码器

T_U32 g_encoder_counter;
T_U32 g_encoder_expect_counter;

//1:wakeup by port, 0: not wakeup by port
static T_U8 g_port_wakeup_flag= 0;
static T_U8 g_encoder_is_disable_monitor_motor_pos = 0;
T_U8 g_encoder_is_monitor = 0;
T_U8 g_encoder_need_stop_flag = 0;

T_U16 g_encoder_hz_counter=0;
T_U16 g_encoder_cap_start=0;
T_U16 g_encoder_cap_end=0;
T_U32 g_encoder_cap_period=0;
T_U8  g_encoder_Hz_done = 0;
T_U8  g_encoder_carry_cntr=0;
float g_encoder_hz=0.0;
T_U16 g_encoder_hz_result=0;
TE_BOOLEAN g_encoder_is_hz_result_ready = E_FALSE;



/*******************************************************************************
**函数名:void DrvEncoderInital(void)
**
**功能:对编码器进行初始化操作，即对光电编码开关进行使能，开启相应中断等。
**
**作者：HJH
**
**日期：2016-05-07
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvEncoderInitial(void)
{ 
  P1SEL1 &= ~BIT0;
  P1SEL0 |=  BIT0;    // P1.0 TA0.CCI1A
  P1DIR  &= ~BIT0;    // P1.0 input
  //ENCODER_ENABLE;   // power on encoder  p4.2
  
  TA0CCTL1 |= CM_1 | CCIS_0 | SCS | CAP | CCIE;// Capture rising edge,
                                               // Use CCI2A
                                               // Synchronous capture,
                                               // Enable capture mode,
                                               // Enable capture interrupt

  TA0CTL = TASSEL__SMCLK|ID_3;                 // Use SMCLK as clock source

  //LoadSysPara(E_SYSTEM_PARA_MOTOR_POSITION, 0);
}

//enable encode count
void DrvEncoderEnable(void)
{
    g_encoder_is_disable_monitor_motor_pos = 0;
    g_encoder_is_monitor = 1;
    g_encoder_need_stop_flag = 0;
    ENCODER_ENABLE;
    // Enable capture mode,
    // Enable capture interrupt
    TA0CCTL1 |= CAP | CCIE;

    TA0CTL |= TACLR |MC__CONTINUOUS |TAIE;         // start timer, continuous

    g_encoder_hz_counter=0;
    g_encoder_cap_start=0;
    g_encoder_cap_end=0;
    g_encoder_cap_period=0;
    g_encoder_Hz_done = 0;
    g_encoder_carry_cntr=0;
    g_encoder_is_hz_result_ready = E_FALSE;

}
//disable encode count
void DrvEncoderDisable(void)
{
    ENCODER_DISABLE;
    g_encoder_is_monitor = 0;
    // Disable capture mode,
    // Disable capture interrupt
    TA0CCTL1 &= ~(CAP | CCIE);

    TA0CTL &= ~(MC__CONTINUOUS|TAIE);             // stop timer
}

//call it can disable motor pos monitor at this running
void DrvEncoderDisableMotorPosMonitorOnce()
{
    g_encoder_is_disable_monitor_motor_pos = 1;
}

/*******************************************************************************
**函数名:T_U32 DrvEncoderCounter(void)
**
**功能:编码计数，对电机转过的码数进行计数
**
**作者：HJH
**
**日期：2016-05-07
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
T_U32 DrvGetEncoderCounter(void)
{
  return g_encoder_counter;
}
/***********************************************
* Description:
*   clear counter of motor, reset to 0
* Argument:
*   param:
*   none
* Return:
*   none
************************************************/
void DrvClearEncoderCounter(void)
{
  g_encoder_counter=0;
}
/*******************************************************************************
**函数名:void DrvEncoderSetSleep(void)
**
**功能:禁止光电编码开关使能，似的光电编码电路处于睡眠的低功耗状态
**
**作者：HJH
**
**日期：2016-05-07
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvEncoderSetSleep(void)
{
  ENCODER_DISABLE;    // power off encoder  p4.2
}

//1:wakeup by port, 0: not wakeup by port
T_U8 DrvPortWakeupFlag()
{
    return g_port_wakeup_flag;
}
//clear the flag as 0
void DrvPortClearWakeupFlag()
{
    g_port_wakeup_flag = 0;
}

/*******************************************************************************
**函数名:__interrupt void Port1Isr(void)
**
**功能:端口1中断服务函数，对端口产生的各类中断进行处理。
**
**作者：HJH
**
**日期：2016-05-07
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#pragma vector = PORT1_VECTOR   //端口1中断
__interrupt void Port1Isr(void) //Port1中断服务函数，用于对产生的端口中断事件进行处理
{
  ENCODER_INT_DIS;          //禁止编码器中断
  
  if(MAIN_INT_FLAG)         //处理主按键中断
  {
  	g_port_wakeup_flag = 1;
     LPM3_EXIT;
     //ExcuteManualWakeup();
  }
	
  PORT1_INT_FLAG_CLR;       //清除中断标志
  //_EINT();                  //开总中断
}

/***********************************************
* Description:
*   TA0 Interrupt Handler
* Argument:
*   param:
*   none
* Return:
*   none
************************************************/
// Timer0_A1 CC1-2, TA Interrupt Handler
#pragma vector = TIMER0_A1_VECTOR
  __interrupt void Timer0_A1_ISR(void)
{
  switch (__even_in_range(TA0IV, TA0IV_TAIFG)) {
    case TA0IV_TA0CCR1:

//			if(g_encoder_counter == 0 && g_encoder_expect_counter == 0)
//				return;
        
        g_encoder_counter++;
        
        if(g_encoder_counter >= g_encoder_expect_counter)
        {
            g_motor_stop_cause = E_MOTOR_STOP_BY_FINISH_EXPECT_TARGET;
            g_encoder_need_stop_flag = 1;
			//MOTOR_POWER_OFF;                //motor power off
			//DrvEncoderDisable(); //disable encoder
        }		     
        
//        if(g_motor_running_dir == E_MOVE_BACKWARD)
//        {
//		    g_motor_position++;
//            SaveMotorPos(E_FALSE);
//        }
//        else if(g_motor_running_dir == E_MOVE_FORWARD)
//        {
//            if (g_motor_position > 0)
//			{
//                g_motor_position--;
//                SaveMotorPos(E_FALSE);
//            }
//        }
//        
//        if(g_motor_running_dir == E_MOVE_FORWARD
//            && g_motor_position == 0
//            && !g_encoder_is_disable_monitor_motor_pos)
//		{
//            //MOTOR_POWER_OFF;                //motor power off
//            g_encoder_need_stop_flag = 1;
//            g_motor_stop_cause = E_MOTOR_STOP_BY_POS_ZERO;
//		    //DrvEncoderDisable(); //disable encoder
//		}

      if(g_encoder_hz_counter < 1){
        g_encoder_cap_start=TA0CCR1;
        g_encoder_Hz_done = 0;
        
      }
      
      if(g_encoder_hz_counter >= ENCODER_NUMBER_GET){ 
        g_encoder_cap_end=TA0CCR1;
        g_encoder_Hz_done=1;
      }
      g_encoder_hz_counter++;

      break;
    case TA0IV_TA0CCR2:
      break;
    case TA0IV_TA0IFG:

      if(g_encoder_hz_counter < ENCODER_NUMBER_GET && g_encoder_hz_counter >= 1)
      { 
        g_encoder_carry_cntr ++;
      }  

      break;
    default:
      break;
  }

  if(g_encoder_Hz_done){

    g_encoder_Hz_done=0;        
    g_encoder_hz_counter=0;
        
    if(g_encoder_carry_cntr<1)
      g_encoder_cap_period=g_encoder_cap_end-g_encoder_cap_start;
    else
    {
      if( g_encoder_cap_end >= g_encoder_cap_start)
      {
        g_encoder_cap_period=(MAX_T_U16*(T_U32)g_encoder_carry_cntr)+(T_U32)g_encoder_cap_end-(T_U32)g_encoder_cap_start;
      } 
      else
      {
        g_encoder_cap_period=(MAX_T_U16*((T_U32)g_encoder_carry_cntr-1))+MAX_T_U16-(T_U32)g_encoder_cap_start+(T_U32)g_encoder_cap_end;
      }
    }
    g_encoder_carry_cntr=0;
    
    g_encoder_hz=(float)ENCODER_NUMBER_GET/((float)g_encoder_cap_period/125000);
    g_encoder_is_hz_result_ready = E_TRUE;
    g_encoder_hz_result=(T_U16)(g_encoder_hz);

  }

  LPM1_EXIT;

}

/********************  end of file ****************************/
