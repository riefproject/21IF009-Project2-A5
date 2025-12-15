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

/** Singly linked list node. */
typedef struct SLLNode {
    void* data;             /**< User payload pointer. */
    struct SLLNode* next;   /**< Next node or NULL. */
}SLLNode;

/** Singly linked list container. */
typedef struct SingleLinkedList {
    SLLNode* head;  /**< Head node pointer. */
    SLLNode* tail;  /**< Tail node pointer. */
    int size;       /**< Number of elements. */
}SingleLinkedList;

/** Doubly linked list node. */
typedef struct DLLNode {
    void* data;             /**< User payload pointer. */
    struct DLLNode* next;   /**< Next node or NULL. */
    struct DLLNode* prev;   /**< Previous node or NULL. */
}DLLNode;

/** Doubly linked list container. */
typedef struct DoublyLinkedList {
    DLLNode* head;  /**< Head node pointer. */
    DLLNode* tail;  /**< Tail node pointer. */
    int size;       /**< Number of elements. */
}DoublyLinkedList;


// ===================================================
//            . . . SINGLE LINKED LIST . . .
// ===================================================
/** Create empty singly linked list.
 *  @return New list or NULL on allocation failure.
 */
RSTAPI SingleLinkedList* createSingleLinkedList();
/** Create a new singly linked list node with data.
 *  @param[in] data Pointer stored in the node (ownership not transferred).
 *  @return New node or NULL on allocation failure.
 */
RSTAPI SLLNode* createSLLNode(void* data);

/** Get node payload.
 *  @param[in] node Node pointer.
 *  @return Stored data pointer, or NULL if node is NULL.
 */
RSTAPI void* SLL_getNodeData(SLLNode* node);
/** Get next node.
 *  @param[in] node Node pointer.
 *  @return Next node or NULL.
 */
RSTAPI SLLNode* SLL_getNextNode(SLLNode* node);

/** Set node payload.
 *  @param[in,out] node Node to modify.
 *  @param[in] data New data pointer.
 */
RSTAPI void SLL_setNodeData(SLLNode* node, void* data);
/** Set next pointer.
 *  @param[in,out] node Node to modify.
 *  @param[in] next Next node pointer.
 */
RSTAPI void SLL_setNextNode(SLLNode* node, SLLNode* next);

/** Insert at front.
 *  @param[in,out] list Target list (non-NULL).
 *  @param[in] data Payload pointer to store (not copied).
 */
RSTAPI void SLL_insertFront(SingleLinkedList* list, void* data);
/** Insert at back.
 *  @param[in,out] list Target list (non-NULL).
 *  @param[in] data Payload pointer to store (not copied).
 */
RSTAPI void SLL_insertBack(SingleLinkedList* list, void* data);
/** Insert at position (0..size).
 *  @param[in,out] list Target list (non-NULL).
 *  @param[in] data Payload pointer to store (not copied).
 *  @param[in] pos Zero-based index where 0 = front, size = back.
 */
RSTAPI void SLL_insertNode(SingleLinkedList* list, void* data, int pos);

/** Remove given node assumed at front.
 *  @param[in,out] list Target list (non-NULL).
 *  @param[in,out] node Node to remove (must belong to list).
 */
RSTAPI void SLL_removeFront(SingleLinkedList* list, SLLNode* node);
/** Remove given node assumed at back.
 *  @param[in,out] list Target list (non-NULL).
 *  @param[in,out] node Node to remove (must belong to list).
 */
RSTAPI void SLL_removeBack(SingleLinkedList* list, SLLNode* node);
/** Remove node at position.
 *  @param[in,out] list Target list (non-NULL).
 *  @param[in,out] node Node to remove (must belong to list).
 *  @param[in] pos Zero-based index of the node.
 */
RSTAPI void SLL_removeNode(SingleLinkedList* list, SLLNode* node, int pos);

/** Get front element data.
 *  @param[in] list Target list.
 *  @return Data pointer at head, or NULL if empty/NULL list.
 */
RSTAPI void* SLL_getFront(SingleLinkedList* list);
/** Get back element data.
 *  @param[in] list Target list.
 *  @return Data pointer at tail, or NULL if empty/NULL list.
 */
RSTAPI void* SLL_getBack(SingleLinkedList* list);
/** Get element data at position.
 *  @param[in] list Target list.
 *  @param[in] pos Zero-based index.
 *  @return Data pointer or NULL on invalid position.
 */
RSTAPI void* SLL_getNode(SingleLinkedList* list, int pos);

/** Delete all nodes but keep list allocated.
 *  @param[in,out] list Target list.
 *  @note Does not free stored data pointers.
 */
RSTAPI void SLL_clearList(SingleLinkedList* list);
/** Delete all nodes and free list.
 *  @param[in,out] list Target list.
 *  @note Does not free stored data pointers.
 */
RSTAPI void SLL_freeList(SingleLinkedList* list);
/** Free a node and its data pointer.
 *  @param[in,out] node Node to free.
 *  @note Frees node->data as well.
 */
RSTAPI void SLL_freeNode(SLLNode* node);

/** Print list head->tail using callback.
 *  @param[in] list Target list.
 *  @param[in] printFunc Callback to print a data pointer.
 */
