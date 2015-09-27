/****************************************************************************
	Luka Penger 2015
	ADC LPC82x
****************************************************************************/
#ifndef __ADC_LPC82x_H 
#define __ADC_LPC82x_H

#include <LPC82x.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <lpc_types.h>
#include <SYSCON_LPC82x.h>
#include <CLOCK_LPC82x.h>

/** Sequence index enumerations, used in various parts of the code for
    register indexing and sequencer selection */
typedef enum {
	ADC_SEQA_IDX,
	ADC_SEQB_IDX
} ADC_SEQ_IDX_T;

/** Maximum sample rate in Hz (12-bit conversions) */
#define ADC_MAX_SAMPLE_RATE 30000000

/**
 * @brief ADC register support bitfields and mask
 */
/** ADC Control register bit fields */
#define ADC_CR_CLKDIV_MASK      (0xFF << 0)				/*!< Mask for Clock divider value */
#define ADC_CR_CLKDIV_BITPOS    (0)						/*!< Bit position for Clock divider value */
#define ADC_CR_ASYNMODE         (1 << 8)				/*!< Asynchronous mode enable bit */
#define ADC_CR_MODE10BIT        (1 << 9)				/*!< 10-bit mode enable bit */
#define ADC_CR_LPWRMODEBIT      (1 << 10)				/*!< Low power mode enable bit */
#define ADC_CR_CALMODEBIT       (1 << 30)				/*!< Self calibration cycle enable bit */
#define ADC_CR_BITACC(n)        ((((n) & 0x1) << 9))	/*!< 12-bit or 10-bit ADC accuracy */
#define ADC_CR_CLKDIV(n)        ((((n) & 0xFF) << 0))	/*!< The APB clock (PCLK) is divided by (this value plus one) to produce the clock for the A/D */
#define ADC_SAMPLE_RATE_CONFIG_MASK (ADC_CR_CLKDIV(0xFF) | ADC_CR_BITACC(0x01))

/** ADC Sequence Control register bit fields */
#define ADC_SEQ_CTRL_CHANSEL(n)   (1 << (n))			/*!< Channel select macro */
#define ADC_SEQ_CTRL_CHANSEL_MASK (0xFFF)				/*!< Channel select mask */

/** ADC hardware trigger sources in SEQ_CTRL */
#define ADC_SEQ_CTRL_HWTRIG_ARM_TXEV     (0 << 12)		/*!< HW trigger input - ARM TXEV */
#define ADC_SEQ_CTRL_HWTRIG_CT32B0_MAT0  (1 << 12)		/*!< HW trigger input - Match output 0 of CT32B0 */
#define ADC_SEQ_CTRL_HWTRIG_CT32B0_MAT1  (2 << 12)		/*!< HW trigger input - Match output 1 of CT32B1 or SCT_OUT0 */
#define ADC_SEQ_CTRL_HWTRIG_SCT_OUT0     (2 << 12)		/*!< HW trigger input - Match output 1 of CT32B1 or SCT_OUT0 */
#define ADC_SEQ_CTRL_HWTRIG_CT16B0_MAT0  (3 << 12)		/*!< HW trigger input - Match output 0 of CT16B0 */
#define ADC_SEQ_CTRL_HWTRIG_CT16B0_MAT1  (4 << 12)		/*!< HW trigger input - Match output 1 of CT16B1 or SCT_OUT1 */
#define ADC_SEQ_CTRL_HWTRIG_SCT_OUT1     (4 << 12)		/*!< HW trigger input - Match output 1 of CT16B1 or SCT_OUT1 */
#define ADC_SEQ_CTRL_HWTRIG_CT16B0_CAP0  (5 << 12)		/*!< HW trigger input - Capture input 0 of CT16B0 */
#define ADC_SEQ_CTRL_HWTRIG_CT16B1_CAP0  (6 << 12)		/*!< HW trigger input - Capture input 0 of CT16B1 */
#define ADC_SEQ_CTRL_HWTRIG_CT32B0_CAP0  (7 << 12)		/*!< HW trigger input - Capture input 0 of CT32B1 */
#define ADC_SEQ_CTRL_HWTRIG_MASK         (0x3F << 12)	/*!< HW trigger input bitfield mask */

