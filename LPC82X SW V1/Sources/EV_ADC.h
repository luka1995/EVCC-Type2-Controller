/****************************************************************************
	Luka Penger 2015
	EV ADC
****************************************************************************/
#ifndef __EV_ADC_H 
#define __EV_ADC_H

#include <lpc_types.h>
#include <SWM_LPC82x.h>
#include <ADC_LPC82x.h>
#include "CLOCK_LPC82x.h"

#define ADC_VCC														3.3
#define ADC_VREFP													ADC_VCC			
#define ADC_VREFN													0.0

void EV_ADC_Init(void);
void EV_ADC_ReadCPVoltage(void);
void EV_ADC_ReadPPVoltage(void);
void EV_ADC_ClearCalculationValues(void);

#endif // end __EV_ADC_H
/****************************************************************************
                            End Of File
****************************************************************************/
