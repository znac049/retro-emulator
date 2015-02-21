#include <string.h>
#include <malloc.h>

#include "AtariEm.h"
#include "Device.h"

Device::Device()
{
  name = NULL;
  setName("Core Device");
}

Device::Device(const char *newName)
{
  name = strdup(newName);
}

Device::~Device()
{
  free(name);
}

const char *Device::getName()
{
  return name;
}

void Device::setName(const char *newName)
{
  if (name != NULL) {
    free(name);
  }

  name = strdup(newName);
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
