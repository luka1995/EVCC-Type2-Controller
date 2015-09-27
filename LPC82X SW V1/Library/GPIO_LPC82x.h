/****************************************************************************
	Luka Penger 2015
	GPIO LPC82x
****************************************************************************/
#ifndef __GPIO_LPC82x_H 
#define __GPIO_LPC82x_H

#include <LPC82x.h>
#include <stdbool.h>
#include <SYSCON_LPC82x.h>
#include <CLOCK_LPC82x.h>

/**
 * @brief	Initialize GPIO block
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @return	Nothing
 */
void GPIO_Init(LPC_GPIO_PORT_Type *pGPIO);

/**
 * @brief	De-Initialize GPIO block
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @return	Nothing
 */
void GPIO_DeInit(LPC_GPIO_PORT_Type *pGPIO);

/**
 * @brief	Set a GPIO pin state via the GPIO byte register
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to set
 * @param	state	: true for high, false for low
 * @return	Nothing
 */
void GPIO_SetPinState(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin, bool state);

/**
 * @brief	Get a GPIO pin state via the GPIO byte register
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to get state for
 * @return	true if the GPIO is high, false if low
 */
bool GPIO_GetPinState(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin);

/**
 * @brief	Set GPIO direction for a single GPIO pin
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to set
 * @param	state	: true for output, false for input
 * @return	Nothing
 */
void GPIO_WriteDirBit(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin, bool state);

/**
 * @brief	Set GPIO direction for a single GPIO pin to an output
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to set direction on as output
 * @return	Nothing
 */
void GPIO_SetPinDIROutput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin);

/**
 * @brief	Set GPIO direction for a single GPIO pin to an input
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to set direction on as input
 * @return	Nothing
 */
void GPIO_SetPinDIRInput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin);

/**
 * @brief	Toggle GPIO direction for a single GPIO pin
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to toggle direction
 * @return	Nothing
 */
void GPIO_TogglePinDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin);

/**
 * @brief	Set GPIO direction for a single GPIO pin
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to set direction for
 * @param	output	: true for output, false for input
 * @return	Nothing
 */
void GPIO_SetPinDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin, bool output);

/**
 * @brief	Get GPIO direction for a single GPIO pin
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: GPIO pin to get direction for
 * @return	true if the GPIO is an output, false if input
 */
bool GPIO_GetPinDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin);

/**
 * @brief	Set Direction for a GPIO port (Obsolete)
 * @param	pGPIO		: The base of GPIO peripheral on the chip
 * @param	bitValue	: GPIO bit to set
 * @param	out			: Direction value, 0 = input, !0 = output
 * @return	None
 * @note	Bits set to '0' are not altered. Obsolete function use the
 * GPIO_SetPortDIR() function instead.
 */
void GPIO_SetDir(LPC_GPIO_PORT_Type *pGPIO, uint32_t bitValue, uint8_t out);

/**
 * @brief	Set GPIO direction for a all selected GPIO pins to an output
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pinMask	: GPIO pin mask to set direction on as output (bits 0..b for pins 0..n)
 * @return	Nothing
 * @note	Sets multiple GPIO pins to the output direction, each bit's position that is
 * high sets the corresponding pin number for that bit to an output.
 */
void GPIO_SetPortDIROutput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask);

/**
 * @brief	Set GPIO direction for a all selected GPIO pins to an input
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pinMask	: GPIO pin mask to set direction on as input (bits 0..b for pins 0..n)
 * @return	Nothing
 * @note	Sets multiple GPIO pins to the input direction, each bit's position that is
 * high sets the corresponding pin number for that bit to an input.
 */
void GPIO_SetPortDIRInput(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask);

/**
 * @brief	Toggle GPIO direction for a all selected GPIO pins
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pinMask	: GPIO pin mask Toggle direction (bits 0..n for pins 0..n)
 * @return	Nothing
 * @note	Toggles multiple GPIO pin's direction, each bit's position that is
 * high toggles direction of the corresponding pin number.
 */
void GPIO_TogglePortDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask);

/**
 * @brief	Set GPIO direction for a all selected GPIO pins to an input or output
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pinMask	: GPIO pin mask to set direction on (bits 0..b for pins 0..n)
 * @param	outSet	: Direction value, false = set as inputs, true = set as outputs
 * @return	Nothing
 * @note	Sets multiple GPIO pins to the input direction, each bit's position that is
 * high sets the corresponding pin number for that bit to an input.
 */
