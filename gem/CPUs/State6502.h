#ifndef _STATE6502_H_
#define _STATE6502_H_

#include <ctype.h>

#include "../gem.h"
#include "CPUState.h"

class MemoryMap;

class State6502 : public CPUState
{
  friend class CPU6502;

 private:
  static const char *_instructionNames[];
  static const int _instructionSizes[];
  static const int _instructionCycles[];
  static const int _addressingModes[];

  MemoryMap *memory;

 public:
  static const int MODE_NUL = 1;
  static const int MODE_IMM = 2;
  static const int MODE_ABS = 3;
  static const int MODE_IND = 4;
  static const int MODE_REL = 5;
  static const int MODE_INX = 6;
  static const int MODE_INY = 7;
  static const int MODE_ZPG = 8;
  static const int MODE_ABX = 9;
  static const int MODE_ABY = 10;
  static const int MODE_ZPX = 11;
  static const int MODE_ZPY = 12;
  static const int MODE_IMP = 13;
  static const int MODE_ACC = 14;

  static const int P_CARRY       = 0x01;
  static const int P_ZERO        = 0x02;
  static const int P_IRQ_DISABLE = 0x04;
  static const int P_DECIMAL     = 0x08;
  static const int P_BREAK       = 0x10;
  static const int P_OVERFLOW    = 0x40;
  static const int P_NEGATIVE    = 0x80;

  static const int NMI_VECTOR_L = 0xfffa;
  static const int NMI_VECTOR_H = 0xfffb;

  static const int RST_VECTOR_L = 0xfffc;
  static const int RST_VECTOR_H = 0xfffd;

  static const int IRQ_VECTOR_L = 0xfffe;
  static const int IRQ_VECTOR_H = 0xffff;

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
