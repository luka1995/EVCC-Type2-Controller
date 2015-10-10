/****************************************************************************
	Luka Penger 2015
	EV STATE MACHINE
****************************************************************************/
#ifndef __EV_STATE_MACHINE_H 
#define __EV_STATE_MACHINE_H

#include <lpc_types.h>
#include "EV_ADC.h"
#include "EV_PWM.h"
#include "EV_OUTPUTS.h"
#include "EV_INPUTS.h"
#include "HELP.h"

#define EV_STATE_A_THRES_HIGH						13.0  // Volts
#define EV_STATE_A_THRES_LOW						11.0  // Volts

#define EV_STATE_B_THRES_HIGH						10.0  // Volts
#define EV_STATE_B_THRES_LOW						8.0  // Volts

#define EV_STATE_C_THRES_HIGH						7.0  // Volts
#define EV_STATE_C_THRES_LOW						5.0  // Volts

#define EV_STATE_D_THRES_HIGH						4.0  // Volts
#define EV_STATE_D_THRES_LOW						2.0  // Volts

#define EV_STATE_E_THRES_HIGH						1.0	// Volts
#define EV_STATE_E_THRES_LOW						-1.0	// Volts

#define EV_STATE_F_THRES_HIGH						-13.0	// Volts
#define EV_STATE_F_THRES_LOW						-11.0	// Volts

typedef enum {
	kEvModuleModeAuto,
	kEvModuleModeManual
} EvModuleMode;

void EV_State_Machine(void);

void EV_State_A1(void);
void EV_State_A2(void);
void EV_State_B1(void);
void EV_State_B2(void);
void EV_State_C1(void);
void EV_State_C2(void);
void EV_State_D1(void);
void EV_State_D2(void);
void EV_State_E(void);
void EV_State_F(void);
void EV_DisableAllOutputs(void);

#endif // end __EV_STATE_MACHINE_H
/****************************************************************************
                            End Of File
****************************************************************************/
