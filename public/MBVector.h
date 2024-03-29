/*
 * MBVector.h -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
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

#ifdef __cplusplus
    extern "C" {
#endif

#include "MBAssert.h"
#include "MBCompare.h"
#include "MBTypes.h"

#define CMBVECTOR_MAGIC 0xD0B0B4A4A87BE62A

typedef struct CMBVector {
    int size;
    int capacity;
    int itemSize;
    int pinCount;
    void *items;

    /*
     * This is only used in debug checks, but sometimes causes
     * incremental build problems if it's actually ifdef'ed.
     */
    DEBUG_ONLY(uint64 magic);
} CMBVector;

void CMBVector_EnsureCapacity(CMBVector *vector, int capacity);

static inline void CMBVector_Create(CMBVector *vector, int itemSize,
                                    int size, int capacity)
{
    ASSERT(itemSize > 0);
    ASSERT(itemSize < MAX_INT32 / 2);
    ASSERT(capacity < MAX_INT32 / 2);
    ASSERT(size >= 0);
    ASSERT(size < MAX_INT32 / 2);

    ASSERT(capacity >= size);

    DEBUG_ONLY(vector->magic = CMBVECTOR_MAGIC);
    vector->size = size;
    vector->capacity = capacity;
    vector->itemSize = itemSize;
    vector->pinCount = 0;

    if (capacity > 0) {
        vector->items = malloc(itemSize * vector->capacity);
    } else {
        vector->items = NULL;
    }
}

static inline void CMBVector_CreateEmpty(CMBVector *vector, int itemSize)
{
    CMBVector_Create(vector, itemSize, 0, 1);
}

static inline void CMBVector_CreateWithSize(CMBVector *vector, int itemSize,
                                            int size)
{
    CMBVector_Create(vector, itemSize, size, size);
}

static inline void CMBVector_Destroy(CMBVector *vector)
{
    ASSERT(vector->pinCount == 0);
    ASSERT(vector->magic == CMBVECTOR_MAGIC);

    DEBUG_ONLY(vector->magic = 0);

    free(vector->items);
    vector->items = NULL;
}

static inline int CMBVector_ItemSize(const CMBVector *vector)
{
    ASSERT(vector != NULL);
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    return vector->itemSize;
}

static inline void CMBVector_Pin(CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    if (mb_debug) {
        vector->pinCount++;
    }
}

static inline void CMBVector_Unpin(CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    if (mb_debug) {
        ASSERT(vector->pinCount > 0);
        vector->pinCount--;
    }
}

static inline int CMBVector_Size(const CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(vector->size >= 0);
    return vector->size;
}

static inline void CMBVector_Resize(CMBVector *vector, int size)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(size >= 0);

    CMBVector_EnsureCapacity(vector, size);
    vector->size = size;
}

static inline void CMBVector_GrowBy(CMBVector *vector, int increment)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(increment >= 0);
    CMBVector_Resize(vector, vector->size + increment);
}

static inline void CMBVector_Grow(CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    CMBVector_GrowBy(vector, 1);
}

static inline void CMBVector_ShrinkBy(CMBVector *vector, int decrement)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(decrement >= 0);
    CMBVector_Resize(vector, vector->size - decrement);
}

static inline void CMBVector_Shrink(CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    CMBVector_ShrinkBy(vector, 1);
}

static inline bool CMBVector_IsEmpty(const CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    return CMBVector_Size(vector) == 0;
}

static inline void CMBVector_MakeEmpty(CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    CMBVector_Resize(vector, 0);
}

static inline void *
CMBVectorGetHelper(CMBVector *vector, int index, int itemSize)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(index >= 0);
    ASSERT(index < vector->size);
    ASSERT(itemSize == vector->itemSize);

    return ((uint8 *)vector->items) + (index * itemSize);
}

static inline const void *
CMBVectorGetHelperConst(const CMBVector *vector, int index, int itemSize)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(index >= 0);
    ASSERT(index < vector->size);
    ASSERT(itemSize == vector->itemSize);

    return ((uint8 *)vector->items) + (index * itemSize);
}

static inline void *CMBVector_GetPtr(CMBVector *vector, int index)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    return CMBVectorGetHelper(vector, index, vector->itemSize);
}

static inline void *CMBVectorGetLastPtrHelper(CMBVector *vector, int itemSize)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    ASSERT(vector->size > 0);
    return CMBVectorGetHelper(vector, vector->size - 1, itemSize);
}

static inline void *CMBVector_GetLastPtr(CMBVector *vector)
{
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    return CMBVectorGetLastPtrHelper(vector, vector->itemSize);
}

static inline void CMBVector_Copy(CMBVector *dest, const CMBVector *src)
{
    ASSERT(src->magic == CMBVECTOR_MAGIC);
    ASSERT(dest->magic == CMBVECTOR_MAGIC);
    ASSERT(dest->itemSize == src->itemSize);

    CMBVector_Resize(dest, 0);
    CMBVector_EnsureCapacity(dest, src->size);
    CMBVector_Resize(dest, src->size);
    memcpy(dest->items, src->items, src->itemSize * src->size);
}


static inline void CMBVector_Consume(CMBVector *dest, CMBVector *src)
{
    ASSERT(src->magic == CMBVECTOR_MAGIC);
    ASSERT(dest->magic == CMBVECTOR_MAGIC);
    ASSERT(dest->itemSize == src->itemSize);

    /*
     * We could technically leave the src pinned, but if
     * you're using Consume with pinned arrays, confirm
     * this is what you were expecting to happen.
     */
    ASSERT(dest->pinCount == 0);
    ASSERT(src->pinCount == 0);

    free(dest->items);
    dest->items = src->items;
    dest->size = src->size;
    dest->capacity = src->capacity;

    /*
     * We just ASSERTed these were both zero, so this shouldn't
     * be necessary, but Valgrind is complaining that this isn't
     * initialized sometimes, and I'm hypothesizing that this
     * might fix it.
     */
    dest->pinCount = src->pinCount;

    CMBVector_CreateEmpty(src, dest->itemSize);
}

