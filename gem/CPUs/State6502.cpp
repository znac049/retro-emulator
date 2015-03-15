#include <stdio.h>
#include <string.h>

#include "State6502.h"
#include "../MemoryMap.h"

#include "6502.inst"

State6502::State6502(MemoryMap *mem) : CPUState(mem) {
  instructionNames = _instructionNames;
  instructionSizes = _instructionSizes;
  instructionCycles = _instructionSizes;
  addressingModes = _addressingModes;

  reset();
}

State6502::~State6502() {
}

void State6502::reset()
{
  CPUState::reset();

  a = 0;
  x = y = 0;

  sp = 0xff;

  pc = memory->peekw(RST_VECTOR_L);
  ir = memory->peek(pc);

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
  running = false;
}

int State6502::load(int addr)
{
  lastPc = pc;
  pc = addr;

  ir = memory->peek(pc);
  pc++;

  opTrap = false;

  instSize = getInstructionSize();
  for (int i = 0; i < instSize-1; i++) {
    args[i] = memory->peek(pc);
    pc++;
  }

  return instSize;
}

byte State6502::getStatusFlag()
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

void State6502::getStatusFlagAsString(char *str, int len)
{
  snprintf(str, len, "[%c%c%c%c%c%c%c%c]",
	   negativeFlag?'N':'n',
	   overflowFlag?'V':'v',
	   '-',
	   breakFlag?'B':'b',
	   decimalModeFlag?'D':'d',
	   irqDisableFlag?'I':'i',
	   zeroFlag?'Z':'z',
	   carryFlag?'C':'c');
}

void State6502::disassembleOp(char *str, int len)
{
  const char *mnemonic = getInstructionName();
  char address[256];

  if (mnemonic == NULL) {
    strncpy(str, "???", len);

    return;
  }

  switch (getAddressingMode()) {
  case MODE_ABS:
    memory->getAddressName(address, sizeof(address), argsw());
    snprintf(str, len, "%s %s", mnemonic, address);
    break;

  case MODE_ABX:
    memory->getAddressName(address, sizeof(address), argsw());
    snprintf(str, len, "%s %s,X", mnemonic, address);
    break;

  case MODE_ABY:
    memory->getAddressName(address, sizeof(address), argsw());
    snprintf(str, len, "%s %s,Y", mnemonic, address);
    break;

  case MODE_IMM:
    snprintf(str, len, "%s #$%02x", mnemonic, args[0]);
    break;

  case MODE_IND:
    memory->getAddressName(address, sizeof(address), argsw());
    snprintf(str, len, "%s (%s)", mnemonic, address);
    break;

  case MODE_INX:
    memory->getAddressName(address, sizeof(address), args[0]);
    snprintf(str, len, "%s (%s,X)", mnemonic, address);
    break;

  case MODE_INY:
    memory->getAddressName(address, sizeof(address), args[0]);
    snprintf(str, len, "%s (%s),Y", mnemonic, address);
    break;

  case MODE_REL:
    memory->getAddressName(address, sizeof(address), pc + (signed char)args[0]);
    snprintf(str, len, "%s %s", mnemonic, address);
    break;

  case MODE_ZPG:
    memory->getAddressName(address, sizeof(address), args[0]);
    snprintf(str, len, "%s %s", mnemonic, address);
    break;

  case MODE_ZPX:
    memory->getAddressName(address, sizeof(address), args[0]);
    snprintf(str, len, "%s %s,X", mnemonic, address);
    break;

  case MODE_ZPY:
    memory->getAddressName(address, sizeof(address), args[0]);
    snprintf(str, len, "%s %s,Y", mnemonic, address);
    break;

  case MODE_IMP:
    snprintf(str, len, "%s", mnemonic);
    break;

  case MODE_ACC:
    snprintf(str, len, "%s", mnemonic);
    break;

  default:
    snprintf(str, len, "!!!! IR=$%02x mode=%d", ir, getAddressingMode());
    break;
  }
}
