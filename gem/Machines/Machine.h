#ifndef _MACHINE_H_
#define _MACHINE_H_

class CPU;
class CPUState;
class MemoryMap;

class Machine {
 protected:
  CPU *proc;
  CPUState *state;
  MemoryMap *memory;
  char *machineName;

 public:
  static const int MaxName = 200;

 public:
  Machine();

  void setName(const char *newName);
  const char *getName();

  virtual void configureDevices();
  virtual void configureProcessor();

  void printSummary();
  
  CPU *getProcessor() { return proc; };
  CPUState *getState() { return state; };
};

#endif
