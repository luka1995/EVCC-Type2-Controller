/****************************************************************************
	Luka Penger 2015
	GPIO LPC82x
****************************************************************************/

#include <GPIO_LPC82x.h>

void GPIO_Init(LPC_GPIO_PORT_Type *pGPIO) {
	Clock_EnablePeriphClock(SYSCTL_CLOCK_GPIO);
}

void GPIO_DeInit(LPC_GPIO_PORT_Type *pGPIO) {
	Clock_DisablePeriphClock(SYSCTL_CLOCK_GPIO);
}

void GPIO_SetPinState(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin, bool state) {
	if (state) {
		pGPIO->SET0 = (1UL << pin);
	} else {
		pGPIO->CLR0 = (1UL << pin);
	}
}

bool GPIO_GetPinState(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin) {
	return (bool)(pGPIO->PIN0 & (1UL << pin));
}

void GPIO_WriteDirBit(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin, bool state) {
	if (state) {
		pGPIO->DIR0 |= (1UL << pin);
	} else {
		pGPIO->DIR0 &= ~(1UL << pin);
	}
}

void GPIO_SetPinDIROutput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin) {
	pGPIO->DIRSET0 = (1UL << pin);
}

void GPIO_SetPinDIRInput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin) {
	pGPIO->DIRCLR0 = (1UL << pin);
}

void GPIO_TogglePinDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin) {
	pGPIO->DIRNOT0 = (1UL << pin);
}

void GPIO_SetPinDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin, bool output) {
	if (output) {
		GPIO_SetPinDIROutput(pGPIO, pin);
	} else {
		GPIO_SetPinDIRInput(pGPIO, pin);
	}
}

bool GPIO_GetPinDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin) {
	return (bool)((pGPIO->DIR0 >> (1UL << pin)) & 1);
}

void GPIO_SetDir(LPC_GPIO_PORT_Type *pGPIO, uint32_t bitValue, uint8_t out) {
	if (out) {
		pGPIO->DIR0 |= bitValue;
	} else {
		pGPIO->DIR0 &= ~bitValue;
	}
}

void GPIO_SetPortDIROutput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask) {
	pGPIO->DIRSET0 = pinMask;
}

void GPIO_SetPortDIRInput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask) {
	pGPIO->DIRCLR0 = pinMask;
}

void GPIO_TogglePortDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask) {
	pGPIO->DIRNOT0 = pinMask;
}

void GPIO_SetPortDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask, bool outSet) {
	if (outSet) {
		GPIO_SetPortDIROutput(pGPIO, pinMask);
	}	else {
		GPIO_SetPortDIRInput(pGPIO, pinMask);
	}
}

void GPIO_SetPortDIRMask(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask, bool outMask) {
	GPIO_SetPortDIRInput(pGPIO, pinMask & ~outMask);
	GPIO_SetPortDIROutput(pGPIO, pinMask & outMask);
}

uint32_t GPIO_GetPortDIR(LPC_GPIO_PORT_Type *pGPIO) {
	return pGPIO->DIR0;
}

void GPIO_SetPortMask(LPC_GPIO_PORT_Type *pGPIO, uint32_t mask) {
	pGPIO->MASK0 = mask;
}

uint32_t GPIO_GetPortMask(LPC_GPIO_PORT_Type *pGPIO) {
	return pGPIO->MASK0;
}

void GPIO_SetPortValue(LPC_GPIO_PORT_Type *pGPIO, uint32_t value) {
	pGPIO->PIN0 = value;
}

uint32_t GPIO_GetPortValue(LPC_GPIO_PORT_Type *pGPIO) {
	return pGPIO->PIN0;
}

void GPIO_SetMaskedPortValue(LPC_GPIO_PORT_Type *pGPIO, uint32_t value) {
	pGPIO->MPIN0 = value;
}

uint32_t GPIO_GetMaskedPortValue(LPC_GPIO_PORT_Type *pGPIO) {
	return pGPIO->MPIN0;
}

void GPIO_SetValue(LPC_GPIO_PORT_Type *pGPIO, uint32_t bitValue) {
	pGPIO->SET0 = bitValue;
}

void GPIO_SetPinToggle(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin) {
	pGPIO->NOT0 = pin;
}

/****************************************************************************
                            End Of File
****************************************************************************/
