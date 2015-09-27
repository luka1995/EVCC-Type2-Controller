/****************************************************************************
	Luka Penger 2015
	SYSCON LPC82x
****************************************************************************/

#include <SYSCON_LPC82x.h>

/* PDSLEEPCFG register mask */
#define PDSLEEPWRMASK   (0x0000FFB7)
#define PDSLEEPDATMASK  (0x00000048)

/* PDWAKECFG and PDRUNCFG register masks */
#define PDWAKEUPWRMASK  (0x00006D00)
#define PDWAKEUPDATMASK (0x000080FF)

void SYSCTL_Map(SYSCTL_BOOT_MODE_REMAP_T remap) {
	LPC_SYSCON->SYSMEMREMAP = (uint32_t)remap;
}

void SYSCTL_AssertPeriphReset(SYSCTL_PERIPH_RESET_T periph) {
	LPC_SYSCON->PRESETCTRL &= ~((1 << (uint32_t)periph) | SYSCTL_PRESETCTRL_RESERVED);
}

void SYSCTL_DeassertPeriphReset(SYSCTL_PERIPH_RESET_T periph) {
	LPC_SYSCON->PRESETCTRL = (1 << (uint32_t) periph) | (LPC_SYSCON->PRESETCTRL & ~SYSCTL_PRESETCTRL_RESERVED);
}

void SYSCTL_PeriphReset(SYSCTL_PERIPH_RESET_T periph) {
	SYSCTL_AssertPeriphReset(periph);
	SYSCTL_DeassertPeriphReset(periph);
}

uint32_t SYSCTL_GetSystemRSTStatus(void) {
	return LPC_SYSCON->SYSRSTSTAT & ~SYSCTL_SYSRSTSTAT_RESERVED;
}

void SYSCTL_ClearSystemRSTStatus(uint32_t reset) {
	LPC_SYSCON->SYSRSTSTAT = reset;
}

uint32_t SYSCTL_GetPORPIOStatus(void) {
	return LPC_SYSCON->PIOPORCAP0 & ~SYSCTL_PIOPORCAP0_RESERVED;
}

void SYSCTL_SetBODLevels(SYSCTL_BODRSTLVL_T rstlvl,
											SYSCTL_BODRINTVAL_T intlvl) {
	LPC_SYSCON->BODCTRL = ((uint32_t)rstlvl) | (((uint32_t)intlvl) << 2);
}

void SYSCTL_EnableBODReset(void) {
	LPC_SYSCON->BODCTRL = (1 << 4) | (LPC_SYSCON->BODCTRL & ~SYSCTL_BODCTRL_RESERVED);
}

void SYSCTL_DisableBODReset(void) {
	LPC_SYSCON->BODCTRL &= ~((1 << 4) | SYSCTL_BODCTRL_RESERVED);
}

void SYSCTL_SetSYSTCKCAL(uint32_t sysCalVal) {
	LPC_SYSCON->SYSTCKCAL = sysCalVal;
}

void SYSCTL_SetIRQLatency(uint32_t latency) {
	LPC_SYSCON->IRQLATENCY = latency;
}

uint32_t SYSCTL_GetIRQLatency(void) {
	return LPC_SYSCON->IRQLATENCY & ~SYSCTL_IRQLATENCY_RESERVED;
}

void SYSCTL_SetNMISource(uint32_t intsrc) {
	/* Disable NMI first */
  LPC_SYSCON->NMISRC &= ~(SYSCTL_NMISRC_ENABLE | SYSCTL_NMISRC_RESERVED);
    
  /* Set new NMI source. */
	LPC_SYSCON->NMISRC = intsrc;
}

void SYSCTL_EnableNMISource(void) {
	LPC_SYSCON->NMISRC = SYSCTL_NMISRC_ENABLE | (LPC_SYSCON->NMISRC & ~SYSCTL_NMISRC_RESERVED);
}

void SYSCTL_DisableNMISource(void) {
	LPC_SYSCON->NMISRC &= ~(SYSCTL_NMISRC_ENABLE | SYSCTL_NMISRC_RESERVED);
}

