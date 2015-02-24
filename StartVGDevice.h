#include "Device.h"
#include "VectorGenerator.h"

#ifndef STARTVGDEVICE_H_
#define STARTVGDEVICE_H_

class StartVGDevice: public Device {
private:
	VectorGenerator *vecGen;

public:
	StartVGDevice(VectorGenerator *vGen);
	~StartVGDevice();

	byte peek(int addr);
	void poke(int addr, byte b);

	int getSize();
};

#endif /* STARTVGDEVICE_H_ */
