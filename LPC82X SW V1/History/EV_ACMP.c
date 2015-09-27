/****************************************************************************
	Luka Penger 2015
	EV PWM
****************************************************************************/

#include "EV_ACMP.h"
#include <SWM_LPC82x.h>

#define EV_ACMP_INPUT_PIN 								23

void EV_ACMP_Init(void) {
	SYSCTL_PowerUp(SYSCTL_SLPWAKE_ACMP_PD);
	Clock_EnablePeriphClock(SYSCTL_CLOCK_ACOMP);
	
	SWM_Init();
	SWM_EnableFixedPin(SWM_FIXED_ACMP_I4);
	
	SWM_DeInit();

	/* COMPEDGE bit on falling and rising edges,
		Positive input = ACMP_I4,
		Negative input = Voltage ladder output,
		No hysteresis */
	LPC_CMP->CTRL |= (1 << 3) | (4 << 8) | (0 << 8) | (0 << 25);
	
	/*
		Enable voltage ladder,
		Ladder select,
		Ladder reference voltage = VDD */
	LPC_CMP->LAD |= (1 << 0) | (2 << 1) | (0 << 6);

	NVIC_EnableIRQ(CMP_IRQn); // Enable interrupt
	
	EV_ACMP_ClearInterrupt();
}

void EV_ACMP_ClearInterrupt(void) {
	LPC_CMP->CTRL |= (1 << 20);
	LPC_CMP->CTRL &= ~(1 << 20);
}

bool EV_ACMP_GetCompStatus(void) {
	return (bool)(LPC_CMP->CTRL & (1 << 21));
}

bool EV_ACMP_GetCompEdgeStatus(void) {
	return (bool)(LPC_CMP->CTRL & (1 << 23));
}

/****************************************************************************
                            End Of File
****************************************************************************/
