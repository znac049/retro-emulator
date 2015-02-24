#include <exception>

#include <string.h>
#include <malloc.h>

#include "AtariEm.h"
#include "WatchDog.h"

WatchDog::WatchDog() : Device("WatchDog")
{
}

WatchDog::~WatchDog()
{
}

byte WatchDog::peek(int addr)
{
  if (addr != 0) {
    throw "Address out of range in WatchDog::peek";
  }

  return 42;;
}

void WatchDog::poke(int addr, byte b)
{
  if (addr != 0) {
    throw "Address out of range in WatchDog::poke";
  }
}

int WatchDog::getSize()
{
  return 1;
}

bool WatchDog::getAddressName(char *str, int len, int addr)
{
  if (addr != 0) {
    throw "Address out of range in WatchDog::getAddressName";
  }

  strncpy(str, "WatchDog_Reset", len);

  return true;
}
