#ifndef _CPUSTATE_H_
#define _CPUSTATE_H_

#include <ctype.h>

#include "MemoryMap.h"

class R6502State {
 private:
  MemoryMap *memory;

 public:
  // 6502 Registers
  byte a;
  byte x;
  byte y;

  byte sp;
  word pc;

  /* Instruction Register */
  byte ir;
  byte args[2];

  word lastPc;

  int instSize;
  bool opTrap;
  bool irqAsserted;
  bool nmiAsserted;

  /* Status Flag Register bits */
  bool carryFlag;
  bool negativeFlag;
  bool zeroFlag;
  bool irqDisableFlag;
  bool decimalModeFlag;
  bool breakFlag;
  bool overflowFlag;

  long stepCounter;

  bool running;

 private:
  word argsw() { return (((word)args[1])<<8) | args[0]; }

 public:
  R6502State(MemoryMap *mem);
  ~R6502State();

  void reset();
  int load(int addr);

  byte getStatusFlag();
  void getStatusFlagAsString(char *str, int len);
  void disassembleOp(char *str, int len);
};

#endif
