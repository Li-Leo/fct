/************************************************
* COPYRIGHT 2016.
*
* NAME: DrvSpi.c  (Slave MCU)
*
* DESCRIPTION:
*   Slave MCU SPI module in the driver layer.
*
* REVISION HISTORY:
*
* END OF FILE HEADER
************************************************/
#include "DrvSpi.h"

T_U8 g_rx_data[8]={0};        //spi rx from master buffer
T_U8 g_feedback_data[8]={0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10};//spi feedback to master
T_U8 g_feedback_hz[8]={0};
T_U8 g_feedback_type=0;
T_U8 volatile g_receive_finished=0;

/***********************************************
*   description:
*       Slave Spi initialization
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void DrvSpixInitial(TE_SPI_CHANNEL cha)
{
  switch(cha)
  {
  case E_SPI_M_S_COM:
    {
      P1SEL1 &=  ~(BIT4|BIT5|BIT6|BIT7);          // P1.4-p1.7: spi
      P1SEL0 |= BIT4|BIT5|BIT6|BIT7;

      UCB0CTLW0 |= UCSWRST;
      UCB0CTLW0 |= UCMSB | UCSYNC | UCSSEL__UCLK ;//| UCCKPL;// MSB first
      UCB0CTLW0 &= ~UC7BIT;    //8 bit
      UCB0CTLW0 &= ~UCMST;     //slave mode
      UCB0CTLW0 |=  UCMODE_0;  //3 wire
      UCB0BR0 = 0;
      UCB0BR1 = 0;

      UCB0CTLW0 &= ~UCSWRST;
      UCB0IE |= UCRXIE;
    }
    break;
  default:
    break;
  };
}
/***********************************************
*   description:
*
*   parameters:
*       N/A
*   return:
*
***********************************************/
TE_BOOLEAN DrvSpixWaitFinish()
{
  if(UCB0STATW & UCBUSY){
    return E_FALSE;
  }else{
    return E_TRUE;
  }
}
/***********************************************
*   description:
*
*   parameters:
*       N/A
*   return:
*
***********************************************/
void DrvSpiEnable(TE_SPI_CHANNEL cha)
{
  UCB0CTLW0 &= ~UCSWRST;  //initialize USCI state machine
  UCB0IE = UCRXIE|UCTXIE; //enable rx tx interrupts
}
/***********************************************
*   description:
*
*   parameters:
*       N/A
*   return:
*
***********************************************/
void DrvSpiDisable(TE_SPI_CHANNEL cha)
{
  UCB0CTLW0 |= UCSWRST;       // **Disable USCI state machine**
  UCB0IE &= ~(UCRXIE|UCTXIE); // disable rx tx interrupt
}
/***********************************************
*   description:
*       slave spi interrupt handle
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  T_U8 static get_head_flag=0,head_flag=0;
  T_U8 static get_feekback_flag=0,get_head_finished=0;
  T_U8 static i=0,j=0,head[2]={0};
  T_U8 static receive_num=0;

  switch(__even_in_range(UCB0IV, USCI_SPI_UCTXIFG))
  {
  case USCI_NONE:
    break;

  case USCI_SPI_UCRXIFG:
/*    
    if(g_enter_low_power_state == 1){
      if (UCB0RXBUF == 0xff)
      {  
        __bic_SR_register_on_exit(LPM3_bits);  // exit low power mode
        InitDriver();                          // re-initial driver
        g_enter_low_power_state=0;
      }     
    }
*/    
    UCB0IFG &= ~UCRXIFG;
    /* receive control command from master */
    if (get_head_flag == 0){
      if (UCB0RXBUF == 0xa5){
        receive_num=0;
        get_head_flag=1;
      }
    }

    if(get_head_flag == 1){
      if(receive_num<8){
        g_rx_data[receive_num]=UCB0RXBUF;
        receive_num ++;
        if(receive_num == 8){
          g_receive_finished=1;
          receive_num=0;
          get_head_flag=0;
        }
      }
    }
    /* receive feedback command from master */
    if(head_flag == 0){
      if(UCB0RXBUF == 0xf5){
        head_flag=1;
        j=0;
        head[j]=UCB0RXBUF;
        j++;
      }
    }
    
    if(head_flag == 1){
      if(UCB0RXBUF == 0x5f){
        if(j<2){
          head[j]=UCB0RXBUF;
          j ++;
          if(j == 2){
            get_head_finished=1;
            j=0;
            head_flag=0;
          }
        }
      }
    }
    
    if(get_head_finished==1){
      get_head_finished=0;
      if(head[0] == 0xf5 && head[1]==0x5f)
        get_feekback_flag=1;
    }

    if(get_feekback_flag){
      if(UCB0RXBUF == 0x55){
        i=0;
        while(!(UCB0IFG&UCTXIFG));       
        if(g_feedback_type==0)
          UCB0TXBUF = g_feedback_data[i];
        else if(g_feedback_type==1)
          UCB0TXBUF = g_feedback_hz[i];
      } else if (UCB0RXBUF == 0xaa){
        while(!(UCB0IFG&UCTXIFG));
        if(g_feedback_type==0)
          UCB0TXBUF = g_feedback_data[i];
        else if(g_feedback_type==1)
          UCB0TXBUF = g_feedback_hz[i];
      }
      if (i<8) {
        i++;
      } else {
        i=0;
        get_feekback_flag=0;
      }
    }
    break;
  case USCI_SPI_UCTXIFG:
    break;
  default:
    break;
  }

#ifdef _DEBUG 
LPM3_EXIT;
#endif

}
/***************************** end of file ************************************/
