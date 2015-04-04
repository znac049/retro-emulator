#include <stdio.h>
#include <string.h>

#include "CPU.h"
#include "../MemoryMap.h"
#include "../Debug.h"
#include "../Misc/TimeStamp.h"
#include "../Misc/Radix.h"

#include "CPU.inst"

void CPU::construct(MemoryMap *mem, const char *name)
{
  int maxInstSize = getMaxBytesPerInstruction();

  Debug::logf(5, "Constructing a CPU: '%s'\n", name);

  memory = mem;
  running = false;

  cpuName = new char[MaxCPUName];
  setName(name);

  endian = getEndianness();

  instructionNames = _instructionNames;
  instructionSizes = _instructionSizes;
  instructionCycles = _instructionSizes;
  addressingModes = _addressingModes;

  instruction = new byte[maxInstSize];

  reset();
}

void CPU::construct(MemoryMap *mem)
{
  construct(mem, "Generic CPU");
}

void CPU::reset()
{
  Debug::logf(3, "%s reset\n", getName());

  sp = 0xff;

  lastPc = pc = 0;
  instSize = loadInstruction(pc);

  opTrap = 0;

  stepCounter = 0L;
  running = false;
}

void CPU::step()
{
  ts.wait();
  checkInterrupts();

  instSize = loadInstruction(pc);
  stepCounter++;

  ts.now();
  executeInstruction(lastPc);
}

void CPU::executeInstruction(int addr)
{
  pc = addr;

  printf("Execute instruction at %s\n", Radix::toString(pc, 16));

  pc++;
}

void CPU::run()
{
  running = true;
  while (running) {
    step();
  }
}

int CPU::disassemble(int addr, char *str, int len)
{
  strncpy(str, "???", len);

  return 1;
}

int CPU::loadInstruction(int addr)
{
  byte *inst = instruction;
  int ir = memory->peek(pc);

  lastPc = pc;
  pc = addr;

  *inst++ = ir;
  pc++;

  opTrap = false;

  instSize = getInstructionSize(ir, addr);
  for (int i = 0; i < instSize-1; i++) {
    *inst++ = memory->peek(pc);
    pc++;
  }

  return instSize+1;
}

byte CPU::getStatusFlag()
{
  return 0;
}

void CPU::getStatusFlagAsString(char *str, int len)
{
  snprintf(str, len, "[???]");
}

void CPU::disassembleOp(char *str, int len)
{
  const char *mnemonic = getInstructionName();
  char address[256];
  int mode;

  if (mnemonic == NULL) {
    strncpy(str, "???", len);

    return;
  }

  mode = getAddressingMode();
  switch (mode) {
  default:
    snprintf(str, len, "!!!! IR=$%02x mode=%d", instruction[0], mode);
    break;
  }
}

void CPU::checkInterrupts()
{
}

void CPU::summary()
{
  char flags[64];
  char src[512];

  disassembleOp(src, sizeof(src));
  printf("$%04x: %-20s ", lastPc, src);

  getStatusFlagAsString(flags, sizeof(flags));
  printf("PC: $%04x  SP:$%02x Flags: %s\n",
	 pc, sp, flags);
}

void CPU::printRegisters()
{
  printf("PC=%s, SP=%s\n", Radix::toString(pc, 16), Radix::toString(sp, 16));
}

void CPU::setRegister(const char *name, int val)
{
  if (strcasecmp(name, "pc") == 0) {
    pc = val;
  }
  else if (strcasecmp(name, "sp") == 0) {
    sp = val;
  }
}

int CPU::getRegister(const char *name)
{
  if (strcasecmp(name, "pc") == 0) {
    return pc;
  }
  else if (strcasecmp(name, "sp") == 0) {
    return sp;
  }
}

bool CPU::isRegister(const char *name)
{
  if ((strcasecmp(name, "pc") == 0) || (strcasecmp(name, "sp") == 0)) {
    return true;
  }

  return false;
}

int CPU::sizeOfRegister(const char *name)
{
  if (isRegister(name)) {
    return 16;
  }

  return 0;
}
