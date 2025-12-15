#ifndef HEAP_H
#define HEAP_H
#include "reestruct_export.h"

#ifndef ALOCATE_SHORTCUTS
#define ALOCATE_SHORTCUTS
#define new(type) (type*)malloc(sizeof(type))
#define delete(ptr) do { free(ptr); ptr = NULL; } while (0)
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/** Comparator returning <0 if a<b, >0 if a>b, 0 if equal. */
typedef int (*HeapCompare)(const void* a, const void* b);

/** Binary heap (min-heap by comparator). */
typedef struct Heap {
    void** data;        /**< Array of element pointers. */
    size_t size;        /**< Number of elements. */
    size_t capacity;    /**< Allocated slots. */
    HeapCompare cmp;    /**< Comparator function. */
} Heap;

/** Create min-heap with default capacity.
 *  @param[in] cmp Comparator (required).
 *  @return Heap pointer or NULL on allocation failure.
 */
RSTAPI Heap* createHeap(HeapCompare cmp);
/** Create min-heap with custom capacity.
 *  @param[in] cmp Comparator (required).
 *  @param[in] capacity Initial capacity (will be clamped to minimum).
 *  @return Heap pointer or NULL on allocation failure.
 */
RSTAPI Heap* createHeapWithCapacity(HeapCompare cmp, size_t capacity);
/** True if heap has no elements.
 *  @param[in] heap Heap pointer.
 */
RSTAPI bool heapIsEmpty(Heap* heap);
/** Number of elements.
 *  @param[in] heap Heap pointer.
 */
RSTAPI size_t heapSize(Heap* heap);
/** Insert item into heap.
 *  @param[in,out] heap Heap pointer.
 *  @param[in] item Payload pointer.
 *  @return True on success, false on allocation failure.
 */
RSTAPI bool heapPush(Heap* heap, void* item);
/** Pop and return smallest element.
 *  @param[in,out] heap Heap pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* heapPop(Heap* heap);
/** Peek smallest element without removing.
 *  @param[in] heap Heap pointer.
 *  @return Payload pointer or NULL if empty/NULL.
 */
RSTAPI void* heapPeek(Heap* heap);
/** Clear contents, keep allocation.
 *  @param[in,out] heap Heap pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void clearHeap(Heap* heap);
/** Free heap and internal array.
 *  @param[in,out] heap Heap pointer.
 *  @note Does not free stored payloads.
 */
RSTAPI void freeHeap(Heap* heap);

#endif
