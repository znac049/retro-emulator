#ifndef ADLXSWITCHES_H_
#define ADLXSWITCHES_H_

#include "Device.h"

#define ADLX_SWITCHES_SIZE 8

class ADlxSwitches :public Device {
 private:
  static const char *portNames[];
  static bool lastReadState[];
  static bool liveReadState[];

 public:
  ADlxSwitches();
  ~ADlxSwitches();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();

  static void press(int addr);
  static void release(int addr);

  bool getAddressName(char *str, int len, int addr);
};

#endif /* ADLXSWITCHES_H_ */
