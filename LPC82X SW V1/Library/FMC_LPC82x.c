/****************************************************************************
	Luka Penger 2015
	FMC LPC82x
****************************************************************************/

#include <FMC_LPC82x.h>

void FMC_SetFLASHAccess(FMC_FLASHTIM_T clks) {
	uint32_t tmp = LPC_FLASHCTRL->FLASHCFG & (~((0x3) | FMC_FLASHCFG_RESERVED));

	/* Don't alter upper bits */
	LPC_FLASHCTRL->FLASHCFG = tmp | clks;
}

void FMC_ComputeSignature(uint32_t start, uint32_t stop) {
	LPC_FLASHCTRL->FMSSTART = (start >> 4);
	LPC_FLASHCTRL->FMSSTOP = (stop >> 4) | FMC_FLASHSIG_BUSY;
}

void FMC_ComputeSignatureBlocks(uint32_t start, uint32_t blocks) {
	FMC_ComputeSignature(start, (start + (blocks * 16)));
}

bool FMC_IsSignatureBusy(void) {
	return (bool) ((LPC_FLASHCTRL->FMSSTOP & FMC_FLASHSIG_BUSY) != 0);
}

uint32_t FMC_GetSignature(void) {
	return LPC_FLASHCTRL->FMSW0;
}

/****************************************************************************
                            End Of File
****************************************************************************/
