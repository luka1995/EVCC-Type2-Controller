/****************************************************************************
	Luka Penger 2015
	EV OUTPUTS
****************************************************************************/

#include "EV_OUTPUTS.h"

void EV_OUTPUTS_Init(void) {
	GPIO_Init(LPC_GPIO_PORT);
	
	// LED 1
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_LED1_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED1_PIN, true);
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_LED1_INSIDE_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED1_INSIDE_PIN, true);
	
	// LED 2
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_LED2_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED2_PIN, true);
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_LED2_INSIDE_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED2_INSIDE_PIN, true);
	
	// Power
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_POWER_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_POWER_PIN, false);
	
	// Ventilation
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_VENTILATION_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_VENTILATION_PIN, false);
	
	// Shut
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_OUTPUT_SHUT_PIN);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_SHUT_PIN, false);
}

void EV_OUTPUTS_SetLED1(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED1_PIN, !state);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED1_INSIDE_PIN, !state);
}

void EV_OUTPUTS_SetLED2(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED2_PIN, !state);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED2_INSIDE_PIN, !state);
}

void EV_OUTPUTS_SetPower(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_POWER_PIN, state);
}

void EV_OUTPUTS_SetVentilation(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_VENTILATION_PIN, state);
}

void EV_OUTPUTS_SetShut(bool state) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_OUTPUT_SHUT_PIN, state);
}

bool EV_OUTPUTS_GetStateLED1(void) {
	return !GPIO_GetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED1_PIN);
}

bool EV_OUTPUTS_GetStateLED2(void) {
	return !GPIO_GetPinState(LPC_GPIO_PORT, EV_OUTPUT_LED2_PIN);
}

bool EV_OUTPUTS_GetStatePower(void) {
	return GPIO_GetPinState(LPC_GPIO_PORT, EV_OUTPUT_POWER_PIN);
}

bool EV_OUTPUTS_GetStateVentilation(void) {
	return GPIO_GetPinState(LPC_GPIO_PORT, EV_OUTPUT_VENTILATION_PIN);
}

bool EV_OUTPUTS_GetStateShut(void) {
	return GPIO_GetPinState(LPC_GPIO_PORT, EV_OUTPUT_SHUT_PIN);
}

/****************************************************************************
                            End Of File
****************************************************************************/
