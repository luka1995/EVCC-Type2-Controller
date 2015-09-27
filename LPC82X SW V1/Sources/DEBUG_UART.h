/****************************************************************************
	Luka Penger 2015
	DEBUG UART
****************************************************************************/
#ifndef __DEBUG_UART_H 
#define __DEBUG_UART_H

#include <UART_LPC82x.h>
#include <SWM_LPC82x.h>
#include <SCT_LPC82x.h>

#define DEBUG_UART_BAUDRATE											38400
#define DEBUG_UART_TXD_PIN											4
#define DEBUG_UART_RXD_PIN											0

void DEBUG_UART_Init(void);


#endif // end __DEBUG_UART_H
/****************************************************************************
                            End Of File
****************************************************************************/
