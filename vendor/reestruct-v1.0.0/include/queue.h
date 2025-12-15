#ifndef QUEUE_H
#define QUEUE_H
#include "linkedlist.h"

struct Queue;

typedef unsigned int uint;
/** FIFO queue built on singly linked list nodes. */
typedef struct Queue {
    SLLNode* Front; /**< Front node. */
    SLLNode* Rear;  /**< Rear node. */
    uint size;      /**< Number of elements. */
}Queue;

// ===================================================
//                . . . QUEUE . . .
// ===================================================

/** Create empty queue.
 *  @return New queue or NULL on allocation failure.
 */
RSTAPI Queue* createQueue(void);

/** Return true if queue has no elements.
 *  @param[in] queue Queue pointer.
 */
RSTAPI bool isQueueEmpty(Queue* queue);
/** Always false for linked queue (no fixed capacity). */
RSTAPI bool isQueueFull(Queue* queue);
/** Number of elements.
 *  @param[in] queue Queue pointer.
 */
RSTAPI uint getQueueSize(Queue* queue);

/** Enqueue data pointer at rear.
 *  @param[in,out] queue Queue pointer.
 *  @param[in] data Payload pointer (not copied).
 */
RSTAPI void Enqueue(Queue* queue, void* data);
/** Dequeue from front, returning data pointer.
 *  @param[in,out] queue Queue pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* Dequeue(Queue* queue);
/** Peek front without removing.
 *  @param[in] queue Queue pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* Queue_Peek(Queue* queue);

/** Remove all elements but keep queue allocated.
 *  @param[in,out] queue Queue pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void clearQueue(Queue* queue);
/** Free all nodes and queue struct.
 *  @param[in,out] queue Queue pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void freeQueue(Queue* queue);

/** Print front->rear using callback.
 *  @param[in] queue Queue pointer.
 *  @param[in] printFunc Callback to print payload.
 */
RSTAPI void printQueue(Queue* queue, void (*printFunc)(void*));
/** Print rear->front using callback.
 *  @param[in] queue Queue pointer.
 *  @param[in] printFunc Callback to print payload.
 */
RSTAPI void printQueueReverse(Queue* queue, void (*printFunc)(void*));

#endif
