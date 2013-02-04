#include "MBString.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"

static INLINE bool MBStringIsNullTerminated(const MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(str->capacity >= str->length + 1);
    return str->chars[str->length] == '\0';
}

/*
 * Ensure the string has capacity for the specified length.
 * (Length does not need to include the terminating NUL).
 * Can only ever enlarge the buffer.
 */
static void MBStringEnsureCapacity(MBString *str, int length)
{
    int myCapacity = str->capacity;
    int myLength = str->length;
    int capacity = length + 1;

    ASSERT(myCapacity > 0);
    ASSERT(myCapacity >= myLength + 1);
    ASSERT(capacity > 0);
    ASSERT(MBStringIsNullTerminated(str));

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

        ASSERT(MBStringIsNullTerminated(str));

        free(temp);
    }

    ASSERT(str->capacity >= length + 1);
}

void MBString_Create(MBString *str)
{
    int size = 8;

    ASSERT(str != NULL);
    str->length = 0;
    str->capacity = size;
    str->chars = malloc(size * sizeof(str->chars[0]));
    str->chars[0] = '\0';
    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_Destroy(MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(MBStringIsNullTerminated(str));

    free(str->chars);
    str->chars = NULL;
}

/*
 * New contents are uninitialized.
 *
 * Note that this could leave unexpected NUL characters in the middle
 * of your string.
 */
void MBString_Resize(MBString *str, int size)
{
    ASSERT(MBStringIsNullTerminated(str));
    MBStringEnsureCapacity(str, size);
    str->length = size;
    str->chars[str->length] = '\0';
    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_MakeEmpty(MBString *str)
{
    ASSERT(str->capacity >= 1);
    str->length = 0;
    str->chars[str->length] = '\0';

    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_Copy(MBString *dest, const MBString *src)
{
    int srcLength = src->length;
    ASSERT(dest != src);
    ASSERT(MBStringIsNullTerminated(src));
    ASSERT(MBStringIsNullTerminated(dest));

    /*
     * Empty the destination so there's nothing to copy if we have to
     * resize the buffer.
     */
    MBString_MakeEmpty(dest);
    MBStringEnsureCapacity(dest, srcLength);
    dest->length = srcLength;
    memcpy(dest->chars, src->chars, srcLength + 1);
    ASSERT(MBStringIsNullTerminated(dest));
}

void MBString_CopyCStr(MBString *dest, const char *cstr)
{
    int len;
    ASSERT(cstr != NULL);
    ASSERT(MBStringIsNullTerminated(dest));

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
    MBStringEnsureCapacity(dest, len);

    memcpy(dest->chars, cstr, len + 1);
    dest->length = len;
    ASSERT(MBStringIsNullTerminated(dest));
}

int MBString_Length(const MBString *str)
{
    ASSERT(MBStringIsNullTerminated(str));
    return str->length;
}

bool MBString_IsEmpty(const MBString *str)
{
    return (MBString_Length(str) == 0);
}

/*
 * Return this string as a null-terminated C-String.
 *
 * The returned string must NOT be freed by the caller.
 * The returned string is not guaranteed to be valid if the
 * string is modified.
 */
const char *MBString_GetCStr(const MBString *str)
{
    ASSERT(str != NULL);
    ASSERT(MBStringIsNullTerminated(str));
    return str->chars;
}

int MBString_FindChar(const MBString *str, char c)
{
    int length = str->length;
    int x;

    ASSERT(MBStringIsNullTerminated(str));

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

    ASSERT(MBStringIsNullTerminated(str));
    ASSERT(MBStringIsNullTerminated(substr));

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
    ASSERT(MBStringIsNullTerminated(str));
    ASSERT(MBStringIsNullTerminated(dest));

    MBString_MakeEmpty(dest);

    for (x = 0; x < len; x++) {
        MBString_AppendChar(dest, str->chars[x + start]);
    }

    ASSERT(dest->length == len);
    ASSERT(MBStringIsNullTerminated(dest));
}

void MBString_ToUpper(MBString *str)
{
    int myLength = str->length;
    char *myChars = str->chars;
    int x;
    ASSERT(MBStringIsNullTerminated(str));

    for (x = 0; x < myLength; x++) {
        myChars[x] = toupper(myChars[x]);
    }
}

void MBString_ToLower(MBString *str)
{
    int myLength = str->length;
    char *myChars = str->chars;
    int x;
    ASSERT(MBStringIsNullTerminated(str));

    for (x = 0; x < myLength; x++) {
        myChars[x] = tolower(myChars[x]);
    }
}

char MBString_GetChar(const MBString *str, int x)
{
    ASSERT(x >= 0);
    ASSERT(x < str->length);
    ASSERT(MBStringIsNullTerminated(str));

    return str->chars[x];
}

void MBString_SetChar(MBString *str, int x, char c)
{
    ASSERT(x >= 0);
    ASSERT(x < str->length);
    ASSERT(MBStringIsNullTerminated(str));

    str->chars[x] = c;
}

/*
 * Fill str with len copies of c starting at index pos.
 */
void MBString_FillChar(MBString *str, char c, int pos, int len)
{
    ASSERT(MBStringIsNullTerminated(str));
    ASSERT(pos >= 0);
    ASSERT(pos < str->length);
    ASSERT(pos + len <= str->length);

    memset(&str->chars[pos], c, len);
    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_AppendChar(MBString *str, char c)
{
    int myLength = str->length;
    ASSERT(MBStringIsNullTerminated(str));
    MBStringEnsureCapacity(str, myLength + 1);
    str->chars[myLength] = c;
    str->chars[myLength+1] = '\0';
    str->length = myLength + 1;
    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_AppendStr(MBString *str, const MBString *suffix)
{
    int myLength = str->length;
    int sufLen = suffix->length;

    ASSERT(MBStringIsNullTerminated(str));
    ASSERT(MBStringIsNullTerminated(suffix));

    MBStringEnsureCapacity(str, myLength + sufLen);

    memcpy(&str->chars[myLength], suffix->chars, sufLen + 1);
    str->length = myLength + sufLen;
    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_PrependChar(MBString *str, char c)
{
    char lastChar;
    char temp;
    int x;

    /*
     * Not terribly efficient...
     */
    MBStringEnsureCapacity(str, str->length + 1);
    str->length++;
    str->chars[str->length] = '\0';

    lastChar = c;
    for (x = 0; x < str->length; x++) {
        temp = str->chars[x];
        str->chars[x] = lastChar;
        lastChar = temp;
    }
    ASSERT(MBStringIsNullTerminated(str));
}

void MBString_PrependStr(MBString *str, const MBString *prefix)
{
    MBString temp;

    ASSERT(MBStringIsNullTerminated(str));
    ASSERT(MBStringIsNullTerminated(prefix));

    /*
     * Not terribly efficient...
     */
    MBString_Create(&temp);
    MBString_Copy(&temp, str);
    MBString_Copy(str, prefix);
    MBString_AppendStr(str, &temp);
    ASSERT(MBStringIsNullTerminated(str));
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
    ASSERT(MBStringIsNullTerminated(consumer));
    ASSERT(MBStringIsNullTerminated(strData));
    MBString_Destroy(consumer);
    *consumer = *strData;
    MBString_Create(strData);

    ASSERT(MBStringIsNullTerminated(strData));
    ASSERT(MBStringIsNullTerminated(consumer));
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

    ASSERT(MBStringIsNullTerminated(lhs));
    ASSERT(MBStringIsNullTerminated(rhs));

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
    bool negative = false;
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

    ASSERT(MBStringIsNullTerminated(str));
}