/** SEQ_CTRL register bit fields */
#define ADC_SEQ_CTRL_HWTRIG_POLPOS       (1 << 18)		/*!< HW trigger polarity - positive edge */
#define ADC_SEQ_CTRL_HWTRIG_SYNCBYPASS   (1 << 19)		/*!< HW trigger bypass synchronisation */
#define ADC_SEQ_CTRL_START               (1 << 26)		/*!< Start conversion enable bit */
#define ADC_SEQ_CTRL_BURST               (1 << 27)		/*!< Repeated conversion enable bit */
#define ADC_SEQ_CTRL_SINGLESTEP          (1 << 28)		/*!< Single step enable bit */
#define ADC_SEQ_CTRL_LOWPRIO             (1 << 29)		/*!< High priority enable bit (regardless of name) */
#define ADC_SEQ_CTRL_MODE_EOS            (1 << 30)		/*!< Mode End of sequence enable bit */
#define ADC_SEQ_CTRL_SEQ_ENA             (1UL << 31)	/*!< Sequence enable bit */

/** ADC global data register bit fields */
#define ADC_SEQ_GDAT_RESULT_MASK         (0xFFF << 4)	/*!< Result value mask */
#define ADC_SEQ_GDAT_RESULT_BITPOS       (4)			/*!< Result start bit position */
#define ADC_SEQ_GDAT_THCMPRANGE_MASK     (0x3 << 16)	/*!< Comparion range mask */
#define ADC_SEQ_GDAT_THCMPRANGE_BITPOS   (16)			/*!< Comparison range bit position */
#define ADC_SEQ_GDAT_THCMPCROSS_MASK     (0x3 << 18)	/*!< Comparion cross mask */
#define ADC_SEQ_GDAT_THCMPCROSS_BITPOS   (18)			/*!< Comparison cross bit position */
#define ADC_SEQ_GDAT_CHAN_MASK           (0xF << 26)	/*!< Channel number mask */
#define ADC_SEQ_GDAT_CHAN_BITPOS         (26)			/*!< Channel number bit position */
#define ADC_SEQ_GDAT_OVERRUN             (1 << 30)		/*!< Overrun bit */
#define ADC_SEQ_GDAT_DATAVALID           (1UL << 31)	/*!< Data valid bit */

/** ADC Data register bit fields */
#define ADC_DR_RESULT(n)           ((((n) >> 4) & 0xFFF))	/*!< Macro for getting the ADC data value */
#define ADC_DR_THCMPRANGE_MASK     (0x3 << 16)			/*!< Comparion range mask */
#define ADC_DR_THCMPRANGE_BITPOS   (16)					/*!< Comparison range bit position */
#define ADC_DR_THCMPRANGE(n)       (((n) >> ADC_DR_THCMPRANGE_BITPOS) & 0x3)
#define ADC_DR_THCMPCROSS_MASK     (0x3 << 18)			/*!< Comparion cross mask */
#define ADC_DR_THCMPCROSS_BITPOS   (18)					/*!< Comparison cross bit position */
#define ADC_DR_THCMPCROSS(n)       (((n) >> ADC_DR_THCMPCROSS_BITPOS) & 0x3)
#define ADC_DR_CHAN_MASK           (0xF << 26)			/*!< Channel number mask */
#define ADC_DR_CHAN_BITPOS         (26)					/*!< Channel number bit position */
#define ADC_DR_CHANNEL(n)          (((n) >> ADC_DR_CHAN_BITPOS) & 0xF)	/*!< Channel number bit position */
#define ADC_DR_OVERRUN             (1 << 30)			/*!< Overrun bit */
#define ADC_DR_DATAVALID           (1UL << 31)			/*!< Data valid bit */
#define ADC_DR_DONE(n)             (((n) >> 31))

