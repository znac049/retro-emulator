#include <stdio.h>
#include <string.h>

#include "../gem.h"
#include "../Debug.h"
#include "../Misc/Radix.h"

#include "Device.h"
#include "DeviceListener.h"

Device::Device()
{
  setName("Core Device");

  listener = NULL;
}

Device::Device(const char *newName)
{
  setName(newName);
  listener = NULL;
}

void Device::setName(const char *newName)
{
  strncpy(name, newName, MAXDEVNAME);
}

byte Device::readByte(int addr)
{
  return 0xff;
}

byte Device::peek(int addr)
{
  byte res;

  if ((addr < 0) || (addr >= size)) {
    char error[MAXSTR];

    snprintf(error, MAXSTR, "Address $%04x (%d) out of range in device '%s::peek'", addr, addr, getName());
    Debug::logf(1, "Error: %s\n", error);

    throw error;
  }

  res = readByte(addr);
  fireReadListener(addr);

  return res;
}

void Device::writeByte(int addr, byte b) {
  Debug::logf(10, "Device::writeByte(%d, 0x%02x)\n", addr, b);
}

void Device::poke(int addr, byte b)
{
  if ((addr < 0) || (addr >= size)) {
    char error[MAXSTR];

    snprintf(error, MAXSTR, "Address $%04x (%d) out of range in device '%s::poke'", addr, addr, getName());
    Debug::logf(1, "Error: %s\n", error);

    throw error;
  }

  if (!canWrite(addr)) {
    Debug::logf(1, "can't write to address %s in %s\n", Radix::toString(addr, 16), getName());
  }
  else {
    writeByte(addr, b);
    fireWriteListener(addr, b);
  }
}

bool Device::save(const char *loc, bool overwrite)
{
  FILE *fd = fopen(loc, "wb");
  int addr = 0;

  if (fd) {
    for (int i=0; i<size; i++) {
      fputc(readByte(i), fd);
    }

    fclose(fd);
  }

  return true;
}

int Device::load(const char *loc)
{
  FILE *fd = fopen(loc, "rb");
  int addr = 0;

  Debug::logf(2, "Loading from file %s\n", loc);
  Debug::logf(4, "memory size is %d\n", size);

  if (fd) {
    Debug::logf(4, "file opened ok\n", size);
    while ((addr < size) && (!feof(fd))) {
      int b = fgetc(fd);

      if (b != EOF) {
	writeByte(addr, (byte) (b & 0xff));
	addr++;
      }
    }

    fclose(fd);
  }
  else {
    Debug::logf(4, "failed to open file\n");
  }

  Debug::logf(1, "%d bytes loaded from file %s\n", addr, loc);

  return addr;
}

bool Device::getAddressName(char *str, int len, int addr)
{
  return false;
}

void Device::setListener(DeviceListener *lstnr)
{
  listener = lstnr;
}

void Device::fireReadListener(int addr)
{
  if (listener != NULL) {
    listener->readListener(this, addr);
  }
}

void Device::fireWriteListener(int addr, byte b)
{
  if (listener != NULL) {
    listener->writeListener(this, addr, b);
  }
}
