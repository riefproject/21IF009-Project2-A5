#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#ifndef ALOCATE_SHORTCUTS
#define ALOCATE_SHORTCUTS
#define new(type) (type*)malloc(sizeof(type))
#define delete(ptr) do { free(ptr); ptr = NULL; } while (0)
#endif

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
SingleLinkedList* createSingleLinkedList();
SLLNode* createSLLNode(void* data);

/*      GETTERS
 * ==================== */
void* SLL_getNodeData(SLLNode* node);
SLLNode* SLL_getNextNode(SLLNode* node);

/*      SETTERS
 * ==================== */
void SLL_setNodeData(SLLNode* node, void* data);
void SLL_setNextNode(SLLNode* node, SLLNode* next);

/*     INSERT NODE
 * ==================== */
void SLL_insertFront(SingleLinkedList* list, void* data);
void SLL_insertBack(SingleLinkedList* list, void* data);
void SLL_insertNode(SingleLinkedList* list, void* data, int pos);

/*     REMOVE NODE
 * ==================== */
void SLL_removeFront(SingleLinkedList* list, SLLNode* node);
void SLL_removeBack(SingleLinkedList* list, SLLNode* node);
void SLL_removeNode(SingleLinkedList* list, SLLNode* node, int pos);

/*       GET NODE
 * ==================== */
void* SLL_getFront(SingleLinkedList* list);
void* SLL_getBack(SingleLinkedList* list);
void* SLL_getNode(SingleLinkedList* list, int pos);

/*  DEALOC AND DESTRUCT
 * ==================== */
void SLL_clearList(SingleLinkedList* list);
void SLL_freeList(SingleLinkedList* list);
void SLL_freeNode(SLLNode* node);

/*      PRINT LIST
* ==================== */
void SLL_printList(SingleLinkedList* list, void (*printFunc)(void*));
void SLL_printReverse(SLLNode* node, void (*printFunc)(void*));
void SLL_printListReverse(SingleLinkedList* list, void (*printFunc)(void*));

// ===================================================
//            . . . DOUBLY LINKED LIST . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
DoublyLinkedList* createDoublyLinkedList();
DLLNode* createDLLNode(void* data);

/*      GETTERS
 * ==================== */
void* DLL_getNodeData(DLLNode* node);
DLLNode* DLL_getNextNode(DLLNode* node);
DLLNode* DLL_getPrevNode(DLLNode* node);

/*      SETTERS
 * ==================== */
void DLL_setNodeData(DLLNode* node, void* data);
void DLL_setNextNode(DLLNode* node, DLLNode* next);
void DLL_setPrevNode(DLLNode* node, DLLNode* prev);

/*     INSERT NODE
 * ==================== */
void DLL_insertFront(DoublyLinkedList* list, void* data);
void DLL_insertBack(DoublyLinkedList* list, void* data);
void DLL_insertNode(DoublyLinkedList* list, void* data, int pos);

/*     REMOVE NODE
 * ==================== */
void DLL_removeFront(DoublyLinkedList* list, DLLNode* node);
void DLL_removeBack(DoublyLinkedList* list, DLLNode* node);
void DLL_removeNode(DoublyLinkedList* list, DLLNode* node, int pos);

/*       GET NODE
 * ==================== */
void* DLL_getFront(DoublyLinkedList* list);
void* DLL_getBack(DoublyLinkedList* list);
void* DLL_getNode(DoublyLinkedList* list, int pos);

/*  DEALOC AND DESTRUCT
 * ==================== */
void DLL_clearList(DoublyLinkedList* list);
void DLL_freeList(DoublyLinkedList* list);
void DLL_freeNode(DLLNode* node);

/*      PRINT LIST
 * ==================== */
void DLL_printList(DoublyLinkedList* list, void (*printFunc)(void*));
void DLL_printListReverse(DoublyLinkedList* list, void (*printFunc)(void*));

#endif