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
Stack* createStack(void);

/*      STATUS CHECKS
 * ==================== */
bool isStackEmpty(Stack* stack);
bool isStackFull(Stack* stack);
uint getStackSize(Stack* stack);

/*     STACK OPERATIONS
 * ==================== */
void push(Stack* stack, void* data);
void* pop(Stack* stack);
void* peek(Stack* stack);

/*  DEALOC AND DESTRUCT
 * ==================== */
void clearStack(Stack* stack);
void freeStack(Stack* stack);

/*      PRINT STACK
 * ==================== */
void printStack(Stack* stack, void (*printFunc)(void*));
void printStackReverse(Stack* stack, void (*printFunc)(void*));

#endif