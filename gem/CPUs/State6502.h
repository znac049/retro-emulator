#ifndef _CPUSTATE_H_
#define _CPUSTATE_H_

#include <ctype.h>

#include "../gem.h"
#include "CPUState.h"

class MemoryMap;

class State6502 public CPUState
{
 private:
  static const char *_instructionNames[];
  static const int _instructionSizes[];
  static const int _instructionCycles[];
  static const int _addressingModes[];

  MemoryMap *memory;

 public:
  enum instructionMode {
    MODE_NUL=0, MODE_IMM, MODE_ABS, MODE_IND, 
    MODE_REL, MODE_INX, MODE_INY, MODE_ZPG,
    MODE_ABX, MODE_ABY, MODE_ZPX, MODE_ZPY,
    MODE_IMP, MODE_ACC
  };

  enum statusMask {
    P_CARRY=0x01, P_ZERO=0x02, P_IRQ_DISABLE=0x04, P_DECIMAL=0x08,
    P_BREAK=0x10, P_OVERFLOW=0x40, P_NEGATIVE=0x80
  };

  // 6502 Registers
  byte a;
  byte x;
  byte y;

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

 public:
  State6502(MemoryMap *mem);
  ~State6502();

  void reset();
  int load(int addr);

  byte getStatusFlag();
  void getStatusFlagAsString(char *str, int len);
  void disassembleOp(char *str, int len);
};

#endif
