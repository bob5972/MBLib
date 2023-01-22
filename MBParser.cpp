/*
 * Parser.cpp -- part of MBLib
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
#include "MBParser.hpp"

char ParserSocketReadChar(void *clientData)
{
    MBSocket *sock = (MBSocket *)clientData;

    ASSERT(sock != NULL);
    return sock->readChar();
}

// Reads the next integer, base 10
int MBParser::readInt()
{
    int oup = 0;

    int state = 0;
    bool isNegative = false;

    fillChar();

    while (hasNextChar) {
        switch (state) {
        case 0: // bad char
            while (nextChar != '-' && !MBUtil_IsDigit(nextChar)) {
                if ((MBUtil_IsWhitespace(nextChar) && shouldEatWhitespace) ||
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
                ASSERT(MBUtil_IsDigit(nextChar));
                state = 2;
            }
            break;
        case 1: // found '-'
            ASSERT(nextChar == '-');
            if (!fillChar()) {
                PANIC("No digits found.\n");
            }

            if (!MBUtil_IsDigit(nextChar)) {
                state = 0;
            } else {
                state = 2;
                isNegative = true;
            }
            break;
        case 2: // found digit
            while (hasNextChar && MBUtil_IsDigit(nextChar)) {
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

void MBParser::eatWhitespace()
{
    fillChar();
    while (hasNextChar && MBUtil_IsWhitespace(nextChar)) {
        fillChar();
    }

    if (hasNextChar) {
        unreadChar();
    }
}


// ie read 12,  34, 23,3
void MBParser::readIntArray(int *a, int size)
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

char MBParser::readChar()
{
    fillChar();

    if (!hasNextChar && shouldPanicOnError) {
        PANIC("Premature end of stream reached.");
    }

    return nextChar;
}

char MBParser::readNonWhitespaceChar()
{
    eatWhitespace();

    fillChar();

    if (!hasNextChar && shouldPanicOnError) {
        PANIC("Premature end of stream reached.\n");
    }

    ASSERT(!hasNextChar || !MBUtil_IsWhitespace(nextChar));
    return nextChar;
}


MBString MBParser::readWord()
{
    MBString oup;
    eatWhitespace();

    fillChar();

    while (hasNextChar && !MBUtil_IsWhitespace(nextChar)) {
        oup.append(nextChar);
        fillChar();
    }

    if (hasNextChar) {
        unreadChar();
    }

    return oup;
}

// reads num words
void MBParser::readWords(MBString *w, int num)
{
    for (int x = 0; x < num; x++) {
        w[x] = readWord();
    }

}

MBString MBParser::readLine()
{
    MBString oup;

    fillChar();
    while (nextChar != '\n') {
        oup.append(nextChar);
        fillChar();
    }

    return oup;
}

double MBParser::readDouble()
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

        if (hasNextChar && MBUtil_IsDigit(nextChar)) {
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


/*
 *  Put a new char in nextChar.
 *  Return true iff we got a char
 *
 *
 *  The contents of nextChar should be invalid by the time any public
 *  read method returns.  Anything that a public method wants to leave
 *  "unread" should be pushed back onto the input buffer.
 *
 *  In other words, each method should assume it needs to call
 *  fillChar to get a new character when it is called, and
 *  after calling any other read methods.
 */
bool MBParser::fillChar()
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
void MBParser::unreadChar()
{
    ASSERT(hasNextChar);
    buffer.push(nextChar);
    hasNextChar = FALSE;
}