void SYSCTL_SetPinInterrupt(uint32_t intno, uint32_t pin) {
	switch (intno) {
		case 0: LPC_SYSCON->PINTSEL0 = (uint32_t)pin; break;
		case 1: LPC_SYSCON->PINTSEL1 = (uint32_t)pin; break;
		case 2: LPC_SYSCON->PINTSEL2 = (uint32_t)pin; break;
		case 3: LPC_SYSCON->PINTSEL3 = (uint32_t)pin; break;
		case 4: LPC_SYSCON->PINTSEL4 = (uint32_t)pin; break;
		case 5: LPC_SYSCON->PINTSEL5 = (uint32_t)pin; break;
		case 6: LPC_SYSCON->PINTSEL6 = (uint32_t)pin; break;
		case 7: LPC_SYSCON->PINTSEL7 = (uint32_t)pin; break;
	}
}

void SYSCTL_EnablePINTWakeup(uint32_t pin) {
	LPC_SYSCON->STARTERP0 = (1 << pin) | (LPC_SYSCON->STARTERP0 & ~SYSCTL_STARTERP0_RESERVED);
}

void SYSCTL_DisablePINTWakeup(uint32_t pin) {
	LPC_SYSCON->STARTERP0 &= ~((1 << pin) | SYSCTL_STARTERP0_RESERVED);
}

void SYSCTL_EnablePeriphWakeup(uint32_t periphmask) {
	LPC_SYSCON->STARTERP1 = periphmask | (LPC_SYSCON->STARTERP0 & ~SYSCTL_STARTERP0_RESERVED);
}

void SYSCTL_DisablePeriphWakeup(uint32_t periphmask) {
	LPC_SYSCON->STARTERP1 &= ~(periphmask | SYSCTL_STARTERP1_RESERVED);
}

uint32_t SYSCTL_GetDeepSleepPD(void) {
	return LPC_SYSCON->PDSLEEPCFG;
}

uint32_t SYSCTL_GetWakeup(void) {
	return LPC_SYSCON->PDAWAKECFG;
}

uint32_t SYSCTL_GetPowerStates(void) {
	return LPC_SYSCON->PDRUNCFG;
}

uint32_t SYSCTL_GetDeviceID(void) {
	return LPC_SYSCON->DEVICE_ID;
}

/* Setup deep sleep behaviour for power down */
void SYSCTL_SetDeepSleepPD(uint32_t sleepmask) {
	/* Update new value */
	LPC_SYSCON->PDSLEEPCFG = PDSLEEPWRMASK | (sleepmask & PDSLEEPDATMASK);
}

/* Setup wakeup behaviour from deep sleep */
void SYSCTL_SetWakeup(uint32_t wakeupmask) {
	/* Update new value */
	LPC_SYSCON->PDAWAKECFG = PDWAKEUPWRMASK | (wakeupmask & PDWAKEUPDATMASK);
}

/* Power down one or more blocks or peripherals */
void SYSCTL_PowerDown(uint32_t powerdownmask) {
	uint32_t pdrun;

	/* Get current power states */
	pdrun = LPC_SYSCON->PDRUNCFG & PDWAKEUPDATMASK;

	/* Disable peripheral states by setting high */
	pdrun |= (powerdownmask & PDWAKEUPDATMASK);

	/* Update power states with required register bits */
	LPC_SYSCON->PDRUNCFG = (PDWAKEUPWRMASK | pdrun);
}

/* Power up one or more blocks or peripherals */
void SYSCTL_PowerUp(uint32_t powerupmask) {
	uint32_t pdrun;

	/* Get current power states */
	pdrun = LPC_SYSCON->PDRUNCFG & PDWAKEUPDATMASK;

	/* Enable peripheral states by setting low */
	pdrun &= ~(powerupmask & PDWAKEUPDATMASK);

	/* Update power states with required register bits */
	LPC_SYSCON->PDRUNCFG = (PDWAKEUPWRMASK | pdrun);
}

/****************************************************************************
                            End Of File
****************************************************************************/
