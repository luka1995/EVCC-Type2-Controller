/****************************************************************************
	Luka Penger 2015
	FMC LPC82x
****************************************************************************/
#ifndef __FMC_LPC82x_H 
#define __FMC_LPC82x_H

#include <LPC82x.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <lpc_types.h>

/**
 * @brief FLASH Memory Controller Unit register block structure
 */
typedef struct {
	__I  uint32_t  RESERVED1[4];
	__IO uint32_t  FLASHCFG;		/*!< Flash Configuration register */
	__I  uint32_t  RESERVED2[3];
	__IO uint32_t  FMSSTART;		/*!< Signature start address register */
	__IO uint32_t  FMSSTOP;			/*!< Signature stop address register */
	__I  uint32_t  RESERVED3;
	__I  uint32_t  FMSW[1];			/*!< Signature word regsiter */
} LPC_FMC_T;

/* Reserved bits masks for registers */
#define FMC_FLASHCFG_RESERVED       (~3)
#define FMC_FMSSTART_RESERVED       0xfffe0000
#define FMC_FMSSTOP_RESERVED        0x7ffe0000

/**
 * @brief FLASH Access time definitions
 */
typedef enum {
	FLASHTIM_20MHZ_CPU = 0,		/*!< Flash accesses use 1 CPU clocks. Use for up to 20 MHz CPU clock*/
	FLASHTIM_30MHZ_CPU = 1, 	/*!< Flash accesses use 2 CPU clocks. Use for up to 30 MHz CPU clock*/
} FMC_FLASHTIM_T;

/**
 * @brief	Set FLASH memory access time in clocks
 * @param	clks	: Clock cycles for FLASH access
 * @return	Nothing
 * @note	For CPU speed up to 20MHz, use a value of 0. For up to 30MHz, use
 *			a value of 1
 */
void FMC_SetFLASHAccess(FMC_FLASHTIM_T clks);

/* Flash signature start and busy status bit */
#define FMC_FLASHSIG_BUSY	(1UL << 31)

/**
 * @brief	Start computation of a signature for a FLASH memory range
 * @param	start	: Starting FLASH address for computation, must be aligned on 16 byte boundary
 * @param	stop	: Ending FLASH address for computation, must be aligned on 16 byte boundary
 * @return	Nothing
 * @note	Only bits 20..4 are used for the FLASH signature computation.
 *			Use the Chip_FMC_IsSignatureBusy() function to determine when the
 *			signature computation operation is complete and the
 *			Chip_FMC_GetSignature() function to get the computed signature.
 */
void FMC_ComputeSignature(uint32_t start, uint32_t stop);

/**
 * @brief	Start computation of a signature for a FLASH memory address and block count
 * @param	start	: Starting FLASH address for computation, must be aligned on 16 byte boundary
 * @param	blocks	: Number of 16 byte blocks used for computation
 * @return	Nothing
 * @note	Only bits 20..4 are used for the FLASH signature computation.
 *			Use the Chip_FMC_IsSignatureBusy() function to determine when the
 *			signature computation operation is complete and the
 *			Chip_FMC_GetSignature() function to get the computed signature.
 */
void FMC_ComputeSignatureBlocks(uint32_t start, uint32_t blocks);

/**
 * @brief	Check for signature geenration completion
 * @return	true if the signature computation is running, false if finished
 */
bool FMC_IsSignatureBusy(void);

/**
 * @brief	Returns the generated FLASH signature value
 * @return	the generated FLASH signature value
 */
uint32_t FMC_GetSignature(void);

#endif // end __FMC_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
