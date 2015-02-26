#include <stdio.h>
#include <string.h>

#include "ADlxInputs.h"

const char *ADlxInputs::portNames[] = {
  "Not_Used0",
  "3KHz",
  "VGHalted",
  "ShieldSwitch",
  "FireSwitch",
  "NotUsed5",
  "SlamSwitch",
  "SelfTestSwitch"
};

bool ADlxInputs::lastReadState[] = {false, false, false, false, false, false, false, false};
bool ADlxInputs::liveReadState[] = {false, false, false, false, false, false, false, false};

ADlxInputs::ADlxInputs(VectorGenerator *vGen) :
  Device("DLX Inputs")
{
  vecGen = vGen;

  for (int i=0; i<ADLX_INPUTS_SIZE; i++) {
    lastReadState[i] = liveReadState[i] = false;
  }
}

ADlxInputs::~ADlxInputs() {
}

byte ADlxInputs::peek(int addr)
{
  if ((addr < 0) || (addr >= ADLX_INPUTS_SIZE)) {
    throw "Address out of range in ADlxInputs::poke";
  }

  switch (addr) {
  case 2:	//VG Halted
    liveReadState[addr] = vecGen->isHalted();
    break;
  }

  if (lastReadState[addr] != liveReadState[addr]) {
    printf("%s %s\n", portNames[addr], liveReadState[addr]?"Set":"Cleared");
    lastReadState[addr] = liveReadState[addr];
  }

  return lastReadState[addr]?0x80:0;
}

void ADlxInputs::poke(int addr, byte b)
{
  throw "Attempt to write to ADLX inputs port";
}

int ADlxInputs::getSize()
{
  return ADLX_INPUTS_SIZE;
}

void ADlxInputs::flip(int addr)
{
  if ((addr < 0) || (addr >= ADLX_INPUTS_SIZE)) {
    throw "Address out of range in ADlxInputs::press";
  }

  liveReadState[addr] = !liveReadState[addr];
}

void ADlxInputs::press(int addr)
{
  if ((addr < 0) || (addr >= ADLX_INPUTS_SIZE)) {
    throw "Address out of range in ADlxInputs::press";
  }

  liveReadState[addr] = true;
}

void ADlxInputs::release(int addr)
{
  if ((addr < 0) || (addr >= ADLX_INPUTS_SIZE)) {
    throw "Address out of range in ADlxInputs::release";
  }

  liveReadState[addr] = true;
}

bool ADlxInputs::getAddressName(char *str, int len, int addr)
{
  if ((addr < 0) || (addr >= ADLX_INPUTS_SIZE)) {
    throw "Address out of range in ADlxInputs::getAddressName";
  }

  strncpy(str, portNames[addr], len);

  return true;
}
