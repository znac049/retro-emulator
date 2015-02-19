#ifndef _DEVICE_H
#define _DEVICE_H_

class Device
{
 public:
  virtual const char *getName();
  virtual byte peek(int addr);
  virtual void poke(int addr, byte b);
};

#endif
