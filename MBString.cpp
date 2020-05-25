/*
 * MBString.cpp -- part of MBLib
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

#include "MBString.hpp"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <istream>
#include <ostream>

#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"

using std::istream;
using std::ostream;

//precondition:  input stream is open for reading
//postcondition: the next string from input stream is has been read
//               and stored in str
istream& operator >> ( istream& is, MBString& str )
{
    char ch;
    str = "";    // empty string, will build one char at-a-time
    is >> ch;    // whitespace skipped, first non-white char in ch

    if (! is.fail()) {
        do
        {
            str += ch;
            is.get(ch);
        } while (! is.fail() && ! isspace(ch));

		// put whitespace back on the stream
        if (isspace(ch)) {
            is.putback(ch);
        }
    }

    return is;
}

//description:   reads a line from input stream is into the string str
//precondition:  input stream is open for reading
//postcondition: chars from input stream is up to '\n' have been read
istream& getline( istream& is, MBString& str )
{

    char ch;
    str = "";     // empty string, will build one char at-a-time

    while (is.get(ch) && ch != '\n') {
        str += ch;
    }

    return is;
}

