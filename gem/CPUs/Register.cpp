#include <stdlib.h>
#include <string.h>

#include "Register.h"
#include "../Debug.h"

Register::Register(const char *reg, int bits)
{
  name = strdup(reg);
  numBits = bits;
  
  if (numBits > 16) {
    Debug::logf(0, "Can't cope with %d bit registers\n");
    exit(1);
  }

  mask = (1<<numBits) - 1;
  val = 0;
}

Register::~Register()
{
  free(name);
}
