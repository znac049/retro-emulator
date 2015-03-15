#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "../gem.h"

#define MAXDEVNAME 128

class DeviceListener;

class Device
{
 protected:
  char name[MAXDEVNAME];;
  DeviceListener *listener;
  int size;

 public:
  Device();
  Device(const char *newName);

  const char *getName() {return name;};
  void setName(const char *newName);

  virtual bool getAddressName(char *str, int len, int addr);

  int getSize() {return size;};
  void setSize(int newSize) {size = newSize;};

  byte peek(int addr);
  void poke(int addr, byte b);

  virtual byte readByte(int addr);
  virtual void writeByte(int addr, byte b);

  bool save(const char *loc, bool overwrite);
  int load(const char *loc);

  void setListener(DeviceListener *lstnr);
  void fireReadListener(int addr);
  void fireWriteListener(int addr, byte val);
};

#endif

