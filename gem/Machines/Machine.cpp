#include "Machine.h"
#include "../MemoryMap.h"
#include "../CPUs/CPU.h"

Machine::Machine()
{
  memory = new MemoryMap(0x10000);

  configureDevices();
  configureProcessor();
}

void Machine::configureDevices()
{
}

void Machine::configureProcessor()
{
  proc = new CPU(memory);
}
