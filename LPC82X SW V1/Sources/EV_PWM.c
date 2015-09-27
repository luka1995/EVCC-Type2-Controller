/****************************************************************************
	Luka Penger 2015
	EV PWM
****************************************************************************/

#include "EV_PWM.h"
#include <SWM_LPC82x.h>
#include <SCT_LPC82x.h>

#include "EV_PWM.h"
#include <SWM_LPC82x.h>
#include <SCT_LPC82x.h>

#define EV_PWM_POSITIVE_OUTPUT_PIN 								27
#define EV_PWM_NEGATIVE_OUTPUT_PIN 								26

#define PWM_FREQ 																	1000 // Freq in Hz
#define PWM_DUTY_CICLE1(x) 												(x / 100)
#define PWM_DEAD_TIME															0 // us
#define PWM_DUTY_CICLE2(x) 												(PWM_DUTY_CICLE1(x) - (2 * PWM_DEAD_TIME * PWM_FREQ * 0.000001)) 

bool pwmRunning = false;
float evPwmDutyCycle = 50.0;
uint32_t sctClockRate;

uint32_t EV_PWM_GetSctClockRate(void) {
	return Clock_GetSystemClockRate() / (((LPC_SCT->CTRL & (0xFF << 5)) >> 5) + 1);
};

void EV_PWM_Init(void) {
	Clock_EnablePeriphClock(SYSCTL_CLOCK_SCT);
	
	LPC_SCT->CONFIG |= (1 << 17); // split timers, auto limit
	LPC_SCT->CTRL |= (1 << 4); // configure SCT as BIDIR
	
	SWM_Init();
	SWM_MovablePinAssign(SWM_SCT_OUT0_O, EV_PWM_POSITIVE_OUTPUT_PIN);
	SWM_MovablePinAssign(SWM_SCT_OUT1_O, EV_PWM_NEGATIVE_OUTPUT_PIN);
	//SWM_MovablePinAssign(SWM_SCT_OUT3_O, 6);
	SWM_DeInit();
	
	sctClockRate = EV_PWM_GetSctClockRate();
	
	LPC_SCT->MATCH0 = (sctClockRate / (2 * PWM_FREQ)); // PWM Half period
	LPC_SCT->MATCHREL0 = (sctClockRate / (2 * PWM_FREQ));
	
	LPC_SCT->MATCH1 = LPC_SCT->MATCH0 * PWM_DUTY_CICLE1(evPwmDutyCycle);
	LPC_SCT->MATCH2 = LPC_SCT->MATCH0 * PWM_DUTY_CICLE2(evPwmDutyCycle);
	LPC_SCT->MATCH3 = 1;

	EV_PWM_SetDutyCycle(evPwmDutyCycle);
	
	LPC_SCT->EV0_STATE = 0xFF;
	LPC_SCT->EV0_CTRL = (1 << 0) | (1 << 12);  // match reg 1
	
	LPC_SCT->EV1_STATE = 0xFF;
	LPC_SCT->EV1_CTRL = (2 << 0) | (1 << 12);  // match reg 2
	
	LPC_SCT->EV2_STATE = 0xFF;
	LPC_SCT->EV2_CTRL = (3 << 0) | (1 << 12);  // match reg 3

	LPC_SCT->OUT0_SET = (1 << 0);  // event 0 sets OUT0
	LPC_SCT->OUTPUTDIRCTRL |= (1 << 0);
	
	LPC_SCT->OUT1_CLR = (1 << 1);  // event 1 clears OUT1
	LPC_SCT->OUTPUTDIRCTRL |= (1 << 2);
	
	LPC_SCT->EV3_STATE = 0xFF;
	LPC_SCT->EV3_CTRL = (0 << 0) | (1 << 12);  // match reg 0
	
	LPC_SCT->OUT3_SET = (1 << 3);
	LPC_SCT->OUT3_CLR = (1 << 2);
	
	LPC_SCT->RES |= 0xF;
	LPC_SCT->OUTPUT = (1 << 0) | (0 << 1);
}

void EV_PWM_Start(void) {
	LPC_SCT->CTRL &= ~(1 << 2);
	
	pwmRunning = true;
}

void EV_PWM_Stop(void) {
	LPC_SCT->CTRL |= (1 << 2);
	
	pwmRunning = false;
}

void EV_PWM_SetDutyCycle(float duty) {
	evPwmDutyCycle = duty;
	
	LPC_SCT->MATCHREL1 = ((sctClockRate / (2 * PWM_FREQ)) * PWM_DUTY_CICLE1(evPwmDutyCycle));
	LPC_SCT->MATCHREL2 = ((sctClockRate / (2 * PWM_FREQ)) * PWM_DUTY_CICLE2(evPwmDutyCycle));
}

/****************************************************************************
                            End Of File
****************************************************************************/
