#ifndef SET_H
#define SET_H
#include "map.h"

/** Ordered set built on map keys. */
typedef struct Set {
    Map* map; /**< Underlying ordered map. */
} Set;

/** Create empty ordered set.
 *  @param[in] cmp Comparator (required).
 *  @return Set pointer or NULL on allocation failure.
 */
RSTAPI Set* createSet(MapCompare cmp);
/** Insert key; returns true if new.
 *  @param[in,out] set Set pointer.
 *  @param[in] key Key pointer (not copied).
 *  @return True if inserted new key; false if already present or on error.
 */
RSTAPI bool setAdd(Set* set, void* key);
/** True if key exists.
 *  @param[in] set Set pointer.
 *  @param[in] key Key pointer.
 */
RSTAPI bool setContains(Set* set, const void* key);
/** Remove key; returns true if key was present.
 *  @param[in,out] set Set pointer.
 *  @param[in] key Key pointer.
 */
RSTAPI bool setRemove(Set* set, const void* key);
/** Number of elements.
 *  @param[in] set Set pointer.
 */
RSTAPI size_t setSize(Set* set);
/** True if empty.
 *  @param[in] set Set pointer.
 */
RSTAPI bool setIsEmpty(Set* set);
/** Traverse keys in order.
 *  @param[in] set Set pointer.
 *  @param[in] visit Callback invoked for each key.
 */
RSTAPI void setTraverse(Set* set, void (*visit)(void* key));
/** Clear all keys.
 *  @param[in,out] set Set pointer.
 */
RSTAPI void clearSet(Set* set);
/** Free set (does not free keys).
 *  @param[in,out] set Set pointer.
 */
RSTAPI void freeSet(Set* set);

#endif
