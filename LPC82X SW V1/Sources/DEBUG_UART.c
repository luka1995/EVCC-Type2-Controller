/****************************************************************************
	Luka Penger 2015
	DEBUG UART
****************************************************************************/

#include "DEBUG_UART.h"

void DEBUG_UART_Init(void) {
	SWM_Init();
	
	SWM_MovablePinAssign(SWM_U0_TXD_O, DEBUG_UART_TXD_PIN);
	SWM_MovablePinAssign(SWM_U0_RXD_I, DEBUG_UART_RXD_PIN);

	SWM_DeInit();
	
	UART_Init(LPC_USART0);
	UART_ConfigData(LPC_USART0, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
	Clock_SetUSARTNBaseClockRate((115200 * 16), true);
	UART_SetBaud(LPC_USART0, DEBUG_UART_BAUDRATE);
	UART_Enable(LPC_USART0);
	UART_TXEnable(LPC_USART0);



}

/****************************************************************************
                            End Of File
****************************************************************************/
