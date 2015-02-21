#include "AtariEm.h"
#include "Device.h"

const char *Device::getName()
{
  return "Unknown Core Device";
}

int Device::getSize() {
  throw "low level getSize should have been overridden";
}

byte Device::peek(int addr)
{
  throw "low level peek should have been overridden";
}

void Device::poke(int addr, byte b)
{
  throw "low level poke should have been overridden";
}
