/****************************************************************************
	Luka Penger 2015
	EV PWM
****************************************************************************/
#ifndef __EV_PWM_H 
#define __EV_PWM_H

#include <lpc_types.h>

#define EV_PWM_POSITIVE_OUTPUT_PIN 				27

#define PWM_FREQ 													1000 // Freq in Hz
#define PWM_DUTY_CICLE1(x) 								(x / 100)
#define PWM_DEAD_TIME											0 // us
#define PWM_DUTY_CICLE2(x) 								(PWM_DUTY_CICLE1(x) - (2 * PWM_DEAD_TIME * PWM_FREQ * 0.000001)) 

#define EV_PWM_MINIMUM_DUTY_CICLE					8.0
#define EV_PWM_MAXIMUM_DUTY_CICLE					97.0

#define EV_CURRENT_PWM_DUTY_6A						10.0
#define EV_CURRENT_PWM_DUTY_10A						16.6
#define EV_CURRENT_PWM_DUTY_13A						21.6
#define EV_CURRENT_PWM_DUTY_16A						26.6
#define EV_CURRENT_PWM_DUTY_20A						33.3
#define EV_CURRENT_PWM_DUTY_32A						53.3
#define EV_CURRENT_PWM_DUTY_40A						66.0
#define EV_CURRENT_PWM_DUTY_50A						84.0
#define EV_CURRENT_PWM_DUTY_60A						88.0
#define EV_CURRENT_PWM_DUTY_70A						92.0
#define EV_CURRENT_PWM_DUTY_80A						96.0

void EV_PWM_Init(void);
void EV_PWM_Start(void);
void EV_PWM_Stop(void);
void EV_PWM_SetDutyCycle(float duty);

#endif // end __EV_PWM_H
/****************************************************************************
                            End Of File
****************************************************************************/
