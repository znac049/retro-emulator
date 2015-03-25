#include <stdio.h>
#include <string.h>

#include "Radix.h"
#include "../Debug.h"

char Radix::pool[MaxTmpStr];
int Radix::radix = 16;
int Radix::ind = 0;

void Radix::set(int newRadix)
{
  if ((newRadix == 2) || (newRadix == 8) || (newRadix == 10) || (newRadix == 16)) {
    radix = newRadix;
  }
}

int Radix::get()
{
  return radix;
}

void Radix::resetPool()
{
  ind = 0;
}

char *Radix::getPool()
{
  return &pool[ind];
}

void Radix::allocBytes(int nBytes)
{
  ind += nBytes;

  if (ind >= MaxTmpStr) {
    Debug::logf(1, "Out of pool space in Radix::allocBytes(%d)\n", nBytes);
  }
}

int Radix::getFreeBytes()
{
  return MaxTmpStr - ind;
}

char *Radix::toRadix(int val, int places)
{
  return toRadix(val, places, 0);
}

char *Radix::toRadix(int val, int places, int reset)
{
  char *res;
  char format[16];
  int freeBytes;

  if (reset) {
    resetPool();
  }

  res = getPool();
  freeBytes = getFreeBytes();

  switch (radix) {
  case 8:
    if (places) {
      snprintf(format, 16, "0%%%do", places);
      snprintf(res, freeBytes, format, val);
    }
    else {
      snprintf(res, freeBytes, "%o", val);
    }
    break;

  case 10:
    if (places) {
      snprintf(format, 16, "%%%dd", places);
      snprintf(res, freeBytes, format, val);
    }
    else {
      snprintf(res, freeBytes, "%d", val);
    }
    break;

  case 16:
    if (places) {
      snprintf(format, 16, "0x%%0%dx", places);
      snprintf(res, freeBytes, format, val);
    }
    else {
      snprintf(res, freeBytes, "%x", val);
    }
    break;

  default:
    *res = '\0';
    break;
  }

  allocBytes(strlen(res) + 1);

  return res;
}