static inline void *CMBVector_GetCArray(CMBVector *vector)
{
    /*
     * Consider pinning the array if you're using this function.
     */
    ASSERT(vector->magic == CMBVECTOR_MAGIC);
    return vector->items;
}

static inline void CMBVector_Sort(CMBVector *v, const CMBComparator *comp)
{
    ASSERT(v->magic == CMBVECTOR_MAGIC);
    ASSERT(v != NULL);
    ASSERT(comp != NULL);
    ASSERT(v->itemSize == comp->itemSize);
    MBCompare_Sort(v->items, v->size, v->itemSize, comp->compareFn,
                   comp->cbData);
}


#define DECLARE_CMBVECTOR_TYPE(_type, _name) \
    typedef struct _name { \
        CMBVector v; \
    } _name ; \
    \
    static inline void _name ## _Create \
    (_name *v, int size, int capacity) \
    { CMBVector_Create(&v->v, sizeof(_type), size, capacity); } \
    static inline void _name ## _CreateEmpty \
    (_name *v) \
    { CMBVector_CreateEmpty(&v->v, sizeof(_type)); } \
    static inline void _name ## _CreateWithSize \
    (_name *v, int size) \
    { CMBVector_CreateWithSize(&v->v, sizeof(_type), size); } \
    static inline void _name ## _Destroy \
    (_name *v) \
    { CMBVector_Destroy(&v->v); } \
    static inline void _name ## _Pin \
    (_name *v) \
    { CMBVector_Pin(&v->v); } \
    static inline void _name ## _Unpin \
    (_name *v) \
    { CMBVector_Unpin(&v->v); } \
    static inline int _name ## _Size \
    (const _name *v) \
    { return CMBVector_Size(&v->v); } \
    static inline void _name ## _Resize \
    (_name *v, int size) \
    { CMBVector_Resize(&v->v, size); } \
    static inline void _name ## _GrowBy \
    (_name *v, int increment) \
    { CMBVector_GrowBy(&v->v, increment); } \
    static inline void _name ## _Grow \
    (_name *v) \
    { CMBVector_Grow(&v->v); } \
    static inline void _name ## _ShrinkBy \
    (_name *v, int decrement) \
    { CMBVector_ShrinkBy(&v->v, decrement); } \
    static inline void _name ## _Shrink \
    (_name *v) \
    { CMBVector_Shrink(&v->v); } \
    static inline bool _name ## _IsEmpty \
    (_name *v) \
    { return CMBVector_IsEmpty(&v->v); } \
    static inline void _name ## _MakeEmpty \
    (_name *v) \
    { CMBVector_MakeEmpty(&v->v); } \
    static inline _type *_name ## _GetCArray \
    (_name *v) \
    { return (_type *)CMBVector_GetCArray(&v->v); } \
    static inline _type *_name ## _GetPtr \
    (_name *v, int index) \
    { return (_type *)CMBVectorGetHelper(&v->v, index, sizeof(_type)); } \
    static inline _type *_name ## _GetLastPtr \
    (_name *v) \
    { return (_type *)CMBVectorGetLastPtrHelper(&v->v, sizeof(_type)); } \
    static inline _type _name ## _GetValue \
    (const _name *v, int index) \
    { return *(_type *)CMBVectorGetHelperConst(&v->v, index, sizeof(_type)); } \
    static inline void _name ## _PutValue \
    (_name *v, int index, _type value) \
    { \
       _type *item = (_type *)CMBVectorGetHelper(&v->v, index, sizeof(_type)); \
      *item = value; \
    } \
    static inline void _name ## _AppendValue \
    (_name *v, _type value) \
    { \
       CMBVector_Grow(&v->v); \
       _type *item = (_type *)CMBVectorGetLastPtrHelper(&v->v, sizeof(_type)); \
       *item = value; \
    } \
    static inline void _name ## _Copy \
    (_name *dest, const _name *src) \
    { CMBVector_Copy(&dest->v, &src->v); } \
    static inline void _name ## _Consume \
    (_name *dest, _name *src) \
    { CMBVector_Consume(&dest->v, &src->v); } \
    static inline void _name ## _EnsureCapacity \
    (_name *v, int capacity) \
    { CMBVector_EnsureCapacity(&v->v, capacity); }


DECLARE_CMBVECTOR_TYPE(int, CMBIntVec);
DECLARE_CMBVECTOR_TYPE(void *, CMBPtrVec);
DECLARE_CMBVECTOR_TYPE(MBVar, CMBVarVec);
DECLARE_CMBVECTOR_TYPE(const char *, CMBCStrVec);

static inline int
CMBIntVec_DecrementValue(CMBIntVec *vec, int index)
{
    int v = CMBIntVec_GetValue(vec, index);
    v--;
    CMBIntVec_PutValue(vec, index, v);
    return v;
}

static inline int
CMBIntVec_IncrementValue(CMBIntVec *vec, int index)
{
    int v = CMBIntVec_GetValue(vec, index);
    v++;
    CMBIntVec_PutValue(vec, index, v);
    return v;
}

#ifdef __cplusplus
    }
#endif

#endif // MBVECTOR_H_201202060000
