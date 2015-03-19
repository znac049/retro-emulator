#include <exception>

#include <malloc.h>

#include "../gem.h"
#include "../Debug.h"

#include "MemoryDevice.h"

MemoryDevice::MemoryDevice(int sizeInBytes, bool ro) : Device()
{
  read_only = ro;
  setSize(sizeInBytes);
  bytes = (byte *) malloc(sizeInBytes);

  for (int i = 0; i < sizeInBytes; i++) {
    bytes[i] = 0x5a;
  }

  if (read_only) {
    setName("ROM");
  }
  else {
    setName("RAM");
  }
}

MemoryDevice::~MemoryDevice()
{
  free(bytes);
}

byte MemoryDevice::readByte(int addr)
{
  return bytes[addr];
}

void MemoryDevice::writeByte(int addr, byte b)
{
  if (read_only) {
    throw "Attempt to write to readonly memory";
  }

  bytes[addr] = b;
}

void MemoryDevice::_set(int addr, byte b)
{
  if ((addr < 0) || (addr >= getSize())) {
    throw "Address out of range in RAM Setter";
  }

  bytes[addr] = b;
}
