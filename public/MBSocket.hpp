#ifndef MBSocket_HPP_201112211636
#define MBSocket_HPP_201112211636

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "mbassert.h"
#include "mbtypes.h"
#include "MBString.hpp"

class MBSocket
{
    public:
        MBSocket()
        {
            reqSockFD = socket(AF_INET, SOCK_STREAM, 0);

            if (reqSockFD < 0) {
                PANIC("Unable to open socket\n");
            }

            bound = FALSE;
            haveConnection = FALSE;
        }

        ~MBSocket()
        {
            close(reqSockFD);
            if (haveConnection) {
                close(conSockFD);
            }
        }

        void bind(uint16 port)
        {
            sockaddr_in serv_addr;
            int err;

            zero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port = htons(port);

            err = ::bind(reqSockFD, (struct sockaddr *) &serv_addr,
                         sizeof(serv_addr));

            if (err < 0) {
                PANIC("Unable to bind socket\n");
            }

            bound = TRUE;
        }

        void listen()
        {
            socklen_t cliLen;

            sockaddr_in cli_addr;

            ASSERT(bound);
            ::listen(reqSockFD, 1);

            cliLen = sizeof(cli_addr);
            conSockFD = accept(reqSockFD, (struct sockaddr *) &cli_addr,
                               &cliLen);
            if (conSockFD < 0) {
                PANIC("Unable to accept connection\n");
            }
            haveConnection = TRUE;
        }

	    void write(const char *c, int size)
	    {
		    int n;
            n = ::write(conSockFD, c, size);

            if (n < 0) {
                PANIC("Unable to write to socket: n=%d\n", n);
            }
        }

        void write(const MBString& str)
        {
            write(str.CStr(), str.length());
        }

        char readChar()
        {
            char c;
            int n;

            ASSERT(haveConnection);

            n = read(conSockFD, &c, 1);
            if (n < 1) {
                PANIC("Unable to read from socket\n");
            }
            return c;
        }

        MBString readLine()
        {
        	char c;
            MBString oup;

            c = readChar();
            while (c != '\n') {
                oup += c;
                c = readChar();
            }

            return oup;
        }

    private:
        int reqSockFD;
        int conSockFD;

        bool bound;
        bool haveConnection;
};

#endif //MBSocket_HPP_201112211636

