#include <string.h>

#include "CPU.h"
#include "../MemoryMap.h"

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

void CPU::setName(const char *newName)
{
  strncpy(cpuName, newName, MaxName);
}

const char *CPU::getName()
{
  return cpuName;
}
