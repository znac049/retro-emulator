#ifndef _ADINPUTS_H_
#define _ADINPUTS_H_

#include "Device.h"
#include "ADInput.h"

#define ADINPUTS_SIZE 8

class ADInputs : public Device {
 protected:
  ADInput inputs[ADINPUTS_SIZE];

 public:
  ADInputs(const char *name);
  ~ADInputs();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();

  bool getAddressName(char *str, int len, int addr);
};

#endif
