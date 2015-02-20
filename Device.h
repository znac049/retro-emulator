#ifndef _DEVICE_H
#define _DEVICE_H_

class Device
{
 public:
  Device();
  ~Device();

  virtual byte peek(int addr) {
    throw "Missing implementation of peek";
  };
  
  virtual void poke(int addr, byte b) {
    throw "Missing implementation of poke";
  };

  virtual const char *getName() { return "Abstract Device"; };
};

#endif
