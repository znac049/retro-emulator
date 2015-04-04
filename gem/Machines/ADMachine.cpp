#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "ADMachine.h"
#include "../MemoryMap.h"
#include "../Debug.h"
#include "../Devices/MemoryDevice.h"

void ADMachine::configureDevices()
{
  Debug::logf(1, "ADMachine::configureDevices()\n");

  setName("Asteroids Deluxe (V3 Roms)");

  ram = new MemoryDevice(2048, 0);
  rom = new MemoryDevice(8192, 1);

  vectorRam = new MemoryDevice(2048, 0);
  vectorRam->setName("Vector RAM");

  vectorRom = new MemoryDevice(4096, 1);
  vectorRom->setName("Vector ROM");

  memory = new MemoryMap(0x10000);
  memory->connect(ram,            0);
  memory->connect(vectorRam,      0x4000);
  memory->connect(vectorRom,      0x4800);
  memory->connect(rom,            0x6000);
  memory->connect(rom,            0xe000);

  rom->load("roms/adlx.rom");
  vectorRom->load("roms/adlx.vrom");
}

ADMachine::~ADMachine()
{
  ualarm(0, 0);
}
