/****************************************************************************
	Luka Penger 2015
	EV INPUTS
****************************************************************************/
#ifndef __EV_INPUTS_H 
#define __EV_INPUTS_H

#include <lpc_types.h>
#include <GPIO_LPC82x.h>

#define EV_INPUT_START_CHARGING_PIN					0
#define EV_INPUT_LOCK_DEVICE_PIN						0

void EV_INPUTS_Init(void);
bool EV_INPUTS_GetStateSwitchStartCharging(void);
bool EV_INPUTS_GetStateSwitchLockDevice(void);

#endif // end __EV_INPUTS_H
/****************************************************************************
                            End Of File
****************************************************************************/
