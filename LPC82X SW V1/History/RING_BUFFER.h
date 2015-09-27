/****************************************************************************
	Luka Penger 2015
	RING BUFFER
****************************************************************************/
#ifndef __RING_BUFFER_H 
#define __RING_BUFFER_H

#include <LPC82x.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <lpc_types.h>

/**
 * @brief Ring buffer structure
 */
typedef struct {
	void *data;
	int count;
	int itemSz;
	uint32_t head;
	uint32_t tail;
} RINGBUFF_T;

/**
 * @def		RB_VHEAD(rb)
 * volatile typecasted head index
 */
#define RB_VHEAD(rb)              (*(volatile uint32_t *) &(rb)->head)

/**
 * @def		RB_VTAIL(rb)
 * volatile typecasted tail index
 */
#define RB_VTAIL(rb)              (*(volatile uint32_t *) &(rb)->tail)

/**
 * @brief	Initialize ring buffer
 * @param	RingBuff	: Pointer to ring buffer to initialize
 * @param	buffer		: Pointer to buffer to associate with RingBuff
 * @param	itemSize	: Size of each buffer item size
 * @param	count		: Size of ring buffer
 * @note	Memory pointed by @a buffer must have correct alignment of
 * 			@a itemSize, and @a count must be a power of 2 and must at
 * 			least be 2 or greater.
 * @return	Nothing
 */
int RingBuffer_Init(RINGBUFF_T *RingBuff, void *buffer, int itemSize, int count);

/**
 * @brief	Resets the ring buffer to empty
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Nothing
 */
void RingBuffer_Flush(RINGBUFF_T *RingBuff);

/**
 * @brief	Return size the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Size of the ring buffer in bytes
 */
int RingBuffer_GetSize(RINGBUFF_T *RingBuff);

/**
 * @brief	Return number of items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Number of items in the ring buffer
 */
int RingBuffer_GetCount(RINGBUFF_T *RingBuff);

/**
 * @brief	Return number of free items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Number of free items in the ring buffer
 */
int RingBuffer_GetFree(RINGBUFF_T *RingBuff);

/**
 * @brief	Return number of items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	1 if the ring buffer is full, otherwise 0
 */
int RingBuffer_IsFull(RINGBUFF_T *RingBuff);

/**
 * @brief	Return empty status of ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	1 if the ring buffer is empty, otherwise 0
 */
int RingBuffer_IsEmpty(RINGBUFF_T *RingBuff);

/**
 * @brief	Insert a single item into ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: pointer to item
 * @return	1 when successfully inserted,
 *			0 on error (Buffer not initialized using
 *			RingBuffer_Init() or attempted to insert
 *			when buffer is full)
 */
int RingBuffer_Insert(RINGBUFF_T *RingBuff, const void *data);

/**
 * @brief	Insert an array of items into ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to first element of the item array
 * @param	num			: Number of items in the array
 * @return	number of items successfully inserted,
 *			0 on error (Buffer not initialized using
 *			RingBuffer_Init() or attempted to insert
 *			when buffer is full)
 */
int RingBuffer_InsertMult(RINGBUFF_T *RingBuff, const void *data, int num);

/**
 * @brief	Pop an item from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to memory where popped item be stored
 * @return	1 when item popped successfuly onto @a data,
 * 			0 When error (Buffer not initialized using
 * 			RingBuffer_Init() or attempted to pop item when
 * 			the buffer is empty)
 */
int RingBuffer_Pop(RINGBUFF_T *RingBuff, void *data);

/**
 * @brief	Pop an array of items from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to memory where popped items be stored
 * @param	num			: Max number of items array @a data can hold
 * @return	Number of items popped onto @a data,
 * 			0 on error (Buffer not initialized using RingBuffer_Init()
 * 			or attempted to pop when the buffer is empty)
 */
int RingBuffer_PopMult(RINGBUFF_T *RingBuff, void *data, int num);

#endif // end __RING_BUFFER_H
/****************************************************************************
                            End Of File
****************************************************************************/
