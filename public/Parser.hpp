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

#ifndef Parser_HPP_201112231325
#define Parser_HPP_201112231325

#include <math.h>

#include "mbtypes.h"
#include "mbassert.h"
#include "mbutil.h"
#include "MBString.hpp"
#include "MBVector.hpp"
#include "MBSocket.hpp"

typedef struct CharReaderInterface {
    void *clientData;
    char (*readChar)(void *clientData);
    bool (*isEOF)(void *clientData);
} CharReaderInterface;

char ParserSocketReadChar(void *clientData);

class Parser
{
    public:
        Parser(const CharReaderInterface *funcs) {
            myInp = *funcs;

            ASSERT(myInp.readChar != NULL);

            hasNextChar = FALSE;
            shouldEatGarbage = TRUE;
            shouldEatWhitespace = TRUE;
            shouldPanicOnError = TRUE;
        }

        Parser(MBSocket &socket)
        {
            Util_Zero(&myInp, sizeof(myInp));
            myInp.clientData = &socket;
            myInp.readChar = ParserSocketReadChar;

            hasNextChar = FALSE;
            shouldEatGarbage = TRUE;
            shouldEatWhitespace = TRUE;
            shouldPanicOnError = TRUE;
        }

        // Reads the next integer, base 10
        int readInt()
        {
            int oup = 0;

            int state = 0;
            bool isNegative = false;

            fillChar();

            while (hasNextChar) {
                switch (state) {
                case 0: // bad char
                    while (nextChar != '-' && !Util_IsDigit(nextChar)) {
                        if ((Util_IsWhitespace(nextChar) && shouldEatWhitespace) ||
                             shouldEatGarbage) {
                            fillChar();
                        } else if (shouldPanicOnError) {
                            PANIC("No digits found.\n");
                        } else {
                            return 0;
                        }
                    }

                    if (nextChar == '-') {
                        state = 1;
                    } else {
                        ASSERT(Util_IsDigit(nextChar));
                        state = 2;
                    }
                    break;
                case 1: // found '-'
                    ASSERT(nextChar == '-');
                    if (!fillChar()) {
                    	PANIC("No digits found.\n");
                	}

                    if (!Util_IsDigit(nextChar)) {
                        state = 0;
                    } else {
                        state = 2;
                        isNegative = true;
                    }
                    break;
                case 2: // found digit
                    while (hasNextChar && Util_IsDigit(nextChar)) {
                        oup *= 10;
                        oup += nextChar - '0';
                        fillChar();
                    }

                    unreadChar();
                    if (isNegative) {
                        oup = -oup;
                    }
                    return oup;
                }
            }

            ASSERT(!hasNextChar);
            if (shouldPanicOnError) {
                PANIC("Premature end of stream reached.\n");
            }

            return 0;
        }

        void eatWhitespace()
        {
            fillChar();
            while (hasNextChar && Util_IsWhitespace(nextChar)) {
                fillChar();
            }

            if (hasNextChar) {
                unreadChar();
            }
        }


        // ie read 12,  34, 23,3
        void readIntArray(int *a, int size)
        {
            for (int x = 0; x < size; x++) {
                a[x] = readInt();
                eatWhitespace();

                fillChar();
                if (hasNextChar && nextChar != ',') {
                    unreadChar();
                }
            }
        }

        char readChar()
        {
            fillChar();

            if (!hasNextChar && shouldPanicOnError) {
                PANIC("Premature end of stream reached.");
            }

            return nextChar;
        }

        char readNonWhitespaceChar()
        {
            eatWhitespace();

            fillChar();

            if (!hasNextChar && shouldPanicOnError) {
                PANIC("Premature end of stream reached.\n");
            }

            ASSERT(!hasNextChar || !Util_IsWhitespace(nextChar));
            return nextChar;
        }


        MBString readWord()
        {
            MBString oup;
            eatWhitespace();

            fillChar();

            while (hasNextChar && !Util_IsWhitespace(nextChar)) {
                oup.append(nextChar);
                fillChar();
            }

            if (hasNextChar) {
                unreadChar();
            }

            return oup;
        }

        // reads num words
        void readWords(MBString *w, int num)
        {
            for (int x = 0; x < num; x++) {
                w[x] = readWord();
            }

        }

        MBString readLine()
        {
            MBString oup;

            fillChar();
            while (nextChar != '\n') {
                oup.append(nextChar);
                fillChar();
            }

            return oup;
        }

        double readDouble()
        {
            double wholeD, fracD;
            double oup;

            int whole = readInt();
            int frac = 0;
            int leadingZeroes = 0;
            int fracDigits;

            fillChar();
            if (hasNextChar && nextChar == '.') {
                fillChar();

                while (hasNextChar && nextChar == '0') {
                    leadingZeroes++;
                    fillChar();
                }

                if (hasNextChar && Util_IsDigit(nextChar)) {
                    unreadChar();
                    frac = readInt();
                }
            }

            wholeD = whole;
            fracD = frac;

            fracDigits = ((int)log10(fracD) + 1);
            fracDigits += leadingZeroes;

            fracD = fracD / pow(10.0, fracDigits);

            oup = wholeD + fracD;
            return oup;
        }

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
        bool fillChar()
        {
            bool gotChar = FALSE;

            if (buffer.size() > 0) {
                nextChar = buffer.pop();
                gotChar = TRUE;
            } else {
                if (myInp.isEOF != NULL) {
                    gotChar = myInp.isEOF(myInp.clientData);
                } else {
                    gotChar = TRUE;
                }

                if (!gotChar) {
                    nextChar = myInp.readChar(myInp.clientData);
                    gotChar = TRUE;
                }
            }

            hasNextChar = gotChar;
            return hasNextChar;
        }

        /*
         * Move the contents of nextChar back into the input buffer.
         */
        void unreadChar()
        {
            ASSERT(hasNextChar);
            buffer.push(nextChar);
            hasNextChar = FALSE;
        }
};

#endif //Parser_HPP_201112231325

