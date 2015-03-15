/*
 * Instructions.h
 *
 *  Created on: 19 Feb 2015
 *      Author: bob
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "AtariEm.h"

class Instructions {
 private:
  static const char *instructionNames[];
  static const int instructionSizes[];
  static const int instructionCycles[];
  static const int addressingModes[];

 public:
  static const char *mnemonic(byte b);
  static const int mode(byte b);
  static const int size(byte b);
  static const int cycles(byte b);
};

#endif /* INSTRUCTIONS_H_ */