/** ADC low/high Threshold register bit fields */
#define ADC_THR_VAL_MASK            (0xFFF << 4)		/*!< Threshold value bit mask */
#define ADC_THR_VAL_POS             (4)					/*!< Threshold value bit position */

/** ADC Threshold select register bit fields */
#define ADC_THRSEL_CHAN_SEL_THR1(n) (1 << (n))			/*!< Select THR1 register for channel n */

/** ADC Interrupt Enable register bit fields */
#define ADC_INTEN_SEQA_ENABLE       (1 << 0)			/*!< Sequence A Interrupt enable bit */
#define ADC_INTEN_SEQB_ENABLE       (1 << 1)			/*!< Sequence B Interrupt enable bit */
#define ADC_INTEN_SEQN_ENABLE(seq)  (1 << (seq))		/*!< Sequence A/B Interrupt enable bit */
#define ADC_INTEN_OVRRUN_ENABLE     (1 << 2)			/*!< Overrun Interrupt enable bit */
#define ADC_INTEN_CMP_DISBALE       (0)					/*!< Disable comparison interrupt value */
#define ADC_INTEN_CMP_OUTSIDETH     (1)					/*!< Outside threshold interrupt value */
#define ADC_INTEN_CMP_CROSSTH       (2)					/*!< Crossing threshold interrupt value */
#define ADC_INTEN_CMP_MASK          (3)					/*!< Comparison interrupt value mask */
#define ADC_INTEN_CMP_ENABLE(isel, ch) (((isel) & ADC_INTEN_CMP_MASK) << ((2 * (ch)) + 3))	/*!< Interrupt selection for channel */

/** ADC Flags register bit fields */
#define ADC_FLAGS_THCMP_MASK(ch)    (1 << (ch))		/*!< Threshold comparison status for channel */
#define ADC_FLAGS_OVRRUN_MASK(ch)   (1 << (12 + (ch)))	/*!< Overrun status for channel */
#define ADC_FLAGS_SEQA_OVRRUN_MASK  (1 << 24)			/*!< Seq A Overrun status */
#define ADC_FLAGS_SEQB_OVRRUN_MASK  (1 << 25)			/*!< Seq B Overrun status */
#define ADC_FLAGS_SEQN_OVRRUN_MASK(seq) (1 << (24 + (seq)))	/*!< Seq A/B Overrun status */
#define ADC_FLAGS_SEQA_INT_MASK     (1 << 28)			/*!< Seq A Interrupt status */
#define ADC_FLAGS_SEQB_INT_MASK     (1 << 29)			/*!< Seq B Interrupt status */
#define ADC_FLAGS_SEQN_INT_MASK(seq) (1 << (28 + (seq)))/*!< Seq A/B Interrupt status */
#define ADC_FLAGS_THCMP_INT_MASK    (1 << 30)			/*!< Threshold comparison Interrupt status */
#define ADC_FLAGS_OVRRUN_INT_MASK   (1UL << 31)			/*!< Overrun Interrupt status */

/** ADC Trim register bit fields */
#define ADC_TRIM_VRANGE_HIGHV       (0 << 5)			/*!< Voltage range bit - High volatge (2.7V to 3.6V) */
#define ADC_TRIM_VRANGE_LOWV        (1 << 5)			/*!< Voltage range bit - Low volatge (1.8V to 2.7V) */

/** ADC Register reserved bit masks */
#define ADC_CHAN_THRSEL_RES 0xFFFFF000
#define ADC_INTEN_RES       0xF8000000
#define ADC_SEQ_CTRL_RES    ((7 << 15) | (0x3F << 20))

