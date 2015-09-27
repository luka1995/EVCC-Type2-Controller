/****************************************************************************
	Luka Penger 2015
	CLOCK LPC82x
****************************************************************************/
#ifndef __CLOCK_LPC82x_H 
#define __CLOCK_LPC82x_H

#include <stdio.h>
#include <LPC82x.h>
#include <stdbool.h>
#include <SYSCON_LPC82x.h>

/* Internal oscillator frequency */
#define SYSCTL_IRC_FREQ     (12000000)

/**
 * Clock sources for system and USB PLLs
 */
typedef enum SYSCTL_PLLCLKSRC {
	SYSCTL_PLLCLKSRC_IRC = 0,		/*!< Internal oscillator */
	SYSCTL_PLLCLKSRC_SYSOSC,		/*!< Crystal (system) oscillator */
	SYSCTL_PLLCLKSRC_RESERVED,
	SYSCTL_PLLCLKSRC_EXT_CLKIN,	/*!< External clock input */
} SYSCTL_PLLCLKSRC_T;

/**
 * Watchdog oscillator analog output frequency selection
 * values enum (plus or minus 40%)
 */
typedef enum WDTLFO_OSC {
	WDTLFO_OSC_ILLEGAL,
	WDTLFO_OSC_0_60,	/*!< 0.6 MHz watchdog/LFO rate */
	WDTLFO_OSC_1_05,	/*!< 1.05 MHz watchdog/LFO rate */
	WDTLFO_OSC_1_40,	/*!< 1.4 MHz watchdog/LFO rate */
	WDTLFO_OSC_1_75,	/*!< 1.75 MHz watchdog/LFO rate */
	WDTLFO_OSC_2_10,	/*!< 2.1 MHz watchdog/LFO rate */
	WDTLFO_OSC_2_40,	/*!< 2.4 MHz watchdog/LFO rate */
	WDTLFO_OSC_2_70,	/*!< 2.7 MHz watchdog/LFO rate */
	WDTLFO_OSC_3_00,	/*!< 3.0 MHz watchdog/LFO rate */
	WDTLFO_OSC_3_25,	/*!< 3.25 MHz watchdog/LFO rate */
	WDTLFO_OSC_3_50,	/*!< 3.5 MHz watchdog/LFO rate */
	WDTLFO_OSC_3_75,	/*!< 3.75 MHz watchdog/LFO rate */
	WDTLFO_OSC_4_00,	/*!< 4.0 MHz watchdog/LFO rate */
	WDTLFO_OSC_4_20,	/*!< 4.2 MHz watchdog/LFO rate */
	WDTLFO_OSC_4_40,	/*!< 4.4 MHz watchdog/LFO rate */
	WDTLFO_OSC_4_60		/*!< 4.6 MHz watchdog/LFO rate */
} WDTLFO_OSC_T;

/**
 * Clock sources for main system clock
 */
typedef enum SYSCTL_MAINCLKSRC {
	SYSCTL_MAINCLKSRC_IRC = 0,		/*!< Internal oscillator */
	SYSCTL_MAINCLKSRC_PLLIN,		/*!< System PLL input */
	SYSCTL_MAINCLKSRC_WDTOSC,		/*!< Watchdog oscillator rate */
	SYSCTL_MAINCLKSRC_PLLOUT,		/*!< System PLL output */
} SYSCTL_MAINCLKSRC_T;

/**
 * System and peripheral clocks enum
 */
typedef enum SYSCTL_CLOCK {
	SYSCTL_CLOCK_SYS = 0,	/*!< System clock */
	SYSCTL_CLOCK_ROM,		/*!< ROM clock */
	SYSCTL_CLOCK_RAM,		/*!< RAM clock */
	SYSCTL_CLOCK_FLASHREG,	/*!< FLASH register interface clock */
	SYSCTL_CLOCK_FLASH,		/*!< FLASH array access clock */
	SYSCTL_CLOCK_I2C0,		/*!< I2C0 clock */
	SYSCTL_CLOCK_GPIO,		/*!< GPIO clock */
	SYSCTL_CLOCK_SWM,		/*!< Switch matrix clock */
	SYSCTL_CLOCK_SCT,		/*!< State configurable timer clock */
	SYSCTL_CLOCK_WKT,		/*!< Self wake-up timer clock */
	SYSCTL_CLOCK_MRT,		/*!< Multi-rate timer clock */
	SYSCTL_CLOCK_SPI0,		/*!< SPI0 clock */
	SYSCTL_CLOCK_SPI1,		/*!< SPI01 clock */
	SYSCTL_CLOCK_CRC,		/*!< CRC clock */
	SYSCTL_CLOCK_UART0,		/*!< UART0 clock */
	SYSCTL_CLOCK_UART1,		/*!< UART1 clock */
	SYSCTL_CLOCK_UART2,		/*!< UART2 clock */
	SYSCTL_CLOCK_WWDT,		/*!< Watchdog clock */
	SYSCTL_CLOCK_IOCON,		/*!< IOCON clock */
	SYSCTL_CLOCK_ACOMP,		/*!< Analog comparator clock */

	/* LPC82x Specific Clocks */
	SYSCTL_CLOCK_I2C1 = 21, /*!< I2C1 Clock */
	SYSCTL_CLOCK_I2C2,      /*!< I2C2 Clock */
	SYSCTL_CLOCK_I2C3,      /*!< I2C3 Clock */
	SYSCTL_CLOCK_ADC,       /*!< 12-Bit ADC Clock */
	SYSCTL_CLOCK_MTB = 26,  /*!< Macro Trace Buffer [USED FOR DEBUGGING] */
	SYSCTL_CLOCK_DMA = 29,  /*!< DMA Clock */
} SYSCTL_CLOCK_T;

