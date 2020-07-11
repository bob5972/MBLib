/*
 * MBRegistry.c -- part of MBLib
 *
 * Copyright (c) 2020 Michael Banack <github@banack.net>
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

#include "MBRegistry.h"

typedef struct MBRegistryNode {
    const char *key;
    const char *value;
} MBRegistryNode;


void MBRegistry_Create(MBRegistry *mreg)
{
    ASSERT(mreg != NULL);
    MBVector_CreateEmpty(&mreg->data, sizeof(MBRegistryNode));

}
void MBRegistry_CreateCopy(MBRegistry *mreg, MBRegistry *toCopy)
{
    ASSERT(mreg != NULL);
    MBRegistry_Create(mreg);
    MBVector_Copy(&mreg->data, &toCopy->data);
}

void MBRegistry_Destroy(MBRegistry *mreg)
{
    ASSERT(mreg != NULL);
    MBVector_Destroy(&mreg->data);
}

bool MBRegistry_ContainsKey(MBRegistry *mreg, const char *key)
{
    ASSERT(mreg != NULL);
    for (uint32 i = 0; i < MBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = MBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

const char *MBRegistry_Get(MBRegistry *mreg, const char *key)
{
    ASSERT(mreg != NULL);
    for (uint32 i = 0; i < MBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = MBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            return n->value;
        }
    }

    return NULL;
}

void MBRegistry_Put(MBRegistry *mreg, const char *key, const char *value)
{
    MBRegistryNode *n;
    ASSERT(mreg != NULL);

    for (uint32 i = 0; i < MBVector_Size(&mreg->data); i++) {
        n = MBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            n->value = value;
            return;
        }
    }

    MBVector_Grow(&mreg->data);
    n = MBVector_GetLastPtr(&mreg->data);
    n->key = key;
    n->value = value;
}

const char *MBRegistry_Remove(MBRegistry *mreg, const char *key)
{
    MBRegistryNode *n;
    const char *oldValue = NULL;
    ASSERT(mreg != NULL);

    for (uint32 i = 0; i < MBVector_Size(&mreg->data); i++) {
        n = MBVector_GetPtr(&mreg->data, i);
        if (strcmp(n->key, key) == 0) {
            MBRegistryNode *last = MBVector_GetLastPtr(&mreg->data);
            oldValue = n->value;

            *n = *last;
            MBVector_Shrink(&mreg->data);
            return oldValue;
        }
    }

    return NULL;
}

void MBRegistry_DebugDump(MBRegistry *mreg)
{
    ASSERT(mreg != NULL);
    for (uint32 i = 0; i < MBVector_Size(&mreg->data); i++) {
        MBRegistryNode *n = MBVector_GetPtr(&mreg->data, i);
        DebugPrint("\t%s => %s\n", n->key, n->value);
    }
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
