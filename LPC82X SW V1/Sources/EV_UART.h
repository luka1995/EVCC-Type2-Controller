/****************************************************************************
	Luka Penger 2015
	EV UART
****************************************************************************/
#ifndef __EV_UART_H 
#define __EV_UART_H

#include <lpc_types.h>
#include <UART_LPC82x.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define EV_UART_BUFFER_SIZE									64
#define EV_UART_SEND_FORMAT_BUFFER_SIZE			64

#define EV_UART_RECEIVE_CHARACTER						'!'
#define EV_UART_SEND_CHARACTER							'>'

#define EV_UART_DE_PIN											14
#define EV_UART_TX_PIN											21
#define EV_UART_RX_PIN											20
#define EV_UART_RE_PIN											22

#define EV_UART															LPC_USART1
#define EV_UART_BAUDRATE										38400
#define EV_IRQNUM      											UART1_IRQn
#define EV_UARTHNDLR   											UART1_IRQHandler

typedef enum {
	kEvUartCommandResetDevice = 0,
	kEvUartCommandLoadFactorySettings = 1,
	kEvUartCommandGetVersionFirmware = 2,
	kEvUartCommandSetAddressDevice = 3,
	kEvUartCommandGetAddressDevice = 4,
	kEvUartCommandGetStateMachineMode = 5,
	kEvUartCommandGetModuleMode = 6,
	kEvUartCommandSwitchModeToManual = 7,
	kEvUartCommandSwitchModeToAuto = 8,
	kEvUartCommandGetStatusOutput = 9,
	kEvUartCommandGetStatusInput = 10,
	kEvUartCommandGetADCValueCPPositive = 11,
	kEvUartCommandGetADCValueCPNegative = 12,
	kEvUartCommandGetADCValuePP = 13,
	kEvUartCommandGetCPPositive = 14,
	kEvUartCommandGetCPNegative = 15,
	kEvUartCommandGetPP = 16,
	kEvUartCommandGetIcPWMDutyCycle = 17,
	kEvUartCommandSetIcMax = 18,
	kEvUartCommandSetIDefault = 19,
	kEvUartCommandGetIDefault = 20,
	kEvUartCommandSwitchPWMOn = 21,
	kEvUartCommandSwitchPWMOff = 22,
	kEvUartCommandGetPWMState = 23,
	kEvUartCommandStartCharging = 24,
	kEvUartCommandStopCharging = 25,
	kEvUartCommandEnableProximityDetection = 26,
	kEvUartCommandDisableProximityDetection = 27,
	kEvUartCommandGetProximityDetectionState = 28
} EvUartCommand;

void EV_UART_Init(void);
void EV_UART_DE_PIN_Set(bool state);
void EV_UART_RE_PIN_Set(bool state);
void EV_UART_SendString(char *string);
void EV_UART_SendStringFormat(char *format, ...);
int EV_UART_SendBuffer(char *data, int numBytes);

int EV_UART_LocateCharacter(char *buffer, int length, char character, int next);
bool EV_UART_BufferIsNumber(char *buffer, int length);

#endif // end __EV_UART_H
/****************************************************************************
                            End Of File
****************************************************************************/
