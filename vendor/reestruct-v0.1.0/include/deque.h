#ifndef DEQUE_H
#define DEQUE_H
#include "linkedlist.h"

struct Deque;
typedef unsigned int uint;

typedef struct Deque {
    SLLNode* Front;
    SLLNode* Rear;
    uint size;
} Deque;

// ===================================================
//                . . . DEQUE . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
RSTAPI Deque* createDeque(void);

/*      STATUS CHECKS
 * ==================== */
RSTAPI bool isDequeEmpty(Deque* queue);
RSTAPI uint getDequeSize(Deque* queue);

/*     ELEMENT ACCESS
 * ==================== */
RSTAPI void* Deque_peekFront(Deque* queue);
RSTAPI void* Deque_peekBack(Deque* queue);

/*     DEQUE OPERATIONS
 * ==================== */
RSTAPI void pushBack(Deque* queue, void* data);
RSTAPI void pushFront(Deque* queue, void* data);
RSTAPI void* popBack(Deque* queue);
RSTAPI void* popFront(Deque* queue);

/*  DEALOC AND DESTRUCT
 * ==================== */
RSTAPI void clearDeque(Deque* queue);
RSTAPI void freeDeque(Deque* queue);

/*      PRINT QUEUE
 * ==================== */
RSTAPI void printDeque(Deque* queue, void (*printFunc)(void*));
RSTAPI void printDequeReverse(Deque* queue, void (*printFunc)(void*));

#endif