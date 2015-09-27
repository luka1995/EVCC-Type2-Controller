/****************************************************************************
	Luka Penger 2015
	UART LPC82x
****************************************************************************/

#include <UART_LPC82x.h>

void UART_Enable(LPC_USART0_Type *pUART) {
	pUART->CFG = UART_CFG_ENABLE | (pUART->CFG & ~UART_CFG_RESERVED);
}

void UART_Disable(LPC_USART0_Type *pUART) {
	pUART->CFG &= ~(UART_CFG_RESERVED | UART_CFG_ENABLE);
}

void UART_TXEnable(LPC_USART0_Type *pUART) {
	pUART->CTL &= ~(UART_CTRL_RESERVED | UART_CTRL_TXDIS);
}

void UART_TXDisable(LPC_USART0_Type *pUART) {
	pUART->CTL = UART_CTRL_TXDIS | (pUART->CTL & ~UART_CTRL_RESERVED);
}

void UART_SendByte(LPC_USART0_Type *pUART, uint8_t data) {
	pUART->TXDAT = (uint32_t) data;
}

uint32_t UART_ReadByte(LPC_USART0_Type *pUART) {
	/* Strip off undefined reserved bits, keep 9 lower bits */
	return (uint32_t) (pUART->RXDAT & 0x000001FF);
}

void UART_IntEnable(LPC_USART0_Type *pUART, uint32_t intMask) {
	pUART->INTENSET = intMask;
}

void UART_IntDisable(LPC_USART0_Type *pUART, uint32_t intMask) {
	pUART->INTENCLR = intMask;
}

uint32_t UART_GetIntsEnabled(LPC_USART0_Type *pUART) {
	return (pUART->INTENSET & ~UART_INTEN_RESERVED);
}

uint32_t UART_GetIntStatus(LPC_USART0_Type *pUART) {
	return (pUART->INTSTAT & ~UART_INTSTAT_RESERVED);
}

void UART_ConfigData(LPC_USART0_Type *pUART, uint32_t config) {
	uint32_t reg;

	reg = pUART->CFG & ~((0x3 << 2) | (0x3 << 4) | (0x1 << 6) | UART_CFG_RESERVED);
	pUART->CFG = reg | config;
}

uint32_t UART_GetStatus(LPC_USART0_Type *pUART) {
	return (pUART->STAT & ~UART_STAT_RESERVED);
}

void UART_ClearStatus(LPC_USART0_Type *pUART, uint32_t stsMask) {
	pUART->STAT = stsMask;
}

void UART_SetOSR(LPC_USART0_Type *pUART, uint32_t ovrVal) {
	pUART->OSR = ovrVal - 1;
}

void UART_SetAddr(LPC_USART0_Type *pUART, uint32_t addr) {
	pUART->ADDR = addr;
}

SYSCTL_CLOCK_T getUARTClockID(LPC_USART0_Type *pUART) {
	if (pUART == LPC_USART0) {
		return SYSCTL_CLOCK_UART0;
	}
	else if (pUART == LPC_USART1) {
		return SYSCTL_CLOCK_UART1;
	}

	return SYSCTL_CLOCK_UART2;
}

void UART_Init(LPC_USART0_Type *pUART) {
	/* Enable USART clock */
	Clock_EnablePeriphClock(getUARTClockID(pUART));

	/* UART reset */
	if (pUART == LPC_USART0) {
		/* Peripheral reset control to USART0 */
		SYSCTL_PeriphReset(RESET_USART0);
	} else if (pUART == LPC_USART1) {
		/* Peripheral reset control to USART1 */
		SYSCTL_PeriphReset(RESET_USART1);
	} else {
		/* Peripheral reset control to USART2 */
		SYSCTL_PeriphReset(RESET_USART2);
	}
}

void UART_DeInit(LPC_USART0_Type *pUART) {
	/* Disable USART clock */
	Clock_DisablePeriphClock(getUARTClockID(pUART));
}

void UART_SetBaud(LPC_USART0_Type *pUART, uint32_t baudrate) {
	uint32_t baudRateGenerator;
	baudRateGenerator = Clock_GetUSARTNBaseClockRate() / (16 * baudrate);
	pUART->BRG = baudRateGenerator - 1;	/* baud rate */
}

void UART_Send(LPC_USART0_Type *pUART, char *data, int numBytes) {
	/*int sent = 0;

	while (sent < numBytes) {
		UART_SendByte(pUART, *data);
		data++;
	}*/
	
	int i = 0;
	
	for (i = 0; i < numBytes; i++) {
		UART_SendByte(pUART, data[i]);
		
		while((UART_GetStatus(pUART) & UART_STAT_TXIDLE) == 0);
	}
}

/****************************************************************************
                            End Of File
****************************************************************************/
