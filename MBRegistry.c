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
void MBRegistry_Destroy(MBRegistry *mreg)
{
    ASSERT(mreg != NULL);
    MBVector_Destroy(&mreg->data);
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

int MBRegistry_GetInt(MBRegistry *mreg, const char *key)
{
    const char *str = MBRegistry_GetCStr(mreg, key);
    if (str == NULL) {
        return 0;
    }

    return atoi(str);
}

bool MBRegistry_GetBool(MBRegistry *mreg, const char *key)
{
    const char *str = MBRegistry_GetCStr(mreg, key);
    if (str == NULL) {
        return FALSE;
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

    PANIC("MBRegistry key is not a bool (key=%s, value=%s)\n",
          key, str);
}
