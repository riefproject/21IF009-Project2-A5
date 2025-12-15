#ifndef MAP_H
#define MAP_H
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
typedef int (*MapCompare)(const void* a, const void* b);

/** Node in ordered map (BST). */
typedef struct MapEntry {
    void* key;                /**< Key pointer. */
    void* value;              /**< Value pointer. */
    struct MapEntry* left;    /**< Left child. */
    struct MapEntry* right;   /**< Right child. */
    struct MapEntry* parent;  /**< Parent. */
} MapEntry;

/** Ordered map (binary search tree). */
typedef struct Map {
    MapEntry* root; /**< Root node. */
    size_t size;    /**< Element count. */
    MapCompare cmp; /**< Key comparator. */
} Map;

/** Create empty ordered map.
 *  @param[in] cmp Comparator (required).
 *  @return Map pointer or NULL on allocation failure.
 */
RSTAPI Map* createMap(MapCompare cmp);
/** Insert or replace key->value; returns true if inserted new key.
 *  @param[in,out] map Map pointer.
 *  @param[in] key Key pointer (not copied).
 *  @param[in] value Value pointer (not copied).
 *  @return True if inserted new key; false if replaced or on error.
 */
RSTAPI bool mapPut(Map* map, void* key, void* value);
/** Get value by key (NULL if absent).
 *  @param[in] map Map pointer.
 *  @param[in] key Key pointer.
 *  @return Value pointer or NULL if not found.
 */
RSTAPI void* mapGet(Map* map, const void* key);
/** True if key exists.
 *  @param[in] map Map pointer.
 *  @param[in] key Key pointer.
 */
RSTAPI bool mapContains(Map* map, const void* key);
/** Remove key and return value, or NULL if absent.
 *  @param[in,out] map Map pointer.
 *  @param[in] key Key pointer.
 *  @return Value pointer or NULL if not found.
 */
RSTAPI void* mapRemove(Map* map, const void* key);
/** Number of stored elements.
 *  @param[in] map Map pointer.
 */
RSTAPI size_t mapSize(Map* map);
/** True if empty.
 *  @param[in] map Map pointer.
 */
RSTAPI bool mapIsEmpty(Map* map);
/** In-order traversal callback (sorted by comparator).
 *  @param[in] map Map pointer.
 *  @param[in] visit Callback invoked for each key/value.
 */
RSTAPI void mapTraverseInOrder(Map* map, void (*visit)(void* key, void* value));
/** Clear entries (does not free keys/values).
 *  @param[in,out] map Map pointer.
 */
RSTAPI void clearMap(Map* map);
/** Free map and nodes (does not free keys/values).
 *  @param[in,out] map Map pointer.
 */
RSTAPI void freeMap(Map* map);

#endif
