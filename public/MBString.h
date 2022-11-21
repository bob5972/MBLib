/*
 * MBString.h -- part of MBLib
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

#ifndef MBString_H_201001091354
#define MBString_H_201001091354

#include <string.h>
#include <stdlib.h>

#include "MBAssert.h"
#include "MBUtil.h"

typedef struct MBString {
    int length;
    int capacity;
    char *chars;
} MBString;

typedef MBString MBStringData;

void MBString_EnsureCapacity(MBString *str, int cap);

int MBString_CountChar(const MBString *str, char x);
int MBString_FindChar(const MBString *str, char x);
int MBString_FindStr(const MBString *str, const MBString *substr);
int MBString_FindCStr(const MBString *str, const char *substr);
bool MBString_StartsWith(const MBString *str, const MBString *prefix);
bool MBString_StartsWithCStr(const MBString *str, const char *prefix);
bool MBString_EndsWithCStr(const MBString *str, const char *prefix);
bool MBString_IsPrefixOfCStr(const MBString *prefix, const char *str);

void MBString_CopySubstr(MBString *dest, const MBString *str,
                         int start, int len);

void MBString_ToUpper(MBString *str);
void MBString_ToLower(MBString *str);
void MBString_StripWS(MBString *str);

void MBString_AppendStr(MBString *str, const MBString *suffix);
void MBString_PrependChar(MBString *str, char c);
void MBString_PrependStr(MBString *str, const MBString *prefix);
void MBString_PrependCStr(MBString *str, const char *prefix);
void MBString_Consume(MBString *consumer, MBString *strData);
void MBString_Truncate(MBString *str, uint32 firstChar, uint32 numChars);

int MBString_Compare(const MBString *lhs, const MBString *rhs);

void MBString_IntToString(MBString *str, int x);

static INLINE bool MBStringIsNulTerminated(const MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(str->capacity >= str->length + 1);
    return str->chars[str->length] == '\0';
}

static INLINE char MBString_GetChar(const MBString *str, int x)
{
    ASSERT(x >= 0);
    ASSERT(x < str->length);
    ASSERT(MBStringIsNulTerminated(str));

    return str->chars[x];
}

static INLINE void MBString_SetChar(MBString *str, int x, char c)
{
    ASSERT(x >= 0);
    ASSERT(x < str->length);
    ASSERT(MBStringIsNulTerminated(str));

    str->chars[x] = c;
}

static INLINE int MBString_Length(const MBString *str)
{
    ASSERT(MBStringIsNulTerminated(str));
    return str->length;
}

static INLINE bool MBString_IsEmpty(const MBString *str)
{
    return (MBString_Length(str) == 0);
}

static INLINE void MBString_MakeEmpty(MBString *str)
{
    ASSERT(str->capacity >= 1);
    str->length = 0;
    str->chars[str->length] = '\0';

    ASSERT(MBStringIsNulTerminated(str));
}

/*
 * Return this string as a null-terminated C-String.
 *
 * The returned string must NOT be freed by the caller.
 * The returned string is not guaranteed to be valid if the
 * string is modified.
 */
static INLINE const char *MBString_GetCStr(const MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(MBStringIsNulTerminated(str));
    return str->chars;
}

/*
 * Return this string as a NUL-termianted C-string.
 * The returned string must be freed by the caller.
 */
static INLINE char *MBString_DupCStr(const MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(MBStringIsNulTerminated(str));
    return strdup(str->chars);
}

static INLINE void MBString_CreateWithCapacity(MBString *str, int cap)
{
    ASSERT(str != NULL);
    ASSERT(cap > 0);
    str->length = 0;
    str->capacity = cap;
    str->chars = (char *)malloc(cap * sizeof(str->chars[0]));
    str->chars[0] = '\0';
    ASSERT(MBStringIsNulTerminated(str));
}

static INLINE void MBString_Create(MBString *str)
{
    MBString_CreateWithCapacity(str, 8);
}

static INLINE void MBString_CreateFromCStr(MBString *str, const char *cstr)
{
    uint len = strlen(cstr);
    MBString_CreateWithCapacity(str, len + 1);
    memcpy(str->chars, cstr, len + 1);
    str->length = len;
    ASSERT(MBStringIsNulTerminated(str));
}

static INLINE void MBString_Destroy(MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(MBStringIsNulTerminated(str));

    free(str->chars);
    str->chars = NULL;
}

/*
 * Fill str with len copies of c starting at index pos.
 */
static INLINE void MBString_FillChar(MBString *str, char c, int pos, int len)
{
    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(pos >= 0);
    ASSERT(pos < str->length);
    ASSERT(pos + len <= str->length);

    memset(&str->chars[pos], c, len);
    ASSERT(MBStringIsNulTerminated(str));
}


/*
 * New contents are uninitialized.
 *
 * Note that this could leave unexpected NUL characters in the middle
 * of your string.
 */
static INLINE void MBString_Resize(MBString *str, int size)
{
    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(size > 0);
    MBString_EnsureCapacity(str, size);
    str->length = size;
    str->chars[str->length] = '\0';
    ASSERT(MBStringIsNulTerminated(str));
}

static INLINE void MBString_AppendChar(MBString *str, char c)
{
    int myLength = str->length;
    ASSERT(MBStringIsNulTerminated(str));
    MBString_EnsureCapacity(str, myLength + 1);
    str->chars[myLength] = c;
    str->chars[myLength+1] = '\0';
    str->length = myLength + 1;
    ASSERT(MBStringIsNulTerminated(str));
}

static INLINE void MBString_Copy(MBString *dest, const MBString *src)
{
    int srcLength = src->length;
    ASSERT(dest != src);
    ASSERT(MBStringIsNulTerminated(src));
    ASSERT(MBStringIsNulTerminated(dest));

    /*
     * Empty the destination so there's nothing to copy if we have to
     * resize the buffer.
     */
    MBString_MakeEmpty(dest);
    MBString_EnsureCapacity(dest, srcLength);
    dest->length = srcLength;
    memcpy(dest->chars, src->chars, srcLength + 1);
    ASSERT(MBStringIsNulTerminated(dest));
}

static INLINE void MBString_CopyCStr(MBString *dest, const char *cstr)
{
    int len;

    if (cstr == NULL) {
        MBString_CopyCStr(dest, "");
        return;
    }

    ASSERT(cstr != NULL);
    ASSERT(MBStringIsNulTerminated(dest));

    MBString_MakeEmpty(dest);

    /*
     * We end up walking cstr twice (once to calculate the length, and
     * once to copy the data).
     *
     * For small strings this should be negligible, and for large strings
     * it avoids potentially having to do multiple copies if we have to
     * resize our buffer.
     */
    len = strlen(cstr);
    MBString_EnsureCapacity(dest, len);

    memcpy(dest->chars, cstr, len + 1);
    dest->length = len;
    ASSERT(MBStringIsNulTerminated(dest));
}


static INLINE void MBString_AppendCStr(MBString *dest, const char *cstr)
{
    int len;
    ASSERT(cstr != NULL);
    ASSERT(MBStringIsNulTerminated(dest));

    /*
     * We end up walking cstr twice (once to calculate the length, and
     * once to copy the data).
     *
     * For small strings this should be negligible, and for large strings
     * it avoids potentially having to do multiple copies if we have to
     * resize our buffer.
     */
    len = strlen(cstr);
    MBString_EnsureCapacity(dest, dest->length + len + 1);

    memcpy(dest->chars + dest->length, cstr, len + 1);
    dest->length += len;
    ASSERT(MBStringIsNulTerminated(dest));
}


#endif //MBString_H_201001091354
