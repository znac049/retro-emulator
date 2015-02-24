/*
 * StartVGDevice.cpp
 *
 *  Created on: 23 Feb 2015
 *      Author: bob
 */

#include "StartVGDevice.h"

StartVGDevice::StartVGDevice(VectorGenerator *vGen) :
	Device("Start VG")
{
	vecGen = vGen;
}

StartVGDevice::~StartVGDevice() {
}

byte StartVGDevice::peek(int addr)
{
  if (addr != 0)  {
    throw "Address out of range in StartVG";
  }

  return 0;
}

void StartVGDevice::poke(int addr, byte b)
{
  if (addr != 0) {
    throw "Address out of range in RAM Setter";
  }

  // Writing anything starts the vector generator

  vecGen->go();
}

int StartVGDevice::getSize()
{
  return 1;
}
