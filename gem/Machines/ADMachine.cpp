#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "ADMachine.h"
#include "../MemoryMap.h"
#include "../Devices/MemoryDevice.h"

void ADMachine::configureDevice()
{
  ram = new MemoryDevice(2048, 0);
  rom = new MemoryDevice(8192, 1);

  vectorRam = new MemoryDevice(2048, 0);
  vectorRam->setName("Vector RAM");

  vectorRom = new MemoryDevice(4096, 1);
  vectorRom->setName("Vector ROM");

  mm = new MemoryMap(0x10000);
  mm->connect(ram,            0);
  mm->connect(vectorRam,      0x4000);
  mm->connect(vectorRom,      0x4800);
  mm->connect(rom,            0x6000);
  mm->connect(rom,            0xe000);

  rom->load("roms/adlx.rom");
  vectorRom->load("roms/adlx.vrom");
}

ADMachine::~ADMachine()
{
  ualarm(0, 0);
}
