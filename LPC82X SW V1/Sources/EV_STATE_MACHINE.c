/****************************************************************************
	Luka Penger 2015
	EV STATE MACHINE
****************************************************************************/

#include "EV_STATE_MACHINE.h"
#include "cmsis_os.h"

EvModuleMode selectedModuleMode = kEvModuleModeAuto;

extern float pilotPositiveVoltage; // 0.0V - 12.0V
extern float pilotNegativeVoltage; // 0.0V - -12.0V

void (*currentStateMachine)(void) = EV_State_A1;

extern bool pwmRunning;
extern float evPwmDutyCycle;

void EV_State_Machine(void) {
	if (selectedModuleMode == kEvModuleModeAuto) {
		if (pilotPositiveVoltage < EV_STATE_A_THRES_HIGH && pilotPositiveVoltage > EV_STATE_A_THRES_LOW) {
			if (currentStateMachine != EV_State_A1) {
				EV_State_A1();
			}
		} else if (pilotPositiveVoltage < EV_STATE_B_THRES_HIGH && pilotPositiveVoltage > EV_STATE_B_THRES_LOW) {
			if (currentStateMachine != EV_State_B1) {
				EV_State_B1();
			}
		} else if (pilotPositiveVoltage < EV_STATE_C_THRES_HIGH && pilotPositiveVoltage > EV_STATE_C_THRES_LOW) {
			if (currentStateMachine != EV_State_C2) {
				EV_State_C2();
			}
		} else if (pilotPositiveVoltage < EV_STATE_D_THRES_HIGH && pilotPositiveVoltage > EV_STATE_D_THRES_LOW) {
			if (currentStateMachine != EV_State_D2) {
				EV_State_D2();
			}
		}
	} else {
		
		
	}
	
	if (pwmRunning && pilotNegativeVoltage < EV_STATE_F_THRES_HIGH && pilotNegativeVoltage < EV_STATE_F_THRES_LOW) {
		if (currentStateMachine != EV_State_F) EV_State_F();
	} else if (pilotPositiveVoltage >= EV_STATE_E_THRES_LOW && pilotPositiveVoltage < EV_STATE_E_THRES_HIGH) {
		if (currentStateMachine != EV_State_E) EV_State_E();
	}
}

// States

void EV_State_A1(void) {
	currentStateMachine = EV_State_A1;

	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(false);
}

void EV_State_A2(void) {
	currentStateMachine = EV_State_A2;
	
	if (!pwmRunning) EV_PWM_Start();
}

void EV_State_B1(void) {
	currentStateMachine = EV_State_B1;
	
	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(true);
}

void EV_State_B2(void) {
	currentStateMachine = EV_State_B2;
	
	if (!pwmRunning) EV_PWM_Start();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(true);
}

void EV_State_C1(void) {
	currentStateMachine = EV_State_C1;
	
	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(true);
}

void EV_State_C2(void) {
	currentStateMachine = EV_State_C2;
	
	if (!pwmRunning) EV_PWM_Start();
	
	EV_OUTPUTS_SetPower(true);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(true);
}

void EV_State_D1(void) {
	currentStateMachine = EV_State_D1;
	
	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(true);
	EV_OUTPUTS_SetShut(true);
}

void EV_State_D2(void) {
	currentStateMachine = EV_State_D2;
	
	if (!pwmRunning) EV_PWM_Start();
	
	EV_OUTPUTS_SetPower(true);
	EV_OUTPUTS_SetVentilation(true);
	EV_OUTPUTS_SetShut(true);
}

void EV_State_E(void) {
	currentStateMachine = EV_State_E;
	
	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(false);
}

void EV_State_F(void) {
	currentStateMachine = EV_State_F;
	
	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(false);
}

void EV_DisableAllOutputs(void) {
	currentStateMachine = EV_State_A1;
	
	if (pwmRunning) EV_PWM_Stop();
	
	EV_OUTPUTS_SetPower(false);
	EV_OUTPUTS_SetVentilation(false);
	EV_OUTPUTS_SetShut(false);
}

/****************************************************************************
                            End Of File
****************************************************************************/
