/****************************************************************************
	Luka Penger 2015
	EV PWM
****************************************************************************/

#include "EV_PWM.h"
#include <SWM_LPC82x.h>
#include <SCT_LPC82x.h>
#include "EV_ADC.h"
#include "HELP.h"

float selectedMaximumCurrent = EV_CURRENT_PWM_DUTY_6A;

bool pwmRunning = false;
float evPwmDutyCycle = EV_PWM_MINIMUM_DUTY_CICLE;
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
	//SWM_MovablePinAssign(SWM_SCT_OUT3_O, 6);
	SWM_DeInit();
	
	sctClockRate = EV_PWM_GetSctClockRate();
	
	LPC_SCT->MATCH0 = (sctClockRate / (2 * PWM_FREQ)); // PWM Half period
	LPC_SCT->MATCHREL0 = (sctClockRate / (2 * PWM_FREQ));
	
	LPC_SCT->MATCH1 = (LPC_SCT->MATCH0 * (1 - PWM_DUTY_CICLE1(evPwmDutyCycle)));
//	LPC_SCT->MATCH2 = (LPC_SCT->MATCH0 * (1 - PWM_DUTY_CICLE2(evPwmDutyCycle)));
	LPC_SCT->MATCH3 = 0;  //EVENT 2
	LPC_SCT->MATCH4 = 3000;  //EVENT 3

	EV_PWM_SetDutyCycle(evPwmDutyCycle);
	
	LPC_SCT->EV0_STATE = 0xFF;
	LPC_SCT->EV0_CTRL = (1 << 0) | (1 << 12);  // match reg 1
	/*
	LPC_SCT->EV1_STATE = 0xFF;
	LPC_SCT->EV1_CTRL = (2 << 0) | (1 << 12);  // match reg 2
	*/
	LPC_SCT->EV2_STATE = 0xFF;
	LPC_SCT->EV2_CTRL = (3 << 0) | (1 << 12);  // match reg 3

	LPC_SCT->EV3_STATE = 0xFF;
	LPC_SCT->EV3_CTRL = (4 << 0) | (1 << 12);  // match reg 4
	
	LPC_SCT->EV4_STATE = 0xFF;
	LPC_SCT->EV4_CTRL = (0 << 0) | (1 << 12);  // match reg 0
	
	LPC_SCT->OUT0_SET = (1 << 0);  // event 0 sets OUT0
	LPC_SCT->OUTPUTDIRCTRL |= (1 << 0);
	/*
	LPC_SCT->OUT1_CLR = (1 << 1);  // event 1 clears OUT1
	LPC_SCT->OUTPUTDIRCTRL |= (1 << 2);
	*/
	LPC_SCT->OUT3_SET = (1 << 2) | (1 << 4);
	LPC_SCT->OUT3_CLR = (1 << 3);
	
	LPC_SCT->RES |= 0xF;
	LPC_SCT->OUTPUT = (1 << 0) | (0 << 1) | (0 << 3);
}

void EV_PWM_Start(void) {
	uint32_t temp;
 
	ADC_DisableSequencer(LPC_ADC, ADC_SEQA_IDX);
	temp = LPC_ADC->SEQA_CTRL;
	temp |= ADC_SEQ_CTRL_CHANSEL(3);
	LPC_ADC->SEQA_CTRL = temp;
	ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);
  LPC_SCT->CTRL &= ~(1 << 2);  //start SCT
	pwmRunning = true;
}

void EV_PWM_Stop(void) {
	uint32_t temp;

	LPC_SCT->CTRL |= (1 << 2);  //halt SCT
	LPC_SCT->CTRL |= (1 << 3);	// clear counter
	ADC_DisableSequencer(LPC_ADC, ADC_SEQA_IDX);
	LPC_SCT->OUTPUT = (1 << 0) | (0 << 1);

	temp = LPC_ADC->SEQA_CTRL;
	temp &= ~ADC_SEQ_CTRL_CHANSEL(3);
	LPC_ADC->SEQA_CTRL = temp;
	ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);
	
	pwmRunning = false;
}

void EV_PWM_SetDutyCycle(float duty) {
	if (duty < EV_PWM_MINIMUM_DUTY_CICLE) {
		duty = EV_PWM_MINIMUM_DUTY_CICLE;
	} else if (duty > EV_PWM_MAXIMUM_DUTY_CICLE) {
		duty = EV_PWM_MAXIMUM_DUTY_CICLE;
	}
	
	evPwmDutyCycle = duty;
	
	LPC_SCT->MATCHREL1 = (LPC_SCT->MATCH0 * (1 - PWM_DUTY_CICLE1(evPwmDutyCycle)));
	//LPC_SCT->MATCHREL2 = (LPC_SCT->MATCH0 * (1 - PWM_DUTY_CICLE2(evPwmDutyCycle)));
	LPC_SCT->MATCHREL3 = 0;
	LPC_SCT->MATCHREL4 = 3000;
}

float EV_PWM_CheckProximityMaxCurrent(float proximityVoltage) {
	float tolerance = 0.2;

	if (proximityVoltage >= (2.57 - tolerance)) {
		return EV_CURRENT_PWM_DUTY_6A;
	} else if (proximityVoltage <= (2.57 + tolerance) && proximityVoltage >= (2.57 - tolerance)) {
		return EV_CURRENT_PWM_DUTY_13A;
	} else if (proximityVoltage <= (2.22 + tolerance) && proximityVoltage >= (2.22 - tolerance)) {
		return EV_CURRENT_PWM_DUTY_16A;
	} else if (proximityVoltage <= (1.82 + tolerance) && proximityVoltage >= (1.82 - tolerance)) {
		return EV_CURRENT_PWM_DUTY_32A;
	} else if (proximityVoltage <= (1.66 + tolerance) && proximityVoltage >= (1.66 - tolerance)) {
		return EV_CURRENT_PWM_DUTY_60A;
	} else {
		return EV_CURRENT_PWM_DUTY_6A;
	}
}

/****************************************************************************
                            End Of File
****************************************************************************/
