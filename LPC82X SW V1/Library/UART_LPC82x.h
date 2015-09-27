/****************************************************************************
	Luka Penger 2015
	UART LPC82x
****************************************************************************/
#ifndef __UART_LPC82x_H 
#define __UART_LPC82x_H

#include <LPC82x.h>
#include <stdbool.h>
#include <CLOCK_LPC82x.h>

#define UART_CFG_ENABLE         (0x01 << 0)
#define UART_CFG_DATALEN_7      (0x00 << 2)		/*!< UART 7 bit length mode */
#define UART_CFG_DATALEN_8      (0x01 << 2)		/*!< UART 8 bit length mode */
#define UART_CFG_DATALEN_9      (0x02 << 2)		/*!< UART 9 bit length mode */
#define UART_CFG_PARITY_NONE    (0x00 << 4)		/*!< No parity */
#define UART_CFG_PARITY_EVEN    (0x02 << 4)		/*!< Even parity */
#define UART_CFG_PARITY_ODD     (0x03 << 4)		/*!< Odd parity */
#define UART_CFG_STOPLEN_1      (0x00 << 6)		/*!< UART One Stop Bit Select */
#define UART_CFG_STOPLEN_2      (0x01 << 6)		/*!< UART Two Stop Bits Select */
#define UART_CFG_CTSEN          (0x01 << 9)		/*!< CTS enable bit */
#define UART_CFG_SYNCEN         (0x01 << 11)	/*!< Synchronous mode enable bit */
#define UART_CFG_CLKPOL         (0x01 << 12)	/*!< Un_RXD rising edge sample enable bit */
#define UART_CFG_SYNCMST        (0x01 << 14)	/*!< Select master mode (synchronous mode) enable bit */
#define UART_CFG_LOOP           (0x01 << 15)	/*!< Loopback mode enable bit */

#define UART_CFG_OETA           (0x01 << 18)    /*!< Output Enable Turnaround time for RS485 */
#define UART_CFG_AUTOADDR       (0x01 << 19)    /*!< Automatic address matching enable */
#define UART_CFG_OESEL          (0x01 << 20)    /*!< Output enable select */
#define UART_CFG_OEPOL          (0x01 << 21)    /*!< Output enable polarity */
#define UART_CFG_RXPOL          (0x01 << 22)    /*!< Receive data polarity */
#define UART_CFG_TXPOL          (0x01 << 22)    /*!< Transmit data polarity */
#define UART_CFG_RESERVED       ((1<<1)|(1<<7)|(1<<8)|(1<<10)|(1<<13)|(3 << 16)|(0xffu<<24))

#define UART_CTRL_TXBRKEN       (0x01 << 1)		/*!< Continuous break enable bit */
#define UART_CTRL_ADDRDET       (0x01 << 2)		/*!< Address detect mode enable bit */
#define UART_CTRL_TXDIS         (0x01 << 6)		/*!< Transmit disable bit */
#define UART_CTRL_CC            (0x01 << 8)		/*!< Continuous Clock mode enable bit */
#define UART_CTRL_CLRCC         (0x01 << 9)		/*!< Clear Continuous Clock bit */
#define UART_CTRL_AUTOBAUD      (1 << 16)       /*!< Enable UART Autobaud */
#define UART_CTRL_RESERVED      (0xFFFEFCB9U)

