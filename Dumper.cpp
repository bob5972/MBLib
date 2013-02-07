#include "Dumper.hpp"

void DumperSocketWrite(void *clientData, const char *c, int size)
{
	MBSocket *s = (MBSocket *)clientData;
	ASSERT(s != NULL);

	s->write(c, size);
}

void DumperSocketFlush(void *clientData)
{
	//NOT_IMPLEMENTED();
}