/* Clock name alias */
#define SYSCTL_CLOCK_I2C       SYSCTL_CLOCK_I2C0
#define SYSCTL_CLOCK_ACMP     SYSCTL_CLOCK_ACOMP

/**
 * Clock sources for CLKOUT
 */
typedef enum SYSCTL_CLKOUTSRC {
	SYSCTL_CLKOUTSRC_IRC = 0,		/*!< Internal oscillator for CLKOUT */
	SYSCTL_CLKOUTSRC_SYSOSC,		/*!< System oscillator for CLKOUT */
	SYSCTL_CLKOUTSRC_WDTOSC,		/*!< Watchdog oscillator for CLKOUT */
	SYSCTL_CLKOUTSRC_MAINSYSCLK,	/*!< Main system clock for CLKOUT */
} SYSCTL_CLKOUTSRC_T;

/**
 * @brief	Set System PLL divider values
 * @param	msel    : PLL feedback divider value
 * @param	psel    : PLL post divider value
 * @return	Nothing
 * @note	See the user manual for how to setup the PLL
 */
void Clock_SetupSystemPLL(uint8_t msel, uint8_t psel);

/**
 * @brief	Read System PLL status
 * @return	true if the PLL is locked, false if not locked
 */
bool Clock_IsSystemPLLLocked(void);

/**
 * @brief	Setup Watchdog oscillator rate and divider
 * @param	wdtclk	: Selected watchdog clock rate
 * @param	div		: Watchdog divider value, even value between 2 and 64
 * @return	Nothing
 * @note	Watchdog rate = selected rate divided by divider rate
 */
void Clock_SetWDTOSC(WDTLFO_OSC_T wdtclk, uint8_t div);

/**
 * @brief   Returns the main clock source
 * @return	Main clock source
 */
SYSCTL_MAINCLKSRC_T Clock_GetMainClockSource(void);

/**
 * @brief	Set system clock divider
 * @param	div	: divider for system clock
 * @return	Nothing
 * @note	Use 0 to disable, or a divider value of 1 to 255. The system clock
 * rate is the main system clock divided by this value.
 */
void Clock_SetSysClockDiv(uint32_t div);

/**
 * @brief	Enable system or peripheral clock
 * @param	clk	: Clock to enable
 * @return	Nothing
 */
void Clock_EnablePeriphClock(SYSCTL_CLOCK_T clk);

/**
 * @brief	Disable system or peripheral clock
 * @param	clk	: Clock to disable
 * @return	Nothing
 */
void Clock_DisablePeriphClock(SYSCTL_CLOCK_T clk);

/**
 * @brief	Set UART divider clock
 * @param	div	: divider for UART clock
 * @return	Nothing
 * @note	Use 0 to disable, or a divider value of 1 to 255. The UART clock
 * rate is the main system clock divided by this value.
 */
void Clock_SetUARTClockDiv(uint32_t div);

/**
 * @brief	Return UART divider
 * @return	divider for UART clock
 * @note	A value of 0 means the clock is disabled.
 */
uint32_t Clock_GetUARTClockDiv(void);

/**
 * @brief	Set The USART Fractional Generator Divider
 * @param   div  :  Fractional Generator Divider value, should be 0xFF
 * @return	Nothing
 */
void SYSCTL_SetUSARTFRGDivider(uint8_t div);

/**
 * @brief	Get The USART Fractional Generator Divider
 * @return	Value of USART Fractional Generator Divider
 */
uint32_t SYSCTL_GetUSARTFRGDivider(void);

/**
 * @brief	Set The USART Fractional Generator Multiplier
 * @param   mult  :  An 8-bit value (0-255) U_PCLK = UARTCLKDIV/(1 + MULT/256)
 * @return	Nothing
 */
void SYSCTL_SetUSARTFRGMultiplier(uint8_t mult);

