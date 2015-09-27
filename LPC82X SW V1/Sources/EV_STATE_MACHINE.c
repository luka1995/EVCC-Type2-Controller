/****************************************************************************
	Luka Penger 2015
	EV STATE MACHINE
****************************************************************************/

#include "EV_STATE_MACHINE.h"
#include "EV_UART.h"

extern float pilotPositiveVoltage; // 0.0V - 12.0V
extern float pilotNegativeVoltage; // 0.0V - -12.0V

void (*currentStateMachine)(void) = EV_State_A1;



extern void EV_UART_SendStringFormat(char *format, ...);
extern void delayMs(unsigned int ms);

void EV_State_Machine(void) {
	if (currentStateMachine == EV_State_A1) {
		EV_UART_SendStringFormat("State: A1\n");
		
		if (pilotPositiveVoltage < EV_STATE_B_THRES_HIGH && pilotPositiveVoltage > EV_STATE_B_THRES_LOW) {
			EV_State_B1();
		} else if (pilotPositiveVoltage < EV_STATE_C_THRES_HIGH && pilotPositiveVoltage > EV_STATE_C_THRES_LOW) {
			EV_State_C1();
		}
	} else if (currentStateMachine == EV_State_A2) {
		EV_UART_SendStringFormat("State: A2\n");
	} else if (currentStateMachine == EV_State_B1) {
		EV_UART_SendStringFormat("State: B1\n");
		
		if (pilotPositiveVoltage < EV_STATE_A_THRES_HIGH && pilotPositiveVoltage > EV_STATE_A_THRES_LOW) {
			EV_State_A1();
		}
	} else if (currentStateMachine == EV_State_B2) {
		EV_UART_SendStringFormat("State: B2\n");
		
		if (pilotPositiveVoltage < EV_STATE_A_THRES_HIGH && pilotPositiveVoltage > EV_STATE_A_THRES_LOW) {
			EV_State_A2();
		} else if (pilotPositiveVoltage < EV_STATE_C_THRES_HIGH && pilotPositiveVoltage > EV_STATE_C_THRES_LOW) {
			EV_State_C2();
		}
	} else if (currentStateMachine == EV_State_C1) {
		EV_UART_SendStringFormat("State: C1\n");
	} else if (currentStateMachine == EV_State_C2) {
		EV_UART_SendStringFormat("State: C2\n");
	} else if (currentStateMachine == EV_State_D1) {
		EV_UART_SendStringFormat("State: D1\n");
	} else if (currentStateMachine == EV_State_D2) {
		EV_UART_SendStringFormat("State: D2\n");
	}
	
	/*if (pilotNegativeVoltage < EV_GROUND_THRED_HIGH || pilotNegativeVoltage < EV_GROUND_THRED_LOW) {
		EV_GroundError();
	}*/
}

// States

void EV_State_A1(void) {
	currentStateMachine = EV_State_A1;
	
	// Close all outputs !
}

void EV_State_A2(void) {
	currentStateMachine = EV_State_A2;
	
	EV_PWM_Stop();
	
	delayMs(100);
	
	EV_State_A1();
}

void EV_State_B1(void) {
	currentStateMachine = EV_State_B1;
	
}

void EV_State_B2(void) {
	currentStateMachine = EV_State_B2;
	
	EV_PWM_Start();
	
	delayMs(100);
}

void EV_State_C1(void) {
	currentStateMachine = EV_State_C1;
}

void EV_State_C2(void) {
	currentStateMachine = EV_State_C2;
}

void EV_State_D1(void) {
	currentStateMachine = EV_State_D1;
}

void EV_State_D2(void) {
	currentStateMachine = EV_State_D2;
}

void EV_GroundError(void) {
	EV_UART_SendString("GROUND ERROR\n");
}

/****************************************************************************
                            End Of File
****************************************************************************/
