#ifndef _CPUSTATE_H_
#define _CPUSTATE_H_

#include <ctype.h>

#include "../gem.h"

class MemoryMap;

class CPUState {
 private:
  static const char *_instructionNames[];
  static const int _instructionSizes[];
  static const int _instructionCycles[];
  static const int _addressingModes[];

  MemoryMap *memory;

 protected:
  const char **instructionNames;
  const int *instructionSizes;
  const int *instructionCycles;
  const int *addressingModes;

 public:
  enum instructionMode {MODE_NONE=0};

  int sp;
  int pc;

  /* Instruction Register */
  byte ir;
  byte args[2];

  int lastPc;

  int instSize;
  bool opTrap;

  long stepCounter;

  bool running;

 protected:
  int argsw() { return ((args[1])<<8) | args[0]; }

 public:
  CPUState(MemoryMap *mem);
  ~CPUState();

  virtual void reset();
  virtual int load(int addr);

  virtual byte getStatusFlag();
  virtual void getStatusFlagAsString(char *str, int len);
  virtual void disassembleOp(char *str, int len);

  virtual const char *getInstructionName();
  virtual int getAddressingMode();
  virtual int getInstructionSize();
  virtual int getInstructionCycles();
};

#endif
