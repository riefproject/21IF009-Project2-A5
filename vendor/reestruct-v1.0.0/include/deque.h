#ifndef DEQUE_H
#define DEQUE_H
#include "linkedlist.h"

struct Deque;
typedef unsigned int uint;

/** Double-ended queue built on singly linked list nodes. */
typedef struct Deque {
    SLLNode* Front; /**< Front node. */
    SLLNode* Rear;  /**< Rear node. */
    uint size;      /**< Number of elements. */
} Deque;

// ===================================================
//                . . . DEQUE . . .
// ===================================================

/** Create empty deque.
 *  @return New deque or NULL on allocation failure.
 */
RSTAPI Deque* createDeque(void);

/** Return true if deque has no elements.
 *  @param[in] queue Deque pointer.
 */
RSTAPI bool isDequeEmpty(Deque* queue);
/** Number of elements.
 *  @param[in] queue Deque pointer.
 */
RSTAPI uint getDequeSize(Deque* queue);

/** Peek front element.
 *  @param[in] queue Deque pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* Deque_peekFront(Deque* queue);
/** Peek back element.
 *  @param[in] queue Deque pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* Deque_peekBack(Deque* queue);

/** Push at back.
 *  @param[in,out] queue Deque pointer.
 *  @param[in] data Payload pointer (not copied).
 */
RSTAPI void pushBack(Deque* queue, void* data);
/** Push at front.
 *  @param[in,out] queue Deque pointer.
 *  @param[in] data Payload pointer (not copied).
 */
RSTAPI void pushFront(Deque* queue, void* data);
/** Pop from back.
 *  @param[in,out] queue Deque pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* popBack(Deque* queue);
/** Pop from front.
 *  @param[in,out] queue Deque pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* popFront(Deque* queue);

/** Remove all elements but keep deque allocated.
 *  @param[in,out] queue Deque pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void clearDeque(Deque* queue);
/** Free all nodes and deque struct.
 *  @param[in,out] queue Deque pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void freeDeque(Deque* queue);

/** Print front->back using callback.
 *  @param[in] queue Deque pointer.
 *  @param[in] printFunc Callback to print payload.
 */
RSTAPI void printDeque(Deque* queue, void (*printFunc)(void*));
/** Print back->front using callback.
 *  @param[in] queue Deque pointer.
 *  @param[in] printFunc Callback to print payload.
 */
RSTAPI void printDequeReverse(Deque* queue, void (*printFunc)(void*));

#endif
