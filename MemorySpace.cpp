#include "AtariEm.h"
#include "MemorySpace.h"

MemorySpace::MemorySpace(int size)
{
}

MemorySpace::~MemorySpace()
{
}

byte MemorySpace::peek(int addr)
{
	return 0;
}

void MemorySpace::poke(int addr, byte b)
{
}

word MemorySpace::peekw(int addr)
{
	return ( (((word) peek(addr+1))<<8) | peek(addr) );
}

void MemorySpace::pokew(int addr, word w)
{
	poke(addr,   w & 0xff);
	poke(addr+1, (w>>8) & 0xff);
}
