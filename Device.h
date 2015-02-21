#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "AtariEm.h"

class Device
{
 public:
  virtual const char *getName();
  virtual int getSize();
  virtual byte peek(int addr);
  virtual void poke(int addr, byte b);
};

#endif
