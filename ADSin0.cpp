#include <stdio.h>
#include <string.h>

#include "ADSin0.h"

ADSin0::ADSin0() :
  ADSin0("Switches (SINP0)")
{
  const char *portNames[] = {
    "Not_Used0",
    "3KHz",
    "VGHalted",
    "ShieldSw",
    "FireSw",
    "DiagStep",
    "SlamSw",
    "SelfTestSw"
  };

  for (int i=0; i<ADINPUTS_SIZE; i++) {
    inputs[i].setName(portNames[i]);
  }
}
