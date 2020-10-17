/*
 * MBString.hpp -- part of MBLib
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

#ifndef MBString_HPP_201302062332
#define MBString_HPP_201302062332

#include <string.h>
#include <stdlib.h>

#ifndef __cplusplus
#error Including C++ Header in a C file.
#endif

#include <istream>
#include <ostream>

#define MBString MBStringData
extern "C" {
#include "MBString.h"
}
#undef MBString

class MBString {
    public:
        //constructs an empty string
        MBString() {
            MBString_Create(&data);
        }

        //constructs an empty string, with the specified buffer size
        explicit MBString(int size) {
            MBString_CreateWithCapacity(&data, size);
            MBString_Resize(&data, size);
        }

        //construct a string of length size, filled with fill
        MBString(int size, char fill) {
            MBString_CreateWithCapacity(&data, size);
            MBString_Resize(&data, size);
            MBString_FillChar(&data, fill, 0, size);
        }

        //Construct from C-String
        MBString(const char * c) {
            MBString_Create(&data);
            MBString_CopyCStr(&data, c);
        }

        MBString(const MBString & str) {
            int len = MBString_Length(&str.data);
            MBString_CreateWithCapacity(&data, len);
            MBString_Copy(&data, &str.data);
        }

        MBString(char c) {
            MBString_Create(&data);
            MBString_AppendChar(&data, c);
        }

        ~MBString() {
            MBString_Destroy(&data);
        }

        const MBString& operator =(const MBString & str) {
            if (this != &str) {
                MBString_Copy(&data, &str.data);
            }
            return *this;
        }

        const MBString& operator =(const char* cstr) {
            MBString_CopyCStr(&data, cstr);
            return *this;
        }

        const MBString & operator =(char c) {
            MBString_Resize(&data, 1);
            MBString_SetChar(&data, 0, c);
            return *this;
        }

        int length() const {
            return MBString_Length(&data);
        }

        bool isEmpty() const {
            return MBString_IsEmpty(&data);
        }

        /*
         * Return this string as a null-terminated C-String.
         *
         * The returned string must NOT be freed by the caller.
         * The returned string is not guaranteed to be valid if the
         * string is modified.
         */
        const char * CStr() const {
            return MBString_GetCStr(&data);
        }

        int countChar(char c) const {
            return MBString_CountChar(&data, c);
        }

        int find(char c) const {
            return MBString_FindChar(&data, c);
        }

        int find(const MBString & substr) const {
            return MBString_FindStr(&data, &substr.data);
        }

        /*
         * return len chars starting at pos
         */
        MBString substr(int pos, int len) const {
            MBString oup;

            MBString_CopySubstr(&oup.data, &data, pos, len);
            return oup;
        }

        MBString toUpper() const {
            MBString oup;

            MBString_Copy(&oup.data, &data);
            MBString_ToUpper(&oup.data);
            return oup;
        }

        MBString toLower() const {
            MBString oup;

            MBString_Copy(&oup.data, &data);
            MBString_ToLower(&oup.data);
            return oup;
        }

        MBString stripWS() const {
            MBString oup;

            MBString_Copy(&oup.data, &data);
            MBString_StripWS(&oup.data);
            return oup;
        }

        char getChar(int k) const {
            return MBString_GetChar(&data, k);
        }

        char lastChar() const {
            return MBString_GetChar(&data, MBString_Length(&data) - 1);
        }

        void setChar(int k, char c) {
            MBString_SetChar(&data, k, c);
        }

        void append(const MBString& str) {
            MBString_AppendStr(&data, &str.data);
        }

        void prepend(const MBString &str) {
            MBString_PrependStr(&data, &str.data);
        }

        void consume(MBString &str) {
            MBString_Consume(&data, &str.data);
        }

        const MBString & operator +=(const MBString & str) {
            append(str);
            return *this;
        }

        //Append char
        void append(char c) {
            MBString_AppendChar(&data, c);
        }

        const MBString & operator +=(char c) {
            append(c);
            return *this;
        }

        int compare(const MBString &rhs) const {
            return MBString_Compare(&data, &rhs.data);
        }

        static MBString toString(int x) {
            MBString oup;

            MBString_IntToString(&oup.data, x);
            return oup;
        }

    private:
        MBStringData data;
};


//IO Functions
static INLINE std::ostream& operator << ( std::ostream& os,
                                          const MBString& str )
{
        return os << str.CStr();
}


std::istream& operator >> ( std::istream& is, MBString& str );
std::istream& getline( std::istream& is, MBString& str );

static INLINE bool
MBString_GetLine( std::istream& is, MBString &str)
{
        getline(is, str);
        return !str.isEmpty();
}


// comparison operators:
static INLINE bool operator == ( const MBString & lhs,
                                 const MBString & rhs )
{
        return lhs.compare(rhs) == 0;
}

static INLINE bool operator != ( const MBString & lhs,
                                 const MBString & rhs )
{
        return lhs.compare(rhs) != 0;
}

static INLINE bool operator <  ( const MBString & lhs,
                                 const MBString & rhs )
{
        return lhs.compare(rhs) < 0;
}

static INLINE bool operator <= ( const MBString & lhs,
                                 const MBString & rhs )
{
        return lhs.compare(rhs) <= 0;
}

static INLINE bool operator >  ( const MBString & lhs,
                                 const MBString & rhs )
{
        return lhs.compare(rhs) > 0;
}

static INLINE bool operator >= ( const MBString & lhs,
                                 const MBString & rhs )
{
        return lhs.compare(rhs) >= 0;
}

// concatenation operator +
static INLINE MBString operator + ( const MBString & lhs,
                                    const MBString & rhs )
{
        MBString oup(lhs.length() + rhs.length());
        oup = lhs;
        oup += rhs;
        return oup;
}

static INLINE MBString operator + ( char ch,
                                    const MBString & str )
{
        MBString oup(1+str.length());
        oup = ch;
        oup += str;
        return oup;
}

static INLINE MBString operator + ( const MBString & str,
                                    char ch )
{
        MBString oup (1+str.length());
        oup = str;
        oup += ch;
        return oup;
}

#endif // MBString_HPP_201302062332
