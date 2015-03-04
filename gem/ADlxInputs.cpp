#include <stdio.h>
#include <string.h>

#include "ADlxInputs.h"

const char *ADlxInputs::portNames[] = {
  "Not_Used0",
  "3KHz",
  "VGHalted",
  "ShieldSw",
  "FireSw",
  "DiagStep",
  "SlamSw",
  "SelfTestSw"
};

bool ADlxInputs::lastReadState[] = {true, true, true, true, true, true, true, true};
bool ADlxInputs::liveReadState[] = {true, true, true, true, true, true, true, true};

ADlxInputs::ADlxInputs(VectorGenerator *vGen) :
  Device("Switches (SINP0)")
{
  vecGen = vGen;

  for (int i=0; i<ADLX_INPUTS_SIZE; i++) {
    lastReadState[i] = liveReadState[i] = true;
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

  liveReadState[addr] = false;
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
