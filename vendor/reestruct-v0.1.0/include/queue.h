#ifndef QUEUE_H
#define QUEUE_H
#include "linkedlist.h"

struct Queue;

typedef unsigned int uint;
typedef struct Queue {
    SLLNode* Front;
    SLLNode* Rear;
    uint size;
}Queue;

// ===================================================
//                . . . QUEUE . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
RSTAPI Queue* createQueue(void);

/*      STATUS CHECKS
 * ==================== */
RSTAPI bool isQueueEmpty(Queue* queue);
RSTAPI bool isQueueFull(Queue* queue);
RSTAPI uint getQueueSize(Queue* queue);

/*     QUEUE OPERATIONS
 * ==================== */
RSTAPI void enqueue(Queue* queue, void* data);
RSTAPI void* dequeue(Queue* queue);
RSTAPI void* Queue_Peek(Queue* queue);

/*  DEALOC AND DESTRUCT
 * ==================== */
RSTAPI void clearQueue(Queue* queue);
RSTAPI void freeQueue(Queue* queue);

/*      PRINT QUEUE
 * ==================== */
RSTAPI void printQueue(Queue* queue, void (*printFunc)(void*));
RSTAPI void printQueueReverse(Queue* queue, void (*printFunc)(void*));

#endif