#ifndef UMAP_H
#define UMAP_H
#include "hashtable.h"

/** Unordered map built on HashTable. */
typedef struct UMap {
    HashTable* table; /**< Underlying hash table. */
} UMap;

/** Create empty unordered map.
 *  @param[in] hash Hash function (required).
 *  @param[in] equals Equality comparator (optional; defaults to pointer equality).
 *  @return Map pointer or NULL on allocation failure.
 */
RSTAPI UMap* createUMap(HashFunc hash, HashEquals equals);
/** Insert or replace key->value; returns true if inserted new key.
 *  @param[in,out] map Map pointer.
 *  @param[in] key Key pointer (not copied).
 *  @param[in] value Value pointer (not copied).
 *  @return True if inserted new key; false if replaced or on error.
 */
RSTAPI bool umapPut(UMap* map, void* key, void* value);
/** Get value by key (NULL if absent).
 *  @param[in] map Map pointer.
 *  @param[in] key Key pointer.
 *  @return Value pointer or NULL if not found.
 */
RSTAPI void* umapGet(UMap* map, const void* key);
/** True if key exists.
 *  @param[in] map Map pointer.
 *  @param[in] key Key pointer.
 */
RSTAPI bool umapContains(UMap* map, const void* key);
/** Remove key and return value, or NULL if absent.
 *  @param[in,out] map Map pointer.
 *  @param[in] key Key pointer.
 *  @return Value pointer or NULL if not found.
 */
RSTAPI void* umapRemove(UMap* map, const void* key);
/** Number of elements.
 *  @param[in] map Map pointer.
 */
RSTAPI size_t umapSize(UMap* map);
/** True if empty.
 *  @param[in] map Map pointer.
 */
RSTAPI bool umapIsEmpty(UMap* map);
/** Clear entries (does not free keys/values).
 *  @param[in,out] map Map pointer.
 */
RSTAPI void clearUMap(UMap* map);
/** Free map (does not free keys/values).
 *  @param[in,out] map Map pointer.
 */
RSTAPI void freeUMap(UMap* map);

#endif
