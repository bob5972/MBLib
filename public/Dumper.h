#ifndef Dumper_H_201112231514
#define Dumper_H_201112231514

#include "mbtypes.h"
#include "mbutil.h"
#include "MBString.h"

typedef struct CharWriterInterface {
    void *clientData;
    void (*writeChar)(void *clientData, char c);
    void (*flush)(void *clientData);
} CharWriterInterface;


class Dumper {
    public:
        Dumper(const CharWriterInterface *w)
        {
            ASSERT(w != NULL);
            ASSERT(w->writeChar != NULL);

            myOup = *w;
        }

        void writeInt(int i)
        {
            MBString str;

            str = MBString::toString(i);
            write(str);
            write(" ");
        }

        void write(const MBString &str)
        {
            for (int x = 0; x < str.size(); x++) {
                writeChar(str[x]);
            }
        }

        void writeChar(char c)
        {
            myOup.writeChar(myOup.clientData, c);
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

};

#endif //Dumper_H_201112231514
