#ifndef _DEVICE_LISTENER_H_
#define _DEVICE_LISTENER_H

#include "Device.h"

class Device;

class DeviceListener {
 public:
  virtual void readListener(Device *dev, int addr);
  virtual void writeListener(Device *dev, int addr, byte val);
};

#endif
