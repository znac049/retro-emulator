#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "ADMachine.h"

#include "../MemoryMap.h"
#include "../CPUs/CPU6502.h"
#include "../Devices/Pokey.h"
#include "../Devices/MemoryDevice.h"

#include "../UI/Console.h"
#include "../TimerListener.h"

void ADMachine::configureDevice()
{
  ram = new MemoryDevice(2048, 0);
  rom = new MemoryDevice(8192, 1);

  vectorRam = new MemoryDevice(2048, 0);
  vectorRam->setName("Vector RAM");

  vectorRom = new MemoryDevice(4096, 1);
  vectorRom->setName("Vector ROM");

  pokey = new Pokey();

  mm = new MemoryMap(0x10000);
  mm->connect(ram,            0);
  mm->connect(pokey,          0x2c00);
  mm->connect(vectorRam,      0x4000);
  mm->connect(vectorRom,      0x4800);
  mm->connect(rom,            0x6000);
  mm->connect(rom,            0xe000);

  rom->load("roms/adlx.rom");
  vectorRom->load("roms/adlx.vrom");

  proc = new CPU6502(mm);
  tty = new Console((CPU6502 *)proc);

  proc->reset();
}

ADMachine::~ADMachine()
{
  ualarm(0, 0);

  delete proc;
  delete tty;
}

void ADMachine::enterCommandLoop()
{
  tty->commandLoop();
}
