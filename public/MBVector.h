#ifndef MBVECTOR_H_201202060000
#define MBVECTOR_H_201202060000

#include <stdlib.h>

#include "mbassert.h"

typedef struct MBVector {
    int size;
    int capacity;
    int itemSize;
    void *items;
} MBVector;

typedef MBVector MBVectorData;

void MBVector_EnsureCapacity(MBVector *vector, int capacity);

static INLINE void MBVector_Create(MBVector *vector, int itemSize,
                                   int size, int capacity)
{
    ASSERT(itemSize > 0);
    ASSERT(capacity >= size);
    ASSERT(capacity > 0);
    ASSERT(size >= 0);

    vector->size = size;
    vector->capacity = capacity;
    vector->itemSize = itemSize;

    vector->items = malloc(itemSize * vector->capacity);
}

static INLINE void MBVector_CreateEmpty(MBVector *vector, int itemSize)
{
    MBVector_Create(vector, itemSize, 0, 1);
}

static INLINE void MBVector_CreateWithSize(MBVector *vector, int itemSize,
                                           int size)
{
    MBVector_Create(vector, itemSize, size, size);
}

static INLINE void MBVector_Destroy(MBVector *vector)
{
    free(vector->items);
    vector->items = NULL;
}

static INLINE int MBVector_Size(const MBVector *vector)
{
    ASSERT(vector->size >= 0);
    return vector->size;
}

static INLINE void MBVector_Resize(MBVector *vector, int size)
{
    ASSERT(size >= 0);

    MBVector_EnsureCapacity(vector, size);
    vector->size = size;
}

static INLINE void MBVector_GrowBy(MBVector *vector, int increment)
{
    ASSERT(increment >= 0);
    MBVector_Resize(vector, vector->size + increment);
}

static INLINE void MBVector_ShrinkBy(MBVector *vector, int decrement)
{
    ASSERT(decrement >= 0);
    MBVector_Resize(vector, vector->size - decrement);
}

static INLINE bool MBVector_IsEmpty(const MBVector *vector)
{
    return MBVector_Size(vector) == 0;
}

static INLINE void MBVector_MakeEmpty(MBVector *vector)
{
    MBVector_Resize(vector, 0);
}

static INLINE void *MBVector_Get(MBVector *vector, int index)
{
    ASSERT(index >= 0);
    ASSERT(index < vector->size);

    return ((uint8 *)vector->items) + (index * vector->itemSize);
}

static INLINE void MBVector_Copy(MBVector *dest, const MBVector *src)
{
    ASSERT(dest->itemSize == src->itemSize);

    MBVector_Resize(dest, 0);
    MBVector_EnsureCapacity(dest, src->size);
    memcpy(dest->items, src->items, src->itemSize * src->size);
}

#endif // MBVECTOR_H_201202060000
