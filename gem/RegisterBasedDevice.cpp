#include "RegisterBasedDevice.h"

RegisterBasedDevice::RegisterBasedDevice(const char *name, int numRegisters) :
	RegisterBasedDevice(name, numRegisters, true, true)
{
}

RegisterBasedDevice::RegisterBasedDevice(const char *name, int numRegisters, bool canRead, bool canWrite) :
	Device()
{
	setName(name);
	registers = new byte[numRegisters];

	readable = canRead;
	writeable = canWrite;
}

