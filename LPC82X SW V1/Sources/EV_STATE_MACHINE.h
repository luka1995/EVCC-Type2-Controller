/****************************************************************************
	Luka Penger 2015
	EV STATE MACHINE
****************************************************************************/
#ifndef __EV_STATE_MACHINE_H 
#define __EV_STATE_MACHINE_H

#include <lpc_types.h>
#include "EV_ADC.h"
#include "EV_PWM.h"


#define EV_STATE_A_THRES_HIGH						12.6  // Volts
#define EV_STATE_A_THRES_LOW						11.4  // Volts

#define EV_STATE_B_THRES_HIGH						9.59  // Volts
#define EV_STATE_B_THRES_LOW						8.37  // Volts

#define EV_STATE_C_THRES_HIGH						6.53  // Volts
#define EV_STATE_C_THRES_LOW						5.47  // Volts

#define EV_STATE_D_THRES_HIGH						3.28  // Volts
#define EV_STATE_D_THRES_LOW						2.59  // Volts

#define EV_GROUND_THRED_HIGH						-12.6	// Volts
#define EV_GROUND_THRED_LOW							-11.4	// Volts

void EV_State_Machine(void);

void EV_State_A1(void);
void EV_State_A2(void);
void EV_State_B1(void);
void EV_State_B2(void);
void EV_State_C1(void);
void EV_State_C2(void);
void EV_State_D1(void);
void EV_State_D2(void);
void EV_GroundError(void);

#endif // end __EV_STATE_MACHINE_H
/****************************************************************************
                            End Of File
****************************************************************************/
