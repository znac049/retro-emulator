#include <exception>

#include <string.h>
#include <malloc.h>

#include "gem.h"
#include "Pokey.h"

const char *Pokey::portNames[] = {
  "PK_POT0_AUDF1",
  "PK_POT1_AUDC1",
  "PK_POT2_AUDF2",
  "PK_POT3_AUDC2",
  "PK_POT4_AUDF3",
  "PK_POT5_AUDC3",
  "PK_POT6_AUDF4",
  "PK_POT7_AUDC4",
  "PK_ALLPOT_AUDCTL",
  "PK_KBCODE_STIMER",
  "PK_RND_SKREG",
  "PK_POTGO",
  "PK_UNSUSED",
  "PK_SIN_SOUT",
  "PK_IRQST_IRQEN",
  "PK_SKSTAT_SKCTLS"
};

Pokey::Pokey() : Device("Pokey")
{
}

Pokey::~Pokey()
{
}

byte Pokey::peek(int addr)
{
  if ((addr < 0) || (addr >= POKEY_SIZE)) {
    throw "Address out of range in Pokey::peek";
  }

  return 42;;
}

void Pokey::poke(int addr, byte b)
{
  if ((addr < 0) || (addr >= POKEY_SIZE)) {
    throw "Address out of range in Pokey::poke";
  }
}

int Pokey::getSize()
{
  return POKEY_SIZE;
}

bool Pokey::getAddressName(char *str, int len, int addr)
{
  if ((addr < 0) || (addr >= POKEY_SIZE)) {
    throw "Address out of range in Pokey::getAddressName";
  }

  strncpy(str, portNames[addr], len);

  return true;
}
