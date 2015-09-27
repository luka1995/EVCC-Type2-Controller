/****************************************************************************
	Luka Penger 2015
	EV PWM
****************************************************************************/
#ifndef __EV_PWM_H 
#define __EV_PWM_H

#include <lpc_types.h>

void EV_PWM_Init(void);
void EV_PWM_Start(void);
void EV_PWM_Stop(void);
void EV_PWM_SetDutyCycle(float duty);

#endif // end __EV_PWM_H
/****************************************************************************
                            End Of File
****************************************************************************/
