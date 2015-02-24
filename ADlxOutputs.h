#ifndef _ADLX_OUTPUTS_H_
#define _ADLX_OUTPUTS_H_

#include "Device.h"

#define ADLX_OUTPUTS_SIZE 8

class ADlxOutputs : public Device
{
 private:
  static const char *portNames[];
  bool outputs[ADLX_OUTPUTS_SIZE];

 public:
  ADlxOutputs();
  ~ADlxOutputs();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();
};

#endif
