/****************************************************************************
	Luka Penger 2015
	SCT LPC82x
****************************************************************************/
#ifndef __SCT_LPC82x_H 
#define __SCT_LPC82x_H

#include <LPC82x.h>
#include <CLOCK_LPC82x.h>
#include <SYSCON_LPC82x.h>
#include <lpc_types.h>

/* Reserved bits masks for registers */
#define SCT_CONFIG_RESERVED             0xfff80000
#define SCT_CTRL_RESERVED               ((7<<13)|(7u<<29))
#define SCT_LIMIT_RESERVED              (~(0x3f|(0x3f<<16))
#define SCT_HALT_RESERVED               (~(0x3f|(0x3f<<16))
#define SCT_STOP_RESERVED               (~(0x3f|(0x3f<<16))
#define SCT_START_RESERVED              (~(0x3f|(0x3f<<16))
#define SCT_STATE_RESERVED              (~(0x1f|(0x1f<<16))
#define SCT_INPUT_RESERVED              (~(0xf|(0xf<<16))
#define SCT_REGMODE_RESERVED            (~(0x1f|(0x1f<<16))
#define SCT_OUTPUT_RESERVED             (~0xf)
#define SCT_OUTPUTDIRCTRL_RESERVED      (~0xff)
#define SCT_RES_RESERVED                (~0xff)
#define SCT_EVEN_RESERVED               (~0x3f)
#define SCT_EVFLAG_RESERVED             (~0x3f)
#define SCT_CONEN_RESERVED              (~0xf)
#define SCT_CONFLAG_RESERVED            (~(0xf|(3u<<30)))
#define SCT_CAPCTRL_RESERVED            (~(0x3f|(0x3f<<16)))
#define SCT_EVn_STATE_RESERVED          (~3)
#define SCT_EVn_CTRL_RESERVED           (0xff800000)
#define SCT_OUTn_SET_RESERVED           (~0x3f)
#define SCT_OUTn_CLR_RESERVED           (~0x3f)

/*
 * @brief Macro defines for SCT configuration register
 */
#define SCT_CONFIG_16BIT_COUNTER        0x00000000				/*!< Operate as 2 16-bit counters */
#define SCT_CONFIG_32BIT_COUNTER        0x00000001				/*!< Operate as 1 32-bit counter */

#define SCT_CONFIG_CLKMODE_BUSCLK       (0x0 << 1)				/*!< Bus clock */
#define SCT_CONFIG_CLKMODE_SCTCLK       (0x1 << 1)				/*!< SCT clock */
#define SCT_CONFIG_CLKMODE_INCLK        (0x2 << 1)				/*!< Input clock selected in CLKSEL field */
#define SCT_CONFIG_CLKMODE_INEDGECLK    (0x3 << 1)				/*!< Input clock edge selected in CLKSEL field */

#define SCT_CONFIG_NORELOAD_U           (0x1 << 7)				/*!< Prevent match register reload */
#define SCT_CONFIG_NORELOAD_L           (0x1 << 7)				/*!< Prevent lower match register reload */
#define SCT_CONFIG_NORELOAD_H           (0x1 << 8)				/*!< Prevent upper match register reload */

#define SCT_CONFIG_AUTOLIMIT_U          (0x1 << 17)				/*!< Limits counter(unified) based on MATCH0 */
#define SCT_CONFIG_AUTOLIMIT_L          (0x1 << 17)				/*!< Limits counter(L) based on MATCH0 */
#define SCT_CONFIG_AUTOLIMIT_H          (0x1 << 18)				/*!< Limits counter(L) based on MATCH0 */

/*
 * @brief Macro defines for SCT control register
 */
#define COUNTUP_TO_LIMIT_THEN_CLEAR_TO_ZERO     0				/*!< Direction for low or unified counter */
#define COUNTUP_TO LIMIT_THEN_COUNTDOWN_TO_ZERO 1

#define SCT_CTRL_STOP_L                 (1 << 1)				/*!< Stop low counter */
#define SCT_CTRL_HALT_L                 (1 << 2)				/*!< Halt low counter */
#define SCT_CTRL_CLRCTR_L               (1 << 3)				/*!< Clear low or unified counter */
#define SCT_CTRL_BIDIR_L(x)             (((x) & 0x01) << 4)		/*!< Bidirectional bit */
#define SCT_CTRL_PRE_L(x)               (((x) & 0xFF) << 5)		/*!< Prescale clock for low or unified counter */

#define COUNTUP_TO_LIMIT_THEN_CLEAR_TO_ZERO     0				/*!< Direction for high counter */
#define COUNTUP_TO LIMIT_THEN_COUNTDOWN_TO_ZERO 1
#define SCT_CTRL_STOP_H                 (1 << 17)				/*!< Stop high counter */
#define SCT_CTRL_HALT_H                 (1 << 18)				/*!< Halt high counter */
#define SCT_CTRL_CLRCTR_H               (1 << 19)				/*!< Clear high counter */
#define SCT_CTRL_BIDIR_H(x)             (((x) & 0x01) << 20)
#define SCT_CTRL_PRE_H(x)               (((x) & 0xFF) << 21)	/*!< Prescale clock for high counter */

/*
 * @brief Macro defines for SCT Conflict resolution register
 */
#define SCT_RES_NOCHANGE                (0)
#define SCT_RES_SET_OUTPUT              (1)
#define SCT_RES_CLEAR_OUTPUT            (2)
#define SCT_RES_TOGGLE_OUTPUT           (3)

