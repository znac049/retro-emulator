#include <string.h>

#include "CPU.h"
#include "../MemoryMap.h"

CPU::CPU(MemoryMap *mem)
{
  memory = mem;
  running = false;

  memory->dump();
}

CPU::~CPU()
{
}

void CPU::reset()
{
}

void CPU::step()
{
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
