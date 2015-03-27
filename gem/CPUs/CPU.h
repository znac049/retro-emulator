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
  MemoryMap *memory;

  bool running;
  bool littleEndian;

  const char **instructionNames;
  const int *instructionSizes;
  const int *instructionCycles;
  const int *addressingModes;

 public:
  static const int MaxName = 100;

  enum instructionMode {MODE_NONE=0};

  char *cpuName;

  int sp;
  int pc;

  byte ir;
  byte args[2];

  int lastPc;

  int instSize;
  bool opTrap;

  long stepCounter;

  long instructionEndTicks;

 protected:
  int argsw() { return ((args[1])<<8) | args[0]; }
  virtual void executeInstruction(int addr);

 public:
  CPU(MemoryMap *mem);
  ~CPU();

  bool isLittleEndian() { return littleEndian; };

  virtual void reset();
  virtual void step();
  virtual void run();
  virtual void incrementPC();

  virtual void summary();

  virtual void checkInterrupts();

  virtual MemoryMap *getMemory();

  virtual int disassemble(int addr, char *str, int len);

  void setName(const char *newName);
  const char *getName();

  virtual int load(int addr);

  virtual byte getStatusFlag();
  virtual void getStatusFlagAsString(char *str, int len);
  virtual void disassembleOp(char *str, int len);

  virtual const char *getInstructionName();
  virtual int getAddressingMode();
  virtual int getInstructionSize();
  virtual int getInstructionCycles();

  virtual void printRegisters();
  virtual void setRegister(const char *reg, int val);
  virtual int getRegister(const char *reg);
  virtual bool isRegister(const char *reg);

  void setInstructionEndTicks(long endTicks);
  void waitForInstructionToEnd();
};

#endif
