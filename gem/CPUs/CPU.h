#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "../gem.h"

class MemoryMap;
class CPUState;

class CPU {
 private:

 protected:
  MemoryMap *memory;
  CPUState *state;
  bool running;
  bool littleEndian;

 public:
  static const int MaxName = 100;

  char *cpuName;

 public:
  CPU(MemoryMap *mem);
  ~CPU();

  bool isLittleEndian() { return littleEndian; };

  virtual void reset();
  virtual void step();
  virtual void run();

  virtual void checkInterrupts();
  virtual void executeInstruction();

  virtual void incrementPC();

  virtual MemoryMap *getMemory();

  virtual int disassemble(int addr, char *str, int len);

  void setName(const char *newName);
  const char *getName();
};

#endif
