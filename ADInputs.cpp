#include <stdio.h>
#include <string.h>

#include "ADInputs.h"

ADInputs::ADInputs(const char *name) :
  Device(name)
{
}

ADInputs::~ADInputs() {
}

byte ADInputs::peek(int addr)
{
  byte res;

  if ((addr < 0) || (addr >= ADINPUTS_SIZE)) {
    throw "Address out of range in ADInputs::poke";
  }

  res = inputs[addr].peek(0);
  fireReadListener(addr, res);

  return res;
}

void ADInputs::poke(int addr, byte b)
{
  throw "Attempt to write to ADLX inputs port";
}

int ADInputs::getSize()
{
  return ADINPUTS_SIZE;
}

bool ADInputs::getAddressName(char *str, int len, int addr)
{
  if ((addr < 0) || (addr >= ADLX_INPUTS_SIZE)) {
    throw "Address out of range in ADInputs::getAddressName";
  }

  strncpy(str, portNames[addr], len);

  return true;
}
