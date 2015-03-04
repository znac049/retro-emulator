#include <stdio.h>
#include <string.h>

#include "R6502.h"
#include "CPUState.h"
#include "Instructions.h"

CPUState::CPUState(MemoryMap *mem) {
  memory = mem;

  reset();
}

CPUState::~CPUState() {
}

void CPUState::reset()
{
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

int CPUState::load(int addr)
{
  lastPc = pc;
  pc = addr;

  ir = memory->peek(pc);
  pc++;

  opTrap = false;

  instSize = Instructions::size(ir);
  for (int i = 0; i < instSize-1; i++) {
    args[i] = memory->peek(pc);
    pc++;
  }

  return instSize;
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
	   negativeFlag?'N':'n',
	   overflowFlag?'V':'v',
	   '-',
	   breakFlag?'B':'b',
	   decimalModeFlag?'D':'d',
	   irqDisableFlag?'I':'i',
	   zeroFlag?'Z':'z',
	   carryFlag?'C':'c');
}

void CPUState::disassembleOp(char *str, int len)
{
  const char *mnemonic = Instructions::mnemonic(ir);
  char address[256];

  if (mnemonic == NULL) {
    strncpy(str, "???", len);

    return;
  }

  switch (Instructions::mode(ir)) {
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
    snprintf(str, len, "!!!! IR=$%02x mode=%d", ir, Instructions::mode(ir));
    break;
  }
}
