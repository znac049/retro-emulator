#ifndef _REGISTERBASEDDEVICE_H_
#define _REGISTERBASEDDEVICE_H_

#include "Device.h"

class RegisterBasedDevice : public Device {
protected:
	bool readable;
	bool writeable;

	byte registers[];

public:
	RegisterBasedDevice(const char *name, int numRegisters);
	RegisterBasedDevice(const char *name, int numRegisters, bool canRead, bool canWrite);
};

#endif
