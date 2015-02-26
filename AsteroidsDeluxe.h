#ifndef _AD_H_
#define _AD_H_
#include <string.h>
#include <unistd.h>
#include <signal.h>

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

class AsteroidsDeluxe {
 private:
  VectorGenerator    *dvg;
  MemoryDevice       *ram;
  MemoryDevice       *rom;
  MemoryDevice       *vectorRam;
  MemoryDevice       *vectorRom;
  ADlxOptionSwitches *optionSwitches;
  ADlxSwitches       *switches;
  ADlxOutputs        *outputs;
  ADlxInputs         *inputs;
  StartVGDevice      *vgStart;
  WatchDog           *watchDog;
  Pokey              *pokey;

  MemoryMap          *mm;

  CPU                *proc;
  Console            *tty;

 protected:

 private:
  static void watchDogSignalHandler(int sig);

 public:
  AsteroidsDeluxe();
  ~AsteroidsDeluxe();

  void enterCommandLoop();
};

#endif
