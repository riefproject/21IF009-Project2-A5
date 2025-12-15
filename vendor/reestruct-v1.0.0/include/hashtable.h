#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "reestruct_export.h"

#ifndef ALOCATE_SHORTCUTS
#define ALOCATE_SHORTCUTS
#define new(type) (type*)malloc(sizeof(type))
#define delete(ptr) do { free(ptr); ptr = NULL; } while (0)
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/** Hash function returning a size_t hash for a key. */
typedef size_t (*HashFunc)(const void* key);
/** Equality comparator for keys. */
typedef bool (*HashEquals)(const void* a, const void* b);

/** Hash table entry (chained bucket). */
typedef struct HTEntry {
    void* key;            /**< Key pointer. */
    void* value;          /**< Value pointer. */
    struct HTEntry* next; /**< Next entry in bucket. */
} HTEntry;

/** Chained hash table. */
typedef struct HashTable {
    HTEntry** buckets; /**< Bucket array. */
    size_t capacity;   /**< Bucket count. */
    size_t size;       /**< Element count. */
    HashFunc hash;     /**< Hash function. */
    HashEquals equals; /**< Key equality. */
} HashTable;

/** Create hash table with initial capacity.
 *  @param[in] capacity Bucket count hint (will be clamped to minimum).
 *  @param[in] hash Hash function (required).
 *  @param[in] equals Equality comparator (optional; defaults to pointer equality).
 *  @return HashTable pointer or NULL on allocation failure.
 */
RSTAPI HashTable* createHashTable(size_t capacity, HashFunc hash, HashEquals equals);
/** Insert or replace key->value; returns true if inserted new key.
 *  @param[in,out] table Table pointer.
 *  @param[in] key Key pointer (not copied).
 *  @param[in] value Value pointer (not copied).
 *  @return True if key was newly inserted; false if replaced or on error.
 */
RSTAPI bool htPut(HashTable* table, void* key, void* value);
/** Lookup value by key (NULL if absent).
 *  @param[in] table Table pointer.
 *  @param[in] key Key to search.
 *  @return Value pointer or NULL if not found.
 */
RSTAPI void* htGet(HashTable* table, const void* key);
/** Return true if key exists.
 *  @param[in] table Table pointer.
 *  @param[in] key Key to search.
 */
RSTAPI bool htContains(HashTable* table, const void* key);
/** Remove key and return associated value, or NULL if absent.
 *  @param[in,out] table Table pointer.
 *  @param[in] key Key to remove.
 *  @return Value pointer or NULL if not found.
 */
RSTAPI void* htRemove(HashTable* table, const void* key);
/** Number of stored elements.
 *  @param[in] table Table pointer.
 */
RSTAPI size_t htSize(HashTable* table);
/** True if no elements.
 *  @param[in] table Table pointer.
 */
RSTAPI bool htIsEmpty(HashTable* table);
/** Clear all entries (does not free keys/values).
 *  @param[in,out] table Table pointer.
 */
RSTAPI void htClear(HashTable* table);
/** Free table and entries (does not free keys/values).
 *  @param[in,out] table Table pointer.
 */
RSTAPI void freeHashTable(HashTable* table);

#endif
