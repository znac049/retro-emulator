#include "AsteroidsDeluxe.h"

void AsteroidsDeluxe::configureDevice()
{
  ram = new MemoryDevice(2048, 0);
  rom = new MemoryDevice(8192, 1);

  vectorRam = new MemoryDevice(2048, 0);
  vectorRam->setName("Vector RAM");

  vectorRom = new MemoryDevice(4096, 1);
  vectorRom->setName("Vector ROM");

  pokey = new Pokey();

  mm->connect(ram,            0);
  mm->connect(pokey,          0x2c00);
  mm->connect(vectorRam,      0x4000);
  mm->connect(vectorRom,      0x4800);
  mm->connect(rom,            0x6000);
  mm->connect(rom,            0xe000);

  rom->load("roms/adlx.rom");
  vectorRom->load("roms/adlx.vrom");

  proc = new R6502(mm);
  tty = new Console(proc);

  proc->reset();
}

AsteroidsDeluxe::~AsteroidsDeluxe()
{
  ualarm(0, 0);

  delete proc;
  delete tty;
}

void AsteroidsDeluxe::enterCommandLoop()
{
  tty->commandLoop();
}
