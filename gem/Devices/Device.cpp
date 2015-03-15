#include <stdio.h>
#include <string.h>

#include "../gem.h"

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

	snprintf(error, MAXSTR, "Address $%04x (%d) out of range in device '%s::poke'", addr, addr, getName());
	throw error;
  }

  res = readByte(addr);
  fireReadListener(addr);

  return res;
}

void writeByte(int addr, byte b) {
}

void Device::poke(int addr, byte b)
{
  if ((addr < 0) || (addr >= size)) {
	char error[MAXSTR];

	snprintf(error, MAXSTR, "Address $%04x (%d) out of range in device '%s::poke'", addr, addr, getName());
	throw error;
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

  if (fd) {
    while ((addr < size) && (!feof(fd))) {
      int b = fgetc(fd);

      if (b != EOF) {
	writeByte(addr, (byte) (b & 0xff));
	addr++;
      }
    }

    fclose(fd);
  }

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
