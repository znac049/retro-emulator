#include <exception>

#include <malloc.h>

#include "AtariEm.h"
#include "ADlxOutputs.h"

const char *ADlxOutputs::portNames[] = {
  "Player1StartLED",
  "Player2StartLED",
  "Not_used2",
  "ShipThrustSound",
  "BankSelect",
  "LeftCoinCounter",
  "CentreCoinCounter",
  "RightCoinCounter"
};

ADlxOutputs::ADlxOutputs() : Device("ADlxOutputs")
{
  for (int i=0; i<ADLX_OUTPUTS_SIZE; i++) {
    outputs[i] = false;
  }
}

ADlxOutputs::~ADlxOutputs()
{
}

byte ADlxOutputs::peek(int addr)
{
  throw "Attempt to read from ADlx output ports";

  return 0xff;
}

void ADlxOutputs::poke(int addr, byte b)
{
  if ((addr < 0) || (addr >= ADLX_OUTPUTS_SIZE)) {
    throw "Address out of range in ADlxOutputs::poke";
  }

  b = (b==0)?false:true;

  if (b != outputs[addr]) {
    printf("%s -> %s\n", portNames[addr], b?"ON":"OFF");

    outputs[addr] = b;
  }
}

int ADlxOutputs::getSize()
{
  return ADLX_OUTPUTS_SIZE;
}

