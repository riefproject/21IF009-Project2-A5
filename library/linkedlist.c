// Created by: Arief F-sa Wijaya

#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

// Table of Content:
// SLL: Line 15
// DLL: Line 

// ===================================================
//            . . . SINGLE LINKED LIST . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
SingleLinkedList* createSinglyLinkedList() {
    SingleLinkedList* list = new(SingleLinkedList);
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for SingleLinkedList\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

SLLNode* createSLLNode(void* data) {
    SLLNode* node = new(SLLNode);
    if (node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for SLLNode\n");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

/*      GETTERS
 * ==================== */
void* getSLLNodeData(SLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return NULL;
    }
    return node->data;
}

SLLNode* getSLLNextNode(SLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return NULL;
    }
    return node->next;
}

/*      SETTERS
 * ==================== */
void setSLLNodeData(SLLNode* node, void* data) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return;
    }
    node->data = data;
}

void setSLLNextNode(SLLNode* node, SLLNode* next) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return;
    }
    node->next = next;
}

/*     INSERT NODE
 * ==================== */
void insertSLLFront(SingleLinkedList* list, void* data) {
    SLLNode* newNode = createSLLNode(data);
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        return;
    }
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->next = list->head;
        list->head = newNode;
    }
    list->size++;
}

void insertSLLBack(SingleLinkedList* list, void* data) {
    SLLNode* newNode = createSLLNode(data);
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        return;
    }
    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

void insertSLLNode(SingleLinkedList* list, void* data, int pos) {
    if (pos < 0 || pos > list->size) {
        fprintf(stderr, "Error: Invalid position for insertion\n");
        return;
    }
    if (pos == 0) {
        insertSLLFront(list, data);
        return;
    }
    if (pos == list->size) {
        insertSLLBack(list, data);
        return;
    }
    SLLNode* newNode = createSLLNode(data);
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        return;
    }
    SLLNode* current = list->head;
    for (int i = 0; i < pos - 1; i++) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    list->size++;
}

/*     REMOVE NODE
 * ==================== */
void removeSLLFront(SingleLinkedList* list, SLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return;
    }
    if (list->head == node) {
        list->head = node->next;
        if (list->head == NULL) {
            list->tail = NULL;
        }
    }
    else {
        SLLNode* current = list->head;
        while (current != NULL && current->next != node) {
            current = current->next;
        }
        if (current != NULL) {
            current->next = node->next;
            if (node == list->tail) {
                list->tail = current;
            }
        }
    }
    delete(node);
    list->size--;
}

void removeSLLBack(SingleLinkedList* list, SLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return;
    }
    if (list->tail == node) {
        list->tail = node->next;
        if (list->tail == NULL) {
            list->head = NULL;
        }
    }
    else {
        SLLNode* current = list->head;
        while (current != NULL && current->next != node) {
            current = current->next;
        }
        if (current != NULL) {
            current->next = node->next;
            if (node == list->head) {
                list->head = current;
            }
        }
    }
    delete(node);
    list->size--;
}

void removeSLLNode(SingleLinkedList* list, SLLNode* node, int pos) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return;
    }
    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "Error: Invalid position for removal\n");
        return;
    }
    if (pos == 0) {
        removeSLLFront(list, node);
        return;
    }
    if (pos == list->size - 1) {
        removeSLLBack(list, node);
        return;
    }
    SLLNode* current = list->head;
    for (int i = 0; i < pos - 1; i++) {
        current = current->next;
    }
    current->next = node->next;
    delete(node);
    list->size--;
}

/*       GET NODE
 * ==================== */
void* SLL_getFront(SingleLinkedList* list) {
    if (list->head == NULL) {
        fprintf(stderr, "Error: List is empty\n");
        return NULL;
    }
    return list->head->data;
}

void* SLL_getBack(SingleLinkedList* list) {
    if (list->tail == NULL) {
        fprintf(stderr, "Error: List is empty\n");
        return NULL;
    }
    return list->tail->data;
}

void* SLL_getNode(SingleLinkedList* list, int pos) {
    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "Error: Invalid position for retrieval\n");
        return NULL;
    }
    SLLNode* current = list->head;
    for (int i = 0; i < pos; i++) {
        current = current->next;
    }
    if (current == NULL) {
        fprintf(stderr, "Error: Node not found\n");
        return NULL;
    }
    return current->data;
}


/*  DEALOC AND DESTRUCT
 * ==================== */
void SLL_clearList(SingleLinkedList* list) {
    SLLNode* current = list->head;
    while (current != NULL) {
        SLLNode* next = current->next;
        delete(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void SLL_freeList(SingleLinkedList* list) {
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return;
    }
    SLL_clearList(list);
    delete(list);
}

void SLL_freeNode(SLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: SLLNode is NULL\n");
        return;
    }
    delete(node->data); // Assuming data is dynamically allocated
    delete(node);
}


/*      PRINT LIST
 * ==================== */
void SLL_printList(SingleLinkedList* list, void (*printFunc)(void*)) {
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return;
    }
    SLLNode* current = list->head;
    while (current != NULL) {
        printFunc(current->data);
        current = current->next;
    }
}

void SLL_printReverse(SLLNode* node, void (*printFunc)(void*)) {
    if (node == NULL) return;
    SLL_printReverse(node->next, printFunc);
    printFunc(node->data);
}

