#include <string.h>

#include "CPU.h"
#include "../MemoryMap.h"
#include "../Timer.h"
#include "../Debug.h"
#include "../Misc/Radix.h"

#include "CPU.inst"

CPU::CPU(MemoryMap *mem)
{
  memory = mem;
  running = false;

  cpuName = new char[MaxName];
  setName("Generic CPU");

  instructionNames = _instructionNames;
  instructionSizes = _instructionSizes;
  instructionCycles = _instructionSizes;
  addressingModes = _addressingModes;

  Debug::logf(10, "Memory map:\n");
  memory->dump();
  reset();
}

CPU::~CPU()
{
}

void CPU::reset()
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

void CPU::step()
{
  long opStart = Timer::getNanoTicks();

  lastPc = pc;

  checkInterrupts();

  ir = memory->peek(pc);
  incrementPC();

  // TODO - this assumes 1MHz clock :(
  setInstructionEndTicks(opStart + (getInstructionCycles() * 1000));

  instSize = getInstructionSize();
  for (int i = 0; i < instSize - 1; i++) {
    args[i] = memory->peek(pc);
    incrementPC();
  }

  stepCounter++;

  executeInstruction(lastPc);
}

void CPU::run()
{
  running = true;
  while (running) {
    step();
  }
}

MemoryMap *CPU::getMemory()
{
  return memory;
}

int CPU::disassemble(int addr, char*str, int len)
{
  strncpy(str, "???", len);

  return 1;
}

void CPU::setName(const char *newName)
{
  strncpy(cpuName, newName, MaxName);
}

const char *CPU::getName()
{
  return cpuName;
}

void CPU::incrementPC() {
  pc++;
  pc &= 0xffff;
}

int CPU::load(int addr)
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
    snprintf(str, len, "!!!! IR=$%02x mode=%d", ir, mode);
    break;
  }
}

const char *CPU::getInstructionName()
{
  return instructionNames[ir];
}

int CPU::getInstructionSize()
{
  return instructionSizes[ir];
}

int CPU::getInstructionCycles()
{
  return instructionCycles[ir];
}

int CPU::getAddressingMode()
{
  return addressingModes[ir];
}

void CPU::setInstructionEndTicks(long endTicks)
{
  instructionEndTicks = endTicks;
}

void CPU::waitForInstructionToEnd()
{
  long now = Timer::getNanoTicks();

  while (now < instructionEndTicks) {
    now = Timer::getNanoTicks();
  }
}

void CPU::checkInterrupts()
{
}

void CPU::executeInstruction(int addr)
{
  pc = addr;

  printf("Execute instruction at %s\n", Radix::toRadix(pc, 4, Radix::RESET));

  pc++;

  lastPc = pc;
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
  printf("PC=%s, SP=%s\n", Radix::toRadix(pc, 4, Radix::RESET), Radix::toRadix(sp, 4));
}
