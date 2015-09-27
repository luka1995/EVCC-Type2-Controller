/****************************************************************************
	Luka Penger 2015
	ADC LPC82x
****************************************************************************/

#include <ADC_LPC82x.h>

/* Initialize the ADC peripheral */
void ADC_Init(LPC_ADC_Type *pADC, uint32_t flags) {
	/* Power up ADC and enable ADC base clock */
	SYSCTL_PowerUp(SYSCTL_SLPWAKE_ADC_PD);
	Clock_EnablePeriphClock(SYSCTL_CLOCK_ADC);

	/* Disable ADC interrupts */
	pADC->INTEN = 0;

	/* Set ADC control options */
	pADC->CTRL = flags;
}

/* Shutdown ADC */
void ADC_DeInit(LPC_ADC_Type *pADC) {
	pADC->INTEN = 0;
	pADC->CTRL = 0;

	/* Stop ADC clock and then power down ADC */
	Clock_DisablePeriphClock(SYSCTL_CLOCK_ADC);
	SYSCTL_PowerDown(SYSCTL_SLPWAKE_ADC_PD);
}

void ADC_SetDivider(LPC_ADC_Type *pADC, uint8_t div) {
	uint32_t temp;

	temp = pADC->CTRL & ~(ADC_CR_CLKDIV_MASK);
	pADC->CTRL = temp | (uint32_t) div;
}

void ADC_SetClockRate(LPC_ADC_Type *pADC, uint32_t rate) {
	ADC_SetDivider(pADC, (uint8_t)(Clock_GetSystemClockRate() / rate) - 1);
}

uint8_t ADC_GetDivider(LPC_ADC_Type *pADC) {
	return pADC->CTRL & ADC_CR_CLKDIV_MASK;
}

/* Start ADC calibration */
void ADC_StartCalibration(LPC_ADC_Type *pADC) {
	/* Set calibration mode */
	pADC->CTRL |= ADC_CR_CALMODEBIT;

	/* Clear ASYNC bit */
	pADC->CTRL &= ~ADC_CR_ASYNMODE;

	/* Setup ADC for about 500KHz (per UM) */
	ADC_SetClockRate(pADC, 500000);

	/* Clearn low power bit */
	pADC->CTRL &= ~ADC_CR_LPWRMODEBIT;

	/* Calibration is only complete when ADC_CR_CALMODEBIT bit has cleared */
}

bool ADC_IsCalibrationDone(LPC_ADC_Type *pADC) {
	return (bool)((pADC->CTRL & ADC_CR_CALMODEBIT) == 0);
}

void ADC_SetSequencerBits(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex, uint32_t bits) {
	if (seqIndex == 0) {
		pADC->SEQA_CTRL = (pADC->SEQA_CTRL & ~ADC_SEQ_CTRL_RES) | bits;
	} else {
		pADC->SEQB_CTRL = (pADC->SEQB_CTRL & ~ADC_SEQ_CTRL_RES) | bits;
	}
}

void ADC_ClearSequencerBits(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex, uint32_t bits) {
	if (seqIndex == 0) {
		pADC->SEQA_CTRL = pADC->SEQA_CTRL & ~(ADC_SEQ_CTRL_RES | bits);
	} else {
		pADC->SEQB_CTRL = pADC->SEQB_CTRL & ~(ADC_SEQ_CTRL_RES | bits);
	}
}

void ADC_SetupSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex, uint32_t options) {
	if (seqIndex == 0) {
		pADC->SEQA_CTRL = options;
	} else {
		pADC->SEQB_CTRL = options;
	}
}

void ADC_EnableSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex) {
	ADC_SetSequencerBits(pADC, seqIndex, ADC_SEQ_CTRL_SEQ_ENA);
}

void ADC_DisableSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex) {
	ADC_ClearSequencerBits(pADC, seqIndex, ADC_SEQ_CTRL_SEQ_ENA);
}

void ADC_StartSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex) {
	ADC_SetSequencerBits(pADC, seqIndex, ADC_SEQ_CTRL_START);
}

void ADC_StartBurstSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex) {
	ADC_SetSequencerBits(pADC, seqIndex, ADC_SEQ_CTRL_BURST);
}

void ADC_StopBurstSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex) {
	ADC_ClearSequencerBits(pADC, seqIndex, ADC_SEQ_CTRL_BURST);
}

uint32_t ADC_GetSequencerDataReg(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex) {
	if (seqIndex == 0) {
		return pADC->SEQA_GDAT;
	} else {
		return pADC->SEQB_GDAT;
	}
}

uint32_t ADC_GetDataReg(LPC_ADC_Type *pADC, uint8_t index) {
	int x;
	switch (index) {
		case 0: x = pADC->DAT0; break;
		case 1: x = pADC->DAT1; break;
		case 2: x = pADC->DAT2; break;
		case 3: x = pADC->DAT3; break;
		case 4: x = pADC->DAT4; break;
		case 5: x = pADC->DAT5; break;
		case 6: x = pADC->DAT6; break;
		case 7: x = pADC->DAT7; break;
		case 8: x = pADC->DAT8; break;
		case 9: x = pADC->DAT9; break;
		case 10: x = pADC->DAT10; break;
		case 11: x = pADC->DAT11; break;
	}
	
	return x;
}

void ADC_SetThrLowValue(LPC_ADC_Type *pADC, uint8_t thrnum, uint16_t value) {
	if (thrnum == 0) {
		pADC->THR0_LOW = (((uint32_t)value) << ADC_THR_VAL_POS);
	} else {
		pADC->THR1_LOW = (((uint32_t)value) << ADC_THR_VAL_POS);
	}
}

void ADC_SetThrHighValue(LPC_ADC_Type *pADC, uint8_t thrnum, uint16_t value) {
	if (thrnum == 0) {
		pADC->THR0_HIGH = (((uint32_t)value) << ADC_THR_VAL_POS);
	} else {
		pADC->THR1_HIGH = (((uint32_t)value) << ADC_THR_VAL_POS);
	}
}

void ADC_SelectTH0Channels(LPC_ADC_Type *pADC, uint32_t channels) {
	pADC->CHAN_THRSEL = pADC->CHAN_THRSEL & ~(ADC_CHAN_THRSEL_RES | channels);
}

void ADC_SelectTH1Channels(LPC_ADC_Type *pADC, uint32_t channels) {
	pADC->CHAN_THRSEL = (pADC->CHAN_THRSEL & ~ADC_CHAN_THRSEL_RES) | channels;
}

void ADC_EnableInt(LPC_ADC_Type *pADC, uint32_t intMask) {
	pADC->INTEN = (pADC->INTEN & ~ADC_INTEN_RES) | intMask;
}

void ADC_DisableInt(LPC_ADC_Type *pADC, uint32_t intMask) {
	pADC->INTEN = pADC->INTEN & ~(ADC_INTEN_RES | intMask);
}

void ADC_SetThresholdInt(LPC_ADC_Type *pADC, uint8_t ch, ADC_INTEN_THCMP_T thInt) {
	pADC->INTEN = (pADC->INTEN & ~(ADC_INTEN_RES | (3 << (3 + (ch * 2))))) | (thInt << (3 + (ch * 2)));
}

uint32_t ADC_GetFlags(LPC_ADC_Type *pADC) {
	return pADC->FLAGS;
}

void ADC_ClearFlags(LPC_ADC_Type *pADC, uint32_t flags) {
	pADC->FLAGS = flags;
}

void ADC_SetTrim(LPC_ADC_Type *pADC, uint32_t trim) {
	pADC->TRM = trim;
}

/****************************************************************************
                            End Of File
****************************************************************************/
