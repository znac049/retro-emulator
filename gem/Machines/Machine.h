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
  void construct();
  void construct(const char *name);

  virtual void configureDevices();
  virtual void configureProcessor();

  void printSummary();
  
  void setName(const char *newName) { strncpy(machineName, newName, MaxName); };
  const char *getName() { return (const char *)machineName; };

  CPU *getProcessor() { return proc; };
  MemoryMap *getMemoryMap() { return memory; };
};

#endif
