#include <stdio.h>
#include <string.h>

#include "ADInput.h"

ADInput::ADInput(const char *name) :
  Device("AD Input")
{
  setName(name);
  lastReadState = liveReadState = 0
}

ADInput::~ADInput() {
}

byte ADInput::peek(int addr)
{
  if (addr != 0) {
    throw "Address out of range in ADInput::poke";
  }

  return lastReadState[addr]?0x80:0;
}

void ADInput::poke(int addr, byte b)
{
  throw "Attempt to write to ADLX input port";
}

int ADInput::getSize()
{
  return 1;
}

bool ADInput::getAddressName(char *str, int len, int addr)
{
  if (addr != 0) {
    throw "Address out of range in ADInput::getAddressName";
  }

  strncpy(str, portName, len);

  return true;
}
