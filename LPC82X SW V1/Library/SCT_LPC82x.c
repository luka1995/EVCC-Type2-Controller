/****************************************************************************
	Luka Penger 2015
	SCT LPC82x
****************************************************************************/

#include <SCT_LPC82x.h>

void SCT_Config(LPC_SCT_Type *pSCT, uint32_t value) {
	pSCT->CONFIG = value;
}

void SCT_SetClrControl(LPC_SCT_Type *pSCT, uint32_t value, FunctionalState ena) {
	if (ena == ENABLE) {
		SCT_SetControl(pSCT, value);
	} else {
		SCT_ClearControl(pSCT, value);
	}
}

void SCT_SetConflictResolution(LPC_SCT_Type *pSCT, uint8_t outnum, uint8_t value) {
	uint32_t tem;
	
	tem = pSCT->RES & ~((0x03 << (2 * outnum))|SCT_RES_RESERVED);
	pSCT->RES = tem | (value << (2 * outnum));
}

void SCT_SetCount(LPC_SCT_Type *pSCT, uint32_t count) {
	pSCT->COUNT = count;
}

void SCT_SetMatchCount(LPC_SCT_Type *pSCT, SCT_MATCH_REG_T n, uint32_t value) {
	switch (n) {
		case SCT_MATCH_0: pSCT->MATCH0 = value; break;
		case SCT_MATCH_1: pSCT->MATCH1 = value; break;
		case SCT_MATCH_2: pSCT->MATCH2 = value; break;
		case SCT_MATCH_3: pSCT->MATCH3 = value; break;
		case SCT_MATCH_4: pSCT->MATCH4 = value; break;
		case SCT_MATCH_5: pSCT->MATCH5 = value; break;
		case SCT_MATCH_6: pSCT->MATCH6 = value; break;
		case SCT_MATCH_7: pSCT->MATCH7 = value; break;
	}
}

void SCT_SetMatchReload(LPC_SCT_Type *pSCT, SCT_MATCH_REG_T n, uint32_t value) {
	switch (n) {
		case SCT_MATCH_0: pSCT->MATCHREL0 = value; break;
		case SCT_MATCH_1: pSCT->MATCHREL1 = value; break;
		case SCT_MATCH_2: pSCT->MATCHREL2 = value; break;
		case SCT_MATCH_3: pSCT->MATCHREL3 = value; break;
		case SCT_MATCH_4: pSCT->MATCHREL4 = value; break;
		case SCT_MATCH_5: pSCT->MATCHREL5 = value; break;
		case SCT_MATCH_6: pSCT->MATCHREL6 = value; break;
		case SCT_MATCH_7: pSCT->MATCHREL7 = value; break;
	}
}

void SCT_EnableEventInt(LPC_SCT_Type *pSCT, SCT_EVENT_T evt) {
	pSCT->EVEN = evt | (pSCT->EVEN & ~SCT_EVEN_RESERVED);
}

void SCT_DisableEventInt(LPC_SCT_Type *pSCT, SCT_EVENT_T evt) {
	pSCT->EVEN &= ~(evt | SCT_EVEN_RESERVED);
}

void SCT_ClearEventFlag(LPC_SCT_Type *pSCT, SCT_EVENT_T evt) {
	pSCT->EVFLAG = evt | (pSCT->EVFLAG & ~SCT_EVFLAG_RESERVED);
}

void SCT_SetControl(LPC_SCT_Type *pSCT, uint32_t value) {
	pSCT->CTRL = value | (pSCT->CTRL & ~SCT_CTRL_RESERVED);
}

void SCT_ClearControl(LPC_SCT_Type *pSCT, uint32_t value) {
	pSCT->CTRL &= ~(value | SCT_CTRL_RESERVED);
}

void SCT_Init(LPC_SCT_Type *pSCT) {
	Clock_EnablePeriphClock(SYSCTL_CLOCK_SCT);
	SYSCTL_PeriphReset(RESET_SCT);
}

void SCT_DeInit(LPC_SCT_Type *pSCT) {
	Clock_DisablePeriphClock(SYSCTL_CLOCK_SCT);
}

/****************************************************************************
                            End Of File
****************************************************************************/
