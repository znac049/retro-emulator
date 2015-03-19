#ifndef _R6502_H_
#define _R6502_H_

#include "../gem.h"
#include "CPU.h"

#define NMOS_WITH_INDIRECT_JMP_BUG 1
#define NMOS_WITH_ROR_BUG 2

class MemoryMap;
class State6502;

class CPU6502 : public CPU {
 protected:
  //MemoryMap *memory;
  State6502 *state;

  int irAddressMode; // Bits 3-5 of IR:  [ | | |X|X|X| | ]
  int irOpMode;      // Bits 6-7 of IR:  [ | | | | | |X|X]
  int effectiveAddress;

  /* Internal scratch space */
  int lo, hi; // Used in address calculation
  int tmp; // Temporary storage

  /* start time of op execution, needed for speed simulation */
  long opBeginTime;

  int behaviour;

 private:
  void handleIrq(word returnPc);
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

 public:
  CPU6502(MemoryMap *mem);
  ~CPU6502();

  void reset();
  void step();
  void run();
  void summary();

  State6502 *getState();
  MemoryMap *getMemory();
  int disassemble(int addr, char*str, int len);
};

#endif
