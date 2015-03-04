#include <string.h>

#include "Processor.h"

Processor::Processor(MemoryMap *mem)
{
  memory = mem;
  running = false;

  memory->dump();
}

Processor::~Processor()
{
}

void Processor::reset()
{
}

void Processor::step()
{
}

void Processor::run()
{
  running = true;
  while (running) {
    step();
  }
}

MemoryMap *Processor::getMemory()
{
  return memory;
}

int Processor::disassemble(int addr, char*str, int len)
{
  strncpy(str, "???", len);

  return 1;
}
