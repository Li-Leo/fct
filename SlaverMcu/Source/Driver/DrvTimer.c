/*******************************************************************************
**�ļ���:DrvTimer.c
**
**����:
**
**����:HJH
**
**����:2016-04-18
**
**�޸�����:
**
**����:
**
**����:
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
**������:void DrvTimerInital(void)
**
**����:���ö�ʱ��TA0��TA1��TA2��TA3��TB0�������������ݰ���
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvTimerInitial(void)
{
  //TA0CTL = TASSEL_2|ID_3; //��ʱ��TA0��ʱ��ԴΪSMCLK��ʹ�ܶ�ʱ���жϣ�ֹͣ��
                          //ʱ������
  //TA0CCTL0 |= CCIE;       //ʹ�ܶ�ʱ��TA0CCR0.CCIFG�ж�
/******************************************************************************/  
  //TA1CTL |= TASSEL__ACLK|ID__2;//��ʱ��TA1��ʱ��ԴΪACLK��ʹ�ܶ�ʱ���жϣ�ֹͣ��
  //TA1EX0 |= TAIDEX_4 ;    // 1/5   tclk=aclk/10=1k                //ʱ������
  //TA1CCTL0 |= CCIE;       //ʹ�ܶ�ʱ��TA1CCR0.CCIFG�ж�
/******************************************************************************/  
  TA2CTL |= TASSEL__SMCLK | ID__2; //��ʱ��TA2��ʱ��ԴΪSMCLK��ʹ�ܶ�ʱ���жϣ�ֹͣ��
                          //ʱ������
  //TA2CCTL0 |= CCIE;       //ʹ�ܶ�ʱ��TA2CCR0.CCIFG�ж�
/******************************************************************************/  
  TA3CTL |= TASSEL__ACLK|ID__2;//��ʱ��TA1��ʱ��ԴΪACLK��ʹ�ܶ�ʱ���жϣ�ֹͣ��
  TA3EX0 |= TAIDEX_4 ;    // 1/5   tclk=aclk/10=1k                //ʱ������
  //TA3CCTL0 |= CCIE;       //ʹ�ܶ�ʱ��TA3CCR0.CCIFG�ж�
  /******************************************************************************/  
  TB0CTL |= TBSSEL_2;     //��ʱ��TA1��ʱ��ԴΪSMCLK��ʹ�ܶ�ʱ���жϣ�ֹͣ��
                            //ʱ������
  //TB0CCTL0 |= CCIE;       //ʹ�ܶ�ʱ��TA1CCR0.CCIFG�ж�

}

/*******************************************************************************
**������:void DrvTimerEnable(T_U16 timer_count,TE_TIMER_CHANNEL cha)
**
**����:����ĳ����ʱ�������趨��ʱʱ����timer_count:��Ҫ��ʱ��ʱ������ʱ��100uSλ��
**
**����ʱ��λ����timer_count = 2�����ʾ��ʱ200uS��cha:���ڶ�ʱ�Ķ�ʱ��ͨ����
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
**
**�޸����ݣ�ȡ�����ݲ���timer_count����Ϊ�ں��������ɺ궨�帳ֵ��
**    timer_count:��Ҫ��ʱ��ʱ������ʱ��1msλ������ʱ��λ
**    ��timer_count = 2�����ʾ��ʱ2ms;

**
**���ߣ�Kevin Shi
**
**���ڣ�
*******************************************************************************/
void DrvTimerEnable(TE_TIMER_CHANNEL cha)
{
  T_U16 timer_count = MSECS_PER_TICK;//��Ҫд�붨ʱ�������Ĵ�����ֵ
  switch(cha)
  {
    case E_TIMER_CHANNEL_A0://���ö�ʱ��TA0����ֵ
      {
        TA0CCR0 = timer_count;
        TA0CTL |= MC_1;//��������ʱ
        TA0CCTL0 |= CCIE;//ʹ�ܶ�ʱ��TA0CCR0.CCIFG�ж�
      }
      break;
      case E_TIMER_CHANNEL_A1://���ö�ʱ��TA1����ֵΪÿ1ms��һ���ж�
      {
        TA1CCR0 = timer_count;
        TA1CTL |= MC_1;//��������ʱ
        TA1CCTL0 |= CCIE;//ʹ�ܶ�ʱ��TA1CCR0.CCIFG�ж�
      }
      break;
      case E_TIMER_CHANNEL_A2://���ö�ʱ��TA2����ֵΪÿ5ms��һ���ж�
      {
        TA2CCR0 = 2499;
        TA2CTL |= MC_1;//��������ʱ
        TA2CCTL0 |= CCIE;//ʹ�ܶ�ʱ��TA2CCR0.CCIFG�ж�
      }
      break;
      case E_TIMER_CHANNEL_A3://���ö�ʱ��TA3����ֵ
      {
        TA3CCR0 = timer_count* 100;
        TA3CTL |= MC_1;//��������ʱ
        TA3CCTL0 |= CCIE;//ʹ�ܶ�ʱ��TA3CCR0.CCIFG�ж�
      }
      break;
      case E_TIMER_CHANNEL_B0://���ö�ʱ��TB0����ֵ
      {
        TB0CCR0 = timer_count;
        TB0CTL |= MC_1;//��������ʱ
        TB0CCTL0 |= CCIE;//ʹ�ܶ�ʱ��TB0CCR0.CCIFG�ж�
      }
      break;
  default:
    ;
    break;
  }
}

