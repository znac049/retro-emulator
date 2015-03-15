#ifndef _MACHINE_H_
#define _MACHINE_H_

class CPU;
class MemoryMap;

class Machine {
 protected:
  CPU *proc;
  MemoryMap *memory;

 public:
  Machine();

  virtual void configureDevices();
  virtual void configureProcessor();
};

#endif
