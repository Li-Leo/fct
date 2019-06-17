/*******************************************************************************
**文件名:DrvClk.c
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
#include "DrvClk.h"

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
#ifdef USE_EXTERNAL_CLOCK_FOR_LOW_POWER    
    PJSEL0 |= BIT4 | BIT5;                     // For LFX
    PJSEL1 &= ~(BIT4 | BIT5);
#endif    
    CSCTL0 = 0xa500;//写入时钟系统秘钥，使能后续时钟配置寄存器
    CSCTL5 = 0x0000;//清除低速、高速晶振失效标志
    
    CSCTL1 |= DCOFSEL_6;//Set DCO to 8M
#ifdef USE_EXTERNAL_CLOCK_FOR_LOW_POWER
    CSCTL2 =  SELA__LFXTCLK| SELS__DCOCLK | SELM__DCOCLK ;
#else
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK ;
#endif
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;;//SMCLK 8分频,ACLK，MCLK时钟均不分频
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
    CSCTL4 = 0x0100;//关闭外部高速时钟
#endif    
    CSCTL6 = 0x000f;//在某些低功耗模式下，允许外设对MODCLK,SMCLK,MCLK,ACLK时钟请求
    CSCTL0_H = 0;

}