/**
 * @brief	Get The USART Fractional Generator Multiplier
 * @return	Value of USART Fractional Generator Multiplier
 */
uint32_t SYSCTL_GetUSARTFRGMultiplier(void);

/**
 * @brief	Set USART 0/1/2 UART base rate (up to main clock rate)
 * @param	rate	: Desired rate for fractional divider/multipler output
 * @param	fEnable	: true to use fractional clocking, false for integer clocking
 * @return	Actual rate generated
 * @note	USARTs 0 - 2 use the same base clock for their baud rate
 *			basis. This function is used to generate that clock, while the
 *			UART driver's SetBaud functions will attempt to get the closest
 *			baud rate from this base clock without altering it. This needs
 *			to be setup prior to individual UART setup.<br>
 *			UARTs need a base clock 16x faster than the baud rate, so if you
 *			need a 115.2Kbps baud rate, you will need a clock rate of at
 *			least (115.2K * 16). The UART base clock is generated from the
 *			main system clock, so fractional clocking may be the only
 *			possible choice when using a low main system clock frequency.
 *			Do not alter the FRGCLKDIV register after this call.
 */
uint32_t Clock_SetUSARTNBaseClockRate(uint32_t rate, bool fEnable);

/**
 * @brief	Get USART 0/1/2 UART base rate
 * @return	USART 0/1/2 UART base rate
 */
uint32_t Clock_GetUSARTNBaseClockRate(void);

/**
 * @brief	Returns the main oscillator clock rate
 * @return	main oscillator clock rate
 */
uint32_t Clock_GetMainOscRate(void);

/**
 * @brief	Returns the internal oscillator (IRC) clock rate
 * @return	internal oscillator (IRC) clock rate
 */
uint32_t Clock_GetIntOscRate(void);

/**
 * @brief	Returns the external clock input rate
 * @return	External clock input rate
 */
uint32_t Clock_GetExtClockInRate(void);

/**
 * @brief	Set System PLL clock source
 * @param	src	: Clock source for system PLL
 * @return	Nothing
 * @note	This function will also toggle the clock source update register
 * to update the clock source
 */
void Clock_SetSystemPLLSource(SYSCTL_PLLCLKSRC_T src);

/**
 * @brief	Bypass System Oscillator and set oscillator frequency range
 * @param	bypass	: Flag to bypass oscillator
 * @param	highfr	: Flag to set oscillator range from 15-25 MHz
 * @return	Nothing
 * @note	Sets the PLL input to bypass the oscillator. This would be
 * used if an external clock that is not an oscillator is attached
 * to the XTALIN pin.
 */
void Clock_SetPLLBypass(bool bypass, bool highfr);

/**
 * @brief	Set main system clock source
 * @param	src	: Clock source for main system
 * @return	Nothing
 * @note	This function will also toggle the clock source update register
 * to update the clock source
 */
void Clock_SetMainClockSource(SYSCTL_MAINCLKSRC_T src);

/**
 * @brief	Set CLKOUT clock source and divider
 * @param	src	: Clock source for CLKOUT
 * @param	div	: divider for CLKOUT clock
 * @return	Nothing
 * @note	Use 0 to disable, or a divider value of 1 to 255. The CLKOUT clock
 * rate is the clock source divided by the divider. This function will
 * also toggle the clock source update register to update the clock
 * source.
 */
void Clock_SetCLKOUTSource(SYSCTL_CLKOUTSRC_T src, uint32_t div);

/**
 * @brief	Return estimated watchdog oscillator rate
 * @return	Estimated watchdog oscillator rate
 * @note	This rate is accurate to plus or minus 40%.
 */
uint32_t Clock_GetWDTOSCRate(void);

/**
 * @brief	Return System PLL input clock rate
 * @return	System PLL input clock rate
 */
uint32_t Clock_GetSystemPLLInClockRate(void);

/**
 * @brief	Return System PLL output clock rate
 * @return	System PLL output clock rate
 */
uint32_t Clock_GetSystemPLLOutClockRate(void);

/**
 * @brief	Return main clock rate
 * @return	main clock rate
 */
uint32_t Clock_GetMainClockRate(void);

/**
 * @brief	Return system clock rate
 * @return	system clock rate
 */
uint32_t Clock_GetSystemClockRate(void);

/**
 * @brief	Get IOCONCLKDIV clock rate
 * @return	The clock rate going to the IOCON glitch filter
 * @note	Use 0 to disable, or a divider value of 1 to 255.
 */
uint32_t Clock_GetIOCONCLKDIVClockRate(void);

/**
 * @brief	Set IOCONCLKDIV divider
 * @param	div	: divider value for IOCONCLKDIV[reg] clock
 * @return	Nothing
 * @note	Use 0 to disable, or a divider value of 1 to 255.
 */
void Clock_SetIOCONCLKDIV(uint8_t div);

#endif // end __CLOCK_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
