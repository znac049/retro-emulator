#include <stdio.h>
#include <string.h>
#include <time.h>

#include "CPU6502.h"
#include "../MemoryMap.h"

#include "6502.inst"

CPU6502::CPU6502(MemoryMap *mem) : CPU(mem) {
  behaviour = NMOS_WITH_INDIRECT_JMP_BUG;

  instructionNames = _instructionNames;
  instructionSizes = _instructionSizes;
  instructionCycles = _instructionSizes;
  addressingModes = _addressingModes;

  reset();
}

CPU6502::~CPU6502() {
}

void CPU6502::reset()
{
  CPU::reset();

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

int CPU6502::load(int addr)
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

void CPU6502::checkInterrupts()
{
  if (nmiAsserted) {
    handleNmi();
  } else if (irqAsserted && !getIrqDisableFlag()) {
    handleIrq(pc);
  }
}

void CPU6502::executeInstruction()
{
  irAddressMode = (ir >> 2) & 0x07;
  irOpMode = ir & 0x03;

  effectiveAddress = 0;

  clearOpTrap();

  switch (irOpMode) {
  case 0:
  case 2:
    switch (irAddressMode) {
    case 0: // #Immediate
      break;
    case 1: // Zero Page
      effectiveAddress = args[0];
      break;
    case 2: // Accumulator - ignored
      break;
    case 3: // Absolute
      effectiveAddress = address(args[0], args[1]);
      break;
    case 5: // Zero Page,X / Zero Page,Y
      if (ir == 0x96 || ir == 0xb6) {
	effectiveAddress = zpyAddress(args[0]);
      } else {
	effectiveAddress = zpxAddress(args[0]);
      }
      break;
    case 7: // Absolute,X / Absolute,Y
      if (ir == 0xbe) {
	effectiveAddress = yAddress(args[0], args[1]);
      } else {
	effectiveAddress = xAddress(args[0], args[1]);
      }
      break;
    }
    break;
  case 1:
    switch (irAddressMode) {
    case 0: // (Zero Page,X)
      tmp = (args[0] + x) & 0xff;
      effectiveAddress = address(memory->peek(tmp), memory->peek(tmp + 1));
      break;
    case 1: // Zero Page
      effectiveAddress = args[0];
      break;
    case 2: // #Immediate
      effectiveAddress = -1;
      break;
    case 3: // Absolute
      effectiveAddress = address(args[0], args[1]);
      break;
    case 4: // (Zero Page),Y
      tmp = address(memory->peek(args[0]),
		    memory->peek((args[0] + 1) & 0xff));
      effectiveAddress = (tmp + y) & 0xffff;
      break;
    case 5: // Zero Page,X
      effectiveAddress = zpxAddress(args[0]);
      break;
    case 6: // Absolute, Y
      effectiveAddress = yAddress(args[0], args[1]);
      break;
    case 7: // Absolute, X
      effectiveAddress = xAddress(args[0], args[1]);
      break;
    }
    break;
  }

  // Execute
  switch (ir) {

    /** Single Byte Instructions; Implied and Relative **/
  case 0x00: // BRK - Force Interrupt - Implied
    if (!getIrqDisableFlag()) {
      handleIrq(pc + 1);
    }
    break;
  case 0x08: // PHP - Push Processor Status - Implied
    // Break flag is always set in the stack value.
    stackPush(getStatusFlag() | 0x10);
    break;
  case 0x10: // BPL - Branch if Positive - Relative
    if (!getNegativeFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0x18: // CLC - Clear Carry Flag - Implied
    clearCarryFlag();
    break;
  case 0x20: // JSR - Jump to Subroutine - Implied
    stackPush((pc - 1 >> 8) & 0xff); // PC high byte
    stackPush(pc - 1 & 0xff);        // PC low byte
    pc = address(args[0], args[1]);
    break;
  case 0x28: // PLP - Pull Processor Status - Implied
    setProcessorStatus(stackPop());
    break;
  case 0x30: // BMI - Branch if Minus - Relative
    if (getNegativeFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0x38: // SEC - Set Carry Flag - Implied
    setCarryFlag();
    break;
  case 0x40: // RTI - Return from Interrupt - Implied
    setProcessorStatus(stackPop());
    lo = stackPop();
    hi = stackPop();
    setProgramCounter(address(lo, hi));
    break;
  case 0x48: // PHA - Push Accumulator - Implied
    stackPush(a);
    break;
  case 0x50: // BVC - Branch if Overflow Clear - Relative
    if (!getOverflowFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0x58: // CLI - Clear Interrupt Disable - Implied
    clearIrqDisableFlag();
    break;
  case 0x60: // RTS - Return from Subroutine - Implied
    lo = stackPop();
    hi = stackPop();
    setProgramCounter((address(lo, hi) + 1) & 0xffff);
    break;
  case 0x68: // PLA - Pull Accumulator - Implied
    a = stackPop();
    setArithmeticFlags(a);
    break;
  case 0x70: // BVS - Branch if Overflow Set - Relative
    if (getOverflowFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0x78: // SEI - Set Interrupt Disable - Implied
    setIrqDisableFlag();
    break;
  case 0x88: // DEY - Decrement Y Register - Implied
    y = --y & 0xff;
    setArithmeticFlags(y);
    break;
  case 0x8a: // TXA - Transfer X to Accumulator - Implied
    a = x;
    setArithmeticFlags(a);
    break;
  case 0x90: // BCC - Branch if Carry Clear - Relative
    if (!getCarryFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0x98: // TYA - Transfer Y to Accumulator - Implied
    a = y;
    setArithmeticFlags(a);
    break;
  case 0x9a: // TXS - Transfer X to Stack Pointer - Implied
    setStackPointer(x);
    break;
  case 0xa8: // TAY - Transfer Accumulator to Y - Implied
    y = a;
    setArithmeticFlags(y);
    break;
  case 0xaa: // TAX - Transfer Accumulator to X - Implied
    x = a;
    setArithmeticFlags(x);
    break;
  case 0xb0: // BCS - Branch if Carry Set - Relative
    if (getCarryFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0xb8: // CLV - Clear Overflow Flag - Implied
    clearOverflowFlag();
    break;
  case 0xba: // TSX - Transfer Stack Pointer to X - Implied
    x = getStackPointer();
    setArithmeticFlags(x);
    break;
  case 0xc8: // INY - Increment Y Register - Implied
    y = ++y & 0xff;
    setArithmeticFlags(y);
    break;
  case 0xca: // DEX - Decrement X Register - Implied
    x = --x & 0xff;
    setArithmeticFlags(x);
    break;
  case 0xd0: // BNE - Branch if Not Equal to Zero - Relative
    if (!getZeroFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0xd8: // CLD - Clear Decimal Mode - Implied
    clearDecimalModeFlag();
    break;
  case 0xe8: // INX - Increment X Register - Implied
    x = ++x & 0xff;
    setArithmeticFlags(x);
    break;
  case 0xea: // NOP
    // Do nothing.
    break;
  case 0xf0: // BEQ - Branch if Equal to Zero - Relative
    if (getZeroFlag()) {
      pc = relAddress(args[0]);
    }
    break;
  case 0xf8: // SED - Set Decimal Flag - Implied
    setDecimalModeFlag();
    break;

    /** JMP *****************************************************************/
  case 0x4c: // JMP - Absolute
    pc = address(args[0], args[1]);
    break;
  case 0x6c: // JMP - Indirect
    lo = address(args[0], args[1]); // Address of low byte

    if (args[0] == 0xff &&
	(behaviour == NMOS_WITH_INDIRECT_JMP_BUG ||
	 behaviour == NMOS_WITH_ROR_BUG)) {
      hi = address(0x00, args[1]);
    } else {
      hi = lo + 1;
    }

    pc = address(memory->peek(lo), memory->peek(hi));
    /* TODO: For accuracy, allow a flag to enable broken behavior of early 6502s:
     *
     * "An original 6502 has does not correctly fetch the target
     * address if the indirect vector falls on a page boundary
     * (e.g. $xxFF where xx is and value from $00 to $FF). In this
     * case fetches the LSB from $xxFF as expected but takes the MSB
     * from $xx00. This is fixed in some later chips like the 65SC02
     * so for compatibility always ensure the indirect vector is not
     * at the end of the page."
     * (http://www.obelisk.demon.co.uk/6502/reference.html#JMP)
     */
    break;


    /** ORA - Logical Inclusive Or ******************************************/
  case 0x09: // #Immediate
    a |= args[0];
    setArithmeticFlags(a);
    break;
  case 0x01: // (Zero Page,X)
  case 0x05: // Zero Page
  case 0x0d: // Absolute
  case 0x11: // (Zero Page),Y
  case 0x15: // Zero Page,X
  case 0x19: // Absolute,Y
  case 0x1d: // Absolute,X
    a |= memory->peek(effectiveAddress);
    setArithmeticFlags(a);
    break;


    /** ASL - Arithmetic Shift Left *****************************************/
  case 0x0a: // Accumulator
    a = asl(a);
    setArithmeticFlags(a);
    break;
  case 0x06: // Zero Page
  case 0x0e: // Absolute
  case 0x16: // Zero Page,X
  case 0x1e: // Absolute,X
    tmp = asl(memory->peek(effectiveAddress));
    memory->poke(effectiveAddress, tmp);
    setArithmeticFlags(tmp);
    break;


    /** BIT - Bit Test ******************************************************/
  case 0x24: // Zero Page
  case 0x2c: // Absolute
    tmp = memory->peek(effectiveAddress);
    setZeroFlag((a & tmp) == 0);
    setNegativeFlag((tmp & 0x80) != 0);
    setOverflowFlag((tmp & 0x40) != 0);
    break;


    /** AND - Logical AND ***************************************************/
  case 0x29: // #Immediate
    a &= args[0];
    setArithmeticFlags(a);
    break;
  case 0x21: // (Zero Page,X)
  case 0x25: // Zero Page
  case 0x2d: // Absolute
  case 0x31: // (Zero Page),Y
  case 0x35: // Zero Page,X
  case 0x39: // Absolute,Y
  case 0x3d: // Absolute,X
    a &= memory->peek(effectiveAddress);
    setArithmeticFlags(a);
    break;


    /** ROL - Rotate Left ***************************************************/
  case 0x2a: // Accumulator
    a = rol(a);
    setArithmeticFlags(a);
    break;
  case 0x26: // Zero Page
  case 0x2e: // Absolute
  case 0x36: // Zero Page,X
  case 0x3e: // Absolute,X
    tmp = rol(memory->peek(effectiveAddress));
    memory->poke(effectiveAddress, tmp);
    setArithmeticFlags(tmp);
    break;


    /** EOR - Exclusive OR **************************************************/
  case 0x49: // #Immediate
    a ^= args[0];
    setArithmeticFlags(a);
    break;
  case 0x41: // (Zero Page,X)
  case 0x45: // Zero Page
  case 0x4d: // Absolute
  case 0x51: // (Zero Page,Y)
  case 0x55: // Zero Page,X
  case 0x59: // Absolute,Y
  case 0x5d: // Absolute,X
    a ^= memory->peek(effectiveAddress);
    setArithmeticFlags(a);
    break;


    /** LSR - Logical Shift Right *******************************************/
  case 0x4a: // Accumulator
    a = lsr(a);
    setArithmeticFlags(a);
    break;
  case 0x46: // Zero Page
  case 0x4e: // Absolute
  case 0x56: // Zero Page,X
  case 0x5e: // Absolute,X
    tmp = lsr(memory->peek(effectiveAddress));
    memory->poke(effectiveAddress, tmp);
    setArithmeticFlags(tmp);
    break;


    /** ADC - Add with Carry ************************************************/
  case 0x69: // #Immediate
    if (decimalModeFlag) {
      a = adcDecimal(a, args[0]);
    } else {
      a = adc(a, args[0]);
    }
    break;
  case 0x61: // (Zero Page,X)
  case 0x65: // Zero Page
  case 0x6d: // Absolute
  case 0x71: // (Zero Page),Y
  case 0x75: // Zero Page,X
  case 0x79: // Absolute,Y
  case 0x7d: // Absolute,X
    if (decimalModeFlag) {
      a = adcDecimal(a, memory->peek(effectiveAddress));
    } else {
      a = adc(a, memory->peek(effectiveAddress));
    }
    break;


    /** ROR - Rotate Right **************************************************/
  case 0x6a: // Accumulator
    a = ror(a);
    setArithmeticFlags(a);
    break;
  case 0x66: // Zero Page
  case 0x6e: // Absolute
  case 0x76: // Zero Page,X
  case 0x7e: // Absolute,X
    tmp = ror(memory->peek(effectiveAddress));
    memory->poke(effectiveAddress, tmp);
    setArithmeticFlags(tmp);
    break;


    /** STA - Store Accumulator *********************************************/
  case 0x81: // (Zero Page,X)
  case 0x85: // Zero Page
  case 0x8d: // Absolute
  case 0x91: // (Zero Page),Y
  case 0x95: // Zero Page,X
  case 0x99: // Absolute,Y
  case 0x9d: // Absolute,X
    memory->poke(effectiveAddress, a);
    break;


    /** STY - Store Y Register **********************************************/
  case 0x84: // Zero Page
  case 0x8c: // Absolute
  case 0x94: // Zero Page,X
    memory->poke(effectiveAddress, y);
    break;


    /** STX - Store X Register **********************************************/
  case 0x86: // Zero Page
  case 0x8e: // Absolute
  case 0x96: // Zero Page,Y
    memory->poke(effectiveAddress, x);
    break;


    /** LDY - Load Y Register ***********************************************/
  case 0xa0: // #Immediate
    y = args[0];
    setArithmeticFlags(y);
    break;
  case 0xa4: // Zero Page
  case 0xac: // Absolute
  case 0xb4: // Zero Page,X
  case 0xbc: // Absolute,X
    y = memory->peek(effectiveAddress);
    setArithmeticFlags(y);
    break;


    /** LDX - Load X Register ***********************************************/
  case 0xa2: // #Immediate
    x = args[0];
    setArithmeticFlags(x);
    break;
  case 0xa6: // Zero Page
  case 0xae: // Absolute
  case 0xb6: // Zero Page,Y
  case 0xbe: // Absolute,Y
    x = memory->peek(effectiveAddress);
    setArithmeticFlags(x);
    break;


    /** LDA - Load Accumulator **********************************************/
  case 0xa9: // #Immediate
    a = args[0];
    setArithmeticFlags(a);
    break;
  case 0xa1: // (Zero Page,X)
  case 0xa5: // Zero Page
  case 0xad: // Absolute
  case 0xb1: // (Zero Page),Y
  case 0xb5: // Zero Page,X
  case 0xb9: // Absolute,Y
  case 0xbd: // Absolute,X
    a = memory->peek(effectiveAddress);
    setArithmeticFlags(a);
    break;


    /** CPY - Compare Y Register ********************************************/
  case 0xc0: // #Immediate
    cmp(y, args[0]);
    break;
  case 0xc4: // Zero Page
  case 0xcc: // Absolute
    cmp(y, memory->peek(effectiveAddress));
    break;


    /** CMP - Compare Accumulator *******************************************/
  case 0xc9: // #Immediate
    cmp(a, args[0]);
    break;
  case 0xc1: // (Zero Page,X)
  case 0xc5: // Zero Page
  case 0xcd: // Absolute
  case 0xd1: // (Zero Page),Y
  case 0xd5: // Zero Page,X
  case 0xd9: // Absolute,Y
  case 0xdd: // Absolute,X
    cmp(a, memory->peek(effectiveAddress));
    break;


    /** DEC - Decrement Memory **********************************************/
  case 0xc6: // Zero Page
  case 0xce: // Absolute
  case 0xd6: // Zero Page,X
  case 0xde: // Absolute,X
    tmp = (memory->peek(effectiveAddress) - 1) & 0xff;
    memory->poke(effectiveAddress, tmp);
    setArithmeticFlags(tmp);
    break;


    /** CPX - Compare X Register ********************************************/
  case 0xe0: // #Immediate
    cmp(x, args[0]);
    break;
  case 0xe4: // Zero Page
  case 0xec: // Absolute
    cmp(x, memory->peek(effectiveAddress));
    break;


    /** SBC - Subtract with Carry (Borrow) **********************************/
  case 0xe9: // #Immediate
    if (decimalModeFlag) {
      a = sbcDecimal(a, args[0]);
    } else {
      a = sbc(a, args[0]);
    }
    break;
  case 0xe1: // (Zero Page,X)
  case 0xe5: // Zero Page
  case 0xed: // Absolute
  case 0xf1: // (Zero Page),Y
  case 0xf5: // Zero Page,X
  case 0xf9: // Absolute,Y
  case 0xfd: // Absolute,X
    if (decimalModeFlag) {
      a = sbcDecimal(a, memory->peek(effectiveAddress));
    } else {
      a = sbc(a, memory->peek(effectiveAddress));
    }
    break;


    /** INC - Increment Memory **********************************************/
  case 0xe6: // Zero Page
  case 0xee: // Absolute
  case 0xf6: // Zero Page,X
  case 0xfe: // Absolute,X
    tmp = (memory->peek(effectiveAddress) + 1) & 0xff;
    memory->poke(effectiveAddress, tmp);
    setArithmeticFlags(tmp);
    break;

    /** Unimplemented Instructions ****************************************/
    // TODO: Create a flag to enable highly-accurate emulation of unimplemented instructions.
  default:
    setOpTrap();
    break;
  }
}

void CPU6502::handleIrq(int returnPc)
{
  handleInterrupt(returnPc, IRQ_VECTOR_L, IRQ_VECTOR_H);
  clearIrq();
}

void CPU6502::handleNmi()
{
  handleInterrupt(pc, NMI_VECTOR_L, NMI_VECTOR_H);
  clearNmi();
}

/**
 * Handle the common behavior of BRK, /IRQ, and /NMI
 *
 * @throws MemoryAccessException
 */
void CPU6502::handleInterrupt(int returnPc, int vectorLow, int vectorHigh)
{
  // Set the break flag before pushing.
  setBreakFlag();

  // Push program counter + 1 onto the stack
  stackPush((returnPc >> 8) & 0xff); // PC high byte
  stackPush(returnPc & 0xff);        // PC low byte
  stackPush(getStatusFlag());

  // Set the Interrupt Disabled flag.  RTI will clear it.
  setIrqDisableFlag();

  // Load interrupt vector address into PC
  pc = memory->peekw(vectorLow);
}

/**
 * Add with Carry, used by all addressing mode implementations of ADC.
 * As a side effect, this will set the overflow and carry flags as
 * needed.
 *
 * @param acc     The current value of the accumulator
 * @param operand The operand
 * @return
 */
int CPU6502::adc(int acc, int operand)
{
  int result = (operand & 0xff) + (acc & 0xff) + getCarryBit();
  int carry6 = (operand & 0x7f) + (acc & 0x7f) + getCarryBit();

  setCarryFlag((result & 0x100) != 0);
  setOverflowFlag(carryFlag ^ ((carry6 & 0x80) != 0));

  result &= 0xff;
  setArithmeticFlags(result);

  return result;
}

/**
 * Add with Carry (BCD).
 */

int CPU6502::adcDecimal(int acc, int operand) {
  int l, h, result;

  l = (acc & 0x0f) + (operand & 0x0f) + getCarryBit();
  if ((l & 0xff) > 9) {
    l += 6;
  }

  h = (acc >> 4) + (operand >> 4) + (l > 15 ? 1 : 0);
  if ((h & 0xff) > 9) {
    h += 6;
  }

  result = (l & 0x0f) | (h << 4);
  result &= 0xff;

  setCarryFlag(h > 15);
  setZeroFlag(result == 0);
  setNegativeFlag(false); // BCD is never negative
  setOverflowFlag(false); // BCD never sets overflow flag

  return result;
}

/**
 * Common code for Subtract with Carry.  Just calls ADC of the
 * one's complement of the operand.  This lets the N, V, C, and Z
 * flags work out nicely without any additional logic.
 */
int CPU6502::sbc(int acc, int operand) {
  int result = adc(acc, ~operand);
  setArithmeticFlags(result);

  return result;
}

/**
 * Subtract with Carry, BCD mode.
 */
int CPU6502::sbcDecimal(int acc, int operand) {
  int l, h, result;

  l = (acc & 0x0f) - (operand & 0x0f) - (carryFlag ? 0 : 1);
  if ((l & 0x10) != 0) {
    l -= 6;
  }

  h = (acc >> 4) - (operand >> 4) - ((l & 0x10) != 0 ? 1 : 0);
  if ((h & 0x10) != 0) {
    h -= 6;
  }

  result = (l & 0x0f) | (h << 4);
  setCarryFlag((h & 0xff) < 15);
  setZeroFlag(result == 0);
  setNegativeFlag(false); // BCD is never negative
  setOverflowFlag(false); // BCD never sets overflow flag

  return (result & 0xff);
}

/**
 * Compare two values, and set carry, zero, and negative flags
 * appropriately.
 */
void CPU6502::cmp(int reg, int operand) {
  int tmp = (reg - operand) & 0xff;

  setCarryFlag(reg >= operand);
  setZeroFlag(tmp == 0);
  setNegativeFlag((tmp & 0x80) != 0); // Negative bit set
}

/**
 * Set the Negative and Zero flags based on the current value of the
 * register operand.
 */
void CPU6502::setArithmeticFlags(int reg) {
  zeroFlag = (reg == 0);
  negativeFlag = (reg & 0x80) != 0;
}

/**
 * Shifts the given value left by one bit, and sets the carry
 * flag to the high bit of the initial value.
 *
 * @param m The value to shift left.
 * @return the left shifted value (m * 2).
 */
int CPU6502::asl(int m) {
  setCarryFlag((m & 0x80) != 0);

  return (m << 1) & 0xff;
}

/**
 * Shifts the given value right by one bit, filling with zeros,
 * and sets the carry flag to the low bit of the initial value.
 */
int CPU6502::lsr(int m) {
  setCarryFlag((m & 0x01) != 0);

  return (m & 0xff) >> 1;
}

/**
 * Rotates the given value left by one bit, setting bit 0 to the value
 * of the carry flag, and setting the carry flag to the original value
 * of bit 7.
 */
int CPU6502::rol(int m) {
  int result = ((m << 1) | getCarryBit()) & 0xff;
  setCarryFlag((m & 0x80) != 0);

  return result;
}

/**
 * Rotates the given value right by one bit, setting bit 7 to the value
 * of the carry flag, and setting the carry flag to the original value
 * of bit 1.
 */
int CPU6502::ror(int m) {
  int result = ((m >> 1) | (getCarryBit() << 7)) & 0xff;
  setCarryFlag((m & 0x01) != 0);

  return result;
}

bool CPU6502::getBreakFlag()
{
  return breakFlag;
}

void CPU6502::setBreakFlag()
{
  breakFlag = true;
}

void CPU6502::setBreakFlag(bool b)
{
  breakFlag = b;
}

void CPU6502::clearBreakFlag()
{
  breakFlag = false;
}

bool CPU6502::getOverflowFlag()
{
  return overflowFlag;
}

void CPU6502::setOverflowFlag()
{
  overflowFlag = true;
}

void CPU6502::setOverflowFlag(bool b)
{
  overflowFlag = b;
}

void CPU6502::clearOverflowFlag()
{
  overflowFlag = false;
}

bool CPU6502::getNegativeFlag()
{
  return negativeFlag;
}

void CPU6502::setNegativeFlag()
{
  negativeFlag = true;
}

void CPU6502::setNegativeFlag(bool b)
{
  negativeFlag = b;
}

void CPU6502::clearNegativeFlag()
{
  negativeFlag = false;
}

bool CPU6502::getCarryFlag()
{
  return carryFlag;
}

bool CPU6502::getCarryBit()
{
  return carryFlag ? 1 : 0;
}

void CPU6502::setCarryFlag()
{
  carryFlag = true;
}

void CPU6502::setCarryFlag(bool b)
{
  carryFlag = b;
}

void CPU6502::clearCarryFlag()
{
  carryFlag = false;
}

bool CPU6502::getZeroFlag()
{
  return zeroFlag;
}

void CPU6502::setZeroFlag()
{
  zeroFlag = true;
}

void CPU6502::setZeroFlag(bool b)
{
  zeroFlag = b;
}

void CPU6502::clearZeroFlag()
{
  zeroFlag = false;
}

bool CPU6502::getIrqDisableFlag()
{
  return irqDisableFlag;
}

void CPU6502::setIrqDisableFlag()
{
  irqDisableFlag = true;
}

void CPU6502::setIrqDisableFlag(bool b)
{
  irqDisableFlag = b;
}

void CPU6502::clearIrqDisableFlag()
{
  irqDisableFlag = false;
}


bool CPU6502::getDecimalModeFlag()
{
  return decimalModeFlag;
}

void CPU6502::setDecimalModeFlag()
{
  decimalModeFlag = true;
}

void CPU6502::setDecimalModeFlag(bool b)
{
  decimalModeFlag = b;
}

void CPU6502::clearDecimalModeFlag()
{
  decimalModeFlag = false;
}

int CPU6502::getProcessorStatus()
{
  return getStatusFlag();
}

void CPU6502::setProcessorStatus(int value) {
  if ((value & P_CARRY) != 0)
    setCarryFlag();
  else
    clearCarryFlag();

  if ((value & P_ZERO) != 0)
    setZeroFlag();
  else
    clearZeroFlag();

  if ((value & P_IRQ_DISABLE) != 0)
    setIrqDisableFlag();
  else
    clearIrqDisableFlag();

  if ((value & P_DECIMAL) != 0)
    setDecimalModeFlag();
  else
    clearDecimalModeFlag();

  if ((value & P_BREAK) != 0)
    setBreakFlag();
  else
    clearBreakFlag();

  if ((value & P_OVERFLOW) != 0)
    setOverflowFlag();
  else
    clearOverflowFlag();

  if ((value & P_NEGATIVE) != 0)
    setNegativeFlag();
  else
    clearNegativeFlag();
}

int CPU6502::getProgramCounter()
{
  return pc;
}

void CPU6502::setProgramCounter(int addr)
{
  pc = addr;
}

int CPU6502::getStackPointer()
{
  return sp;
}

void CPU6502::setStackPointer(int offset)
{
  sp = offset;
}

/**
 * Set the illegal instruction trap.
 */
void CPU6502::setOpTrap() {
  opTrap = true;
}

/**
 * Clear the illegal instruction trap.
 */
void CPU6502::clearOpTrap() {
  opTrap = false;
}


/**
 * Push an item onto the stack, and decrement the stack counter.
 * Will wrap-around if already at the bottom of the stack (This
 * is the same behavior as the real 6502)
 */
void CPU6502::stackPush(int data)
{
  memory->poke(0x100 + sp, data);

  if (sp == 0) {
    sp = 0xff;
  } else {
    sp--;
  }
}


/**
 * Pre-increment the stack pointer, and return the top of the stack.
 * Will wrap-around if already at the top of the stack (This
 * is the same behavior as the real 6502)
 */
int CPU6502::stackPop()
{
  if (sp == 0xff) {
    sp = 0x00;
  } else {
    sp++;
  }

  return memory->peek(0x100 + sp);
}

/**
 * Peek at the value currently at the top of the stack
 */
int CPU6502::stackPeek()
{
  int sp = sp;

  if (sp == 0xff) {
    sp = 0x00;
  } else {
    sp++;
  }

  return memory->peek(0x100 + sp);
}

/**
 * Simulate transition from logic-high to logic-low on the INT line.
 */
void CPU6502::assertIrq() {
  irqAsserted = true;
}

/**
 * Simulate transition from logic-low to logic-high of the INT line.
 */
void CPU6502::clearIrq() {
  irqAsserted = false;
}

/**
 * Simulate transition from logic-high to logic-low on the NMI line.
 */
void CPU6502::assertNmi() {
  nmiAsserted = true;
}

/**
 * Simulate transition from logic-low to logic-high of the NMI line.
 */
void CPU6502::clearNmi() {
  nmiAsserted = false;
}

/**
 * Given two bytes, return an address.
 */
int CPU6502::address(int lowByte, int hiByte)
{
  return ((hiByte << 8) | lowByte) & 0xffff;
}

/**
 * Given a hi byte and a low byte, return the Absolute,X
 * offset address.
 */
int CPU6502::xAddress(int lowByte, int hiByte)
{
  return (address(lowByte, hiByte) + x) & 0xffff;
}

/**
 * Given a hi byte and a low byte, return the Absolute,Y
 * offset address.
 */
int CPU6502::yAddress(int lowByte, int hiByte)
{
  return (address(lowByte, hiByte) + y) & 0xffff;
}

/**
 * Given a single byte, compute the Zero Page,X offset address.
 */
int CPU6502::zpxAddress(int zp)
{
  return (zp + x) & 0xff;
}

/**
 * Given a single byte, compute the offset address.
 */
int CPU6502::relAddress(byte offset)
{
  return (pc + (signed char)offset) & 0xffff;
}

/**
 * Given a single byte, compute the Zero Page,Y offset address.
 */
int CPU6502::zpyAddress(int zp)
{
  return (zp + y) & 0xff;
}

void CPU6502::summary()
{
  char flags[64];
  char src[512];

  disassembleOp(src, sizeof(src));
  printf("$%04x: %-20s ", lastPc, src);

  getStatusFlagAsString(flags, sizeof(flags));
  printf("PC: $%04x  A:$%02x X:$%02x Y:$%02x SP:$%02x Flags: %s\n",
	 pc, a, x, y, sp, flags);
}

MemoryMap *CPU6502::getMemory() {
  return memory;
}

int CPU6502::disassemble(int addr, char*str, int len)
{
  strncpy(str, "???", len);

  return 1;
}

byte CPU6502::getStatusFlag()
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

void CPU6502::getStatusFlagAsString(char *str, int len)
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

void CPU6502::disassembleOp(char *str, int len)
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
