#ifndef _RAM_H_
#define _RAM_H_

#include "Device.h"

class RAMDevice : public Device
{
 private:
  byte *bytes;
  int ram_size;

 public:
  RAMDevice(int size_in_bytes);
  ~RAMDevice();

  byte Get(int addr);
  void Set(int addr, byte b);
};

#endif
