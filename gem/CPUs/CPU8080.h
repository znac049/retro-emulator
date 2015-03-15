#ifndef _i8080_H_
#define _i8080_H_

#include "gem.h"
#include "Processor.h"
#include "MemoryMap.h"
#include "i8080State.h"
#include "i8080Instructions.h"

class i8080 : public Processor {
 public:
  i8080(MemoryMap *mem);
  ~i8080();

  void reset();
  void step();
  void run();
  void summary();

  i8080State *getState();
  MemoryMap *getMemory();
  int disassemble(int addr,char *str, int len);
};

#endif
