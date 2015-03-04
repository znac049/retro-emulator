#include "Machine.h"

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
  proc = new R6502(memory);
}
