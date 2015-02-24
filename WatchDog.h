/*
 * Atari Watchdog device.
 */

#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "Device.h"

class WatchDog : public Device
{
 private:

 public:
  WatchDog();
  ~WatchDog();

  byte peek(int addr);
  void poke(int addr, byte b);
  const char *getName();
  int getSize();

  bool getAddressName(char *str, int len, int addr);
};

#endif
