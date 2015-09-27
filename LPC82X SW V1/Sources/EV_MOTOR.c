/****************************************************************************
	Luka Penger 2015
	EV MOTOR
****************************************************************************/

#include "EV_MOTOR.h"
#include "cmsis_os.h"
#include <GPIO_LPC82x.h>

#define EV_MOTOR_FORWARD_OUTPUT_PIN 								24
#define EV_MOTOR_REVERSE_OUTPUT_PIN 								25

void EV_MOTOR_Init(void) {
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_MOTOR_FORWARD_OUTPUT_PIN); // Motor Forward -> Output
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, EV_MOTOR_REVERSE_OUTPUT_PIN); // Motor Reverse -> Output
	
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_FORWARD_OUTPUT_PIN, false);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_REVERSE_OUTPUT_PIN, false);
	
	//EV_Motor_Unlock();
}

void EV_Motor_Lock(void) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_FORWARD_OUTPUT_PIN, false);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_REVERSE_OUTPUT_PIN, true);
	
	osDelay(1000);
	
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_FORWARD_OUTPUT_PIN, false);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_REVERSE_OUTPUT_PIN, false);
}

void EV_Motor_Unlock(void) {
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_FORWARD_OUTPUT_PIN, true);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_REVERSE_OUTPUT_PIN, false);
	
	osDelay(1000);
	
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_FORWARD_OUTPUT_PIN, false);
	GPIO_SetPinState(LPC_GPIO_PORT, EV_MOTOR_REVERSE_OUTPUT_PIN, false);
}

/****************************************************************************
                            End Of File
****************************************************************************/
