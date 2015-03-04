#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "MemoryMap.h"

class Processor {
 protected:
  MemoryMap *memory;
  bool running;

 public:
  Processor(MemoryMap *mem);
  ~Processor();

  virtual void reset();
  virtual void step();
  virtual void run();

  virtual MemoryMap *getMemory();

  virtual int disassemble(int addr,char *str,int len);
};

#endif
