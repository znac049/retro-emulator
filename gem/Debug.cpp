#include <stdio.h>
#include <stdarg.h>

#include "Debug.h"

int Debug::debugLevel = 0;

int Debug::setLevel(int level)
{
  int oldLevel = debugLevel;

  if (level > 99) {
    level = 99;
  }

  debugLevel = level;

  return oldLevel;
}

void Debug::logf(int level, const char *fmt, ...)
{
  if (level <= debugLevel) {
    va_list args;

    printf("[Dbg: %d] ", level);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
  }
}
