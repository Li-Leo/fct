/*******************************************************************************
**文件名:DrvTimer.c
**
**功能:
**
**作者:HJH
**
**日期:2016-04-18
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#include "DrvTimer.h"
#include "..\Kernel\CtlTime.h"
#include "DrvMotor.h"
#include "CtlMotor.h"

#define TOTAL   6
#define ON      2

T_U32 g_time_ms_elapse = 0;
//T_U16 timer_count = 0;

/*******************************************************************************
**函数名:void DrvTimerInital(void)
**
**功能:配置定时器TA0，TA1，TA2，TA3和TB0，具体配置内容包括
**
**作者：HJH
**
**日期：2016-04-18
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvTimerInitial(void)
{
  //TA0CTL = TASSEL_2|ID_3; //定时器TA0的时钟源为SMCLK，使能定时器中断，停止定
                          //时器工作
  //TA0CCTL0 |= CCIE;       //使能定时器TA0CCR0.CCIFG中断
/******************************************************************************/  
  //TA1CTL |= TASSEL__ACLK|ID__2;//定时器TA1的时钟源为ACLK，使能定时器中断，停止定
  //TA1EX0 |= TAIDEX_4 ;    // 1/5   tclk=aclk/10=1k                //时器工作
  //TA1CCTL0 |= CCIE;       //使能定时器TA1CCR0.CCIFG中断
/******************************************************************************/  
  TA2CTL |= TASSEL__SMCLK | ID__2; //定时器TA2的时钟源为SMCLK，使能定时器中断，停止定
                          //时器工作
  //TA2CCTL0 |= CCIE;       //使能定时器TA2CCR0.CCIFG中断
/******************************************************************************/  
  TA3CTL |= TASSEL__ACLK|ID__2;//定时器TA1的时钟源为ACLK，使能定时器中断，停止定
  TA3EX0 |= TAIDEX_4 ;    // 1/5   tclk=aclk/10=1k                //时器工作
  //TA3CCTL0 |= CCIE;       //使能定时器TA3CCR0.CCIFG中断
  /******************************************************************************/  
  TB0CTL |= TBSSEL_2;     //定时器TA1的时钟源为SMCLK，使能定时器中断，停止定
                            //时器工作
  //TB0CCTL0 |= CCIE;       //使能定时器TA1CCR0.CCIFG中断

}

/*******************************************************************************
**函数名:void DrvTimerEnable(T_U16 timer_count,TE_TIMER_CHANNEL cha)
**
**功能:开启某个定时器，并设定定时时长。timer_count:需要定时的时长，定时以100uS位基
**
**本定时单位，如timer_count = 2，则表示定时200uS；cha:用于定时的定时器通道号
**
**作者：HJH
**
**日期：2016-04-18
**
**修改内容：取消传递参数timer_count，改为在函数体中由宏定义赋值。
**    timer_count:需要定时的时长，定时以1ms位基本定时单位
**    如timer_count = 2，则表示定时2ms;

**
**作者：Kevin Shi
**
**日期：
*******************************************************************************/
void DrvTimerEnable(TE_TIMER_CHANNEL cha)
{
  T_U16 timer_count = MSECS_PER_TICK;//将要写入定时器计数寄存器的值
  switch(cha)
  {
    case E_TIMER_CHANNEL_A0://设置定时器TA0计数值
      {
        TA0CCR0 = timer_count;
        TA0CTL |= MC_1;//启动增计时
        TA0CCTL0 |= CCIE;//使能定时器TA0CCR0.CCIFG中断
      }
      break;
      case E_TIMER_CHANNEL_A1://设置定时器TA1计数值为每1ms进一次中断
      {
        TA1CCR0 = timer_count;
        TA1CTL |= MC_1;//启动增计时
        TA1CCTL0 |= CCIE;//使能定时器TA1CCR0.CCIFG中断
      }
      break;
      case E_TIMER_CHANNEL_A2://设置定时器TA2计数值为每5ms进一次中断
      {
        TA2CCR0 = 2499;
        TA2CTL |= MC_1;//启动增计时
        TA2CCTL0 |= CCIE;//使能定时器TA2CCR0.CCIFG中断
      }
      break;
      case E_TIMER_CHANNEL_A3://设置定时器TA3计数值
      {
        TA3CCR0 = timer_count* 100;
        TA3CTL |= MC_1;//启动增计时
        TA3CCTL0 |= CCIE;//使能定时器TA3CCR0.CCIFG中断
      }
      break;
      case E_TIMER_CHANNEL_B0://设置定时器TB0计数值
      {
        TB0CCR0 = timer_count;
        TB0CTL |= MC_1;//启动增计时
        TB0CCTL0 |= CCIE;//使能定时器TB0CCR0.CCIFG中断
      }
      break;
  default:
    ;
    break;
  }
}

