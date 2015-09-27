/****************************************************************************
	Luka Penger 2015
	PIN INT LPC82x
****************************************************************************/
#ifndef __PININT_LPC82x_H 
#define __PININT_LPC82x_H

#include <LPC82x.h>
#include <stdbool.h>

/* Reserved bits masks for registers */
#define PININT_ISEL_RESERVED    (~0xff)
#define PININT_IENR_RESERVED    (~0xff)
#define PININT_SIENR_RESERVED   (~0xff)
#define PININT_CIENR_RESERVED   (~0xff)
#define PININT_IENF_RESERVED    (~0xff)
#define PININT_SIENF_RESERVED   (~0xff)
#define PININT_CIENF_RESERVED   (~0xff)
#define PININT_RISE_RESERVED    (~0xff)
#define PININT_FALL_RESERVED    (~0xff)
#define PININT_IST_RESERVED     (~0xff)
#define PININT_PMCTRL_RESERVED  (~0xff000003)
#define PININT_PMSRC_RESERVED   0xff
#define PININT_PMCFG_RESERVED   (1<<7)

/**
 * LPC8xx Pin Interrupt and Pattern match engine register
 * bit fields and macros
 */
/* PININT interrupt control register */
#define PININT_PMCTRL_PMATCH_SEL (1 << 0) 
#define PININT_PMCTRL_RXEV_ENA   (1 << 1) 

/* PININT Bit slice source register bits */
#define PININT_SRC_BITSOURCE_START  8
#define PININT_SRC_BITSOURCE_MASK   7

/* PININT Bit slice configuration register bits */
#define PININT_SRC_BITCFG_START  8
#define PININT_SRC_BITCFG_MASK   7

/**
 * LPC8xx Pin Interrupt channel values
 */
#define PININTCH0         (1 << 0)
#define PININTCH1         (1 << 1)
#define PININTCH2         (1 << 2)
#define PININTCH3         (1 << 3)
#define PININTCH4         (1 << 4)
#define PININTCH5         (1 << 5)
#define PININTCH6         (1 << 6)
#define PININTCH7         (1 << 7)
#define PININTCH(ch)      (1 << (ch))

/**
 * LPC8xx Pin Matching Interrupt bit slice enum values
 */
typedef enum PININT_BITSLICE {
	PININTBITSLICE0 = 0,	/*!< PININT Bit slice 0 */
	PININTBITSLICE1 = 1,	/*!< PININT Bit slice 1 */
	PININTBITSLICE2 = 2,	/*!< PININT Bit slice 2 */
	PININTBITSLICE3 = 3,	/*!< PININT Bit slice 3 */
	PININTBITSLICE4 = 4,	/*!< PININT Bit slice 4 */
	PININTBITSLICE5 = 5,	/*!< PININT Bit slice 5 */
	PININTBITSLICE6 = 6,	/*!< PININT Bit slice 6 */
	PININTBITSLICE7 = 7	/*!< PININT Bit slice 7 */
} PININT_BITSLICE_T;

/**
 * LPC8xx Pin Matching Interrupt bit slice configuration enum values
 */
typedef enum PININT_BITSLICE_CFG {
    PININT_PATTERNCONST1           = 0x0,	/*!< Contributes to product term match */
    PININT_PATTERNRISING           = 0x1,	/*!< Rising edge */    
    PININT_PATTERNFALLING          = 0x2,	/*!< Falling edge */    
    PININT_PATTERNRISINGRFALLING   = 0x3,	/*!< Rising or Falling edge */    
    PININT_PATTERNHIGH             = 0x4,	/*!< High level */    
    PININT_PATTERNLOW              = 0x5,	/*!< Low level */    
    PININT_PATTERCONST0            = 0x6,	/*!< Never contributes for match */    
    PININT_PATTEREVENT             = 0x7	/*!< Match occurs on event */    
} PININT_BITSLICE_CFG_T;

