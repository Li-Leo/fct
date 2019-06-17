/*******************************************************************************
**�ļ���:DrvClk.c
**
**����:ʱ�ӳ�ʼ��
**
**����:
**
**����:2016-03-17
**
**�޸�����:
**
**����:
**
**����:
**
*******************************************************************************/
#include "DrvClk.h"

/*******************************************************************************
**������:void ClockInit(void)
**
**����:�˿ڳ�ʼ�����˿ڹ���ѡ�񡢶˿ڷ�������
**
**���ߣ�
**
**���ڣ�
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void ClockInit(void)
{
#ifdef USE_EXTERNAL_CLOCK_FOR_LOW_POWER    
    PJSEL0 |= BIT4 | BIT5;                     // For LFX
    PJSEL1 &= ~(BIT4 | BIT5);
#endif    
    CSCTL0 = 0xa500;//д��ʱ��ϵͳ��Կ��ʹ�ܺ���ʱ�����üĴ���
    CSCTL5 = 0x0000;//������١����پ���ʧЧ��־
    
    CSCTL1 |= DCOFSEL_6;//Set DCO to 8M
#ifdef USE_EXTERNAL_CLOCK_FOR_LOW_POWER
    CSCTL2 =  SELA__LFXTCLK| SELS__DCOCLK | SELM__DCOCLK ;
#else
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK ;
#endif
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;;//SMCLK 8��Ƶ,ACLK��MCLKʱ�Ӿ�����Ƶ
#ifdef USE_EXTERNAL_CLOCK_FOR_LOW_POWER
    CSCTL4 &= ~LFXTOFF;
    //CSCTL4 |= LFXTOFF;
    CSCTL4 &= ~(LFXTDRIVE0|LFXTDRIVE1);
    do
    {
      CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
#else
    CSCTL4 = 0x0100;//�ر��ⲿ����ʱ��
#endif    
    CSCTL6 = 0x000f;//��ĳЩ�͹���ģʽ�£����������MODCLK,SMCLK,MCLK,ACLKʱ������
    CSCTL0_H = 0;

}
