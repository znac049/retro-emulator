#ifndef _MACHINE_H_
#define _MACHINE_H_

class CPU;
class MemoryMap;

class Machine {
 protected:
  CPU *proc;
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
  MemoryMap *getMemoryMap() { return memory; };
};

#endif
