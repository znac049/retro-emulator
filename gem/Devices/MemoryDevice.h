#ifndef _RAM_H_
#define _RAM_H_

#include "Device.h"

class MemoryDevice : public Device
{
 private:
  bool read_only;
  byte *bytes;

 public:
  MemoryDevice(int size_in_bytes, bool ro);
  ~MemoryDevice();

  byte readByte(int addr);
  void writeByte(int addr, byte b);
  bool canWrite(int addr);

  void _set(int addr, byte b);

  const char *getName();
};

#endif
