#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "Machine.h"
#include "../Debug.h"
#include "../MemoryMap.h"
#include "../CPUs/CPU.h"
#include "../Devices/MemoryDevice.h"

void Machine::construct(const char *name)
{
  machineName = (char *)malloc(MaxName);
  setName(name);

  memory = new MemoryMap(0x10000);

  configureDevices();
  configureProcessor();  
}

void Machine::construct()
{
  construct("Unnamed Machine");
}

void Machine::configureDevices()
{
  Debug::logf(1, "Machine::configureDevices()\n");
  MemoryDevice *ram = new MemoryDevice(0x10000, false);

  memory->connect(ram, 0);
}

void Machine::configureProcessor()
{
  Debug::logf(10, "configureProcessor()\n");
  proc = new CPU(memory);
}

void Machine::printSummary()
{
  printf("Current machine:\n");
  printf("Name: %s\n", getName());
  printf("CPU: %s\n", proc->getName());

  memory->dump();
}
