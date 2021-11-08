/*
 * Dumper.hpp -- part of MBLib
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

#ifndef Dumper_HPP_201112231514
#define Dumper_HPP_201112231514

#include "MBTypes.h"
#include "MBUtil.h"
#include "MBString.hpp"
#include "MBSocket.hpp"

typedef struct CharWriterInterface {
    void *clientData;
    void (*write)(void *clientData, const char *c, int size);
    void (*flush)(void *clientData);
} CharWriterInterface;

void DumperSocketWrite(void *clientData, const char *c, int size);
void DumperSocketFlush(void *clientData);

class Dumper {
    public:
        Dumper(const CharWriterInterface *w)
        {
            ASSERT(w != NULL);
            ASSERT(w->write != NULL);

            myOup = *w;
        }

        Dumper(MBSocket &s)
        {
            MBUtil_Zero(&myOup, sizeof(myOup));

            myOup.clientData = &s;
            myOup.write = DumperSocketWrite;
            myOup.flush = DumperSocketFlush;

        }

        void writeInt(int i)
        {
            MBString str;

            str = MBString::toString(i);
            write(str);
            write(" ");
        }

        void writeChar(char c)
        {
            write(c);
        }

        void writeWord(const MBString &w)
        {
            write(w);
            write(" ");
        }

        void writeString(const MBString &w)
        {
            write(w);
        }

        void endLine()
        {
            write("\n");
        }

        void writeLine(const MBString &w)
        {
            write(w);
            write("\n");
        }

        void flush()
        {
            if (myOup.flush != NULL) {
                myOup.flush(myOup.clientData);
            }
        }

    private:
        CharWriterInterface myOup;

        void write(const MBString &str)
        {
            myOup.write(myOup.clientData, str.CStr(), str.length());
        }
};

#endif //Dumper_HPP_201112231514
