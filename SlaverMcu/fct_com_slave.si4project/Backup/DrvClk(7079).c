/*******************************************************************************
**�ļ���:DrcClk.c
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
#include"DrvClk.h"
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
  CSCTL0 = 0xa500;          // Unlock CS registers
  CSCTL5 = 0x0000;          // Clear XT1 and XT2 fault flag

  CSCTL1 = DCOFSEL_3;       // Set DCO to 4M
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK ;
  CSCTL3 = DIVA__1 | DIVS__8| DIVM__1;;  //Set ACLK,MCLK divider to 1, set SMCLK
                                         //divider to 1/8, SMCLK=500k
  CSCTL6 = 0x000f;          // enable request to MODCLK,SMCLK,MCLK,ACLK
  CSCTL0_H = 0;             // Lock CS registers 
}
