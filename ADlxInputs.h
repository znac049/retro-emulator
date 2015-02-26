#ifndef ADLXINPUTS_H_
#define ADLXINPUTS_H_

#include "Device.h"
#include "VectorGenerator.h"

#define ADLX_INPUTS_SIZE 8

class ADlxInputs : public Device {
 private:
  static const char *portNames[];
  static bool lastReadState[];
  static bool liveReadState[];

  VectorGenerator *vecGen;

 public:
  ADlxInputs(VectorGenerator *vGen);
  ~ADlxInputs();

  byte peek(int addr);
  void poke(int addr, byte b);
  int getSize();

  static void flip(int addr);
  static void press(int addr);
  static void release(int addr);

  bool getAddressName(char *str, int len, int addr);
};

#endif /* ADLXINPUTS_H_ */
