/*
 * MBParser.hpp -- part of MBLib
 *
 * Copyright (c) 2015-2023 Michael Banack <github@banack.net>
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

#ifndef MBPARSER_HPP_201112231325
#define MBPARSER_HPP_201112231325

#include <math.h>

#include "MBTypes.h"
#include "MBAssert.h"
#include "MBUtil.h"
#include "MBString.hpp"
#include "MBVector.hpp"
#include "MBSocket.hpp"

typedef struct CharReaderInterface {
    void *clientData;
    char (*readChar)(void *clientData);
    bool (*isEOF)(void *clientData);
} CharReaderInterface;

char MBParserSocketReadChar(void *clientData);

class MBParser
{
    public:
        MBParser(const CharReaderInterface *funcs) {
            myInp = *funcs;

            ASSERT(myInp.readChar != NULL);

            hasNextChar = FALSE;
            shouldEatGarbage = TRUE;
            shouldEatWhitespace = TRUE;
            shouldPanicOnError = TRUE;
        }

        MBParser(MBSocket &socket)
        {
            MBUtil_Zero(&myInp, sizeof(myInp));
            myInp.clientData = &socket;
            myInp.readChar = MBParserSocketReadChar;

            hasNextChar = FALSE;
            shouldEatGarbage = TRUE;
            shouldEatWhitespace = TRUE;
            shouldPanicOnError = TRUE;
        }

        // Reads the next integer, base 10
        int readInt();

        void eatWhitespace();

        // ie read 12,  34, 23,3
        void readIntArray(int *a, int size);

        char readChar();

        char readNonWhitespaceChar();


        MBString readWord();

        // reads num words
        void readWords(MBString *w, int num);

        MBString readLine();

        double readDouble();

    private:
        CharReaderInterface myInp;

        MBVector<char> buffer;

        bool shouldEatGarbage;
        bool shouldEatWhitespace;
        bool shouldPanicOnError;

        char nextChar;
        bool hasNextChar;

        /*
         *  Put a new char in nextChar.
         *  Return true iff we got a char

         *
         *  The contents of nextChar should be invalid by the time any public
         *  read method returns.  Anything that a public method wants to leave
         *  "unread" should be pushed back onto the input buffer.
         *
         *  In other words, each method should assume it needs to call
         *  fillChar to get a new character when it is called, and
         *  after calling any other read methods.
         */
        bool fillChar();

        /*
         * Move the contents of nextChar back into the input buffer.
         */
        void unreadChar();
};

#endif // MBPARSER_HPP_201112231325

