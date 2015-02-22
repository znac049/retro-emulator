#ifndef _ADLX_DIG_OP_H_
#define _ADLX_DIG_OP_H_

#include "Device.h"

#define ADLX_DIG_OP_SIZE 8

class ADlxDigitalOutputs : public Device
{
 private:
  static const char *portNames[];
  bool outputs[ADLX_DIG_OP_SIZE];

 public:
  ADlxDigitalOutputs();
  ~ADlxDigitalOutputs();

  byte peek(int addr);
  void poke(int addr, byte b);
  const char *getName();
  int getSize();
};

#endif
