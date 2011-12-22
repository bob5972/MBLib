#ifndef MBSocket_H_201112211636
#define MBSocket_H_201112211636

class MBSocket {
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

            memset((char *) &serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port = htons(port);

            err = bind(reqSockFD, (struct sockaddr *) &serv_addr,
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
            listen(reqSockFD, 1);

            cli:en = sizeof(cli_addr);
            conSockFD = accept(reqSockFD, (struct sockaddr *) &cli_addr, 
                               &cliLen);
            if (conSockFD < 0) 
                PANIC("Unable to accept connection\n");
            }
            haveConnection = TRUE;
        }

        void write(MBString str)
        {
            int n;
            n = write(conSockFD, str.cstr(), str.size());
            if (n < 0) {
                PANIC("Unable to write to socket\n");
            }
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
            bool error = FALSE;
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

#endif //MBSocket_H_201112211636

