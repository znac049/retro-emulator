#ifndef _CPU6502_H_
#define _CPU6502_H_

#include "../gem.h"
#include "CPU.h"

#define NMOS_WITH_INDIRECT_JMP_BUG 1
#define NMOS_WITH_ROR_BUG 2

class CPU6502 : public CPU {
 private:
  static const char *_instructionNames[];
  static const int _instructionSizes[];
  static const int _instructionCycles[];
  static const int _addressingModes[];

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

 protected:
  int irAddressMode; // Bits 3-5 of IR:  [ | | |X|X|X| | ]
  int irOpMode;      // Bits 6-7 of IR:  [ | | | | | |X|X]
  int effectiveAddress;

  int lo, hi; // Used in address calculation
  int tmp;

  int behaviour;

 public:
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

 private:
  void handleIrq(int returnPc);
  void handleNmi();
  void handleInterrupt(int returnPc, int vect_l, int vec_h);

  int adc(int acc, int op);
  int adcDecimal(int acc,int op);
  int sbc(int acc,int op);
  int sbcDecimal(int acc,int op);

  void cmp(int acc, int op);

  void setArithmeticFlags(int reg);

  int asl(int m);
  int lsr(int m);
  int rol(int m);
  int ror(int m);

  // Flags
  bool getBreakFlag();
  void setBreakFlag();
  void setBreakFlag(bool b);
  void clearBreakFlag();

  bool getOverflowFlag();
  void setOverflowFlag();
  void setOverflowFlag(bool b);
  void clearOverflowFlag();

  bool getNegativeFlag();
  void setNegativeFlag();
  void setNegativeFlag(bool b);
  void clearNegativeFlag();

  bool getCarryFlag();
  bool getCarryBit();
  void setCarryFlag();
  void setCarryFlag(bool b);
  void clearCarryFlag();

  bool getZeroFlag();
  void setZeroFlag();
  void setZeroFlag(bool b);
  void clearZeroFlag();

  bool getIrqDisableFlag();
  void setIrqDisableFlag();
  void setIrqDisableFlag(bool b);
  void clearIrqDisableFlag();

  bool getDecimalModeFlag();
  void setDecimalModeFlag();
  void setDecimalModeFlag(bool b);
  void clearDecimalModeFlag();

  int getProcessorStatus();
  void setProcessorStatus(int value);

  int getProgramCounter();
  void setProgramCounter(int addr);
  int getStackPointer();
  void setStackPointer(int offset);

  void setOpTrap();
  void clearOpTrap();

  void stackPush(int data);
  int stackPop();
  int stackPeek();

  void assertIrq();
  void clearIrq();
  void assertNmi();
  void clearNmi();

  int address(int lowByte, int hiByte);
  int xAddress(int lowByte, int hiByte);
  int yAddress(int lowByte, int hiByte);
  int zpxAddress(int zp);
  int relAddress(byte offset);
  int zpyAddress(int zp);

  int argsw() { return (instruction[2] << 8) | instruction[1]; };

 public:
  void construct(MemoryMap *mem);
  void reset();
  void summary();

  int disassemble(int addr, char*str, int len);

  byte getStatusFlag();
  void getStatusFlagAsString(char *str, int len);
  void disassembleOp(char *str, int len);

  void checkInterrupts();
  void executeInstruction(int addr);
};

#endif
