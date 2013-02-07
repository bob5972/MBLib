/*
 * MBVector.c --
 */

#include "MBVector.h"

void MBVector_EnsureCapacity(MBVector *vector, int capacity)
{
    int newCap;
    ASSERT(vector->capacity > 0);
    ASSERT(vector->size >= 0);
    ASSERT(capacity >= 0);
    ASSERT(vector->itemSize > 0);

    if (vector->capacity >= capacity) {
        return;
    }

    newCap = vector->capacity;
    while (newCap < capacity) {
        newCap *= 2;
    }
    ASSERT(newCap > vector->capacity);

    vector->items = realloc(vector->items, vector->itemSize * newCap);
}