#define UART_STAT_RXRDY         (0x01 << 0)			/*!< Receiver ready */
#define UART_STAT_RXIDLE        (0x01 << 1)			/*!< Receiver idle */
#define UART_STAT_TXRDY         (0x01 << 2)			/*!< Transmitter ready for data */
#define UART_STAT_TXIDLE        (0x01 << 3)			/*!< Transmitter idle */
#define UART_STAT_CTS           (0x01 << 4)			/*!< Status of CTS signal */
#define UART_STAT_DELTACTS      (0x01 << 5)			/*!< Change in CTS state */
#define UART_STAT_TXDISINT      (0x01 << 6)			/*!< Transmitter disabled */
#define UART_STAT_OVERRUNINT    (0x01 << 8)			/*!< Overrun Error interrupt flag. */
#define UART_STAT_RXBRK         (0x01 << 10)		/*!< Received break */
#define UART_STAT_DELTARXBRK    (0x01 << 11)		/*!< Change in receive break detection */
#define UART_STAT_START         (0x01 << 12)		/*!< Start detected */
#define UART_STAT_FRM_ERRINT    (0x01 << 13)		/*!< Framing Error interrupt flag */
#define UART_STAT_PAR_ERRINT    (0x01 << 14)		/*!< Parity Error interrupt flag */
#define UART_STAT_RXNOISEINT    (0x01 << 15)		/*!< Received Noise interrupt flag */
#define UART_STAT_ABERR         (0x01 << 16)        /*!< Auto baud error */
#define UART_STAT_RESERVED      ((1<<7)|(1<<9)|(0xFFFEU<<16))

#define UART_INTEN_RXRDY        (0x01 << 0)			/*!< Receive Ready interrupt */
#define UART_INTEN_TXRDY        (0x01 << 2)			/*!< Transmit Ready interrupt */
#define UART_INTEN_DELTACTS     (0x01 << 5)			/*!< Change in CTS state interrupt */
#define UART_INTEN_TXDIS        (0x01 << 6)			/*!< Transmitter disable interrupt */
#define UART_INTEN_OVERRUN      (0x01 << 8)			/*!< Overrun error interrupt */
#define UART_INTEN_DELTARXBRK   (0x01 << 11)		/*!< Change in receiver break detection interrupt */
#define UART_INTEN_START        (0x01 << 12)		/*!< Start detect interrupt */
#define UART_INTEN_FRAMERR      (0x01 << 13)		/*!< Frame error interrupt */
#define UART_INTEN_PARITYERR    (0x01 << 14)		/*!< Parity error interrupt */
#define UART_INTEN_RXNOISE      (0x01 << 15)		/*!< Received noise interrupt */
#define UART_INTEN_TXIDLE       (0x01 << 3)         /*!< TX Idle enable/clear */
#define UART_INTEN_ABERR        (0x01 << 16)        /*!< Auto baud error */
#define UART_INTEN_RESERVED     ((1<<1)|(1<<4)|(1<<7)|(3<<9)|(0xfffeu<<16))
#define UART_INTSTAT_RESERVED   ((1<<1)|(1<<4)|(1<<7)|(3<<9)|(0xfffeu<<16))

void UART_Enable(LPC_USART0_Type *pUART);

void UART_Disable(LPC_USART0_Type *pUART);

void UART_TXEnable(LPC_USART0_Type *pUART);

void UART_TXDisable(LPC_USART0_Type *pUART);

void UART_SendByte(LPC_USART0_Type *pUART, uint8_t data);

uint32_t UART_ReadByte(LPC_USART0_Type *pUART);

void UART_IntEnable(LPC_USART0_Type *pUART, uint32_t intMask);

void UART_IntDisable(LPC_USART0_Type *pUART, uint32_t intMask);

uint32_t UART_GetIntsEnabled(LPC_USART0_Type *pUART);

uint32_t UART_GetIntStatus(LPC_USART0_Type *pUART);

void UART_ConfigData(LPC_USART0_Type *pUART, uint32_t config);

uint32_t UART_GetStatus(LPC_USART0_Type *pUART);

void UART_ClearStatus(LPC_USART0_Type *pUART, uint32_t stsMask);

void UART_SetOSR(LPC_USART0_Type *pUART, uint32_t ovrVal);

void UART_SetAddr(LPC_USART0_Type *pUART, uint32_t addr);

SYSCTL_CLOCK_T getUARTClockID(LPC_USART0_Type *pUART);

void UART_Init(LPC_USART0_Type *pUART);

void UART_DeInit(LPC_USART0_Type *pUART);

void UART_SetBaud(LPC_USART0_Type *pUART, uint32_t baudrate);

void UART_Send(LPC_USART0_Type *pUART, char *data, int numBytes);

#endif // end __UART_LPC82x_H
/****************************************************************************
                            End Of File
****************************************************************************/
