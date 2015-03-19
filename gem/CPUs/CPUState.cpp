#include <stdio.h>
#include <string.h>

#include "../Debug.h"
#include "CPUState.h"
#include "../MemoryMap.h"

#include "CPU.inst"

CPUState::CPUState(MemoryMap *mem) {
  memory = mem;

  instructionNames = _instructionNames;
  instructionSizes = _instructionSizes;
  instructionCycles = _instructionSizes;
  addressingModes = _addressingModes;

  Debug::logf(10, "Memory map:\n");
  memory->dump();
  reset();
}

CPUState::~CPUState() {
}

void CPUState::reset()
{
  sp = 0xff;

  pc = 0;
  ir = memory->peek(pc);

  lastPc = 0;

  instSize = 0;

  opTrap = 0;

  stepCounter = 0L;
  running = false;
}

int CPUState::load(int addr)
{
  lastPc = pc;
  pc = addr;

  ir = memory->peek(pc);
  pc++;

  opTrap = false;

  instSize = getInstructionSize();
  for (int i = 0; i < instSize-1; i++) {
    args[i] = memory->peek(pc);
    pc++;
  }

  return instSize;
}

byte CPUState::getStatusFlag()
{
  return 0;
}

void CPUState::getStatusFlagAsString(char *str, int len)
{
  snprintf(str, len, "[???]");
}

void CPUState::disassembleOp(char *str, int len)
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
    snprintf(str, len, "!!!! IR=$%02x mode=%d", ir, mode);
    break;
  }
}

const char *CPUState::getInstructionName()
{
  return instructionNames[ir];
}

int CPUState::getInstructionSize()
{
  return instructionSizes[ir];
}

int CPUState::getInstructionCycles()
{
  return instructionCycles[ir];
}

int CPUState::getAddressingMode()
{
  return addressingModes[ir];
}
