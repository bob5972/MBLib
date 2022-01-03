/*
 * MBRegistry.c -- part of MBLib
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "MBRegistry.h"
#include "MBVector.h"
#include "MBString.h"
#include "MBStrTable.h"

#define MBREGISTRY_MAGIC 0x8255349905402963

typedef struct MBRegistry {
    DEBUG_ONLY(
        uint64 magic;
    );
    CMBVector data;
    MBStrTable *backingTable;
} MBRegistry;


typedef struct MBRegistryNode {
    const char *key;
    const char *value;
} MBRegistryNode;

static void MBRegistryAddToTable(MBRegistry *mreg, char *s);
static const char *MBRegistryDupToTable(MBRegistry *mreg, const char *s);


MBRegistry *MBRegistry_Alloc()
{
    MBRegistry *mreg = malloc(sizeof(*mreg));
    ASSERT(mreg != NULL);
    DEBUG_ONLY(
        mreg->magic = ((uintptr_t)mreg) ^ MBREGISTRY_MAGIC;
    );
    CMBVector_CreateEmpty(&mreg->data, sizeof(MBRegistryNode));
    mreg->backingTable = MBStrTable_Alloc();
    return mreg;
}

MBRegistry *MBRegistry_AllocCopy(MBRegistry *toCopy)
{
    MBRegistry *mreg = MBRegistry_Alloc();

    if (toCopy == NULL) {
        return mreg;
    }

    ASSERT(mreg->magic == ((uintptr_t)mreg ^ MBREGISTRY_MAGIC));
    ASSERT(toCopy->magic == ((uintptr_t)toCopy ^ MBREGISTRY_MAGIC));

    CMBVector_Copy(&mreg->data, &toCopy->data);
    mreg->backingTable = MBStrTable_AllocChild(toCopy->backingTable);
    return mreg;
}

void MBRegistry_Free(MBRegistry *mreg)
{
    if (mreg == NULL) {
        return;
    }

    DEBUG_ONLY(
        ASSERT(mreg->magic == ((uintptr_t)mreg ^ MBREGISTRY_MAGIC));
        mreg->magic = 0;
    );

    CMBVector_Destroy(&mreg->data);
    MBStrTable_Free(mreg->backingTable);
    free(mreg);
}

bool MBRegistry_ContainsKey(MBRegistry *mreg, const char *key)
{
    ASSERT(mreg != NULL);
    for (uint32 i = 0; i < CMBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = CMBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

const char *MBRegistry_Get(MBRegistry *mreg, const char *key)
{
    ASSERT(mreg != NULL);
    for (uint32 i = 0; i < CMBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = CMBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            return n->value;
        }
    }

    return NULL;
}

static void MBRegistryPutHelper(MBRegistry *mreg,
                                const char *key, bool constKey,
                                const char *value, bool constValue)
{
    MBRegistryNode *n;
    ASSERT(mreg != NULL);

    for (uint32 i = 0; i < CMBVector_Size(&mreg->data); i++) {
        n = CMBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            n->value = value;
            return;
        }
    }

    CMBVector_Grow(&mreg->data);
    n = CMBVector_GetLastPtr(&mreg->data);
    n->key = key;
    n->value = value;
}

/*
 * Assumes the supplied strings are constants, and the caller won't
 * change them.
 */
void MBRegistry_PutConst(MBRegistry *mreg, const char *key, const char *value)
{
    MBRegistryPutHelper(mreg, key, TRUE, value, TRUE);
}

/*
 * Copies the supplied strings, so they need not be valid after the call.
 */
void MBRegistry_PutCopy(MBRegistry *mreg, const char *key, const char *value)
{
    const char *newKey = MBRegistryDupToTable(mreg, key);
    const char *newValue = MBRegistryDupToTable(mreg, value);

    MBRegistryPutHelper(mreg, newKey, FALSE, newValue, FALSE);
}

const char *MBRegistry_Remove(MBRegistry *mreg, const char *key)
{
    MBRegistryNode *n;
    const char *oldValue = NULL;
    ASSERT(mreg != NULL);

    for (uint32 i = 0; i < CMBVector_Size(&mreg->data); i++) {
        n = CMBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            MBRegistryNode *last = CMBVector_GetLastPtr(&mreg->data);
            oldValue = n->value;

            *n = *last;
            CMBVector_Shrink(&mreg->data);
            return oldValue;
        }
    }

    return NULL;
}


void MBRegistry_MakeEmpty(MBRegistry *mreg)
{
    ASSERT(mreg != NULL);
    CMBVector_MakeEmpty(&mreg->data);
}

static void MBRegistryAddToTable(MBRegistry *mreg, char *s)
{
    MBStrTable_AddFree(mreg->backingTable, s);
}

static const char *MBRegistryDupToTable(MBRegistry *mreg, const char *s)
{
    return MBStrTable_AddCopy(mreg->backingTable, s);
}

