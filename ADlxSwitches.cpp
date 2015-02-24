#include <stdio.h>
#include <string.h>

#include "ADlxSwitches.h"

const char *ADlxSwitches::portNames[] = {
  "LeftCoinSwitch",
  "CentreCoinSwitch",
  "RightCoinSwitch",
  "Player1StartSwitch",
  "Player2StartSwitch",
  "ThrustSwitch",
  "RotateRightSwitch",
  "RotateLeftSwitch"
};

bool ADlxSwitches::lastReadState[] = {false, false, false, false, false, false, false, false};
bool ADlxSwitches::liveReadState[] = {false, false, false, false, false, false, false, false};

ADlxSwitches::ADlxSwitches() : Device("Switches")
{
}

ADlxSwitches::~ADlxSwitches()
{
}

byte ADlxSwitches::peek(int addr)
{
  if ((addr < 0) || (addr >= ADLX_SWITCHES_SIZE)) {
    throw "Address out of range in ADlxSwitches::poke";
  }

  if (lastReadState[addr] != liveReadState[addr]) {
    printf("%s %s\n", portNames[addr], liveReadState[addr]?"Pressed":"Released");
    lastReadState[addr] = liveReadState[addr];
  }

  return lastReadState[addr]?0x80:0;
}

void ADlxSwitches::poke(int addr, byte b)
{
  throw "Attempt to write to ADLX switches port";
}

int ADlxSwitches::getSize()
{
  return ADLX_SWITCHES_SIZE;
}

void ADlxSwitches::press(int addr)
{
  if ((addr < 0) || (addr >= ADLX_SWITCHES_SIZE)) {
    throw "Address out of range in ADlxSwitches::press";
  }

  liveReadState[addr] = true;
}

void ADlxSwitches::release(int addr)
{
  if ((addr < 0) || (addr >= ADLX_SWITCHES_SIZE)) {
    throw "Address out of range in ADlxSwitches::release";
  }

  liveReadState[addr] = true;
}

bool ADlxSwitches::getAddressName(char *str, int len, int addr)
{
  if ((addr < 0) || (addr >= ADLX_SWITCHES_SIZE)) {
    throw "Address out of range in ADlxSwitches::getAddressName";
  }

  strncpy(str, portNames[addr], len);

  return true;
}
