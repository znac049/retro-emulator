#include <exception>

#include <malloc.h>

#include "AtariEm.h"
#include "Pokey.h"

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
