#ifndef STACK_H  
#define STACK_H
#include "linkedlist.h"

struct Stack;

typedef unsigned int uint;
/** LIFO stack built on singly linked list nodes. */
typedef struct Stack {
    SLLNode* Top; /**< Top node pointer. */
    uint size;    /**< Number of elements. */
}Stack;

// ===================================================
//                . . . STACK . . .
// ===================================================

/** Create empty stack.
 *  @return New stack or NULL on allocation failure.
 */
RSTAPI Stack* createStack(void);

/** Return true if stack has no elements.
 *  @param[in] stack Stack pointer.
 */
RSTAPI bool isStackEmpty(Stack* stack);
/** Always false for linked stack (no fixed capacity). */
RSTAPI bool isStackFull(Stack* stack);
/** Number of elements.
 *  @param[in] stack Stack pointer.
 */
RSTAPI uint getStackSize(Stack* stack);

/** Push data pointer onto stack.
 *  @param[in,out] stack Stack pointer.
 *  @param[in] data Payload pointer (not copied).
 */
RSTAPI void push(Stack* stack, void* data);
/** Pop and return top data pointer.
 *  @param[in,out] stack Stack pointer.
 *  @return Data pointer or NULL if empty/NULL.
 */
RSTAPI void* pop(Stack* stack);
/** Return top data pointer without removing.
 *  @param[in] stack Stack pointer.
 *  @return Data pointer or NULL if empty/NULL.
 */
RSTAPI void* Stack_Peek(Stack* stack);

/** Remove all elements but keep stack allocated.
 *  @param[in,out] stack Stack pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void clearStack(Stack* stack);
/** Free all nodes and stack struct.
 *  @param[in,out] stack Stack pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void freeStack(Stack* stack);

/** Print top->bottom using callback.
 *  @param[in] stack Stack pointer.
 *  @param[in] printFunc Callback to print a payload.
 */
RSTAPI void printStack(Stack* stack, void (*printFunc)(void*));
/** Print bottom->top using callback.
 *  @param[in] stack Stack pointer.
 *  @param[in] printFunc Callback to print a payload.
 */
RSTAPI void printStackReverse(Stack* stack, void (*printFunc)(void*));

#endif
