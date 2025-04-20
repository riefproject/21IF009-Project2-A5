#ifndef STACK_H  
#define STACK_H
#include "linkedlist.h"

struct Stack;

typedef unsigned int uint;
typedef struct Stack {
    SLLNode* Top;
    uint size;
}Stack;

// ===================================================
//                . . . STACK . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
RSTAPI Stack* createStack(void);

/*      STATUS CHECKS
 * ==================== */
RSTAPI bool isStackEmpty(Stack* stack);
RSTAPI bool isStackFull(Stack* stack);
RSTAPI uint getStackSize(Stack* stack);

/*     STACK OPERATIONS
 * ==================== */
RSTAPI void push(Stack* stack, void* data);
RSTAPI void* pop(Stack* stack);
RSTAPI void* Stack_Peek(Stack* stack);

/*  DEALOC AND DESTRUCT
 * ==================== */
RSTAPI void clearStack(Stack* stack);
RSTAPI void freeStack(Stack* stack);

/*      PRINT STACK
 * ==================== */
RSTAPI void printStack(Stack* stack, void (*printFunc)(void*));
RSTAPI void printStackReverse(Stack* stack, void (*printFunc)(void*));

#endif