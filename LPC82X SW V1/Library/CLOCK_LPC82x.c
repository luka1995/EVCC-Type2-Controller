/****************************************************************************
	Luka Penger 2015
	CLOCK LPC82x
****************************************************************************/

#include <CLOCK_LPC82x.h>

const uint32_t OscRateIn = 12000000;
const uint32_t ExtRateIn = 0;

/* Inprecise clock rates for the watchdog oscillator */
static const uint32_t wdtOSCRate[WDTLFO_OSC_4_60 + 1] = {
	0,					/* WDT_OSC_ILLEGAL */
	600000,				/* WDT_OSC_0_60 */
	1050000,			/* WDT_OSC_1_05 */
	1400000,			/* WDT_OSC_1_40 */
	1750000,			/* WDT_OSC_1_75 */
	2100000,			/* WDT_OSC_2_10 */
	2400000,			/* WDT_OSC_2_40 */
	2700000,			/* WDT_OSC_2_70 */
	3000000,			/* WDT_OSC_3_00 */
	3250000,			/* WDT_OSC_3_25 */
	3500000,			/* WDT_OSC_3_50 */
	3750000,			/* WDT_OSC_3_75 */
	4000000,			/* WDT_OSC_4_00 */
	4200000,			/* WDT_OSC_4_20 */
	4400000,			/* WDT_OSC_4_40 */
	4600000				/* WDT_OSC_4_60 */
};

uint32_t Clock_GetWDTLFORate(uint32_t reg) {
	uint32_t div;
	WDTLFO_OSC_T clk;

	/* Get WDT oscillator settings */
	clk = (WDTLFO_OSC_T) ((reg >> 5) & 0xF);
	div = reg & 0x1F;

	/* Compute clock rate and divided by divde value */
	return wdtOSCRate[clk] / ((div + 1) << 1);
}

uint32_t Clock_GetPLLFreq(uint32_t PLLReg, uint32_t inputRate) {
	uint32_t m_val = ((PLLReg & 0x1F) + 1);

	return (inputRate * m_val);
}

void Clock_SetupSystemPLL(uint8_t msel, uint8_t psel) {
	LPC_SYSCON->SYSPLLCTRL = (msel & 0x1F) | ((psel & 0x3) << 5);
}

bool Clock_IsSystemPLLLocked(void) {
	return (bool)((LPC_SYSCON->SYSPLLSTAT & 1) != 0);
}

void Clock_SetWDTOSC(WDTLFO_OSC_T wdtclk, uint8_t div) {
	LPC_SYSCON->WDTOSCCTRL  = (((uint32_t)wdtclk) << 5) | ((div >> 1) - 1);
}

SYSCTL_MAINCLKSRC_T Clock_GetMainClockSource(void) {
	return (SYSCTL_MAINCLKSRC_T) (LPC_SYSCON->MAINCLKSEL & ~SYSCTL_MAINCLKSEL_RESERVED);
}

void Clock_SetSysClockDiv(uint32_t div) {
	LPC_SYSCON->SYSAHBCLKDIV  = div;
}

void Clock_EnablePeriphClock(SYSCTL_CLOCK_T clk) {
	LPC_SYSCON->SYSAHBCLKCTRL = (1 << clk) | (LPC_SYSCON->SYSAHBCLKCTRL & ~SYSCTL_SYSAHBCLKCTRL_RESERVED);
}

void Clock_DisablePeriphClock(SYSCTL_CLOCK_T clk) {
	LPC_SYSCON->SYSAHBCLKCTRL &= ~((1 << clk) | SYSCTL_SYSAHBCLKCTRL_RESERVED);
}

void Clock_SetUARTClockDiv(uint32_t div) {
	LPC_SYSCON->UARTCLKDIV = div;
}

uint32_t Clock_GetUARTClockDiv(void) {
	return LPC_SYSCON->UARTCLKDIV & ~SYSCTL_UARTCLKDIV_RESERVED;
}

void SYSCTL_SetUSARTFRGDivider(uint8_t div) {
	LPC_SYSCON->UARTFRGDIV = (uint32_t)div;
}

uint32_t SYSCTL_GetUSARTFRGDivider(void) {
	return LPC_SYSCON->UARTFRGDIV & ~SYSCTL_UARTFRGDIV_RESERVED;
}

void SYSCTL_SetUSARTFRGMultiplier(uint8_t mult) {
	LPC_SYSCON->UARTFRGMULT = (uint32_t)mult;
}

uint32_t SYSCTL_GetUSARTFRGMultiplier(void) {
	return LPC_SYSCON->UARTFRGMULT & ~SYSCTL_UARTFRGMULT_RESERVED;
}

uint32_t Clock_SetUSARTNBaseClockRate(uint32_t rate, bool fEnable) {
	uint32_t div, inclk;

	/* Input clock into FRG block is the main system clock */
	inclk = Clock_GetMainClockRate();

	/* Get integer divider for coarse rate */
	div = inclk / rate;
	if (div == 0) {
		div = 1;
	}

	/* Approximated rate with only integer divider */
	Clock_SetUARTClockDiv((uint8_t) div);

	if (fEnable) {
		uint32_t uart_fra_multiplier;

		/* Reset FRG */
		SYSCTL_PeriphReset(RESET_UARTFBRG);

		/* Enable fractional divider */
		SYSCTL_SetUSARTFRGDivider(0xFF);

		/* Compute the fractional divisor (the lower byte is the
		   fractional portion) */
		uart_fra_multiplier = ((inclk / div) * 256) / rate;

		/* ...just the fractional portion (the lower byte) */
		SYSCTL_SetUSARTFRGMultiplier((uint8_t)uart_fra_multiplier);
	} else {
		/* Disable fractional generator and use integer divider only */
		SYSCTL_SetUSARTFRGDivider(0);
	}

	return Clock_GetUSARTNBaseClockRate();
}

