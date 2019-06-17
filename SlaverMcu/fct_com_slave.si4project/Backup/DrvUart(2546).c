#include "DrvUart.h"
#include "stdio.h"
#include "stddef.h"

typedef struct
{
  T_U8 first_rx_index;
  T_U8 last_rx_index;
  T_U8 rx_buff[RX_BUFF_SIZE];  
  T_U8 new_data_flag;
}TS_UART_RX_BUFF;

static TS_UART_RX_BUFF g_uart_rx_buff[E_UART_MAX];
/***********************************************
* Description:
*	Uart0 init
* Argument:
*    none
* Return:
*	 none
************************************************/
void DrvUart0Init(void)
{
  P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 UART operation
  P2SEL1 &= ~(BIT0 | BIT1);

  // Configure USCI_A0 for UART mode
  UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
  UCA0CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK=500k
  UCA0BR0 = 4;                              // 500000/115200 = 4.34(0x4900) 500000/9600 = 52.08(0x0400)
  UCA0MCTLW = 0x4900;                       // 500000/115200 - INT(1000000/115200)=0.34
                                            // UCBRSx value = 0x49 (See UG)
  // 250000/115200 = 2.17,  2M: 17 4A00
  UCA0BR1 = 0;
  UCA0CTL1 &= ~UCSWRST;                     // release from reset
  
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
/***********************************************
* Description:
*	Uart1 init
* Argument:
*    none
* Return:
*	 none
************************************************/
void DrvUart1Init(void)
{
  P3SEL0 |= BIT4 | BIT5;                    // USCI_A1 UART operation
  P3SEL1 &= ~(BIT4 | BIT5);

  // Configure USCI_A0 for UART mode
  UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
  UCA1CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK=500k
  UCA1BR0 = 4;                              // 500000/115200 = 4.34(0x4900) 500000/9600 = 52.08(0x0400)
  UCA1MCTLW = 0x4900;                       // 500000/115200 - INT(1000000/115200)=0.34
                                            // UCBRSx value = 0x49 (See UG)
  UCA1BR1 = 0;
  UCA1CTL1 &= ~UCSWRST;                     // release from reset
  
  UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
/***********************************************
* Description:
*	Uart0 send
* Argument:
*
* Return:
*	
************************************************/
void Uart0SendByte(T_U8 TXByte)
{
  while(!(UCA0IFG & UCTXIFG));
  UCA0TXBUF = TXByte;
}

void Uart0SendData(const T_U8 *tx_data, T_U8 len)
{
  while(len--){
    Uart0SendByte(*tx_data++);
  }
}

void Uart0SendString(T_U8 *s)
{
  while(*s != '\0'){
    Uart0SendByte(*s++);
  }
}
/***********************************************
* Description:
*	Uart1 send
* Argument:
*
* Return:
*	
************************************************/
void Uart1SendByte(T_U8 TXByte)
{
  while(!(UCA1IFG & UCTXIFG));
  UCA1TXBUF = TXByte;
}

void Uart1SendData(const T_U8 *tx_data, T_U8 len)
{
  //T_U8 i;
  
  //i=0;
  while(len--){
    Uart1SendByte(*tx_data++);
    //i++;
  }
}

void Uart1SendString(T_U8 *s)
{
  while(*s != '\0'){
    Uart1SendByte(*s++);
  }
}

void UartPrint(TE_UART_PORT port, T_U32 data, T_U8 *name, TE_DATA_FORMAT data_type)
{
  T_U8 buf[50];                                //buffer size: 50 bytes
  
  if(data_type == E_DEC)
    sprintf((char*)buf,"%-15s: %ld\r\n",name,data);
  else if(data_type == E_HEX)
    sprintf((char*)buf,"%-15s: 0x%lx\r\n",name,data);
  
  if(port == E_UART0)
    Uart0SendString(buf);
  else if(port == E_UART1)
    Uart1SendString(buf);
}


void UartOnReceiveNewByte(TE_UART_PORT port, T_U8 ch)
{
  T_S32 next_index; 

  next_index = g_uart_rx_buff[port].last_rx_index;
  next_index++;
  if (next_index>=RX_BUFF_SIZE)
  {
    next_index = 0;
  }
  //if buff is full, return
  if (next_index == g_uart_rx_buff[port].first_rx_index)
  {
    //not save the data
  }
  else
  {
    //save it
    g_uart_rx_buff[port].rx_buff[g_uart_rx_buff[port].last_rx_index] = ch;
    g_uart_rx_buff[port].last_rx_index  = next_index;          
  }

  //set new data flag
  g_uart_rx_buff[port].new_data_flag = E_TRUE;

}


T_S32 UartReceivedBufferSize(TE_UART_PORT uart_port)
{
  T_S32 size;
  size = g_uart_rx_buff[uart_port].last_rx_index - g_uart_rx_buff[uart_port].first_rx_index;
  if (size < 0)
  {
      size += RX_BUFF_SIZE;
  }

  return size;
}
T_U8 UartReceiveByte(TE_UART_PORT uart_port)
{
  T_U8 ret;
  T_S32 first_index;

  //wait buff not empty
  while(g_uart_rx_buff[uart_port].first_rx_index == g_uart_rx_buff[uart_port].last_rx_index)
  {
  };

  //get one char
  first_index = g_uart_rx_buff[uart_port].first_rx_index;
  ret = g_uart_rx_buff[uart_port].rx_buff[first_index];
  //move to next index
  first_index++;
  if(first_index >= RX_BUFF_SIZE)
  {
      first_index = 0;
  }
  g_uart_rx_buff[uart_port].first_rx_index = first_index;

  return ret;
}
T_U8 UartGetNewDataFlag(TE_UART_PORT uart_port)
{
  return g_uart_rx_buff[uart_port].new_data_flag;
}

void UartClearNewDataFlag(TE_UART_PORT uart_port)
{
  g_uart_rx_buff[uart_port].new_data_flag = 0;
}
/***********************************************
* Description:
*	Uart1 ISR
* Argument:
*   none
* Return:
*	none
************************************************/
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{

  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      UartOnReceiveNewByte(E_UART0, UCA0RXBUF);
      //if (UCA0RXBUF != 0xaa)
          //printf("err\n");
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
  LPM1_EXIT;
}
/***********************************************
* Description:
*	Uart1 ISR
* Argument:
*   none
* Return:
*	none
************************************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)

{
  switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      UartOnReceiveNewByte(E_UART1, UCA1RXBUF);
      //Uart0SendByte(UCA1RXBUF);
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }

  LPM1_EXIT;
}

/***********************************************
* Description:
*   interface for C IO function in IAR
* Argument:
*   handle:
*   buffer:
*   size:
*
* Return:
*
************************************************/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
    unsigned int i;


    (void)handle;                        /* Parameter is not used, suppress unused argument warning */
	
    for (i = 0; i < size; i++) 
	{
        //buffer[i] = DrvUsartGetChar();
    }

    return i;
}

/***********************************************
* Description:
*   interface for C IO function in IAR
* Argument:
*   handle:
*   buffer:
*   size:
*
* Return:
*
************************************************/
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    unsigned int i;

    (void)handle;                        /* Parameter is not used, suppress unused argument warning */
    for (i = 0; i < size; i++) 
	{
        /* Send '\r' before each '\n'. */
        if (buffer[i] == '\n') 
		{
            Uart0SendByte('\r');
        }
        
        Uart0SendByte((T_U8)buffer[i]);

    }
    /* Wait until last character is sent */
    return i;
}

/****************************  end of file ************************************/
