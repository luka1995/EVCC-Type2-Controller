/****************************************************************************
	Luka Penger 2015
	WWDT LPC82x
****************************************************************************/

#include <WWDT_LPC82x.h>
#include <CLOCK_LPC82x.h>

/* Initialize the Watchdog timer */
void WWDT_Init(LPC_WWDT_Type *pWWDT) {
	Clock_EnablePeriphClock(SYSCTL_CLOCK_WWDT);

	/* Disable watchdog */
	pWWDT->MOD       = 0;
	pWWDT->TC        = 0xFF;
	pWWDT->WARNINT   = 0xFFFF;
	pWWDT->WINDOW    = 0xFFFFFF;
}

/* Shutdown the Watchdog timer */
void WWDT_DeInit(LPC_WWDT_Type *pWWDT) {
	Clock_DisablePeriphClock(SYSCTL_CLOCK_WWDT);
}

void WWDT_SetTimeOut(LPC_WWDT_Type *pWWDT, uint32_t timeout) {
	pWWDT->TC = timeout;
}

void WWDT_Feed(LPC_WWDT_Type *pWWDT) {
	pWWDT->FEED = 0xAA;
	pWWDT->FEED = 0x55;
}

void WWDT_SetWarning(LPC_WWDT_Type *pWWDT, uint32_t timeout) {
	pWWDT->WARNINT = timeout;
}

void WWDT_SetWindow(LPC_WWDT_Type *pWWDT, uint32_t timeout) {
	pWWDT->WINDOW = timeout;
}

void WWDT_SetOption(LPC_WWDT_Type *pWWDT, uint32_t options) {
	pWWDT->MOD = options | (pWWDT->MOD & ~WWDT_MOD_RESERVED);
}

void WWDT_UnsetOption(LPC_WWDT_Type *pWWDT, uint32_t options) {
	pWWDT->MOD &= (~options) & WWDT_WDMOD_BITMASK;
}

void WWDT_Start(LPC_WWDT_Type *pWWDT) {
	WWDT_SetOption(pWWDT, WWDT_WDMOD_WDEN);
	WWDT_Feed(pWWDT);
}

uint32_t WWDT_GetStatus(LPC_WWDT_Type *pWWDT) {
	return pWWDT->MOD;
}

void WWDT_ClearStatusFlag(LPC_WWDT_Type *pWWDT, uint32_t status) {
	if (status & WWDT_WDMOD_WDTOF) {
		pWWDT->MOD &= (~WWDT_WDMOD_WDTOF) & WWDT_WDMOD_BITMASK;
	}

	if (status & WWDT_WDMOD_WDINT) {
		pWWDT->MOD = WWDT_WDMOD_WDINT | (pWWDT->MOD & ~WWDT_MOD_RESERVED);
	}
}

uint32_t WWDT_GetCurrentCount(LPC_WWDT_Type *pWWDT) {
	return pWWDT->TV;
}

/****************************************************************************
                            End Of File
****************************************************************************/
