#ifndef USET_H
#define USET_H
#include "hashtable.h"

/** Unordered set built on HashTable keys. */
typedef struct USet {
    HashTable* table; /**< Underlying hash table. */
} USet;

/** Create empty unordered set.
 *  @param[in] hash Hash function (required).
 *  @param[in] equals Equality comparator (optional; defaults to pointer equality).
 *  @return Set pointer or NULL on allocation failure.
 */
RSTAPI USet* createUSet(HashFunc hash, HashEquals equals);
/** Insert key; returns true if new.
 *  @param[in,out] set Set pointer.
 *  @param[in] key Key pointer (not copied).
 *  @return True if inserted new key; false if already present or on error.
 */
RSTAPI bool usetAdd(USet* set, void* key);
/** True if key exists.
 *  @param[in] set Set pointer.
 *  @param[in] key Key pointer.
 */
RSTAPI bool usetContains(USet* set, const void* key);
/** Remove key; returns true if key was present.
 *  @param[in,out] set Set pointer.
 *  @param[in] key Key pointer.
 */
RSTAPI bool usetRemove(USet* set, const void* key);
/** Number of elements.
 *  @param[in] set Set pointer.
 */
RSTAPI size_t usetSize(USet* set);
/** True if empty.
 *  @param[in] set Set pointer.
 */
RSTAPI bool usetIsEmpty(USet* set);
/** Clear keys (does not free keys).
 *  @param[in,out] set Set pointer.
 */
RSTAPI void clearUSet(USet* set);
/** Free set (does not free keys).
 *  @param[in,out] set Set pointer.
 */
RSTAPI void freeUSet(USet* set);

#endif
