/*
 * CPU.h
 *
 *  Created on: 19 Feb 2015
 *      Author: bob
 */

#ifndef CPU_H_
#define CPU_H_

#include "MemoryMap.h"
#include "CPUState.h"

#define P_CARRY       0x01
#define P_ZERO        0x02
#define P_IRQ_DISABLE 0x04
#define P_DECIMAL     0x08
#define P_BREAK       0x10
/* Bit 5 is always set */
#define P_OVERFLOW    0x40
#define P_NEGATIVE    0x80

/* NMI vector */
#define NMI_VECTOR_L 0xfffa
#define NMI_VECTOR_H 0xfffb

/* Reset vector */
#define RST_VECTOR_L 0xfffc
#define RST_VECTOR_H 0xfffd

/* IRQ vector */
#define IRQ_VECTOR_L 0xfffe
#define IRQ_VECTOR_H 0xffff

#define NMOS_WITH_INDIRECT_JMP_BUG 1
#define NMOS_WITH_ROR_BUG 2

class CPU {
 private:
  MemoryMap *memory;
  CPUState *state;

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

  void incrementPC();

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

  void delayLoop(int opcode);
  long getNanoTicks();

 public:
  CPU(MemoryMap *mem);
  ~CPU();

  void reset();
  void step();
  void run();
  void summary();

  CPUState *getState();
  MemoryMap *getMemory();
};

#endif /* CPU_H_ */
