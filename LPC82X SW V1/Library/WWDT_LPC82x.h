/****************************************************************************
	Luka Penger 2015
	WWDT LPC82x
****************************************************************************/
#ifndef __WWDT_LPC82x_H 
#define __WWDT_LPC82x_H

#include <LPC82x.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <lpc_types.h>

/*!< WDT oscillator frequency value */
#define WDT_OSC     (LPC8XX_IRC_FREQ)

/* Reserved bits masks for registers */
#define WWDT_MOD_RESERVED       (~0x3f)
#define WWDT_TC_RESERVED        0xff000000
#define WWDT_FEED_RESERVED      (~0xff)
#define WWDT_TV_RESERVED        0xff000000
#define WWDT_WARNINT_RESERVED   (~0x3ff)
#define WWDT_WINDOW_RESERVED    0xff000000

/**
 * @brief Watchdog Mode register definitions
 */
/** Watchdog Mode Bitmask */
#define WWDT_WDMOD_BITMASK          ((uint32_t) 0x1F)
/** WWDT interrupt enable bit */
#define WWDT_WDMOD_WDEN             ((uint32_t) (1 << 0))
/** WWDT interrupt enable bit */
#define WWDT_WDMOD_WDRESET          ((uint32_t) (1 << 1))
/** WWDT time out flag bit */
#define WWDT_WDMOD_WDTOF            ((uint32_t) (1 << 2))
/** WDT Time Out flag bit */
#define WWDT_WDMOD_WDINT            ((uint32_t) (1 << 3))

/**
 * @brief	Initialize the Watchdog timer
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @return	None
 */
void WWDT_Init(LPC_WWDT_Type *pWWDT);

/**
 * @brief	Shutdown the Watchdog timer
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @return	None
 */
void WWDT_DeInit(LPC_WWDT_Type *pWWDT);

/**
 * @brief	Set WDT timeout constant value used for feed
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	timeout	: WDT timeout in ticks, between WWDT_TICKS_MIN and WWDT_TICKS_MAX
 * @return	none
 */
void WWDT_SetTimeOut(LPC_WWDT_Type *pWWDT, uint32_t timeout);

/**
 * @brief	Feed watchdog timer
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @return	None
 * @note	If this function isn't called, a watchdog timer warning will occur.
 * After the warning, a timeout will occur if a feed has happened.
 */
void WWDT_Feed(LPC_WWDT_Type *pWWDT);

/**
 * @brief	Set WWDT warning interrupt
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	timeout	: WDT warning in ticks, between 0 and 1023
 * @return	None
 * @note	This is the number of ticks after the watchdog interrupt that the
 * warning interrupt will be generated.
 */
void WWDT_SetWarning(LPC_WWDT_Type *pWWDT, uint32_t timeout);

/**
 * @brief	Set WWDT window time
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	timeout	: WDT timeout in ticks, between WWDT_TICKS_MIN and WWDT_TICKS_MAX
 * @return	None
 * @note	The watchdog timer must be fed between the timeout from the Chip_WWDT_SetTimeOut()
 * function and this function, with this function defining the last tick before the
 * watchdog window interrupt occurs.
 */
void WWDT_SetWindow(LPC_WWDT_Type *pWWDT, uint32_t timeout);

/**
 * @brief	Enable watchdog timer options
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	options	: An or'ed set of options of values
 *						WWDT_WDMOD_WDEN, WWDT_WDMOD_WDRESET, and WWDT_WDMOD_WDPROTECT
 * @return	None
 * @note	You can enable more than one option at once (ie, WWDT_WDMOD_WDRESET |
 * WWDT_WDMOD_WDPROTECT), but use the WWDT_WDMOD_WDEN after all other options
 * are set (or unset) with no other options. If WWDT_WDMOD_LOCK is used, it cannot
 * be unset.
 */
void WWDT_SetOption(LPC_WWDT_Type *pWWDT, uint32_t options);

/**
 * @brief	Disable/clear watchdog timer options
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	options	: An or'ed set of options of values
 *						WWDT_WDMOD_WDEN, WWDT_WDMOD_WDRESET, and WWDT_WDMOD_WDPROTECT
 * @return	None
 * @note	You can disable more than one option at once (ie, WWDT_WDMOD_WDRESET |
 * WWDT_WDMOD_WDTOF).
 */
void WWDT_UnsetOption(LPC_WWDT_Type *pWWDT, uint32_t options);

/**
 * @brief	Enable WWDT activity
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @return	None
 */
void WWDT_Start(LPC_WWDT_Type *pWWDT);

/**
 * @brief	Read WWDT status flag
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @return	Watchdog status, an Or'ed value of WWDT_WDMOD_*
 */
uint32_t WWDT_GetStatus(LPC_WWDT_Type *pWWDT);

/**
 * @brief	Clear WWDT interrupt status flags
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	status	: Or'ed value of status flag(s) that you want to clear, should be:
 *              - WWDT_WDMOD_WDTOF: Clear watchdog timeout flag
 *              - WWDT_WDMOD_WDINT: Clear watchdog warning flag
 * @return	None
 */
void WWDT_ClearStatusFlag(LPC_WWDT_Type *pWWDT, uint32_t status);

/**
 * @brief	Get the current value of WDT
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @return	current value of WDT
 */
uint32_t WWDT_GetCurrentCount(LPC_WWDT_Type *pWWDT);

#endif // end __WWDT_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