/*******************************************************************************
**������:void DrvTimerDisable(TE_TIMER_CHANNEL cha)
**
**����:��ֹĳ����ʱ���������������ʱ���жϱ�־
**
**���ߣ�HJH
**
**���ڣ�2016-04-18
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvTimerDisable(TE_TIMER_CHANNEL cha)
{
   switch(cha)
  {
    case E_TIMER_CHANNEL_A0://���ö�ʱ��TA0����ֵ
      {
        TA0CTL &= ~MC_1;//ֹͣ����ʱ
        TA0CCTL0 &= ~CCIE;//��ֹ��ʱ��TA0CCR0.CCIFG�ж�
        TA0CCTL0 &= ~CCIFG;//�����ʱ���жϱ�־
      }
      break;
      case E_TIMER_CHANNEL_A1://���ö�ʱ��TA1����ֵ
      {
        TA1CTL &= ~MC_1;//ֹͣ����ʱ
        TA1CCTL0 &= ~CCIE;//��ֹ��ʱ��TA1CCR0.CCIFG�ж�
        TA1CCTL0 &= ~CCIFG;//�����ʱ���жϱ�־
      }
      break;
      case E_TIMER_CHANNEL_A2://���ö�ʱ��TA2����ֵ
      {
        TA2CTL   &= ~MC_1;//ֹͣ����ʱ
        TA2CCTL0 &= ~CCIE;//��ֹ��ʱ��TA2CCR0.CCIFG�ж�
        TA2CCTL0 &= ~CCIFG;//�����ʱ���жϱ�־
      }
      break;
      case E_TIMER_CHANNEL_A3://���ö�ʱ��TA3����ֵ
      {
        TA3CTL   &= ~MC_1;//ֹͣ����ʱ
        TA3CCTL0 &= ~CCIE;//��ֹ��ʱ��TA3CCR0.CCIFG�ж�
        TA3CCTL0 &= ~CCIFG;//�����ʱ���жϱ�־
      }
      break;
      case E_TIMER_CHANNEL_B0://���ö�ʱ��TB0����ֵ
      {
        TB0CTL   &= ~MC_1;//ֹͣ����ʱ
        TB0CCTL0 &= ~CCIE;//��ֹ��ʱ��TB0CCR0.CCIFG�ж�
        TB0CCTL0 &= ~CCIFG;//�����ʱ���жϱ�־
      }
      break;
  default:
    ;
    break;
  }
}

/*******************************************************************************
**������:__interrupt void TimerA0Isr(void)
**
**����:��ʱTimer_A TA0�жϷ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-19
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0Isr(void)
{
    #ifdef _DEBUG 
    LPM3_EXIT;
    #endif

}

/*******************************************************************************
**������:__interrupt void TimerA1Isr(void)
**
**����:��ʱTimer_A TA1�жϷ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-19
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TimerA1Isr(void)
{
    #ifdef _DEBUG 
  LPM3_EXIT;
  #endif
}

/*******************************************************************************
**������:__interrupt void TimerA2Isr(void)
**
**����:��ʱTimer_A TA2�жϷ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-19
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2Isr(void)
{
	CtlTimeOnOneTick();
    LPM1_EXIT;
}

/*******************************************************************************
**������:__interrupt void TimerA3Isr(void)
**
**����:��ʱTimer_A TA3�жϷ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-19
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
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
**������:__interrupt void TimerB0Isr(void)
**
**����:��ʱTimer_B TB0�жϷ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-19
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0Isr(void)
{
  DrvTimerDisable(E_TIMER_CHANNEL_B0);
    #ifdef _DEBUG 
  LPM3_EXIT;
  #endif
}

