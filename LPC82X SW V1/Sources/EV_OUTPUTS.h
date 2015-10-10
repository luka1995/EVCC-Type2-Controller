/****************************************************************************
	Luka Penger 2015
	EV OUTPUTS
****************************************************************************/
#ifndef __EV_OUTPUTS_H 
#define __EV_OUTPUTS_H

#include <lpc_types.h>
#include <GPIO_LPC82x.h>

#define EV_OUTPUT_LED1_PIN 											19
#define EV_OUTPUT_LED2_PIN 											18
#define EV_OUTPUT_POWER_PIN 										17
#define EV_OUTPUT_VENTILATION_PIN 							0
#define EV_OUTPUT_SHUT_PIN 											28

void EV_OUTPUTS_Init(void);
void EV_OUTPUTS_SetLED1(bool state);
void EV_OUTPUTS_SetLED2(bool state);
void EV_OUTPUTS_SetPower(bool state);
void EV_OUTPUTS_SetVentilation(bool state);
void EV_OUTPUTS_SetShut(bool state);
bool EV_OUTPUTS_GetStateLED1(void);
bool EV_OUTPUTS_GetStateLED2(void);
bool EV_OUTPUTS_GetStatePower(void);
bool EV_OUTPUTS_GetStateVentilation(void);
bool EV_OUTPUTS_GetStateShut(void);

#endif // end __EV_OUTPUTS_H
/****************************************************************************
                            End Of File
****************************************************************************/
