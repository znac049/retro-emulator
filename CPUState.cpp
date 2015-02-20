/*
 * CPUState.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: bob
 */

#include <stdio.h>
#include <string.h>

#include "CPU.h"
#include "CPUState.h"
#include "Instructions.h"

CPUState::CPUState(MemorySpace *mem) {
  memory = mem;

  a = 0;

  x = y = 0;

  sp = 0xff;

  pc = memory->peekw(RST_VECTOR_L);

  ir = 0;
  lastPc = 0;

  instSize = 0;

  opTrap = 0;
  irqAsserted = nmiAsserted = false;

  carryFlag = false;
  negativeFlag = false;
  zeroFlag = false;
  irqDisableFlag = false;
  decimalModeFlag = false;
  breakFlag = false;
  overflowFlag = false;

  stepCounter = 0L;
}

CPUState::~CPUState() {
}

byte CPUState::getStatusFlag()
{
  byte status = 0x20; /* Bit 5 is always set */

  if (carryFlag) {
    status |= P_CARRY;
  }

  if (zeroFlag) {
    status |= P_ZERO;
  }

  if (irqDisableFlag) {
    status |= P_IRQ_DISABLE;
  }

  if (decimalModeFlag) {
    status |= P_DECIMAL;
  }

  if (breakFlag) {
    status |= P_BREAK;
  }

  if (overflowFlag) {
    status |= P_OVERFLOW;
  }

  if (negativeFlag) {
    status |= P_NEGATIVE;
  }

  return status;
}

void CPUState::getStatusFlagAsString(char *str, int len)
{
  snprintf(str, len, "[%c%c%c%c%c%c%c%c]",
	   negativeFlag?'N':'.',
	   overflowFlag?'V':'.',
	   '-',
	   breakFlag?'B':'.',
	   decimalModeFlag?'D':'.',
	   irqDisableFlag?'I':'.',
	   zeroFlag?'Z':'.',
	   carryFlag?'C':'.');
}

void CPUState::disassembleOp(char *str, int len)
{
  const char *mnemonic = Instructions::mnemonic(ir);

  if (mnemonic == NULL) {
    strncpy(str, "???", len);

    return;
  }

  switch (Instructions::mode(ir)) {
  case MODE_ABS:
    snprintf(str, len, "%s $%04x", mnemonic, argsw());
    break;

  case MODE_ABX:
    snprintf(str, len, "%s $%04x,X", mnemonic, argsw());
    break;

  case MODE_ABY:
    snprintf(str, len, "%s $%04x,Y", mnemonic, argsw());
    break;

  case MODE_IMM:
    snprintf(str, len, "%s #$%02x", mnemonic, args[0]);
    break;

  case MODE_IND:
    snprintf(str, len, "%s ($%04x)", mnemonic, argsw());
    break;

  case MODE_INX:
    snprintf(str, len, "%s ($%02x,X)", mnemonic, args[0]);
    break;

  case MODE_INY:
    snprintf(str, len, "%s ($%02x),Y", mnemonic, args[0]);
    break;

  case MODE_REL:
    snprintf(str, len, "%s $02x", mnemonic, args[0]);
    break;

  case MODE_ZPG:
    snprintf(str, len, "%s $02x", mnemonic, args[0]);
    break;

  case MODE_ZPX:
    snprintf(str, len, "%s $02x,X", mnemonic, args[0]);
    break;

  case MODE_ZPY:
    snprintf(str, len, "%s $02x,Y", mnemonic, args[0]);
    break;
  }
}

/*
  public String getInstructionByteStatus() {
  switch (Cpu.instructionSizes[ir]) {
  case 0:
  case 1:
  return HexUtil.wordToHex(lastPc) + "  " +
  HexUtil.byteToHex(ir) + "      ";
  case 2:
  return HexUtil.wordToHex(lastPc) + "  " +
  HexUtil.byteToHex(ir) + " " +
  HexUtil.byteToHex(args[0]) + "   ";
  case 3:
  return HexUtil.wordToHex(lastPc) + "  " +
  HexUtil.byteToHex(ir) + " " +
  HexUtil.byteToHex(args[0]) + " " +
  HexUtil.byteToHex(args[1]);
  default:
  return null;
  }
  }


  }
*/