RSTAPI void SLL_printList(SingleLinkedList* list, void (*printFunc)(void*));
/** Print from a node backwards (recursive helper).
 *  @param[in] node Start node.
 *  @param[in] printFunc Callback to print a data pointer.
 */
RSTAPI void SLL_printReverse(SLLNode* node, void (*printFunc)(void*));
/** Print list tail-first using recursion.
 *  @param[in] list Target list.
 *  @param[in] printFunc Callback to print a data pointer.
 */
RSTAPI void SLL_printListReverse(SingleLinkedList* list, void (*printFunc)(void*));

// ===================================================
//            . . . DOUBLY LINKED LIST . . .
// ===================================================

/** Create empty doubly linked list.
 *  @return New list or NULL on allocation failure.
 */
RSTAPI DoublyLinkedList* createDoublyLinkedList();
/** Create a new doubly linked list node.
 *  @param[in] data Payload pointer to store.
 *  @return New node or NULL on allocation failure.
 */
RSTAPI DLLNode* createDLLNode(void* data);

/** Get node payload.
 *  @param[in] node Target node.
 *  @return Stored data pointer or NULL if node is NULL.
 */
RSTAPI void* DLL_getNodeData(DLLNode* node);
/** Get next node.
 *  @param[in] node Target node.
 *  @return Next node or NULL.
 */
RSTAPI DLLNode* DLL_getNextNode(DLLNode* node);
/** Get previous node.
 *  @param[in] node Target node.
 *  @return Previous node or NULL.
 */
RSTAPI DLLNode* DLL_getPrevNode(DLLNode* node);

/** Set node payload.
 *  @param[in,out] node Target node.
 *  @param[in] data New data pointer.
 */
RSTAPI void DLL_setNodeData(DLLNode* node, void* data);
/** Set next pointer.
 *  @param[in,out] node Target node.
 *  @param[in] next Next node pointer.
 */
RSTAPI void DLL_setNextNode(DLLNode* node, DLLNode* next);
/** Set previous pointer.
 *  @param[in,out] node Target node.
 *  @param[in] prev Previous node pointer.
 */
RSTAPI void DLL_setPrevNode(DLLNode* node, DLLNode* prev);

/** Insert at front.
 *  @param[in,out] list Target list.
 *  @param[in] data Payload pointer (not copied).
 */
RSTAPI void DLL_insertFront(DoublyLinkedList* list, void* data);
/** Insert at back.
 *  @param[in,out] list Target list.
 *  @param[in] data Payload pointer (not copied).
 */
RSTAPI void DLL_insertBack(DoublyLinkedList* list, void* data);
/** Insert at position (0..size).
 *  @param[in,out] list Target list.
 *  @param[in] data Payload pointer (not copied).
 *  @param[in] pos Zero-based index.
 */
RSTAPI void DLL_insertNode(DoublyLinkedList* list, void* data, int pos);

/** Remove node at front.
 *  @param[in,out] list Target list.
 *  @param[in,out] node Node to remove (must belong to list).
 */
RSTAPI void DLL_removeFront(DoublyLinkedList* list, DLLNode* node);
/** Remove node at back.
 *  @param[in,out] list Target list.
 *  @param[in,out] node Node to remove (must belong to list).
 */
RSTAPI void DLL_removeBack(DoublyLinkedList* list, DLLNode* node);
/** Remove node at position.
 *  @param[in,out] list Target list.
 *  @param[in,out] node Node to remove (must belong to list).
 *  @param[in] pos Zero-based index.
 */
RSTAPI void DLL_removeNode(DoublyLinkedList* list, DLLNode* node, int pos);

/** Get front element data.
 *  @param[in] list Target list.
 *  @return Data pointer at head or NULL if empty.
 */
RSTAPI void* DLL_getFront(DoublyLinkedList* list);
/** Get back element data.
 *  @param[in] list Target list.
 *  @return Data pointer at tail or NULL if empty.
 */
RSTAPI void* DLL_getBack(DoublyLinkedList* list);
/** Get element data at position.
 *  @param[in] list Target list.
 *  @param[in] pos Zero-based index.
 *  @return Data pointer or NULL on invalid position.
 */
RSTAPI void* DLL_getNode(DoublyLinkedList* list, int pos);

/** Delete all nodes but keep list allocated.
 *  @param[in,out] list Target list.
 *  @note Does not free stored data pointers.
 */
RSTAPI void DLL_clearList(DoublyLinkedList* list);
/** Delete all nodes and free list.
 *  @param[in,out] list Target list.
 *  @note Does not free stored data pointers.
 */
RSTAPI void DLL_freeList(DoublyLinkedList* list);
/** Free a node and its data pointer.
 *  @param[in,out] node Node to free.
 *  @note Frees node->data as well.
 */
RSTAPI void DLL_freeNode(DLLNode* node);

/** Print list head->tail using callback.
 *  @param[in] list Target list.
 *  @param[in] printFunc Callback to print a data pointer.
 */
RSTAPI void DLL_printList(DoublyLinkedList* list, void (*printFunc)(void*));
/** Print list tail->head using callback.
 *  @param[in] list Target list.
 *  @param[in] printFunc Callback to print a data pointer.
 */
RSTAPI void DLL_printListReverse(DoublyLinkedList* list, void (*printFunc)(void*));

#endif
