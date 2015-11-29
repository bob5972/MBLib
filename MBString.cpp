/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
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

