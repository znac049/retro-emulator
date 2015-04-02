#ifndef _ADMACHINE_H_
#define _ADMACHINE_H_

#include "Machine.h"

class MemoryMap;
class MemoryDevice;
class Pokey;
class Console;

class ADMachine : public Machine {
 private:
  MemoryDevice       *ram;
  MemoryDevice       *rom;
  MemoryDevice       *vectorRam;
  MemoryDevice       *vectorRom;

 public:
  ~ADMachine();

  virtual void configureDevices();
};

#endif
