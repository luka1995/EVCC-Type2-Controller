/****************************************************************************
	Luka Penger 2015
	EV ADC
****************************************************************************/

#include "EV_ADC.h"

bool sequenceAComplete = false, sequenceBComplete = false;

unsigned int proximityADCValue; // 0 - 4095
unsigned int pilotPositiveADCValue; // 0 - 4095
unsigned int pilotNegativeADCValue; // 0 - 4095
float proximityVoltage; // 0.0V - 3.3V
float pilotPositiveVoltage; // 0.0V - 12.0V
float pilotNegativeVoltage; // 0.0V - -12.0V
bool adcCPVoltageMeasured = false, adcPPVoltageMeasured = false;

void EV_ADC_Init(void) {
	ADC_Init(LPC_ADC, 0);
	
	ADC_StartCalibration(LPC_ADC);
	while (!(ADC_IsCalibrationDone(LPC_ADC)));
	
	ADC_SetClockRate(LPC_ADC, 12000000);
	
	SWM_Init();
	SWM_EnableFixedPin(SWM_FIXED_ADC10);
	SWM_EnableFixedPin(SWM_FIXED_ADC3);
	SWM_EnableFixedPin(SWM_FIXED_ADC9);
	SWM_DeInit();

	LPC_IOCON->PIO0_13 &= ~(3 << 3); // pull-up/down Inactive
	LPC_IOCON->PIO0_23 &= ~(3 << 3); // pull-up/down Inactive
	LPC_IOCON->PIO0_17 &= ~(3 << 3); // pull-up/down Inactive
	
	ADC_SetSequencerBits(LPC_ADC, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(10) |
																							(3 << 12) | 
																							ADC_SEQ_CTRL_HWTRIG_POLPOS | 
																							ADC_SEQ_CTRL_SINGLESTEP |
																							ADC_SEQ_CTRL_MODE_EOS));
	
	ADC_SetSequencerBits(LPC_ADC, ADC_SEQB_IDX, (ADC_SEQ_CTRL_CHANSEL(9) |
																							ADC_SEQ_CTRL_HWTRIG_POLPOS | 
																							ADC_SEQ_CTRL_SINGLESTEP |
																							ADC_SEQ_CTRL_MODE_EOS));
																							
	ADC_EnableInt(LPC_ADC, ADC_INTEN_SEQA_ENABLE);
	NVIC_SetPriority(ADC_SEQA_IRQn, 1);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);
	ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);
	
	ADC_EnableInt(LPC_ADC, ADC_INTEN_SEQB_ENABLE);
	NVIC_SetPriority(ADC_SEQB_IRQn, 2);
	NVIC_EnableIRQ(ADC_SEQB_IRQn);
	ADC_EnableSequencer(LPC_ADC, ADC_SEQB_IDX);
}

void EV_ADC_ReadCPVoltage(void) {
	pilotPositiveADCValue = ADC_DR_RESULT(ADC_GetDataReg(LPC_ADC, 10));  // PWM high voltage
	pilotNegativeADCValue = ADC_DR_RESULT(ADC_GetDataReg(LPC_ADC, 3));  // PWM low voltage
	
	pilotPositiveVoltage = 4 * (((2 * pilotPositiveADCValue * (ADC_VREFP - ADC_VREFN)) / 0xFFF) - ADC_VCC);
	pilotNegativeVoltage = -4 * (((2 * pilotNegativeADCValue * (ADC_VREFP - ADC_VREFN)) / 0xFFF) - ADC_VCC);
	
	adcCPVoltageMeasured = true;
}

void EV_ADC_ReadPPVoltage(void) {
	proximityADCValue = ADC_DR_RESULT(ADC_GetDataReg(LPC_ADC, 9));  // Proximity voltage
	
	proximityVoltage = (((ADC_VREFP - ADC_VREFN) / 4095) * proximityADCValue);
	
	adcPPVoltageMeasured = true;
}

/*****************************************************************************
 * Interrupt
 ****************************************************************************/

void ADC_SEQA_IRQHandler(void) {
	/* Get pending interrupts */
	unsigned int pending = ADC_GetFlags(LPC_ADC);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequenceAComplete = true;
	}
	
	/* Clear SEQA_INT interrupt */
	ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
}

void ADC_SEQB_IRQHandler(void) {
	/* Get pending interrupts */
	unsigned int pending = ADC_GetFlags(LPC_ADC);
	
	/* Sequence B completion interrupt */
	if (pending & ADC_FLAGS_SEQB_INT_MASK) {
		sequenceBComplete = true;
	}
	
	/* Clear SEQB_INT interrupt */
	ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQB_INT_MASK);
}

/****************************************************************************
                            End Of File
****************************************************************************/
