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
