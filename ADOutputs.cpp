#include <exception>

#include <malloc.h>

#include "gem.h"
#include "ADOutputs.h"

ADOutputs::ADOutputs() : Device("ADOutputs")
{
  const char *portNames[] = {
    "Player1StartLED",
    "Player2StartLED",
    "Not_used2",
    "ShipThrustSound",
    "BankSelect",
    "LeftCoinCounter",
    "CentreCoinCounter",
    "RightCoinCounter"
  };

  for (int i=0; i<ADOUTPUTS_SIZE; i++) {
    outputs[i].setName(portNames[i]);
  }
}

ADOutputs::~ADOutputs()
{
}

byte ADOutputs::peek(int addr)
{
  throw "Attempt to read from ADlx outputs ports";

  return 0xff;
}

void ADOutputs::poke(int addr, byte b)
{
  if ((addr < 0) || (addr >= ADOUTPUTS_SIZE)) {
    throw "Address out of range in ADOutputs::poke";
  }

  outputs[addr].poke(0, b);
}

int ADOutputs::getSize()
{
  return ADOUTPUTS_SIZE;
}

