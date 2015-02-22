#include <stdio.h>
#include <string.h>

#include "AtariEm.h"
#include "MemoryDevice.h"
#include "Pokey.h"
#include "ADlxDigitalOutputs.h"
#include "CPU.h"

int main(int argc, char *argv[])
{
  MemoryDevice ram(2048, 0);
  MemoryDevice rom(8192, 1);
  MemoryDevice vectorRam(2048, 0);
  MemoryDevice vectorRom(4096, 1);
  ADlxDigitalOutputs ops;
  Pokey pokey;

  MemoryMap mm(0xffff + 1);
  CPU *proc;

  mm.connect(&ops, 0x3c00);

  vectorRom.setName("Vector ROM");
  mm.connect(&vectorRom, 0x4800);

  vectorRam.setName("Vector RAM");
  mm.connect(&vectorRam, 0x4000);

  mm.connect(&pokey, 0x2c00);

  mm.connect(&ram, 0);
  mm.connect(&rom, 0x6000);
  mm.connect(&rom, 0xe000);

  rom.load("roms/adlx.rom");

  proc = new CPU(&mm);
  proc->reset();

  printf("Emulator starting.\n");

  try {
    for (int i=0; i<2048; i++) {
      mm.poke(i, 0);
    }

    for (int i=0; i<5000; i++) {
      proc->step();
      proc->summary();
    }
  }
  catch (const char *msg) {
    printf("Caught exception: '%s'\n", msg);
  }

  return 0;
}
