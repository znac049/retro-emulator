#include <exception>

#include <malloc.h>

#include "AtariEm.h"
#include "RAMDevice.h"

RAMDevice::RAMDevice(int size_in_bytes)
  : Device()
{
  ram_size = size_in_bytes;
  bytes = (byte *) malloc(ram_size);

  SetName("RAM");

  for (int i = 0; i < ram_size; i++) {
    bytes[i] = 0;
  }
}

RAMDevice::~RAMDevice()
{
  free(bytes);
  ram_size = 0;
}

byte RAMDevice::Get(int addr)
{
  if ((addr < 0) || (addr >= ram_size)) {
    throw "Address out of range in RAM Getter";
  }

  return bytes[addr];
}

void RAMDevice::Set(int addr, byte b)
{
  if ((addr < 0) || (addr >= ram_size)) {
    throw "Address out of range in RAM Setter";
  }

  bytes[addr] = b;
}
