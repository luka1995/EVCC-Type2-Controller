/****************************************************************************
	Luka Penger 2015
	EV UART
****************************************************************************/

#include "EV_UART.h"
#include <SWM_LPC82x.h>
#include "CLOCK_LPC82x.h"
#include <GPIO_LPC82x.h>

char EV_UART_Buffer[EV_UART_BUFFER_SIZE];
int EV_UART_BufferLength = 0;
int EV_UART_ReceivingState = 0;

bool EV_UART_DataReceived = false;

void EV_UART_Init(void) {
	GPIO_Init(LPC_GPIO_PORT);
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_UART_DE_PIN);
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_UART_RE_PIN);

	EV_UART_DE_PIN_Set(false);
	EV_UART_RE_PIN_Set(false);
	
	SWM_Init();	
	Clock_SetUARTClockDiv(1);	// divided by 1
	SWM_MovablePinAssign(SWM_U1_TXD_O, EV_UART_TX_PIN);
	SWM_MovablePinAssign(SWM_U1_RXD_I, EV_UART_RX_PIN);
	SWM_DeInit();
	
	UART_Init(EV_UART);
	UART_ConfigData(EV_UART, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
	Clock_SetUSARTNBaseClockRate((115200 * 16), true);
	UART_SetBaud(EV_UART, EV_UART_BAUDRATE);
	UART_Enable(EV_UART);
	UART_TXEnable(EV_UART);
	UART_IntEnable(EV_UART, UART_INTEN_RXRDY);

	NVIC_EnableIRQ(EV_IRQNUM);
}

void EV_UART_DE_PIN_Set(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_UART_DE_PIN, state);
}

void EV_UART_RE_PIN_Set(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_UART_RE_PIN, state);
}

void EV_UART_SendString(char *string) {
	EV_UART_SendBuffer(string, strlen(string));
}

void EV_UART_SendStringFormat(char *format, ...) {
	char data[EV_UART_SEND_FORMAT_BUFFER_SIZE];
	va_list arguments;
	
	va_start(arguments, format);
	vsprintf(data, format, arguments);
	va_end(arguments);
	
	EV_UART_SendString(data);
}

int EV_UART_SendBuffer(char *data, int numBytes) {
	int sent = 0;
	
	UART_IntDisable(EV_UART, UART_INTEN_RXRDY);
	EV_UART_DE_PIN_Set(true);
	EV_UART_RE_PIN_Set(true);
	
	UART_Send(EV_UART, data, numBytes);
	
	UART_IntEnable(EV_UART, UART_INTEN_RXRDY);
	EV_UART_DE_PIN_Set(false);
	EV_UART_RE_PIN_Set(false);
	
	return sent;
}

/*****************************************************************************
 * Interrupt
 ****************************************************************************/

void EV_UARTHNDLR(void) {
	uint8_t ch = UART_ReadByte(EV_UART);
	
	if (ch == EV_UART_RECEIVE_CHARACTER && EV_UART_ReceivingState == 0) {
		EV_UART_ReceivingState = 1;
		EV_UART_BufferLength = 0;
	} else if (EV_UART_ReceivingState > 0) {
		if (ch == 0x0D) {
			EV_UART_ReceivingState = 2;
		} else if (ch == 0x0A) {
			EV_UART_ReceivingState = 0;
			
			EV_UART_DataReceived = true;
		} else if (EV_UART_ReceivingState == 1) {
			if (EV_UART_BufferLength > EV_UART_BUFFER_SIZE) {
				EV_UART_ReceivingState = 0;
				EV_UART_DataReceived = false;
				EV_UART_BufferLength = 0;
			} else {
				EV_UART_Buffer[EV_UART_BufferLength] = ch;
			
				EV_UART_BufferLength++;
			}
		} else if (EV_UART_ReceivingState == 2) {
			EV_UART_ReceivingState = 0;
			EV_UART_DataReceived = false;
			EV_UART_BufferLength = 0;
		}
	}
}

/*****************************************************************************
 * String help
 ****************************************************************************/

int EV_UART_LocateCharacter(char *buffer, int length, char character, int next) {
	int i = 0, which = 0;
	
	for (i = 0; i < length; i++) {
		if (buffer[i] == character) {
			if (which == next) {
				return i;
			} else {
				which++;
			}
		}
	}
	
	return 0;
}

bool EV_UART_BufferIsNumber(char *buffer, int length) {
	int i = 0;
	
	for (i = 0; i < length; i++) {
		if (buffer[i] < 0x30 || buffer[i] > 0x39) {
			return false;
		}
	}
	
	if (length > 0) {
		return true;
	} else {
		return false;
	}
}

/****************************************************************************
                            End Of File
****************************************************************************/
