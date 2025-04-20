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
Queue* createQueue(void);

/*      STATUS CHECKS
 * ==================== */
bool isQueueEmpty(Queue* queue);
bool isQueueFull(Queue* queue);
uint getQueueSize(Queue* queue);

/*     QUEUE OPERATIONS
 * ==================== */
void enqueue(Queue* queue, void* data);
void* dequeue(Queue* queue);
void* peek(Queue* queue);

/*  DEALOC AND DESTRUCT
 * ==================== */
void clearQueue(Queue* queue);
void freeQueue(Queue* queue);

/*      PRINT QUEUE
 * ==================== */
void printQueue(Queue* queue, void (*printFunc)(void*));
void printQueueReverse(Queue* queue, void (*printFunc)(void*));

#endif