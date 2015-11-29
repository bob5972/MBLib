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



#ifndef Dumper_HPP_201112231514
#define Dumper_HPP_201112231514

#include "mbtypes.h"
#include "mbutil.h"
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
            Util_Zero(&myOup, sizeof(myOup));

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