/**
 * @brief	Initialize the ADC peripheral
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	flags	: ADC flags for init (ADC_CR_MODE10BIT and/or ADC_CR_LPWRMODEBIT)
 * @return	Nothing
 * @note	To select low-power ADC mode, enable the ADC_CR_LPWRMODEBIT flag.
 * To select 10-bit conversion mode, enable the ADC_CR_MODE10BIT flag.<br>
 * Example: ADC_Init(LPC_ADC, (ADC_CR_MODE10BIT | ADC_CR_LPWRMODEBIT));
 */
void ADC_Init(LPC_ADC_Type *pADC, uint32_t flags);

/**
 * @brief	Shutdown ADC
 * @param	pADC	: The base of ADC peripheral on the chip
 * @return	Nothing
 * @note	Disables the ADC clocks and ADC power
 */
void ADC_DeInit(LPC_ADC_Type *pADC);

/**
 * @brief	Set ADC divider
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	div		: ADC divider value to set minus 1
 * @return	Nothing
 * @note	The value is used as a divider to generate the ADC
 * clock rate from the ADC input clock. The ADC input clock is based
 * on the system clock. Valid values for this function are from 0 to 255
 * with 0=divide by 1, 1=divide by 2, 2=divide by 3, etc.<br>
 * Do not decrement this value by 1.<br>
 * To set the ADC clock rate to 1MHz, use the following function:<br>
 * ADC_SetDivider(LPC_ADC, (Clock_GetSystemClockRate() / 1000000) - 1);
 */
void ADC_SetDivider(LPC_ADC_Type *pADC, uint8_t div);

/**
 * @brief	Set ADC clock rate
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	rate	: rate in Hz to set ADC clock to (maximum ADC_MAX_SAMPLE_RATE)
 * @return	Nothing
 * @note	When ADC is set to ADC_CR_ASYNMODE this function has no effect. The
 * rate mentioned in @a rate is the sampling clock rate and not the frequency at
 * at which the conversion will be done. Example setting @a rate to 30 MHz will
 * get a sampling rate of 1.2M samples per second.
 */
void ADC_SetClockRate(LPC_ADC_Type *pADC, uint32_t rate);

/**
 * @brief	Get ADC divider
 * @param	pADC	: The base of ADC peripheral on the chip
 * @return	the current ADC divider
 * @note	This function returns the divider that is used to generate the
 * ADC frequency. The returned value must be incremented by 1. The
 * frequency can be determined with the following function:<br>
 * adc_freq = Clock_GetSystemClockRate() / (ADC_GetDivider(LPC_ADC) + 1);
 */
uint8_t ADC_GetDivider(LPC_ADC_Type *pADC);

/**
 * @brief	Start ADC calibration
 * @param	pADC	: The base of ADC peripheral on the chip
 * @return	Nothing
 * @note	Calibration is not done as part of ADC_Init(), but
 * is required after the call to ADC_Init() or after returning
 * from a power-down state. Calibration may alter the ADC_CR_ASYNMODE
 * and ADC_CR_LPWRMODEBIT flags ni the CTRL register.
 */
void ADC_StartCalibration(LPC_ADC_Type *pADC);

/**
 * @brief	Start ADC calibration
 * @param	pADC	: The base of ADC peripheral on the chip
 * @return	TRUE if calibration is complete, otherwise FALSE.
 */
bool ADC_IsCalibrationDone(LPC_ADC_Type *pADC);

/**
 * @brief	Helper function for safely setting ADC sequencer register bits
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to set bits for
 * @param	bits		: Or'ed bits of a sequencer register to set
 * @return	Nothing
 * @note	This function will safely set the ADC sequencer register bits
 * while maintaining bits 20..25 as 0, regardless of the read state of those bits.
 */
void ADC_SetSequencerBits(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex, uint32_t bits);

/**
 * @brief	Helper function for safely clearing ADC sequencer register bits
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to clear bits for
 * @param	bits		: Or'ed bits of a sequencer register to clear
 * @return	Nothing
 * @note	This function will safely clear the ADC sequencer register bits
 * while maintaining bits 20..25 as 0, regardless of the read state of those bits.
 */
void ADC_ClearSequencerBits(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex, uint32_t bits);

