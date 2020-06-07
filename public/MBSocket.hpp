/*
 * MBSocket.hpp -- part of MBLib
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

            MBUtil_Zero((char *) &serv_addr, sizeof(serv_addr));
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

