/*******************************************************************************
**文件名:DrcClk.c
**
**功能:时钟初始化
**
**作者:
**
**日期:2016-03-17
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#include"DrvClk.h"
/*******************************************************************************
**函数名:void ClockInit(void)
**
**功能:端口初始化、端口功能选择、端口方向配置
**
**作者：
**
**日期：
**
**修改内容：
**
**作者：
**
**日期：
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