/**
 * @brief	Sets up ADC conversion sequencer A or B
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to setup
 * @param	options		: OR'ed Sequencer options to setup (see notes)
 * @return	Nothing
 * @note	Sets up sequencer options for a conversion sequence. This function
 * should be used to setup the selected channels for the sequence, the sequencer
 * trigger, the trigger polarity, synchronization bypass, priority, and mode. All
 * options are passed to the functions as a OR'ed list of values. This function will
 * disable/clear the sequencer start/burst/single step/enable if they are enabled.<br>
 * Select the channels by OR'ing in one or more ADC_SEQ_CTRL_CHANSEL(ch) values.<br>
 * Select the hardware trigger by OR'ing in one ADC_SEQ_CTRL_HWTRIG_* value.<br>
 * Select a positive edge hardware trigger by OR'ing in ADC_SEQ_CTRL_HWTRIG_POLPOS.<br>
 * Select trigger bypass synchronisation by OR'ing in ADC_SEQ_CTRL_HWTRIG_SYNCBYPASS.<br>
 * Select ADC single step on trigger/start by OR'ing in ADC_SEQ_CTRL_SINGLESTEP.<br>
 * Select higher priority conversion on the other sequencer by OR'ing in ADC_SEQ_CTRL_LOWPRIO.<br>
 * Select end of seqeuence instead of end of conversion interrupt by OR'ing in ADC_SEQ_CTRL_MODE_EOS.<br>
 * Example for setting up sequencer A (channels 0-2, trigger on high edge of PIO0_2, interrupt on end of sequence):<br>
 * ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX, (
 *     ADC_SEQ_CTRL_CHANSEL(0) | ADC_SEQ_CTRL_CHANSEL(1) | ADC_SEQ_CTRL_CHANSEL(2) |
 *     ADC_SEQ_CTRL_HWTRIG_PIO0_2 | ADC_SEQ_CTRL_HWTRIG_POLPOS | ADC_SEQ_CTRL_MODE_EOS));
 */
void ADC_SetupSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex, uint32_t options);

/**
 * @brief	Enables a sequencer
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to enable
 * @return	Nothing
 */
void ADC_EnableSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex);

/**
 * @brief	Disables a sequencer
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to disable
 * @return	Nothing
 */
void ADC_DisableSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex);

/**
 * @brief	Forces a sequencer trigger event (software trigger of ADC)
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to start
 * @return	Nothing
 * @note	This function sets the START bit for the sequencer to force a
 * single conversion sequence or a single step conversion.
 */
void ADC_StartSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex);

/**
 * @brief	Starts sequencer burst mode
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to start burst on
 * @return	Nothing
 * @note	This function sets the BURST bit for the sequencer to force
 * continuous conversion. Use ADC_StopBurstSequencer() to stop the
 * ADC burst sequence.
 */
void ADC_StartBurstSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex);

/**
 * @brief	Stops sequencer burst mode
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to stop burst on
 * @return	Nothing
 */
void ADC_StopBurstSequencer(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex);

/** ADC sequence global data register threshold comparison range enumerations */
typedef enum {
	ADC_DR_THCMPRANGE_INRANGE,
	ADC_DR_THCMPRANGE_RESERVED,
	ADC_DR_THCMPRANGE_BELOW,
	ADC_DR_THCMPRANGE_ABOVE
} ADC_DR_THCMPRANGE_T;

/** ADC sequence global data register threshold comparison cross enumerations */
typedef enum {
	ADC_DR_THCMPCROSS_NOCROSS,
	ADC_DR_THCMPCROSS_RESERVED,
	ADC_DR_THCMPCROSS_DOWNWARD,
	ADC_DR_THCMPCROSS_UPWARD
} ADC_DR_THCMPCROSS_T;

