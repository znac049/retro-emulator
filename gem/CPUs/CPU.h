#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "../gem.h"

class MemoryMap;

class CPU {
 private:

 protected:
  MemoryMap *memory;
  bool running;
  bool littleEndian;

 public:
  CPU(MemoryMap *mem);
  ~CPU();

  bool isLittleEndian() { return littleEndian; };

  virtual void reset();
  virtual void step();
  virtual void run();

  virtual MemoryMap *getMemory();

  virtual int disassemble(int addr, char *str, int len);
};

#endif
