#ifndef _DEVICE_H
#define _DEVICE_H_

class Device
{
 public:
  const char *getName();
  byte peek(int addr);
  void poke(int addr, byte b);
};

#endif
