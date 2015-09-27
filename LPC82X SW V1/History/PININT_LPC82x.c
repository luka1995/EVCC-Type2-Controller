/****************************************************************************
	Luka Penger 2015
	PIN INT LPC82x
****************************************************************************/

#include <PININT_LPC82x.h>

void PININT_SetPinModeEdge(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->ISEL &= ~(pins | PININT_ISEL_RESERVED);
}

void PININT_SetPinModeLevel(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->ISEL = pins | (pPININT->ISEL & ~PININT_ISEL_RESERVED);
}

uint32_t PININT_GetHighEnabled(LPC_PIN_INT_Type *pPININT) {
	return pPININT->IENR & ~PININT_IENR_RESERVED;
}

void PININT_EnableIntHigh(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->SIENR = pins;
}

void PININT_DisableIntHigh(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->CIENR = pins;
}

uint32_t PININT_GetLowEnabled(LPC_PIN_INT_Type *pPININT) {
	return pPININT->IENF & ~PININT_IENF_RESERVED;
}

void PININT_EnableIntLow(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->SIENF = pins;
}

void PININT_DisableIntLow(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->CIENF = pins;
}

uint32_t PININT_GetRiseStates(LPC_PIN_INT_Type *pPININT) {
	return pPININT->RISE & ~PININT_RISE_RESERVED;
}

void PININT_ClearRiseStates(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->RISE = pins;
}

uint32_t PININT_GetFallStates(LPC_PIN_INT_Type *pPININT) {
	return pPININT->FALL & ~PININT_FALL_RESERVED;
}

void PININT_ClearFallStates(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->FALL = pins;
}

uint32_t PININT_GetIntStatus(LPC_PIN_INT_Type *pPININT) {
	return pPININT->IST& ~PININT_IST_RESERVED;
}

void PININT_ClearIntStatus(LPC_PIN_INT_Type *pPININT, uint32_t pins) {
	pPININT->IST = pins;
}

void PININT_SetPatternMatchSrc(LPC_PIN_INT_Type *pPININT, uint8_t chan, PININT_BITSLICE_T slice) {
	uint32_t pmsrc_reg;
  
	/* Source source for pattern matching */ 
	pmsrc_reg = pPININT->PMSRC & ~((PININT_SRC_BITSOURCE_MASK << (PININT_SRC_BITSOURCE_START + (slice * 3)))
		| PININT_PMSRC_RESERVED);
	pPININT->PMSRC = pmsrc_reg | (chan << (PININT_SRC_BITSOURCE_START + (slice * 3)));
}

void PININT_SetPatternMatchConfig(LPC_PIN_INT_Type *pPININT, PININT_BITSLICE_T slice, 
        PININT_BITSLICE_CFG_T slice_cfg, bool end_point) {
	uint32_t pmcfg_reg;
    
	/* Configure bit slice configuration */
	pmcfg_reg = pPININT->PMCFG & ~((PININT_SRC_BITCFG_MASK << (PININT_SRC_BITCFG_START + (slice * 3)))
		| PININT_PMCFG_RESERVED);
	pPININT->PMCFG = pmcfg_reg | (slice_cfg << (PININT_SRC_BITCFG_START + (slice * 3)));

	/* If end point is true, enable the bits */
	if (end_point == true) {
        /* By default slice 7 is final component */
		if (slice != PININTBITSLICE7) {
			pPININT->PMCFG = (0x1 << slice) | (pPININT->PMCFG & ~PININT_PMCFG_RESERVED);
		}
	}
}

void PININT_EnablePatternMatch(LPC_PIN_INT_Type *pPININT) {
	pPININT->PMCTRL = PININT_PMCTRL_PMATCH_SEL | (pPININT->PMCTRL & ~PININT_PMCTRL_RESERVED);
}

void PININT_DisablePatternMatch(LPC_PIN_INT_Type *pPININT) {
	pPININT->PMCTRL &= ~(PININT_PMCTRL_PMATCH_SEL | PININT_PMCTRL_RESERVED);
}

void PININT_EnablePatternMatchRxEv(LPC_PIN_INT_Type *pPININT) {
	pPININT->PMCTRL = PININT_PMCTRL_RXEV_ENA | (pPININT->PMCTRL & ~PININT_PMCTRL_RESERVED);
}

void PININT_DisablePatternMatchRxEv(LPC_PIN_INT_Type *pPININT) {
	pPININT->PMCTRL &= ~(PININT_PMCTRL_RXEV_ENA | PININT_PMCTRL_RESERVED);
}

/****************************************************************************
                            End Of File
****************************************************************************/
