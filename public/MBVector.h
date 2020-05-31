/*
 * MBVector.h -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <github@banack.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

static INLINE void *
MBVectorGetHelper(MBVector *vector, int index, int itemSize)
{
    ASSERT(index >= 0);
    ASSERT(index < vector->size);
    ASSERT(itemSize == vector->itemSize);

    return ((uint8 *)vector->items) + (index * itemSize);
}

static INLINE const void *
MBVectorGetHelperConst(const MBVector *vector, int index, int itemSize)
{
    ASSERT(index >= 0);
    ASSERT(index < vector->size);
    ASSERT(itemSize == vector->itemSize);

    return ((uint8 *)vector->items) + (index * itemSize);
}

static INLINE void *MBVector_GetPtr(MBVector *vector, int index)
{
    return MBVectorGetHelper(vector, index, vector->itemSize);
}

static INLINE void MBVector_Copy(MBVector *dest, const MBVector *src)
{
    ASSERT(dest->itemSize == src->itemSize);

    MBVector_Resize(dest, 0);
    MBVector_EnsureCapacity(dest, src->size);
    MBVector_Resize(dest, src->size);
    memcpy(dest->items, src->items, src->itemSize * src->size);
}


static INLINE void MBVector_Consume(MBVector *dest, MBVector *src)
{
    ASSERT(dest->itemSize == src->itemSize);
    
    free(dest->items);
    dest->items = src->items;
    dest->size = src->size;
    dest->capacity = src->capacity;
    MBVector_CreateEmpty(src, dest->itemSize);
}

static INLINE void *MBVector_GetCArray(MBVector *vector)
{
    return vector->items;
}


#define DECLARE_MBVECTOR_TYPE(_type, _name) \
    typedef struct _name { \
        MBVectorData v; \
    } _name ; \
    \
    static INLINE void _name ## _Create \
    (_name *v, int size, int capacity) \
    { MBVector_Create(&v->v, sizeof(_type), size, capacity); } \
    static INLINE void _name ## _CreateEmpty \
    (_name *v) \
    { MBVector_CreateEmpty(&v->v, sizeof(_type)); } \
    static INLINE void _name ## _CreateWithSize \
    (_name *v, int size) \
    { MBVector_CreateWithSize(&v->v, sizeof(_type), size); } \
    static INLINE void _name ## _Destroy \
    (_name *v) \
    { MBVector_Destroy(&v->v); } \
    static INLINE int _name ## _Size \
    (_name *v) \
    { return MBVector_Size(&v->v); } \
    static INLINE void _name ## _Resize \
    (_name *v, int size) \
    { MBVector_Resize(&v->v, size); } \
    static INLINE void _name ## _GrowBy \
    (_name *v, int increment) \
    { MBVector_GrowBy(&v->v, increment); } \
    static INLINE void _name ## _ShrinkBy \
    (_name *v, int decrement) \
    { MBVector_ShrinkBy(&v->v, decrement); } \
    static INLINE bool _name ## _IsEmpty \
    (_name *v) \
    { return MBVector_IsEmpty(&v->v); } \
    static INLINE void _name ## _MakeEmpty \
    (_name *v) \
    { MBVector_MakeEmpty(&v->v); } \
    static INLINE _type *_name ## _GetCArray \
    (_name *v) \
    { return (_type *)MBVector_GetCArray(&v->v); } \
    static INLINE _type *_name ## _GetPtr \
    (_name *v, int index) \
    { return (_type *)MBVectorGetHelper(&v->v, index, sizeof(_type)); } \
    static INLINE _type _name ## _GetValue \
    (const _name *v, int index) \
    { return *(_type *)MBVectorGetHelperConst(&v->v, index, sizeof(_type)); } \
    static INLINE void _name ## _PutValue \
    (_name *v, int index, _type value) \
    { \
       _type *item = (_type *)MBVectorGetHelper(&v->v, index, sizeof(_type)); \
      *item = value; \
    } \
    static INLINE void _name ## _Copy \
    (_name *dest, const _name *src) \
    { MBVector_Copy(&dest->v, &src->v); } \
    static INLINE void _name ## _Consume \
    (_name *dest, _name *src) \
    { MBVector_Consume(&dest->v, &src->v); } \
    static INLINE void _name ## _EnsureCapacity \
    (_name *v, int capacity) \
    { MBVector_EnsureCapacity(&v->v, capacity); }


DECLARE_MBVECTOR_TYPE(int, MBIntVector);

#endif // MBVECTOR_H_201202060000