static void
MBRegistryLoad(MBRegistry *mreg, const char *filename,
               bool subset)
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    uint read;
    MBString key;
    MBString value;
    MBString mbline;

    ASSERT(mreg != NULL);

    MBString_Create(&key);
    MBString_Create(&value);


    file = fopen(filename, "r");
    if (file == NULL) {
        PANIC("Failed to open file: %s\n", filename);
    }

    read = getline(&line, &len, file);

    if (read == -1) {
        PANIC("File is empty: file=%s\n", filename);
    }

    MBString_CreateFromCStr(&mbline, line);
    MBString_StripWS(&mbline);
    if (!MBString_StartsWithCStr(&mbline, "MReg::")) {
        PANIC("File is missing MReg prefix: file=%s\n", filename);
    }

    if (!MBString_EndsWithCStr(&mbline, "::Version=5")) {
        if (MBString_FindCStr(&mbline, "::Version=") == -1) {
            PANIC("File is missing MReg version\n");
        }
        PANIC("Bad MReg Version: file=%s, prefix=%s\n", filename,
              MBString_GetCStr(&mbline));
    }

    MBString_Destroy(&mbline);

    while ((read = getline(&line, &len, file)) != -1) {
        char *d = strstr(line, "=");
        VERIFY(d != NULL);
        VERIFY(strstr(d + 1, "=") == NULL);
        *d = '\0';

        MBString_CopyCStr(&key, line);
        MBString_CopyCStr(&value, d + 1);

        MBString_StripWS(&key);
        MBString_StripWS(&value);

        // This doesn't properly handle quoted entries.
        VERIFY(MBString_GetChar(&key, 0) != '"');
        VERIFY(MBString_GetChar(&key, 0) != '\'');

        char *ckey = MBString_DupCStr(&key);
        char *cvalue = MBString_DupCStr(&value);

        MBRegistryAddToTable(mreg, ckey);
        MBRegistryAddToTable(mreg, cvalue);

        if (subset) {
            ASSERT(MBRegistry_ContainsKey(mreg, ckey));
        }

        MBRegistry_PutConst(mreg, ckey, cvalue);
    }

    MBString_Destroy(&key);
    MBString_Destroy(&value);

    free(line);
    fclose(file);
}

void MBRegistry_Load(MBRegistry *mreg, const char *filename)
{
    MBRegistryLoad(mreg, filename, FALSE);
}

void MBRegistry_LoadSubset(MBRegistry *mreg, const char *filename)
{
    MBRegistryLoad(mreg, filename, TRUE);
}

void
MBRegistry_Save(MBRegistry *mreg, const char *filename)
{
    FILE *file;

    ASSERT(mreg != NULL);

    file = fopen(filename, "w");
    VERIFY(file != NULL);

    fprintf(file, "MReg::MBLib::Version=5\n");

    for (uint32 i = 0; i < CMBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = CMBVector_GetPtr(&mreg->data, i);

        if (strstr(n->key, "\"") != NULL) {
            ASSERT(strstr(n->key, "'") == NULL);
            fprintf(file, "'%s' = ", n->key);
        } else if (strstr(n->key, " ") != NULL ||
                   strstr(n->key, "=") != NULL) {
            fprintf(file, "\"%s\" = ", n->key);
        } else {
            fprintf(file, "%s = ", n->key);
        }

        if (strstr(n->value, "\"") != NULL) {
            ASSERT(strstr(n->value, "'") == NULL);
            fprintf(file, "'%s'\n", n->value);
        } else if (strstr(n->value, " ") != NULL ||
                   strstr(n->value, "=") != NULL) {
            fprintf(file, "\"%s\"\n", n->value);
        } else {
            fprintf(file, "%s\n", n->value);
        }
    }

    fclose(file);
}

void MBRegistry_DebugDump(MBRegistry *mreg)
{
    ASSERT(mreg != NULL);
    for (uint32 i = 0; i < CMBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = CMBVector_GetPtr(&mreg->data, i);
        DebugPrint("\t%s => %s\n", n->key, n->value);
    }
}

void MBRegistry_PutAll(MBRegistry *dest, MBRegistry *src, const char *prefix)
{
    bool usePrefix;
    MBString key;

    if (prefix == NULL || strcmp(prefix, "") == 0) {
        usePrefix = FALSE;
    } else {
        usePrefix = TRUE;
        MBString_Create(&key);
    }

    for (uint32 i = 0; i < CMBVector_Size(&src->data); i++) {
        MBRegistryNode *n = CMBVector_GetPtr(&src->data, i);

        if (usePrefix) {
            MBString_CopyCStr(&key, prefix);
            MBString_AppendCStr(&key, n->key);
            MBRegistry_PutCopy(dest, MBString_GetCStr(&key), n->value);
        } else {
            MBRegistry_PutCopy(dest, n->key, n->value);
        }
    }

    if (usePrefix) {
        MBString_Destroy(&key);
    }
}

