#ifndef ADLXOPTIONSWITCHES_H_
#define ADLXOPTIONSWITCHES_H_

#include "Device.h"

class ADlxOptionSwitches : public Device {
 private:
  byte optionBits;

 public:
  ADlxOptionSwitches();
  ~ADlxOptionSwitches();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();
};

#endif /* ADLXOPTIONSWITCHES_H_ */
