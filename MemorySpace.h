#ifndef _MEMORYSPACE_H_
#define _MEMORYSPACE_H_

class MemorySpace
{
 private:

 public:
  MemorySpace(int size);
  ~MemorySpace();

  byte peek(int addr);
  void poke(int addr, byte b);

  void registerDevice(Device *dev, int addr);
};

#endif
