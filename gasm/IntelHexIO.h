#ifndef _INTEL_HEX_H_
#define _INTEL_HEX_H_

#include <stdio.h>

#define MAX_IHEX_LINE 524

class IntelHex
{
 private:
  static int hexByte(const char *buf);

 public:
  static bool writeBytes(FILE *fd, char *buffer, int numBytes, int address);
  static int readBytes(FILE *fd, char *buffer, int bufLen);
};

#endif