/**
 * @brief	Read a ADC sequence global data register
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param	seqIndex	: Sequencer to read
 * @return	Current raw value of the ADC sequence A or B global data register
 * @note	This function returns the raw value of the data register and clears
 * the overrun and datavalid status for the register. Once this register is read,
 * the following functions can be used to parse the raw value:<br>
 * uint32_t adcDataRawValue = ADC_ReadSequencerDataReg(LPC_ADC, ADC_SEQA_IDX); // Get raw value
 * uint32_t adcDataValue = ADC_DR_RESULT(adcDataRawValue); // Aligned and masked ADC data value
 * ADC_DR_THCMPRANGE_T adcRange = (ADC_DR_THCMPRANGE_T) ADC_DR_THCMPRANGE(adcDataRawValue); // Sample range compared to threshold low/high
 * ADC_DR_THCMPCROSS_T adcRange = (ADC_DR_THCMPCROSS_T) ADC_DR_THCMPCROSS(adcDataRawValue); // Sample cross compared to threshold low
 * uint32_t channel = ADC_DR_CHANNEL(adcDataRawValue); // ADC channel for this sample/data
 * bool adcDataOverrun = (bool) ((adcDataRawValue & ADC_DR_OVERRUN) != 0); // Data overrun flag
 * bool adcDataValid = (bool) ((adcDataRawValue & ADC_SEQ_GDAT_DATAVALID) != 0); // Data valid flag
 */
uint32_t ADC_GetSequencerDataReg(LPC_ADC_Type *pADC, ADC_SEQ_IDX_T seqIndex);

/**
 * @brief	Read a ADC data register
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	index	: Data register to read, 1-8
 * @return	Current raw value of the ADC data register
 * @note	This function returns the raw value of the data register and clears
 * the overrun and datavalid status for the register. Once this register is read,
 * the following functions can be used to parse the raw value:<br>
 * uint32_t adcDataRawValue = ADC_ReadSequencerDataReg(LPC_ADC, ADC_SEQA_IDX); // Get raw value
 * uint32_t adcDataValue = ADC_DR_RESULT(adcDataRawValue); // Aligned and masked ADC data value
 * ADC_DR_THCMPRANGE_T adcRange = (ADC_DR_THCMPRANGE_T) ADC_DR_THCMPRANGE(adcDataRawValue); // Sample range compared to threshold low/high
 * ADC_DR_THCMPCROSS_T adcRange = (ADC_DR_THCMPCROSS_T) ADC_DR_THCMPCROSS(adcDataRawValue); // Sample cross compared to threshold low
 * uint32_t channel = ADC_DR_CHANNEL(adcDataRawValue); // ADC channel for this sample/data
 * bool adcDataOverrun = (bool) ((adcDataRawValue & ADC_DR_OVERRUN) != 0); // Data overrun flag
 * bool adcDataValid = (bool) ((adcDataRawValue & ADC_SEQ_GDAT_DATAVALID) != 0); // Data valid flag
 */
uint32_t ADC_GetDataReg(LPC_ADC_Type *pADC, uint8_t index);

/**
 * @brief	Set Threshold low value in ADC
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param   thrnum      : Threshold register value (1 for threshold register 1, 0 for threshold register 0)
 * @param   value       : Threshold low data value (should be 12-bit value)
 * @return	None
 */
void ADC_SetThrLowValue(LPC_ADC_Type *pADC, uint8_t thrnum, uint16_t value);

/**
 * @brief	Set Threshold high value in ADC
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param   thrnum	: Threshold register value (1 for threshold register 1, 0 for threshold register 0)
 * @param   value	: Threshold high data value (should be 12-bit value)
 * @return	None
 */
void ADC_SetThrHighValue(LPC_ADC_Type *pADC, uint8_t thrnum, uint16_t value);

/**
 * @brief	Select threshold 0 values for comparison for selected channels
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param   channels	: An OR'ed value of one or more ADC_THRSEL_CHAN_SEL_THR1(ch) values
 * @return	None
 * @note	Select multiple channels to use the threshold 0 comparison.<br>
 * Example:<br>
 * ADC_SelectTH0Channels(LPC_ADC, (ADC_THRSEL_CHAN_SEL_THR1(1) | ADC_THRSEL_CHAN_SEL_THR1(2))); // Selects channels 1 and 2 for threshold 0
 */
