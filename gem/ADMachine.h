#ifndef _AD_H_
#define _AD_H_

#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "Machine.h"
#include "MemoryDevice.h"
#include "Pokey.h"
#include "R6502.h"

#include "Console.h"
#include "TimerListener.h"

class AsteroidsDeluxe : public Machine {
 private:
  MemoryDevice       *ram;
  MemoryDevice       *rom;
  MemoryDevice       *vectorRam;
  MemoryDevice       *vectorRom;

  Pokey              *pokey;

  Console            *tty;

 protected:

 public:
  void configureDevice();

  void enterCommandLoop();
  void timerFired();
};

#endif
