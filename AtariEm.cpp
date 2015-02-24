#include <stdio.h>
#include <string.h>

#include "AtariEm.h"
#include "MemoryDevice.h"
#include "Pokey.h"
#include "ADlxOutputs.h"
#include "ADlxSwitches.h"
#include "ADlxInputs.h"
#include "ADlxOptionSwitches.h"
#include "VectorGenerator.h"
#include "StartVGDevice.h"
#include "WatchDog.h"
#include "CPU.h"
#include "Console.h"

int main(int argc, char *argv[])
{
  VectorGenerator dvg;
  MemoryDevice ram(2048, 0);
  MemoryDevice rom(8192, 1);
  MemoryDevice vectorRam(2048, 0);
  MemoryDevice vectorRom(4096, 1);
  ADlxOptionSwitches optionSwitches;
  ADlxSwitches switches;
  ADlxOutputs outputs;
  ADlxInputs inputs(&dvg);
  StartVGDevice vgStart(&dvg);
  WatchDog watchDog;
  Pokey pokey;

  MemoryMap mm(0xffff + 1);

  CPU *proc;
  Console *tty;

  vectorRam.setName("Vector RAM");
  vectorRom.setName("Vector ROM");
  
  // Turn on the self test switch
  inputs.press(7);

  mm.connect(&ram,            0);
  mm.connect(&inputs,         0x2000);
  mm.connect(&switches,       0x2400);
  mm.connect(&optionSwitches, 0x2800);
  mm.connect(&pokey,          0x2c00);
  mm.connect(&vgStart,        0x3000);
  mm.connect(&watchDog,       0x3400);
  mm.connect(&outputs,        0x3c00);
  mm.connect(&vectorRam,      0x4000);
  mm.connect(&vectorRom,      0x4800);
  mm.connect(&rom,            0x6000);
  mm.connect(&rom,            0xe000);

  rom.load("roms/adlx.rom");

  proc = new CPU(&mm);
  proc->reset();

  tty = new Console(proc);
  tty->commandLoop();

  delete proc;
  delete tty;

  return 0;
}
