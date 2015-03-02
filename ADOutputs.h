#ifndef _ADLX_OUTPUTS_H_
#define _ADLX_OUTPUTS_H_

#include "Device.h"
#include "ADOutput.h"

#define ADOUTPUTS_SIZE 8

class ADOutputs : public Device
{
 private:
  ADOutput outputs[ADOUTPUTS_SIZE];

 public:
  ADOutputs();
  ~ADOutputs();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();
};

#endif
