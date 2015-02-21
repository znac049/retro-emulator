#include <exception>

#include <malloc.h>

#include "AtariEm.h"
#include "MemoryDevice.h"

MemoryDevice::MemoryDevice(int size_in_bytes, short int ro)
{
  read_only = ro;
  ram_size = size_in_bytes;
  bytes = (byte *) malloc(ram_size);

  for (int i = 0; i < ram_size; i++) {
    bytes[i] = 0;
  }
}

MemoryDevice::~MemoryDevice()
{
  free(bytes);
  ram_size = 0;
}

byte MemoryDevice::peek(int addr)
{
  if ((addr < 0) || (addr >= ram_size)) {
    throw "Address out of range in RAM Getter";
  }

  return bytes[addr];
}

void MemoryDevice::poke(int addr, byte b)
{
  if (read_only) {
    throw "Attempt to write to readonly memory";
  }

  if ((addr < 0) || (addr >= ram_size)) {
    throw "Address out of range in RAM Setter";
  }

  bytes[addr] = b;
}

const char *MemoryDevice::getName()
{
  if (read_only) {
    return "ROM";
  }
  
  return "RAM";
}

int MemoryDevice::getSize()
{
  return ram_size;
}