/**
 * SCT Match register values enum
 */
typedef enum SCT_MATCH_REG {
	SCT_MATCH_0 = 0,	/*!< SCT Match register 0 */
	SCT_MATCH_1 = 1,	/*!< SCT Match register 1 */
	SCT_MATCH_2 = 2,	/*!< SCT Match register 2 */
	SCT_MATCH_3 = 3,	/*!< SCT Match register 3 */
	SCT_MATCH_4 = 4,		/*!< SCT Match register 4 */
	SCT_MATCH_5 = 5,	/*!< SCT Match register 5 */
	SCT_MATCH_6 = 6,	/*!< SCT Match register 6 */
	SCT_MATCH_7 = 7,	/*!< SCT Match register 7 */
} SCT_MATCH_REG_T;

/**
 * SCT Event values enum
 */
typedef enum SCT_EVENT {
	SCT_EVT_0  = (1 << 0),	/*!< Event 0 */
	SCT_EVT_1  = (1 << 1),	/*!< Event 1 */
	SCT_EVT_2  = (1 << 2),	/*!< Event 2 */
	SCT_EVT_3  = (1 << 3),	/*!< Event 3 */
	SCT_EVT_4  = (1 << 4),	/*!< Event 4 */
	SCT_EVT_5  = (1 << 5),	/*!< Event 5 */
	SCT_EVT_6  = (1 << 6),	/*!< Event 6 */
	SCT_EVT_7  = (1 << 7),	/*!< Event 7 */
} SCT_EVENT_T;

/**
 * @brief	Configures the State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	value	: The 32-bit CONFIG register value
 * @return	Nothing
 */
void SCT_Config(LPC_SCT_Type *pSCT, uint32_t value);

/**
 * @brief	Set or Clear the Control register
 * @param	pSCT			: Pointer to SCT register block
 * @param	value			: SCT Control register value
 * @param	ena             : ENABLE - To set the fields specified by value
 *                          : DISABLE - To clear the field specified by value
 * @return	Nothing
 * Set or clear the control register bits as specified by the \a value
 * parameter. If \a ena is set to ENABLE, the mentioned register fields
 * will be set. If \a ena is set to DISABLE, the mentioned register
 * fields will be cleared
 */
void SCT_SetClrControl(LPC_SCT_Type *pSCT, uint32_t value, FunctionalState ena);

/**
 * @brief	Set the conflict resolution
 * @param	pSCT			: Pointer to SCT register block
 * @param	outnum			: Output number
 * @param	value           : Output value
 *                          - SCT_RES_NOCHANGE		:No change
 *					        - SCT_RES_SET_OUTPUT	:Set output
 *					        - SCT_RES_CLEAR_OUTPUT	:Clear output
 *					        - SCT_RES_TOGGLE_OUTPUT :Toggle output
 *                          : SCT_RES_NOCHANGE
 *                          : DISABLE - To clear the field specified by value
 * @return	Nothing
 * Set conflict resolution for the output \a outnum
 */
void SCT_SetConflictResolution(LPC_SCT_Type *pSCT, uint8_t outnum, uint8_t value);

/**
 * @brief	Set unified count value in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	count	: The 32-bit count value
 * @return	Nothing
 */
void SCT_SetCount(LPC_SCT_Type *pSCT, uint32_t count);

/**
 * @brief	Set unified match count value in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	n		: Match register value
 * @param	value	: The 32-bit match count value
 * @return	Nothing
 */
void SCT_SetMatchCount(LPC_SCT_Type *pSCT, SCT_MATCH_REG_T n, uint32_t value);

/**
 * @brief	Set unified match reload count value in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	n		: Match register value
 * @param	value	: The 32-bit match count reload value
 * @return	Nothing
 */
void SCT_SetMatchReload(LPC_SCT_Type *pSCT, SCT_MATCH_REG_T n, uint32_t value);

/**
 * @brief	Enable the interrupt for the specified event in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	evt		: Event value
 * @return	Nothing
 */
void SCT_EnableEventInt(LPC_SCT_Type *pSCT, SCT_EVENT_T evt);

/**
 * @brief	Disable the interrupt for the specified event in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	evt		: Event value
 * @return	Nothing
 */
void SCT_DisableEventInt(LPC_SCT_Type *pSCT, SCT_EVENT_T evt);

/**
 * @brief	Clear the specified event flag in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	evt		: Event value
 * @return	Nothing
 */
void SCT_ClearEventFlag(LPC_SCT_Type *pSCT, SCT_EVENT_T evt);

/**
 * @brief	Set control register in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	value	: Value (ORed value of SCT_CTRL_* bits)
 * @return	Nothing
 */
void SCT_SetControl(LPC_SCT_Type *pSCT, uint32_t value);

/**
 * @brief	Clear control register in State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @param	value	: Value (ORed value of SCT_CTRL_* bits)
 * @return	Nothing
 */
void SCT_ClearControl(LPC_SCT_Type *pSCT, uint32_t value);

/**
 * @brief	Initializes the State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @return	Nothing
 */
void SCT_Init(LPC_SCT_Type *pSCT);

/**
 * @brief	Deinitializes the State Configurable Timer
 * @param	pSCT	: The base of SCT peripheral on the chip
 * @return	Nothing
 */
void SCT_DeInit(LPC_SCT_Type *pSCT);

#endif // end __SCT_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
