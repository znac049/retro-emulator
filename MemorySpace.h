#ifndef _MEMORYSPACE_H_
#define _MEMORYSPACE_H_

class MemorySpace
{
 private:

 public:
  MemorySpace(int size);
  ~MemorySpace();

  byte get(int addr);
};

#endif