uint32_t Clock_GetIOCONCLKDIVClockRate(void) {
	uint32_t div = LPC_SYSCON->IOCONCLKDIV6 & ~SYSCTL_IOCONCLKDIV_RESERVED;
	uint32_t main_clk = Clock_GetMainClockRate();
	
	return (div == 0) ? 0 : (main_clk / div);
}

void Clock_SetIOCONCLKDIV(uint8_t div) {
	LPC_SYSCON->IOCONCLKDIV6 = div;
}

uint32_t Clock_GetMainOscRate(void) {
	return OscRateIn;
}

uint32_t Clock_GetIntOscRate(void) {
	return SYSCTL_IRC_FREQ;
}

uint32_t Clock_GetExtClockInRate(void) {
	return ExtRateIn;
}

void Clock_SetSystemPLLSource(SYSCTL_PLLCLKSRC_T src) {
	LPC_SYSCON->SYSPLLCLKSEL  = (uint32_t)src;
    
    /* sequnce a 0 followed by 1 to update PLL source selection */
	LPC_SYSCON->SYSPLLCLKUEN  = 0;  
	LPC_SYSCON->SYSPLLCLKUEN  = 1;  
}

void Clock_SetPLLBypass(bool bypass, bool highfr) {
	uint32_t ctrl = 0;

	if (bypass) {
		ctrl |= (1 << 0);
	}
	if (highfr) {
		ctrl |= (1 << 1);
	}

	LPC_SYSCON->SYSOSCCTRL = ctrl;
}

void Clock_SetMainClockSource(SYSCTL_MAINCLKSRC_T src) {
	LPC_SYSCON->MAINCLKSEL  = (uint32_t)src;

	LPC_SYSCON->MAINCLKUEN  = 0;
	LPC_SYSCON->MAINCLKUEN  = 1;
}

void Clock_SetCLKOUTSource(SYSCTL_CLKOUTSRC_T src, uint32_t div) {
	LPC_SYSCON->CLKOUTSEL = (uint32_t) src;
	
	LPC_SYSCON->CLKOUTUEN = 0;
	LPC_SYSCON->CLKOUTUEN = 1;
	LPC_SYSCON->CLKOUTDIV = div;
}

uint32_t Clock_GetWDTOSCRate(void) {
	return Clock_GetWDTLFORate(LPC_SYSCON->WDTOSCCTRL & ~SYSCTL_WDTOSCCTRL_RESERVED);
}

uint32_t Clock_GetSystemPLLInClockRate(void) {
	uint32_t clkRate;

	switch ((SYSCTL_PLLCLKSRC_T) (LPC_SYSCON->SYSPLLCLKSEL & 0x3)) {
	case SYSCTL_PLLCLKSRC_IRC:
		clkRate = Clock_GetIntOscRate();
		break;

	case SYSCTL_PLLCLKSRC_SYSOSC:
		clkRate = Clock_GetMainOscRate();
		break;

	case SYSCTL_PLLCLKSRC_EXT_CLKIN:
		clkRate = Clock_GetExtClockInRate();
		break;

	default:
		clkRate = 0;
	}

	return clkRate;
}

uint32_t Clock_GetSystemPLLOutClockRate(void) {
	return Clock_GetPLLFreq((LPC_SYSCON->SYSPLLCTRL & ~SYSCTL_SYSPLLCTRL_RESERVED),
								 Clock_GetSystemPLLInClockRate());
}

uint32_t Clock_GetMainClockRate(void) {
	uint32_t clkRate = 0;

	switch ((SYSCTL_MAINCLKSRC_T) (LPC_SYSCON->MAINCLKSEL & 0x3)) {
	case SYSCTL_MAINCLKSRC_IRC:
		clkRate = Clock_GetIntOscRate();
		break;

	case SYSCTL_MAINCLKSRC_PLLIN:
		clkRate = Clock_GetSystemPLLInClockRate();
		break;

	case SYSCTL_MAINCLKSRC_WDTOSC:
		clkRate = Clock_GetWDTOSCRate();
		break;

	case SYSCTL_MAINCLKSRC_PLLOUT:
		clkRate = Clock_GetSystemPLLOutClockRate();
		break;
	}

	return clkRate;
}

uint32_t Clock_GetSystemClockRate(void) {
	return Clock_GetMainClockRate() / (LPC_SYSCON->SYSAHBCLKDIV & ~SYSCTL_SYSAHBCLKDIV_RESERVED);
}

uint32_t Clock_GetUSARTNBaseClockRate(void) {
	uint64_t inclk;
	uint32_t div;

	div = (uint32_t)Clock_GetUARTClockDiv();
	if (div == 0) {
		/* Divider is 0 so UART clock is disabled */
		inclk = 0;
	} else {
		uint32_t mult, divf;

		/* Input clock into FRG block is the divided main system clock */
		inclk = (uint64_t) (Clock_GetMainClockRate() / div);

		divf = SYSCTL_GetUSARTFRGDivider();
		if (divf == 0xFF) {
			/* Fractional part is enabled, get multiplier */
			mult = (uint32_t)SYSCTL_GetUSARTFRGMultiplier();

			/* Get fractional error */
			inclk = (inclk * 256) / (uint64_t) (256 + mult);
		}
	}

	return (uint32_t) inclk;
}

/****************************************************************************
                            End Of File
****************************************************************************/