/*******************************************************************************
**函数名:void DrvTimerDisable(TE_TIMER_CHANNEL cha)
**
**功能:禁止某个定时器工作，并清除定时器中断标志
**
**作者：HJH
**
**日期：2016-04-18
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
void DrvTimerDisable(TE_TIMER_CHANNEL cha)
{
   switch(cha)
  {
    case E_TIMER_CHANNEL_A0://设置定时器TA0计数值
      {
        TA0CTL &= ~MC_1;//停止增计时
        TA0CCTL0 &= ~CCIE;//禁止定时器TA0CCR0.CCIFG中断
        TA0CCTL0 &= ~CCIFG;//清除定时器中断标志
      }
      break;
      case E_TIMER_CHANNEL_A1://设置定时器TA1计数值
      {
        TA1CTL &= ~MC_1;//停止增计时
        TA1CCTL0 &= ~CCIE;//禁止定时器TA1CCR0.CCIFG中断
        TA1CCTL0 &= ~CCIFG;//清除定时器中断标志
      }
      break;
      case E_TIMER_CHANNEL_A2://设置定时器TA2计数值
      {
        TA2CTL   &= ~MC_1;//停止增计时
        TA2CCTL0 &= ~CCIE;//禁止定时器TA2CCR0.CCIFG中断
        TA2CCTL0 &= ~CCIFG;//清除定时器中断标志
      }
      break;
      case E_TIMER_CHANNEL_A3://设置定时器TA3计数值
      {
        TA3CTL   &= ~MC_1;//停止增计时
        TA3CCTL0 &= ~CCIE;//禁止定时器TA3CCR0.CCIFG中断
        TA3CCTL0 &= ~CCIFG;//清除定时器中断标志
      }
      break;
      case E_TIMER_CHANNEL_B0://设置定时器TB0计数值
      {
        TB0CTL   &= ~MC_1;//停止增计时
        TB0CCTL0 &= ~CCIE;//禁止定时器TB0CCR0.CCIFG中断
        TB0CCTL0 &= ~CCIFG;//清除定时器中断标志
      }
      break;
  default:
    ;
    break;
  }
}

/*******************************************************************************
**函数名:__interrupt void TimerA0Isr(void)
**
**功能:定时Timer_A TA0中断服务函数，
**
**作者：HJH
**
**日期：2016-04-19
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0Isr(void)
{
    #ifdef _DEBUG 
    LPM3_EXIT;
    #endif

}

/*******************************************************************************
**函数名:__interrupt void TimerA1Isr(void)
**
**功能:定时Timer_A TA1中断服务函数，
**
**作者：HJH
**
**日期：2016-04-19
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TimerA1Isr(void)
{
    #ifdef _DEBUG 
  LPM3_EXIT;
  #endif
}

/*******************************************************************************
**函数名:__interrupt void TimerA2Isr(void)
**
**功能:定时Timer_A TA2中断服务函数，
**
**作者：HJH
**
**日期：2016-04-19
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2Isr(void)
{
	CtlTimeOnOneTick();
    LPM1_EXIT;
}

/*******************************************************************************
**函数名:__interrupt void TimerA3Isr(void)
**
**功能:定时Timer_A TA3中断服务函数，
**
**作者：HJH
**
**日期：2016-04-19
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#pragma vector = TIMER3_A0_VECTOR
__interrupt void TimerA3Isr(void)
{
//	  T_U8 static count_s=0;
//	  if(count_s<ON)
//	    CtlMotorStart(E_MOVE_FORWARD); //plate move forward
//	  else
//	    CtlMotorStop();
//	  
//	  if(count_s<TOTAL)
//	    count_s++;
//	  else
//	    count_s=0;
  DrvTimerDisable(E_TIMER_CHANNEL_A3);
    #ifdef _DEBUG 
  LPM3_EXIT;
  #endif
}

/*******************************************************************************
**函数名:__interrupt void TimerB0Isr(void)
**
**功能:定时Timer_B TB0中断服务函数，
**
**作者：HJH
**
**日期：2016-04-19
**
**修改内容：
**
**作者：
**
**日期：
*******************************************************************************/
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0Isr(void)
{
  DrvTimerDisable(E_TIMER_CHANNEL_B0);
    #ifdef _DEBUG 
  LPM3_EXIT;
  #endif
}

