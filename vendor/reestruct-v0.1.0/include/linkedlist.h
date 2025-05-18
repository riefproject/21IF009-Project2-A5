#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "reestruct_export.h"

#ifndef ALOCATE_SHORTCUTS
#define ALOCATE_SHORTCUTS
#define new(type) (type*)malloc(sizeof(type))
#define delete(ptr) do { free(ptr); ptr = NULL; } while (0)
#endif

#include <stdbool.h>

struct DoublyLinkedList;
struct SingleLinkedList;
struct DLLNode;
struct SLLNode;

typedef struct SLLNode {
    void* data;
    struct SLLNode* next;
}SLLNode;

typedef struct SingleLinkedList {
    SLLNode* head;
    SLLNode* tail;
    int size;
}SingleLinkedList;

typedef struct DLLNode {
    void* data;
    struct DLLNode* next;
    struct DLLNode* prev;
}DLLNode;

typedef struct DoublyLinkedList {
    DLLNode* head;
    DLLNode* tail;
    int size;
}DoublyLinkedList;


// ===================================================
//            . . . SINGLE LINKED LIST . . .
// ===================================================
/*      CONSTRUCTOR
 * ==================== */
RSTAPI SingleLinkedList* createSingleLinkedList();
RSTAPI SLLNode* createSLLNode(void* data);

/*      GETTERS
 * ==================== */
RSTAPI void* SLL_getNodeData(SLLNode* node);
RSTAPI SLLNode* SLL_getNextNode(SLLNode* node);

/*      SETTERS
 * ==================== */
RSTAPI void SLL_setNodeData(SLLNode* node, void* data);
RSTAPI void SLL_setNextNode(SLLNode* node, SLLNode* next);

/*     INSERT NODE
 * ==================== */
RSTAPI void SLL_insertFront(SingleLinkedList* list, void* data);
RSTAPI void SLL_insertBack(SingleLinkedList* list, void* data);
RSTAPI void SLL_insertNode(SingleLinkedList* list, void* data, int pos);

/*     REMOVE NODE
 * ==================== */
RSTAPI void SLL_removeFront(SingleLinkedList* list, SLLNode* node);
RSTAPI void SLL_removeBack(SingleLinkedList* list, SLLNode* node);
RSTAPI void SLL_removeNode(SingleLinkedList* list, SLLNode* node, int pos);

/*       GET NODE
 * ==================== */
RSTAPI void* SLL_getFront(SingleLinkedList* list);
RSTAPI void* SLL_getBack(SingleLinkedList* list);
RSTAPI void* SLL_getNode(SingleLinkedList* list, int pos);

/*  DEALOC AND DESTRUCT
 * ==================== */
RSTAPI void SLL_clearList(SingleLinkedList* list);
RSTAPI void SLL_freeList(SingleLinkedList* list);
RSTAPI void SLL_freeNode(SLLNode* node);

/*      PRINT LIST
* ==================== */
RSTAPI void SLL_printList(SingleLinkedList* list, void (*printFunc)(void*));
RSTAPI void SLL_printReverse(SLLNode* node, void (*printFunc)(void*));
RSTAPI void SLL_printListReverse(SingleLinkedList* list, void (*printFunc)(void*));

// ===================================================
//            . . . DOUBLY LINKED LIST . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
RSTAPI DoublyLinkedList* createDoublyLinkedList();
RSTAPI DLLNode* createDLLNode(void* data);

/*      GETTERS
 * ==================== */
RSTAPI void* DLL_getNodeData(DLLNode* node);
RSTAPI DLLNode* DLL_getNextNode(DLLNode* node);
RSTAPI DLLNode* DLL_getPrevNode(DLLNode* node);

/*      SETTERS
 * ==================== */
RSTAPI void DLL_setNodeData(DLLNode* node, void* data);
RSTAPI void DLL_setNextNode(DLLNode* node, DLLNode* next);
RSTAPI void DLL_setPrevNode(DLLNode* node, DLLNode* prev);

/*     INSERT NODE
 * ==================== */
RSTAPI void DLL_insertFront(DoublyLinkedList* list, void* data);
RSTAPI void DLL_insertBack(DoublyLinkedList* list, void* data);
RSTAPI void DLL_insertNode(DoublyLinkedList* list, void* data, int pos);

/*     REMOVE NODE
 * ==================== */
RSTAPI void DLL_removeFront(DoublyLinkedList* list, DLLNode* node);
RSTAPI void DLL_removeBack(DoublyLinkedList* list, DLLNode* node);
RSTAPI void DLL_removeNode(DoublyLinkedList* list, DLLNode* node, int pos);

/*       GET NODE
 * ==================== */
RSTAPI void* DLL_getFront(DoublyLinkedList* list);
RSTAPI void* DLL_getBack(DoublyLinkedList* list);
RSTAPI void* DLL_getNode(DoublyLinkedList* list, int pos);

/*  DEALOC AND DESTRUCT
 * ==================== */
RSTAPI void DLL_clearList(DoublyLinkedList* list);
RSTAPI void DLL_freeList(DoublyLinkedList* list);
RSTAPI void DLL_freeNode(DLLNode* node);

/*      PRINT LIST
 * ==================== */
RSTAPI void DLL_printList(DoublyLinkedList* list, void (*printFunc)(void*));
RSTAPI void DLL_printListReverse(DoublyLinkedList* list, void (*printFunc)(void*));

#endif