/**
 * @brief	Configure the pins as edge sensitive in Pin interrupt block 
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_SetPinModeEdge(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Configure the pins as level sensitive in Pin interrupt block 
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_SetPinModeLevel(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Return current PININT rising edge or high level interrupt enable state
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	A bifield containing the high edge/level interrupt enables for each
 * interrupt. Bit 0 = PININT0, 1 = PININT1, etc.
 * For each bit, a 0 means the high edge/level interrupt is disabled, while a 1
 * means it's enabled.
 */
uint32_t PININT_GetHighEnabled(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Enable high edge/level PININT interrupts for pins
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins to enable (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_EnableIntHigh(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Disable high edge/level PININT interrupts for pins
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins to disable (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_DisableIntHigh(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Return current PININT falling edge or low level interrupt enable state
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	A bifield containing the low edge/level interrupt enables for each
 * interrupt. Bit 0 = PININT0, 1 = PININT1, etc.
 * For each bit, a 0 means the low edge/level interrupt is disabled, while a 1
 * means it's enabled.
 */
uint32_t PININT_GetLowEnabled(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Enable low edge/level PININT interrupts for pins
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins to enable (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_EnableIntLow(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Disable low edge/level PININT interrupts for pins
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins to disable (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_DisableIntLow(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Return pin states that have a detected latched high edge (RISE) state
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	PININT states (bit n = high) with a latched rise state detected
 */
uint32_t PININT_GetRiseStates(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Clears pin states that had a latched high edge (RISE) state
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins with latched states to clear
 * @return	Nothing
 */
void PININT_ClearRiseStates(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Return pin states that have a detected latched falling edge (FALL) state
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	PININT states (bit n = high) with a latched rise state detected
 */
uint32_t PININT_GetFallStates(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Clears pin states that had a latched falling edge (FALL) state
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pins with latched states to clear
 * @return	Nothing
 */
void PININT_ClearFallStates(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Get interrupt status from Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	Interrupt status (bit n for PININTn = high means interrupt ie pending)
 */
uint32_t PININT_GetIntStatus(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Clear interrupt status in Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	pins	: Pin interrupts to clear (ORed value of PININTCH*)
 * @return	Nothing
 */
void PININT_ClearIntStatus(LPC_PIN_INT_Type *pPININT, uint32_t pins);

/**
 * @brief	Set source for pattern match in Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @param	chan	: PININT channel number (From 0 to 7)
 * @param	slice	: PININT slice number
 * @return	Nothing
 */
void PININT_SetPatternMatchSrc(LPC_PIN_INT_Type *pPININT, uint8_t chan, PININT_BITSLICE_T slice);

/**
 * @brief	Configure the pattern matcch in Pin interrupt block
 * @param	pPININT		: The base address of Pin interrupt block 
 * @param	slice		: PININT slice number
 * @param	slice_cfg	: PININT slice configuration value (enum PININT_BITSLICE_CFG_T)
 * @param	end_point	: If true, current slice is final component
 * @return	Nothing
 */
void PININT_SetPatternMatchConfig(LPC_PIN_INT_Type *pPININT, PININT_BITSLICE_T slice, 
        PININT_BITSLICE_CFG_T slice_cfg, bool end_point);

/**
 * @brief	Enable pattern match interrupts in Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	Nothing
 */
void PININT_EnablePatternMatch(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Disable pattern match interrupts in Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	Nothing
 */
void PININT_DisablePatternMatch(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Enable RXEV output in Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	Nothing
 */
void PININT_EnablePatternMatchRxEv(LPC_PIN_INT_Type *pPININT);

/**
 * @brief	Disable RXEV output in Pin interrupt block
 * @param	pPININT	: The base address of Pin interrupt block 
 * @return	Nothing
 */
void PININT_DisablePatternMatchRxEv(LPC_PIN_INT_Type *pPININT);

#endif // end __PININT_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
