#ifndef _RAM_H_
#define _RAM_H_

#include "Device.h"

class MemoryDevice : public Device
{
 private:
  short int read_only;
  byte *bytes;
  int ram_size;

 public:
  MemoryDevice(int size_in_bytes, short int ro);
  ~MemoryDevice();

  byte peek(int addr);
  void poke(int addr, byte b);
  const char *getName();
};

#endif
