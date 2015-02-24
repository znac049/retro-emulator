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

bool Device::save(const char *loc)
{
  return false;
}

int Device::load(const char *loc)
{
  FILE *fd = fopen(loc, "rb");
  int size = getSize();
  int addr = 0;

  //printf("Device::load('%s')\n", loc);

  if (fd) {
    //printf("Reading up to %d bytes from '%s'...\n", size, loc);

    while ((addr < size) && (!feof(fd))) {
      int b = fgetc(fd);

      if (b != EOF) {
	_set(addr, (byte) (b & 0xff));
	addr++;
      }
    }

    //printf("...read %d bytes.\n", addr);

    fclose(fd);
  }

  return addr;
}

bool Device::getAddressName(char *str, int len, int addr)
{
  return false;
}
