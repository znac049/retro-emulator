#ifndef _AD_H_
#define _AD_H_

#include "Machine.h"

class MemoryMap;
class MemoryDevice;
class Pokey;
class Console;

class ADMachine : public Machine {
 private:
  MemoryMap          *mm;

  MemoryDevice       *ram;
  MemoryDevice       *rom;
  MemoryDevice       *vectorRam;
  MemoryDevice       *vectorRom;

 protected:

 public:
  ~ADMachine();

  void configureDevice();
};

#endif
