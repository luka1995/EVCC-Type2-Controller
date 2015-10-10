/****************************************************************************
	Luka Penger 2015
	EV INPUTS
****************************************************************************/

#include "EV_INPUTS.h"

void EV_INPUTS_Init(void) {
	GPIO_Init(LPC_GPIO_PORT);
	
	// Switch start charging
	
	GPIO_SetPinDIRInput(LPC_GPIO_PORT, EV_INPUT_START_CHARGING_PIN);
	
	// Switch stop charging

	GPIO_SetPinDIRInput(LPC_GPIO_PORT, EV_INPUT_LOCK_DEVICE_PIN);
}

bool EV_INPUTS_GetStateSwitchStartCharging(void) {
	return !GPIO_GetPinState(LPC_GPIO_PORT, EV_INPUT_START_CHARGING_PIN);
}

bool EV_INPUTS_GetStateSwitchLockDevice(void) {
	return !GPIO_GetPinState(LPC_GPIO_PORT, EV_INPUT_LOCK_DEVICE_PIN);
}

/****************************************************************************
                            End Of File
****************************************************************************/