void MBRegistry_SplitOnPrefix(MBRegistry *dest, MBRegistry *src,
                              const char *prefix, bool keepPrefix)
{
    MBString key;
    MBString prefixStr;
    uint32 prefixLength;

    ASSERT(prefix != NULL);
    MBString_Create(&key);
    MBString_Create(&prefixStr);
    MBString_CopyCStr(&prefixStr, prefix);
    prefixLength = MBString_Length(&prefixStr);

    for (uint32 i = 0; i < CMBVector_Size(&src->data); i++) {
        MBRegistryNode *n = CMBVector_GetPtr(&src->data, i);

        if (MBString_IsPrefixOfCStr(&prefixStr, n->key)) {
            uint32 keyLength;
            MBString_MakeEmpty(&key);
            MBString_AppendCStr(&key, n->key);
            keyLength = MBString_Length(&key);
            MBString_Truncate(&key, prefixLength, keyLength - prefixLength);
            MBRegistry_PutCopy(dest, MBString_GetCStr(&key), n->value);
        }
    }

    MBString_Destroy(&key);
    MBString_Destroy(&prefixStr);
}

int MBRegistry_GetIntD(MBRegistry *mreg, const char *key, int defValue)
{
    int64 val = MBRegistry_GetInt64D(mreg, key, defValue);

    ASSERT(val <= MAX_INT32);
    ASSERT(val >= MIN_INT32);

    return val;
}

int64 MBRegistry_GetInt64D(MBRegistry *mreg, const char *key, int64 defValue)
{
    const char *str = MBRegistry_GetCStr(mreg, key);
    if (str == NULL) {
        return defValue;
    }

    //XXX ASSERT it's a number ?
    int base = 0;
    int x = 0;
    if (str[0] == '-' || str[0] == '+') {
        x++;
    }
    if (str[x] == '0') {
        if (str[x+1] != 'x' && str[x+1] != 'X') {
            // Avoid using octal consts... nobody wants those.
            base = 10;
        }
    }

    ASSERT(sizeof(long long) == sizeof(int64));
    return strtoll(str, NULL, base);
}

uint MBRegistry_GetUintD(MBRegistry *mreg, const char *key, uint defValue)
{
    uint64 val = MBRegistry_GetUint64D(mreg, key, defValue);

    ASSERT(val <= MAX_UINT);
    ASSERT(val >= MIN_UINT);

    return val;
}

uint64 MBRegistry_GetUint64D(MBRegistry *mreg, const char *key, uint64 defValue)
{
    const char *str = MBRegistry_GetCStr(mreg, key);
    if (str == NULL) {
        return defValue;
    }

    //XXX ASSERT it's a number ?
    int base = 0;
    int x = 0;
    if (str[0] == '-' || str[0] == '+') {
        x++;
    }
    if (str[x] == '0') {
        if (str[x+1] != 'x' && str[x+1] != 'X') {
            // Avoid using octal consts... nobody wants those.
            base = 10;
        }
    }

    ASSERT(sizeof(unsigned long long) == sizeof(uint64));
    return strtoull(str, NULL, base);
}

bool MBRegistry_GetBoolD(MBRegistry *mreg, const char *key, bool defValue)
{
    const char *str = MBRegistry_GetCStr(mreg, key);
    if (str == NULL) {
        return defValue;
    }

    if (strcmp(str, "TRUE") == 0 ||
        strcmp(str, "true") == 0 ||
        strcmp(str, "1") == 0) {
        return TRUE;
    }

    if (strcmp(str, "FALSE") == 0 ||
        strcmp(str, "false") == 0 ||
        strcmp(str, "0") == 0) {
        return FALSE;
    }

    PANIC("MBRegistry key is not a bool (key=%s, value=%s)\n", key, str);
}

float MBRegistry_GetFloatD(MBRegistry *mreg,
                           const char *key, float defValue)
{
    const char *str = MBRegistry_GetCStr(mreg, key);
    if (str == NULL) {
        return defValue;
    }

    return strtof(str, NULL);
}

const char *
MBRegistry_GetCStrD(MBRegistry *mreg, const char *key,
                    const char *defValue)
{
    const char *str = MBRegistry_Get(mreg, key);
    if (str == NULL) {
        return defValue;
    }
    return str;
}

uint
MBRegistry_NumEntries(const MBRegistry *mreg)
{
    return CMBVector_Size(&mreg->data);
}

const char *
MBRegistry_GetKeyAt(MBRegistry *mreg, uint i)
{
    MBRegistryNode *n = CMBVector_GetPtr(&mreg->data, i);
    return n->key;
}

const char *
MBRegistry_GetValueAt(MBRegistry *mreg, uint i)
{
    MBRegistryNode *n = CMBVector_GetPtr(&mreg->data, i);
    return n->value;
}
