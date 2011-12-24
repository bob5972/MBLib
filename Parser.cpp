#include "Parser.h"

char ParserSocketReadChar(void *clientData)
{
    MBSocket *sock = (MBSocket *)clientData;
    
    ASSERT(sock != NULL);
    return sock->readChar();
}
	

