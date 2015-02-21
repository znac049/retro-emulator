#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "AtariEm.h"

class Device
{
 protected:
  char *name;

 public:
  Device();
  Device(const char *newName);
  ~Device();

  const char *getName();
  void setName(const char *newName);

  virtual int getSize();
  virtual byte peek(int addr);
  virtual void poke(int addr, byte b);
};

#endif