void GPIO_SetPortDIR(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask, bool outSet);

/**
 * @brief	Set GPIO direction for set of pins as output and a set of pins as input
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pinMask	: GPIO pin mask to set direction on (bits 0..b for pins 0..n)
 * @param	outMask	: GPIO IN/OUT mask to set pin n as output set bit-n (see note for more info)
 * @return	Nothing
 * @note	Sets multiple GPIO pins to the input or output, to change the direction of PIN0_1,
 * PIN_05, PIN0_10 and PIN0_18, set the @a pinMask to ((1 << 1) | (1 << 5) | (1 << 10) | (1 << 18))
 * to set PIN0_5 and PIN0_18 to output and PIN0_1 and PIN0_10 as input set @a outMask to
 * ((1 << 5) | (1 << 18)).
 */
void GPIO_SetPortDIRMask(LPC_GPIO_PORT_Type *pGPIO, uint8_t pinMask, bool outMask);

/**
 * @brief	Get GPIO direction for a all GPIO pins
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @return	a bitfield containing the input and output states for each pin
 * @note	For pins 0..n, a high state in a bit corresponds to an output state for the
 * same pin, while a low  state corresponds to an input state.
 */
uint32_t GPIO_GetPortDIR(LPC_GPIO_PORT_Type *pGPIO);

/**
 * @brief	Set GPIO port mask value for GPIO masked read and write
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	mask	: Mask value for read and write (only low bits are enabled)
 * @return	Nothing
 * @note	Controls which bits are set or unset when using the masked
 * GPIO read and write functions. A low state indicates the pin is settable
 * and readable via the masked write and read functions.
 */
void GPIO_SetPortMask(LPC_GPIO_PORT_Type *pGPIO, uint32_t mask);

/**
 * @brief	Get GPIO port mask value used for GPIO masked read and write
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @return	Returns value set with the Chip_GPIO_SetPortMask() function.
 * @note	A high bit in the return value indicates that that GPIO pin for the
 * port cannot be set using the masked write function.
 */
uint32_t GPIO_GetPortMask(LPC_GPIO_PORT_Type *pGPIO);

/**
 * @brief	Set all GPIO raw pin states (regardless of masking)
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	value	: Value to set all GPIO pin states (0..n) to
 * @return	Nothing
 */
void GPIO_SetPortValue(LPC_GPIO_PORT_Type *pGPIO, uint32_t value);

/**
 * @brief	Get all GPIO raw pin states (regardless of masking)
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @return	Current (raw) state of all GPIO pins
 */
uint32_t GPIO_GetPortValue(LPC_GPIO_PORT_Type *pGPIO);

/**
 * @brief	Set all GPIO pin states, but mask via the MASKP0 register
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	value	: Value to set all GPIO pin states (0..n) to
 * @return	Nothing
 */
void GPIO_SetMaskedPortValue(LPC_GPIO_PORT_Type *pGPIO, uint32_t value);

/**
 * @brief	Get all GPIO pin statesm but mask via the MASKP0 register
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @return	Current (masked) state of all GPIO pins
 */
uint32_t GPIO_GetMaskedPortValue(LPC_GPIO_PORT_Type *pGPIO);

/**
 * @brief	Set a GPIO port/bit to the high state
 * @param	pGPIO		: The base of GPIO peripheral on the chip
 * @param	bitValue	: bit(s) in the port to set high
 * @return	None
 * @note	Any bit set as a '0' will not have it's state changed. This only
 * applies to ports configured as an output. It is recommended to use the
 * GPIO_SetPortOutHigh() function instead.
 */
void GPIO_SetValue(LPC_GPIO_PORT_Type *pGPIO, uint32_t bitValue);

/**
 * @brief	Toggle an individual GPIO output pin to the opposite state
 * @param	pGPIO	: The base of GPIO peripheral on the chip
 * @param	pin		: pin number (0..n) to toggle
 * @return	None
 * @note	Any bit set as a '0' will not have it's state changed. This only
 * applies to ports configured as an output.
 */
void GPIO_SetPinToggle(LPC_GPIO_PORT_Type *pGPIO, uint8_t pin);

#endif // end __GPIO_LPC8xx_H
/****************************************************************************
                            End Of File
****************************************************************************/
