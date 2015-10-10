/****************************************************************************
	Luka Penger 2015
	EV ADC
****************************************************************************/

#include "EV_ADC.h"

bool sequenceComplete = false;

int pilotPositiveADCValue; // 0 - 4095
int pilotNegativeADCValue; // 0 - 4095
float pilotPositiveVoltage; // 0.0V - 12.0V
float pilotNegativeVoltage; // 0.0V - -12.0V
bool adcVoltageMeasured = false;

unsigned int adcCh10Voltage = 0, adcCh3Voltage = 0, adcCurrentSample = 0;

void EV_ADC_Init(void) {
	ADC_Init(LPC_ADC, 0);
	
	ADC_StartCalibration(LPC_ADC);
	while (!(ADC_IsCalibrationDone(LPC_ADC)));
	
	ADC_SetClockRate(LPC_ADC, 1200000);
	
	SWM_Init();
	SWM_EnableFixedPin(SWM_FIXED_ADC10);
	SWM_EnableFixedPin(SWM_FIXED_ADC3);
	SWM_DeInit();

	LPC_IOCON->PIO0_13 &= ~(3 << 3); // pull-up/down Inactive
	LPC_IOCON->PIO0_23 &= ~(3 << 3); // pull-up/down Inactive
	
	ADC_SetSequencerBits(LPC_ADC, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(10) |
																							(3 << 12) | 
																							ADC_SEQ_CTRL_HWTRIG_POLPOS | 
																							ADC_SEQ_CTRL_SINGLESTEP |
																							ADC_SEQ_CTRL_MODE_EOS));
	
	ADC_EnableInt(LPC_ADC, ADC_INTEN_SEQA_ENABLE);
	NVIC_SetPriority(ADC_SEQA_IRQn, 1);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);
	ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);
}

void EV_ADC_ReadCPVoltage(void) {
	adcCh3Voltage = ADC_DR_RESULT(ADC_GetDataReg(LPC_ADC, 3));
	adcCh10Voltage = ADC_DR_RESULT(ADC_GetDataReg(LPC_ADC, 10));
	
	pilotPositiveVoltage = 4 * (((2 * adcCh10Voltage * (ADC_VREFP - ADC_VREFN)) / 0xFFF) - ADC_VCC);
	pilotNegativeVoltage = -4 * (((2 * adcCh3Voltage * (ADC_VREFP - ADC_VREFN)) / 0xFFF) - ADC_VCC);
	
	adcVoltageMeasured = true;
}

/*****************************************************************************
 * Interrupt
 ****************************************************************************/

void ADC_SEQA_IRQHandler(void) {
	/* Get pending interrupts */
	unsigned int pending = ADC_GetFlags(LPC_ADC);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequenceComplete = true;
	}
	
	/* Clear SEQA_INT interrupt */
	ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
}

/****************************************************************************
                            End Of File
****************************************************************************/
