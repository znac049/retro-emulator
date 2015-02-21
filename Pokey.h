#ifndef _POKEY_H_
#define _POKEY_H_

#include "Device.h"

#define POKEY_SIZE 16

class Pokey : public Device
{
 private:

 public:
  Pokey();
  ~Pokey();

  byte peek(int addr);
  void poke(int addr, byte b);
  const char *getName();
  int getSize();
};

#endif
