/*
 * MBStrTable.c -- part of MBLib
 *
 * Copyright (c) 2020-2021 Michael Banack <github@banack.net>
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

#include "MBStrTable.h"
#include "MBLock.h"

#define MBSTRTABLE_MAGIC 0x1874919423812155

typedef struct MBStrTable {
    DEBUG_ONLY(
        uint64 magic;
    );

    uint referenceCount;
    MBStrTable *parent;
    bool hasChild;
    CMBCStrVec strings;
} MBStrTable;

struct {
    MBLock lock;
    bool lockInitialized;
} gMBStrTableData;

void MBStrTable_Init()
{
    if (!mb_lock) {
        ASSERT(!gMBStrTableData.lockInitialized);
    } else {
        MBLock_Create(&gMBStrTableData.lock);
        gMBStrTableData.lockInitialized = TRUE;
    }
}

void MBStrTable_Exit()
{
    if (!mb_lock) {
        ASSERT(!gMBStrTableData.lockInitialized);
    } else {
        ASSERT(gMBStrTableData.lockInitialized);
        MBLock_Destroy(&gMBStrTableData.lock);
        gMBStrTableData.lockInitialized = FALSE;
    }
}

static void MBStrTableLock()
{
    if (mb_lock) {
        ASSERT(gMBStrTableData.lockInitialized);
        MBLock_Lock(&gMBStrTableData.lock);
    }
}

static void MBStrTableUnlock()
{
    if (mb_lock) {
        ASSERT(gMBStrTableData.lockInitialized);
        MBLock_Unlock(&gMBStrTableData.lock);
    }
}

MBStrTable *MBStrTable_Alloc()
{
    MBStrTable *st;
    st = MBUtil_ZAlloc(sizeof(*st));
    CMBCStrVec_CreateEmpty(&st->strings);
    st->referenceCount = 1;

    DEBUG_ONLY(
        st->magic = ((uintptr_t)st) ^ MBSTRTABLE_MAGIC;
    );

    return st;
}

MBStrTable *MBStrTable_AllocChild(MBStrTable *parent)
{
    if (parent == NULL) {
        return NULL;
    }

    MBStrTable *st = MBStrTable_Alloc();

    st->parent = parent;

    ASSERT(parent->magic == ((uintptr_t)parent ^ MBSTRTABLE_MAGIC));

    MBStrTableLock();
    parent->referenceCount++;
    parent->hasChild = TRUE;
    MBStrTableUnlock();

    return st;
}

void MBStrTable_Free(MBStrTable *st)
{
    uint i;
    bool doFree = FALSE;
    bool freeParent = FALSE;

    if (st == NULL) {
        return;
    }

    DEBUG_ONLY(
        ASSERT(st->magic == ((uintptr_t)st ^ MBSTRTABLE_MAGIC));
        st->magic = 0;
    );

    if (st->hasChild || st->parent != NULL) {
        MBStrTableLock();

        ASSERT(st->referenceCount > 0);
        st->referenceCount--;

        if (st->referenceCount == 0) {
            doFree = TRUE;

            if (st->parent != NULL) {
                ASSERT(st->parent->referenceCount > 0);
                st->parent->referenceCount--;

                if (st->parent->referenceCount == 0) {
                    freeParent = TRUE;
                }
            }
        }
        MBStrTableUnlock();
    } else {
        ASSERT(st->parent == NULL);
        ASSERT(!st->hasChild);

        ASSERT(st->referenceCount == 1);
        st->referenceCount--;
    }

    if (freeParent) {
        /*
         * We now own the parent's threading, so we can do this
         * unlocked.
         */
        MBStrTable_Free(st->parent);
        st->parent = NULL;
    }

    if (doFree) {
        for (i = 0; i < CMBCStrVec_Size(&st->strings); i++) {
            const char *cstr;
            cstr = CMBCStrVec_GetValue(&st->strings, i);
            if (cstr != NULL) {
                free((char *)cstr);
            }
        }

        CMBCStrVec_Destroy(&st->strings);
        free(st);
    } else {
        /*
         * Do nothing.  Let the children free this.
         */
    }
}

const char *MBStrTable_AddCopy(MBStrTable *st, const char *cstr)
{
    ASSERT(st != NULL);
    ASSERT(cstr != NULL);

    /*
     * XXX: Could use hashes to check if the string was already here?
     */
    const char *newCStr = strdup(cstr);
    MBStrTable_AddFree(st, newCStr);
    return newCStr;
}

/*
 * Adds the provided string to this table.  It will free it when
 * the table is freed.
 */
void MBStrTable_AddFree(MBStrTable *st, const char *cstr)
{
    ASSERT(st != NULL);
    ASSERT(cstr != NULL);

    /*
     * XXX: Could use hashes to check if the string was already here?
     */
    CMBCStrVec_AppendValue(&st->strings, cstr);
}