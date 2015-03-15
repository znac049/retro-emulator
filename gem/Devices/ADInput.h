#ifndef _ADINPUT_H_
#define _ADINPUT_H_

#include "Device.h"

class ADInput : public Device {
 private:
  const char *portName;
  byte lastReadState;
  byte liveReadState;

 public:
  ADInput(const char *name);
  ~ADlxInput();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();

  bool getAddressName(char *str, int len, int addr);
};
