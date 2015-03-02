#include <stdio.h>
#include <string.h>

#include "ADOutput.h"

ADOutput::ADOutput() :
  Device("AD Output")
{
  output = 0;
}

ADOutput::~ADOutput() {
}

byte ADOutput::peek(int addr)
{
  throw "Attempt to read from ADlx output ports";
}

void ADOutput::poke(int addr, byte b)
{
  if (addr != 0) {
    throw "Address out of range in ADlxOutputs::poke";
  }

  output = b;
}

int ADOutput::getSize()
{
  return 1;
}

bool ADOutput::getAddressName(char *str, int len, int addr)
{
  if (addr != 0) {
    throw "Address out of range in ADOutput::getAddressName";
  }

  strncpy(str, portName, len);

  return true;
}
