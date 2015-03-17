/*
 * Acia6850.cpp
 *
 *  Created on: 17 Mar 2015
 *      Author: bob
 */

#include "Acia6850.h"

Acia6850::Acia6850() : Device("6850 Acia") {
	setSize(2);

    receiveIrqEnabled = false;
    transmitIrqEnabled = false;
    overrun = false;

	lastTxWrite = 0;
    lastRxRead = 0;

    rxChar = 0;
    txChar = 0;

    rxFull  = false;
    txEmpty = true;

    setBaudRate(2400);
}

Acia6850::~Acia6850() {
}

byte Acia6850::readByte(int addr)
{
	byte res;

	switch (addr) {
	case 0: // Status Register
		res = getStatusRegister();
		break;

	case 1: // RX Register
		res = rxRead();
		break;
	}

	return res;
}

void Acia6850::writeByte(int addr, byte b)
{
	switch (addr) {
	case 0: // Control Register
		setCommandRegister(b);
		break;

	case 1: // TX Register
		txWrite(b);
		break;
	}
}

bool Acia6850::getAddressName(char *str, int len, int addr)
{
	if (addr) {
		strncpy(str, "RX_TX_REG", len);
	}
	else {
		strncpy(str, "CTRL_STATUS_REG", len);
	}

	return true;
}

long int Acia6850::calculateBaudRateDelay()
{
	if (baudRate > 0) {
		// TODO: This is a pretty rough approximation based on 8 bits per character,
		// and 1/baudRate per bit. It could certainly be improved
		return (long)((1.0 / baudRate) * 1000000000 * 8);
	} else {
		return 0;
	}
}

int Acia6850::getBaudRate()
{
	return baudRate;
}

void Acia6850::setBaudRate(int rate)
{
	baudRate = rate;
	baudRateDelay = calculateBaudRateDelay();
}

int Acia6850::rxRead()
{
	lastRxRead = Timer::getNanoTicks();

	overrun = false;
	rxFull = false;
	return rxChar;
}

void Acia6850::rxWrite(byte b)
{
	if (rxFull) {
		overrun = true;
	}

	rxFull = true;

	if (receiveIrqEnabled) {
		// TODO raise IRQ
	}

	rxChar = b;
}

int Acia6850::txRead()
{
	txEmpty = true;

	if (transmitIrqEnabled) {
		// TODO raise IRQ
	}

	return txChar;
}

void Acia6850::txWrite(byte b)
{
	lastTxWrite = Timer::getNanoTicks();

	txChar = b;
	txEmpty = false;
}

bool Acia6850::hasTxChar()
{
	return !txEmpty;
}

bool Acia6850::hasRxChar()
{
    return rxFull;
}

void Acia6850::setCommandRegister(byte b)
{
	commandRegister = b;

	// Bits 0 & 1 control the master reset
	if((commandRegister & 0x01) != 0 && (commandRegister & 0x02) != 0) {
		reset();
	}

	// Bit 7 controls receiver IRQ behavior
	receiveIrqEnabled = (commandRegister & 0x80) != 0;
	// Bits 5 & 6 controls transmit IRQ behavior
	transmitIrqEnabled = (commandRegister & 0x20) != 0 && (commandRegister & 0x40) == 0;
}

byte Acia6850::getStatusRegister()
{
	// TODO: Parity Error, Framing Error, DTR, DSR, and Interrupt flags.
	int res = 0;
	long int now = Timer::getNanoTicks();

	if (rxFull && now >= (lastRxRead + baudRateDelay)) {
		res |= 0x01;
	}

	if (txEmpty && now >= (lastTxWrite + baudRateDelay)) {
		res |= 0x02;
	}
	if (overrun) {
		res |= 0x20;
	}

	return res;
}

void Acia6850::reset()
{
	overrun = false;
	rxFull = false;
	txEmpty = true;
}
