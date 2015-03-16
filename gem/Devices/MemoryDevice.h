#ifndef _RAM_H_
#define _RAM_H_

#include "Device.h"

class MemoryDevice : public Device
{
 private:
  bool read_only;
  byte *bytes;
  int ram_size;

 public:
  MemoryDevice(int size_in_bytes, bool ro);
  ~MemoryDevice();

  byte peek(int addr);
  void poke(int addr, byte b);

  void _set(int addr, byte b);

  const char *getName();
  int getSize();
};

#endif
