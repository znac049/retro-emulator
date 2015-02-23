#include <exception>

#include <malloc.h>

#include "AtariEm.h"
#include "ADlxDigitalOutputs.h"

const char *ADlxDigitalOutputs::portNames[] = {
  "1 Player Start LED",
  "2 Player Start LED",
  "Not used!",
  "Ship Thrust Sound",
  "Bank Select",
  "Left Coin Counter",
  "Centre Coin Counter",
  "Right Coin Counter"
};

ADlxDigitalOutputs::ADlxDigitalOutputs() : Device("ADlxDigOutputs")
{
  for (int i=0; i<ADLX_DIG_OP_SIZE; i++) {
    outputs[i] = false;
  }
}

ADlxDigitalOutputs::~ADlxDigitalOutputs()
{
}

byte ADlxDigitalOutputs::peek(int addr)
{
  throw "Attempt to read from ADlx output ports";

  return 0xff;
}

void ADlxDigitalOutputs::poke(int addr, byte b)
{
  if ((addr < 0) || (addr >= ADLX_DIG_OP_SIZE)) {
    throw "Address out of range in ADlxDigitalOutputs::poke";
  }

  b = (b==0)?false:true;

  if (b != outputs[addr]) {
    printf("%s -> %s\n", portNames[addr], b?"ON":"OFF");

    outputs[addr] = b;
  }
}

int ADlxDigitalOutputs::getSize()
{
  return ADLX_DIG_OP_SIZE;
}