void ADC_SelectTH0Channels(LPC_ADC_Type *pADC, uint32_t channels);

/**
 * @brief	Select threshold 1 value for comparison for selected channels
 * @param	pADC		: The base of ADC peripheral on the chip
 * @param   channels	: An OR'ed value of one or more ADC_THRSEL_CHAN_SEL_THR1(ch) values
 * @return	None
 * @note	Select multiple channels to use the 1 threshold comparison.<br>
 * Example:<br>
 * ADC_SelectTH1Channels(LPC_ADC, (ADC_THRSEL_CHAN_SEL_THR1(4) | ADC_THRSEL_CHAN_SEL_THR1(5))); // Selects channels 4 and 5 for 1 threshold
 */
void ADC_SelectTH1Channels(LPC_ADC_Type *pADC, uint32_t channels);

/**
 * @brief	Enable interrupts in ADC (sequencers A/B and overrun)
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	intMask	: Interrupt values to be enabled (see notes)
 * @return	None
 * @note	Select one or more OR'ed values of ADC_INTEN_SEQA_ENABLE,
 * ADC_INTEN_SEQB_ENABLE, and ADC_INTEN_OVRRUN_ENABLE to enable the
 * specific ADC interrupts.
 */
void ADC_EnableInt(LPC_ADC_Type *pADC, uint32_t intMask);

/**
 * @brief	Disable interrupts in ADC (sequencers A/B and overrun)
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	intMask	: Interrupt values to be disabled (see notes)
 * @return	None
 * @note	Select one or more OR'ed values of ADC_INTEN_SEQA_ENABLE,
 * ADC_INTEN_SEQB_ENABLE, and ADC_INTEN_OVRRUN_ENABLE to disable the
 * specific ADC interrupts.
 */
void ADC_DisableInt(LPC_ADC_Type *pADC, uint32_t intMask);

/** Threshold interrupt event options */
typedef enum {
	ADC_INTEN_THCMP_DISABLE,
	ADC_INTEN_THCMP_OUTSIDE,
	ADC_INTEN_THCMP_CROSSING,
} ADC_INTEN_THCMP_T;

/**
 * @brief	Enable a threshold event interrupt in ADC
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	ch		: ADC channel to set threshold inetrrupt for, 1-8
 * @param	thInt	: Selected threshold interrupt type
 * @return	None
 */
void ADC_SetThresholdInt(LPC_ADC_Type *pADC, uint8_t ch, ADC_INTEN_THCMP_T thInt);

/**
 * @brief	Get flags register in ADC
 * @param	pADC	: The base of ADC peripheral on the chip
 * @return  Flags register value (ORed ADC_FLAG* values)
 * @note	Mask the return value of this function with the ADC_FLAGS_*
 * definitions to determine the overall ADC interrupt events.<br>
 * Example:<br>
 * if (ADC_GetFlags(LPC_ADC) & ADC_FLAGS_THCMP_MASK(3) // Check of threshold comp status for ADC channel 3
 */
uint32_t ADC_GetFlags(LPC_ADC_Type *pADC);

/**
 * @brief	Clear flags register in ADC
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	flags	: An Or'ed values of ADC_FLAGS_* values to clear
 * @return  Flags register value (ORed ADC_FLAG* values)
 */
void ADC_ClearFlags(LPC_ADC_Type *pADC, uint32_t flags);

/**
 * @brief	Set Trim register in ADC
 * @param	pADC	: The base of ADC peripheral on the chip
 * @param	trim	: Trim value (ADC_TRIM_VRANGE_HIGHV or ADC_TRIM_VRANGE_LOWV)
 * @return	None
 */
void ADC_SetTrim(LPC_ADC_Type *pADC, uint32_t trim);

#endif // end __ADC_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
