#include <stdio.h>
#include <string.h>

#include "AtariEm.h"
#include "MemorySpace.h"
#include "MemoryDevice.h"

int main(int argc, char *argv[])
{
  MemoryDevice ram(2048, 0);

  printf("Hello\n");

  try {
    for (int i=0; i<2048; i++) {
      ram.poke(i, i & 0xff);
    }

    for (int i=0; i<2048; i++) {
      if (ram.peek(i) != (i & 0xff)) {
	printf("Error at address $04x\n", i);
      }
    }
  }
  catch (const char *msg) {
    printf("Caught exception: '%s'\n", msg);
  }

  return 0;
}
