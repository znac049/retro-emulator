#include "AsteroidsDeluxe.h"

AsteroidsDeluxe::AsteroidsDeluxe()
{
  dvg = new VectorGenerator();

  ram = new MemoryDevice(2048, 0);
  rom = new MemoryDevice(8192, 1);

  vectorRam = new MemoryDevice(2048, 0);
  vectorRam->setName("Vector RAM");

  vectorRom = new MemoryDevice(4096, 1);
  vectorRom->setName("Vector ROM");

  optionSwitches = new ADlxOptionSwitches();

  switches = new ADlxSwitches();

  outputs = new ADlxOutputs();

  inputs = new ADlxInputs(dvg);
  inputs->press(7); // Turn on the self test switch

  vgStart = new StartVGDevice(dvg);

  watchDog = new WatchDog();

  pokey = new Pokey();
  

  // Now plug it all together
  mm = new MemoryMap(0xffff + 1);

  mm->connect(ram,            0);
  mm->connect(inputs,         0x2000);
  mm->connect(switches,       0x2400);
  mm->connect(optionSwitches, 0x2800);
  mm->connect(pokey,          0x2c00);
  mm->connect(vgStart,        0x3000);
  mm->connect(watchDog,       0x3400);
  mm->connect(outputs,        0x3c00);
  mm->connect(vectorRam,      0x4000);
  mm->connect(vectorRom,      0x4800);
  mm->connect(rom,            0x6000);
  mm->connect(rom,            0xe000);

  rom->load("roms/adlx.rom");

  proc = new CPU(mm);
  tty = new Console(proc);

  // Start a regular timer running at 3KHz (333uS)
  signal(SIGALRM, watchDogSignalHandler);
  ualarm(166, 166);

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

void AsteroidsDeluxe::watchDogSignalHandler(int sig)
{
  static long ticks=0;

  ticks++;

  ADlxInputs::flip(1);
}
