#include <stdio.h>
#include <string.h>

#include "ADSin1.h"

ADSin1::ADSin1() :
  ADSin1("Switches (SINP1)")
{
  const char *portNames[] = {
    "LeftCoinSw",
    "CentreCoinSw",
    "RightCoinSw",
    "Player1StartSw",
    "Player2StartSw",
    "ThrustSw",
    "RotateRightSw",
    "RotateLeftSw"
  };

  for (int i=0; i<ADINPUTS_SIZE; i++) {
    inputs[i].setName(portNames[i]);
  }
}
