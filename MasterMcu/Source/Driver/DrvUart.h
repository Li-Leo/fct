#ifndef DRVUART_H
#define DRVUART_H

#include <msp430fr5989.h>
#include "TypeDefine.h"

#define RX_BUFF_SIZE 64
#define M_Uart0EnableInterrupt() UCA0IE |= UCRXIE
#define M_Uart0DisableInterrupt() UCA0IE &= ~UCRXIE
#define M_Uart1EnableInterrupt() UCA1IE |= UCRXIE
#define M_Uart1DisableInterrupt() UCA1IE &= ~UCRXIE
typedef enum
{
	E_DEC,
	E_HEX,
	E_DATA_FORMAT_MAX,
} TE_DATA_FORMAT;

typedef enum
{
	E_UART0, //communicate with PC 
	E_UART1, //communicate with internal MCU
	E_UART_MAX,
} TE_UART_PORT;

void DrvUart0Init(void);
void Uart0SendByte(T_U8 TXData);
void Uart0SendData(const T_U8 *tx_data, T_U8 len);
void Uart0SendString(T_U8 *string);

void DrvUart1Init(void);
void Uart1SendByte(T_U8 TXData);
void Uart1SendData(const T_U8 *tx_data, T_U8 len);
void Uart1SendString(T_U8 *string);

void UartPrint(TE_UART_PORT port, T_U32 data, T_U8 *name, TE_DATA_FORMAT data_type);
T_S32 UartReceivedBufferSize(TE_UART_PORT uart_port);
T_U8 UartReceiveByte(TE_UART_PORT uart_port);
T_U8 UartGetNewDataFlag(TE_UART_PORT uart_port);
void UartClearNewDataFlag(TE_UART_PORT uart_port);
#endif
