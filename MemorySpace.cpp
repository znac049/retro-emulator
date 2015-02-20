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

void MemorySpace::registerDevice(Device *dev, int addr)
{
}

