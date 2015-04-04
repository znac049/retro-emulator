/*!
  \file   CPU.h
  \brief  Base class for generic CPU  

  <long description>

  \author Bob Green <bob@wookey.org.uk>
  \date   2015-04-03
*/

#ifndef _CPU_H_
#define _CPU_H_

#include "../gem.h"

class MemoryMap;

class CPU {
 private:
  static const char *_instructionNames[];
  static const int _instructionSizes[];
  static const int _instructionCycles[];
  static const int _addressingModes[];

 protected:
  enum endianness {littleEndian, bigEndian};
  enum instructionMode {MODE_NONE=0};

  MemoryMap *memory;

  bool running;
  int endian;

  const char **instructionNames;
  const int *instructionSizes;
  const int *instructionCycles;
  const int *addressingModes;

 public:
  static const int MaxCPUName = 100;

  char *cpuName;

  int sp;
  int pc;

  byte *instruction;

  int lastPc;

  int instSize;
  bool opTrap;

  long stepCounter;

  long instructionEndTicks;

 protected:
  virtual void executeInstruction(int addr);

 public:
  void setName(const char *newName) { strncpy(cpuName, newName, MaxCPUName); };
  const char *getName() { return cpuName; };

  virtual void construct(MemoryMap *mem);
  virtual void construct(MemoryMap *mem, const char *name);

  virtual int getMaxBytesPerInstruction() { return 1; };
  virtual int getEndianness() { return endian; };

  virtual void reset();
  virtual void step();
  virtual void run();

  virtual void incrementPC() { pc = (pc + 1) & 0xffff; };
  virtual int loadInstruction(int addr);

  virtual void summary();

  virtual void checkInterrupts();

  virtual int disassemble(int addr, char *str, int len);

  virtual MemoryMap *getMemory() { return memory; };

  virtual void setRegister(const char *name, int val);
  virtual int getRegister(const char *name);
  virtual int sizeOfRegister(const char *name);
  virtual bool isRegister(const char *name);

  virtual byte getStatusFlag();
  virtual void getStatusFlagAsString(char *str, int len);

  virtual void disassembleOp(char *str, int len);

  virtual const char *getInstructionName() { return getInstructionName(instruction[0], lastPc); };
  virtual const char *getInstructionName(int ir, int addr) { return instructionNames[ir]; };

  virtual int getAddressingMode() { return getAddressingMode(instruction[0], lastPc); };
  virtual int getAddressingMode(int ir, int addr) { return addressingModes[ir]; };

  virtual int getInstructionSize() { return getInstructionSize(instruction[0], lastPc); };
  virtual int getInstructionSize(int ir, int addr) { return instructionSizes[ir]; };

  virtual int getInstructionCycles() { getInstructionCycles(instruction[0], lastPc); };
  virtual int getInstructionCycles(int ir, int addr) { return instructionCycles[ir]; };

  virtual void printRegisters();
  virtual void setRegister(const char *reg, int val);
  virtual int getRegister(const char *reg);
  virtual bool isRegister(const char *reg);

  void setInstructionEndTicks(long endTicks);
  void waitForInstructionToEnd();
};

#endif
