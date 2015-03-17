/*
 * Acia6850.h
 *
 *  Created on: 17 Mar 2015
 *      Author: bob
 */

#ifndef _ACIA6850_H_
#define _ACIA6850_H_

#include "Device.h"

class Acia6850 : public Device {
private:
    bool receiveIrqEnabled = false;
    bool transmitIrqEnabled = false;
    bool overrun = false;

	long int lastTxWrite = 0;
    long int lastRxRead = 0;
    long int baudRateDelay = 0;
    int  baudRate = 0;

    int rxChar = 0;
    int txChar = 0;

    bool rxFull  = false;
    bool txEmpty = true;

    byte commandRegister;

private:
	long int calculateBaudRateDelay();
	int getBaudRate();
	void setBaudRate(int rate);
	int rxRead();
	void rxWrite(byte b);
	int txRead();
	void txWrite(byte b);
	bool hasTxChar();
	bool hasRxChar();
	void setCommandRegister(byte b);
	byte getStatusRegister();
	void reset();

public:
	Acia6850();
	~Acia6850();

	byte readByte(int addr);
	void writeByte(int addr, byte b);

	bool getAddressName(char *str, int len, int addr);
};

#endif
