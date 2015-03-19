#include <string.h>

#include "CPU.h"
#include "../MemoryMap.h"
#include "../Timer.h"
#include "../Debug.h"

CPU::CPU(MemoryMap *mem)
{
  memory = mem;
  running = false;

  cpuName = new char[MaxName];
  setName("Generic CPU");
}

CPU::~CPU()
{
}

void CPU::reset()
{
  state->reset();
}

void CPU::step()
{
  long opStart = Timer::getNanoTicks();

  state->lastPc = state->pc;

  checkInterrupts();

  state->ir = memory->peek(state->pc);

  incrememntPC();

  // TODO - this assumes 1MHz clock :(
  state->setInstructionEndTicks(opStart + (state->getInstructionCycles() * 1000));

  state->instSize = state->getInstructionSize();
  for (int i = 0; i < state->instSize - 1; i++) {
    state->args[i] = memory->peek(state->pc);
    incrementPC();
  }

  state->stepCounter++;

  executeInstruction();
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
  state->pc++;
  state->pc &= 0xffff;
}

