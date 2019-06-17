/************************************************
* COPYRIGHT 2016.
*
* NAME: DrvSpi.c  (Master MCU)
*
* DESCRIPTION:
*   Master MCU SPI module in the driver layer.
*
* REVISION HISTORY:
*
* END OF FILE HEADER
************************************************/

#include "DrvSpi.h"
#include "DrvClk.h"
T_U8 g_rx_data[8]={0};

/***********************************************
*   description:
*       Master Spi initialization
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void DrvSpixInitial(TE_SPI_CHANNEL cha)
{
  UCB0CTLW0 |= UCSWRST;
  switch(cha)
  {
  case E_SPI_M_S_COM:
    P1SEL1 &=  ~(BIT4|BIT5|BIT6|BIT7);          // P1.4-p1.7: spi
    P1SEL0 |= BIT4|BIT5|BIT6|BIT7;

    UCB0CTLW0 = UCSWRST;
    UCB0CTLW0 |=  UCMSB | UCSYNC ;//| UCCKPL;
    UCB0CTLW0 &= ~UC7BIT;         //8 bit
    UCB0CTLW0 |=  UCMST;          //master mode
    UCB0CTLW0 |=  UCMODE_0;       //3 wire
    UCB0CTLW0 |=  UCSSEL__SMCLK;  //clk smclk

    UCB0BR0 = CPU_FREQ/400000;    //set to 50k
    UCB0BR1 = 0x00;
    
    UCB0CTLW0 &= ~UCSWRST;
    //UCB0IE |= UCRXIE;
    break;
  default:
    break;
  };
}
/***********************************************
*   description:
*       Spi Write one byte to UCB0TXBUF
*   parameters:
*       N/A
*   return:
*
***********************************************/
void DrvSpixWriteByte(T_U8 tx_data)
{
  while(!(UCB0IFG&UCTXIFG));
  UCB0TXBUF = tx_data;
  __delay_cycles(500);
}
/***********************************************
*   description:
*       Spi read one byte from UCB0RXBUF
*   parameters:
*       N/A
*   return:
*      T_U8 rx_data
***********************************************/
T_U8 DrvSpixReadByte(void)
{
  T_U8 rx_data=0; 
  
  while(!(UCB0IFG&UCRXIFG));
  rx_data = UCB0RXBUF;
  return rx_data;
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
  UCB0CTLW0 &= ~UCSWRST;  // **Initialize USCI state machine**
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
*
*   parameters:
*       N/A
*   return:
*
***********************************************/
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)

{
  static T_U8 i=0;
  switch(__even_in_range(UCB0IV, USCI_SPI_UCTXIFG))
  {
  case USCI_NONE: break;
  case USCI_SPI_UCRXIFG:
    g_rx_data[i] = UCB0RXBUF;
    
    if(i<8) i++;
    else i=0;
    //UCA0IFG &= ~UCRXIFG;
    
    break;
  case USCI_SPI_UCTXIFG:
    break;
  default:
    break;
  }
}
