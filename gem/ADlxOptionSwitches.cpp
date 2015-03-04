/*
 * ADlxOptionSwitches.cpp
 *
 *  Created on: 23 Feb 2015
 *      Author: bob
 */

#include "ADlxOptionSwitches.h"

ADlxOptionSwitches::ADlxOptionSwitches() : Device("Option Switches")
{
	optionBits = 0xff;
}

ADlxOptionSwitches::~ADlxOptionSwitches() {
}

byte ADlxOptionSwitches::peek(int addr)
{
  int bits;

  if ((addr < 0) || (addr >= 4)) {
	throw "Address out of range in ADlxOptionSwitches::poke";
  }

  bits = 6 - (addr<<1);

  return (optionBits>>bits)&0x03;
}

void ADlxOptionSwitches::poke(int addr, byte b)
{
  throw "Attempt to write to ADLX option switches port";
}

int ADlxOptionSwitches::getSize()
{
  return 4;
}
