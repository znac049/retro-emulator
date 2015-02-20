#ifndef _MEMORYSPACE_H_
#define _MEMORYSPACE_H_

#include "AtariEm.h"

class MemorySpace
{
 private:

 public:
  MemorySpace(int size);
  ~MemorySpace();

  byte peek(int addr);
  void poke(int addr, byte b);

  word peekw(int addr);
  void pokew(int addr, word w);
};

#endif
