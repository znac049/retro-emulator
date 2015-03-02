#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "Processor.h"
#include "MemoryMap.h"

class Machine {
 protected:
  Processor *proc;
  MemoryMap *memory;

 public:
  Machine();

  virtual void configureDevides();
  virtual void configureProcessor();
};

#endif
