/*
 * MBString.c -- part of MBLib
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

#include "MBString.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"

/*
 * Ensure the string has capacity for the specified length.
 * (Length does not need to include the terminating NUL).
 * Can only ever enlarge the buffer.
 *
 * The logical length of the underlying string is not changed.
 */
void MBString_EnsureCapacity(MBString *str, int cap)
{
    int myCapacity = str->capacity;
    int myLength = str->length;
    int capacity = cap + 1;

    ASSERT(myCapacity > 0);
    ASSERT(myCapacity >= myLength + 1);
    ASSERT(capacity > 0);
    ASSERT(MBStringIsNulTerminated(str));

    if (myCapacity < capacity) {
        char *temp = str->chars;
        while (myCapacity < capacity) {
            ASSERT(myCapacity > 0);
            myCapacity *= 2;
        }

        ASSERT(myCapacity >= myLength + 1);

        str->chars = malloc(myCapacity * sizeof(str->chars[0]));
        ASSERT(str->chars != NULL);
        str->capacity = myCapacity;
        ASSERT(temp[myLength] == '\0');
        memcpy(str->chars, temp, myLength + 1);

        ASSERT(MBStringIsNulTerminated(str));

        free(temp);
    }

    ASSERT(str->capacity >= cap + 1);
}

int MBString_CountChar(const MBString *str, char c)
{
    int length = str->length;
    int x;
    int count = 0;

    ASSERT(MBStringIsNulTerminated(str));

    for (x = 0; x < length; x++) {
        if (str->chars[x] == c) {
            count++;
        }
    }
    return count;
}

int MBString_FindChar(const MBString *str, char c)
{
    int length = str->length;
    int x;

    ASSERT(MBStringIsNulTerminated(str));

    for (x = 0; x < length; x++) {
        if (str->chars[x] == c) {
            return x;
        }
    }
    return -1;
}

int MBString_FindStr(const MBString *str, const MBString *substr)
 {
    int x = 0;
    int myLength = str->length;
    int subLen = substr->length;

    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(MBStringIsNulTerminated(substr));

    if (myLength == 0) {
        return -1;
    }

    if (substr->length == 0) {
        return 0;
    }

    x = 0;
    while (x + subLen - 1 < myLength) {
        const char *start = &str->chars[x];

        if (memcmp(start, substr->chars, subLen) == 0) {
            return x;
        }

        x++;
    }

    return -1;
}

bool MBString_StartsWith(const MBString *str, const MBString *prefix)
{
    int myLength = str->length;
    int prefixLen = prefix->length;

    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(MBStringIsNulTerminated(prefix));

    if (prefixLen > myLength) {
        return FALSE;
    }

    if (prefixLen == 0) {
        return TRUE;
    }

    return memcmp(&str->chars[0], &prefix->chars[0], prefixLen) == 0;
}


bool MBString_StartsWithCStr(const MBString *str, const char *prefix)
{
    int myLength = str->length;
    int prefixLen = strnlen(prefix, myLength + 1);

    ASSERT(MBStringIsNulTerminated(str));

    if (prefixLen > myLength) {
        return FALSE;
    }

    if (prefixLen == 0) {
        return TRUE;
    }

    return memcmp(&str->chars[0], prefix, prefixLen) == 0;
}


bool MBString_IsPrefixOfCStr(const MBString *prefix, const char *str)
{
    int prefixLen = prefix->length;
    int strLength = strnlen(str, prefixLen + 1);

    ASSERT(MBStringIsNulTerminated(prefix));

    if (prefixLen > strLength) {
        return FALSE;
    }

    if (prefixLen == 0) {
        return TRUE;
    }

    return memcmp(&prefix->chars[0], str, prefixLen) == 0;
}


/*
 * Copy len chars from str starting at start.
 */
void MBString_CopySubstr(MBString *dest, const MBString *str,
                         int start, int len)
{
    int x;

    ASSERT(start + len <= str->length);
    ASSERT(start >= 0);
    ASSERT(str);
    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(MBStringIsNulTerminated(dest));

    MBString_MakeEmpty(dest);

    for (x = 0; x < len; x++) {
        MBString_AppendChar(dest, str->chars[x + start]);
    }

    ASSERT(dest->length == len);
    ASSERT(MBStringIsNulTerminated(dest));
}

void MBString_ToUpper(MBString *str)
{
    int myLength = str->length;
    char *myChars = str->chars;
    int x;
    ASSERT(MBStringIsNulTerminated(str));

    for (x = 0; x < myLength; x++) {
        myChars[x] = toupper(myChars[x]);
    }
}

void MBString_ToLower(MBString *str)
{
    int myLength = str->length;
    char *myChars = str->chars;
    int x;
    ASSERT(MBStringIsNulTerminated(str));

    for (x = 0; x < myLength; x++) {
        myChars[x] = tolower(myChars[x]);
    }
}

/**
 * Strip white-space from both ends of the string.
 */
