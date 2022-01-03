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

typedef struct MBStrTable {
    CMBCStrVec strings;
} MBStrTable;

MBStrTable *MBStrTable_Alloc()
{
    MBStrTable *st;
    st = malloc(sizeof(*st));
    CMBCStrVec_Create(&st->strings, 0, 16);
    return st;
}

void MBStrTable_Free(MBStrTable *st)
{
    uint i;

    ASSERT(st != NULL);

    for (i = 0; i < CMBCStrVec_Size(&st->strings); i++) {
        const char *cstr;
        cstr = CMBCStrVec_GetValue(&st->strings, i);
        if (cstr != NULL) {
            free((char *)cstr);
        }
    }

    CMBCStrVec_Destroy(&st->strings);
    free(st);
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