void SLL_printListReverse(SingleLinkedList* list, void (*printFunc)(void*)) {
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return;
    }

    SLL_printReverse(list->head, printFunc);
}



// ===================================================
//            . . . DOUBLY LINKED LIST . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */

DoublyLinkedList* createDoublyLinkedList() {
    DoublyLinkedList* list = new(DoublyLinkedList);
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for DoublyLinkedList\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

DLLNode* createDLLNode(void* data) {
    DLLNode* node = new(DLLNode);
    if (node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for DLLNode\n");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/*      GETTERS
 * ==================== */

void* DLL_getNodeData(DLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return NULL;
    }
    return node->data;
}

DLLNode* DLL_getNextNode(DLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return NULL;
    }
    return node->next;
}

DLLNode* DLL_getPrevNode(DLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return NULL;
    }
    return node->prev;
}

/*      SETTERS
 * ==================== */

void DLL_setNodeData(DLLNode* node, void* data) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    node->data = data;
}

void DLL_setNextNode(DLLNode* node, DLLNode* next) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    node->next = next;
}

void DLL_setPrevNode(DLLNode* node, DLLNode* prev) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    node->prev = prev;
}

/*     INSERT NODE
 * ==================== */

void DLL_insertFront(DoublyLinkedList* list, void* data) {
    DLLNode* newNode = createDLLNode(data);
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        return;
    }
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    list->size++;
}

void DLL_insertBack(DoublyLinkedList* list, void* data) {
    DLLNode* newNode = createDLLNode(data);
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        return;
    }
    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

void DLL_insertNode(DoublyLinkedList* list, void* data, int pos) {
    if (pos < 0 || pos > list->size) {
        fprintf(stderr, "Error: Invalid position for insertion\n");
        return;
    }
    if (pos == 0) {
        DLL_insertFront(list, data);
        return;
    }
    if (pos == list->size) {
        DLL_insertBack(list, data);
        return;
    }
    DLLNode* newNode = createDLLNode(data);
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        return;
    }
    DLLNode* current = list->head;
    for (int i = 0; i < pos - 1; i++) {
        current = current->next;
    }
    newNode->next = current->next;
    newNode->prev = current;
    if (current->next != NULL) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    list->size++;
}

/*     REMOVE NODE
 * ==================== */

void DLL_removeFront(DoublyLinkedList* list, DLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    if (list->head == node) {
        list->head = node->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        }
        else {
            list->tail = NULL;
        }
    }
    else {
        node->prev->next = node->next;
        if (node->next != NULL) {
            node->next->prev = node->prev;
        }
    }
    DLL_freeNode(node);
    list->size--;
}

void DLL_removeBack(DoublyLinkedList* list, DLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    if (list->tail == node) {
        list->tail = node->prev;
        if (list->tail != NULL) {
            list->tail->next = NULL;
        }
        else {
            list->head = NULL;
        }
    }
    else {
        node->next->prev = node->prev;
        if (node->prev != NULL) {
            node->prev->next = node->next;
        }
    }
    DLL_freeNode(node);
    list->size--;
}

void DLL_removeNode(DoublyLinkedList* list, DLLNode* node, int pos) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "Error: Invalid position for removal\n");
        return;
    }
    if (pos == 0) {
        DLL_removeFront(list, node);
        return;
    }
    if (pos == list->size - 1) {
        DLL_removeBack(list, node);
        return;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    DLL_freeNode(node);
    list->size--;
}

/*       GET NODE
 * ==================== */

void* DLL_getFront(DoublyLinkedList* list) {
    if (list->head == NULL) {
        fprintf(stderr, "Error: List is empty\n");
        return NULL;
    }
    return list->head->data;
}

void* DLL_getBack(DoublyLinkedList* list) {
    if (list->tail == NULL) {
        fprintf(stderr, "Error: List is empty\n");
        return NULL;
    }
    return list->tail->data;
}

void* DLL_getNode(DoublyLinkedList* list, int pos) {
    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "Error: Invalid position for retrieval\n");
        return NULL;
    }
    DLLNode* current = list->head;
    for (int i = 0; i < pos; i++) {
        current = current->next;
    }
    return current->data;
}

/*  DEALOC AND DESTRUCT
 * ==================== */

void DLL_clearList(DoublyLinkedList* list) {
    DLLNode* current = list->head;
    while (current != NULL) {
        DLLNode* next = current->next;
        DLL_freeNode(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void DLL_freeList(DoublyLinkedList* list) {
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return;
    }
    DLL_clearList(list);
    delete(list);
    list = NULL;
}

void DLL_freeNode(DLLNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: DLLNode is NULL\n");
        return;
    }
    delete(node->data);
    delete(node);
    node = NULL;
}

/*      PRINT LIST
 * ==================== */

void DLL_printList(DoublyLinkedList* list, void (*printFunc)(void*)) {
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return;
    }
    DLLNode* current = list->head;
    while (current != NULL) {
        printFunc(current->data);
        current = current->next;
    }
    printf("\n");
}

void DLL_printListReverse(DoublyLinkedList* list, void (*printFunc)(void*)) {
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return;
    }
    DLLNode* current = list->tail;
    while (current != NULL) {
        printFunc(current->data);
        current = current->prev;
    }
    printf("\n");
}