void MBString_StripWS(MBString *str)
{
    int myLength = str->length;
    char *myChars = str->chars;
    uint x;

    ASSERT(MBStringIsNulTerminated(str));

    x = 0;
    while (x < myLength && MBUtil_IsWhitespace(myChars[x])) {
        x++;
    }

    if (x == myLength) {
        MBString_MakeEmpty(str);
        return;
    }

    str->length = myLength - x;
    memmove(myChars, &myChars[x], str->length + 1);
    ASSERT(MBStringIsNulTerminated(str));

    myLength = str->length;
    x = myLength - 1;
    ASSERT(!MBUtil_IsWhitespace(myChars[0]));
    while (x > 0 && MBUtil_IsWhitespace(myChars[x])) {
        x--;
    }
    str->length = x + 1;
    myChars[x + 1] = '\0';
    ASSERT(MBStringIsNulTerminated(str));
}

void MBString_AppendStr(MBString *str, const MBString *suffix)
{
    int myLength = str->length;
    int sufLen = suffix->length;

    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(MBStringIsNulTerminated(suffix));

    MBString_EnsureCapacity(str, myLength + sufLen);

    memcpy(&str->chars[myLength], suffix->chars, sufLen + 1);
    str->length = myLength + sufLen;
    ASSERT(MBStringIsNulTerminated(str));
}

void MBString_PrependChar(MBString *str, char c)
{
    /*
     * Ensure we have room for one extra char, and then move them
     * all down (including the terminating NUL).
     */
    MBString_EnsureCapacity(str, str->length + 1);
    memmove(&str->chars[1], &str->chars[0], str->length + 1);

    str->length++;
    str->chars[0] = c;
    ASSERT(MBStringIsNulTerminated(str));
}

void MBString_PrependStr(MBString *str, const MBString *prefix)
{
    ASSERT(MBStringIsNulTerminated(str));
    ASSERT(MBStringIsNulTerminated(prefix));

    MBString_EnsureCapacity(str, str->length + prefix->length);
    memmove(&str->chars[prefix->length], &str->chars[0], str->length + 1);
    memcpy(&str->chars[0], &prefix->chars[0], prefix->length);
    str->length += prefix->length;
    ASSERT(MBStringIsNulTerminated(str));
}


void MBString_PrependCStr(MBString *str, const char *prefix)
{
    uint32 prefixLen = strlen(prefix);

    ASSERT(MBStringIsNulTerminated(str));

    MBString_EnsureCapacity(str, str->length + prefixLen);
    memmove(&str->chars[prefixLen], &str->chars[0], str->length + 1);
    memcpy(&str->chars[0], prefix, prefixLen);
    str->length += prefixLen;
    ASSERT(MBStringIsNulTerminated(str));
}

/*
 * Make consumer equal to str, and then leave strData empty.
 *
 * This is basically a way to throw large strings around without copying them.
 *
 * strData will still need to be destroyed.
 */
void MBString_Consume(MBString *consumer, MBString *strData)
{
    ASSERT(MBStringIsNulTerminated(consumer));
    ASSERT(MBStringIsNulTerminated(strData));
    MBString_Destroy(consumer);
    *consumer = *strData;
    MBString_Create(strData);

    ASSERT(MBStringIsNulTerminated(strData));
    ASSERT(MBStringIsNulTerminated(consumer));
}

/*
 * Compare strings lexicographically
 *
 * return > 0 if lhs > rhs
 * return = 0 if lhs == rhs
 * return < 0 if lhs < rhs
 */
int MBString_Compare(const MBString *lhs, const MBString *rhs)
 {
    int maxx;
    int result;
    int lLen = lhs->length;
    int rLen = rhs->length;

    ASSERT(MBStringIsNulTerminated(lhs));
    ASSERT(MBStringIsNulTerminated(rhs));

    if (lLen < rLen) {
        maxx = lLen;
    } else {
        maxx = rLen;
    }

    result = memcmp(lhs->chars, rhs->chars, maxx);
    if (result != 0) {
        return result;
    }

    if (lLen < rLen) {
        return -1;
    } else if (lLen > rLen) {
        return 1;
    }

    return 0;
}

/*
 * Not terribly efficient, but effective.
 */
void MBString_IntToString(MBString *str, int x)
{
    bool negative = FALSE;
    int digit;
    char c;
    int i;

    if (x == 0) {
        return MBString_CopyCStr(str, "0");
    }

    MBString_MakeEmpty(str);

    negative = FALSE;
    if (x < 0) {
        negative = TRUE;
        x = -x;
    }

    while (x > 0) {
        digit = x % 10;
        c = '0' + digit;

        MBString_AppendChar(str, c);
        x -= digit;
        x /= 10;
    }

    int size = MBString_Length(str);
    for (i = 0; i < size / 2; i++) {
        c = MBString_GetChar(str, i);
        MBString_SetChar(str, i, MBString_GetChar(str, size - i - 1));
        MBString_SetChar(str, size - i - 1, c);
    }

    if (negative) {
        MBString_PrependChar(str, '-');
    }

    ASSERT(MBStringIsNulTerminated(str));
}
