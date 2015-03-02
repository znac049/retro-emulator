#ifndef _ADOUTPUT_H_
#define _ADOUTPUT_H_

#include "Device.h"

class ADOutput : public Device {
 private:
  const char *portName;
  byte output;

 public:
  ADOutput();
  ~ADlxOutput();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();
};
