#include <stdio.h>
#include <time.h>

#include "Instructions.h"
#include "CPU.h"

CPU::CPU(MemoryMap *mem) {
  behaviour = NMOS_WITH_INDIRECT_JMP_BUG;

  memory = mem;
  state = new CPUState(memory);

  memory->dump();
}

CPU::~CPU() {
}

void CPU::reset()
{
  state->reset();
}

void CPU::run()
{
  while (state->running) {
    step();
  }
}

void CPU::step()
{
  opBeginTime = getNanoTicks();

  // Store the address from which the IR was read, for debugging
  state->lastPc = state->pc;

  // Check for Interrupts before doing anything else.
  // This will set the PC and jump to the interrupt vector.
  if (state->nmiAsserted) {
    handleNmi();
  } else if (state->irqAsserted && !getIrqDisableFlag()) {
    handleIrq(state->pc);
  }

  // Fetch memory location for this instruction.
  state->ir = memory->peek(state->pc);
  irAddressMode = (state->ir >> 2) & 0x07;
  irOpMode = state->ir & 0x03;

  //printf("IR=$%02x  AMode=%d  OpMode=%d\n", state->ir, irAddressMode, irOpMode);

  incrementPC();

  clearOpTrap();

  // Decode the instruction and operands
  state->instSize = Instructions::size(state->ir);
  for (int i = 0; i < state->instSize - 1; i++) {
    state->args[i] = memory->peek(state->pc);
    // Increment PC after reading
    incrementPC();
  }

  //printf("instSize=%d\n", state->instSize);

  state->stepCounter++;

  // Get the data from the effective address (if any)
  effectiveAddress = 0;

  switch (irOpMode) {
  case 0:
  case 2:
    switch (irAddressMode) {
    case 0: // #Immediate
      break;
    case 1: // Zero Page
      effectiveAddress = state->args[0];
      break;
    case 2: // Accumulator - ignored
      break;
    case 3: // Absolute
      effectiveAddress = address(state->args[0], state->args[1]);
      break;
    case 5: // Zero Page,X / Zero Page,Y
      if (state->ir == 0x96 || state->ir == 0xb6) {
	effectiveAddress = zpyAddress(state->args[0]);
      } else {
	effectiveAddress = zpxAddress(state->args[0]);
      }
      break;
    case 7: // Absolute,X / Absolute,Y
      if (state->ir == 0xbe) {
	effectiveAddress = yAddress(state->args[0], state->args[1]);
      } else {
	effectiveAddress = xAddress(state->args[0], state->args[1]);
      }
      break;
    }
    break;
  case 1:
    switch (irAddressMode) {
    case 0: // (Zero Page,X)
      tmp = (state->args[0] + state->x) & 0xff;
      effectiveAddress = address(memory->peek(tmp), memory->peek(tmp + 1));
      break;
    case 1: // Zero Page
      effectiveAddress = state->args[0];
      break;
    case 2: // #Immediate
      effectiveAddress = -1;
      break;
    case 3: // Absolute
      effectiveAddress = address(state->args[0], state->args[1]);
      break;
    case 4: // (Zero Page),Y
      tmp = address(memory->peek(state->args[0]),
		    memory->peek((state->args[0] + 1) & 0xff));
      effectiveAddress = (tmp + state->y) & 0xffff;
      break;
    case 5: // Zero Page,X
      effectiveAddress = zpxAddress(state->args[0]);
      break;
    case 6: // Absolute, Y
      effectiveAddress = yAddress(state->args[0], state->args[1]);
      break;
    case 7: // Absolute, X
      effectiveAddress = xAddress(state->args[0], state->args[1]);
      break;
    }
    break;
  }

  // Execute
  switch (state->ir) {

    /** Single Byte Instructions; Implied and Relative **/
  case 0x00: // BRK - Force Interrupt - Implied
    if (!getIrqDisableFlag()) {
      handleIrq(state->pc + 1);
    }
    break;
  case 0x08: // PHP - Push Processor Status - Implied
    // Break flag is always set in the stack value.
    stackPush(state->getStatusFlag() | 0x10);
    break;
  case 0x10: // BPL - Branch if Positive - Relative
    if (!getNegativeFlag()) {
      state->pc = relAddress(state->args[0]);
    }
    break;
  case 0x18: // CLC - Clear Carry Flag - Implied
    clearCarryFlag();
    break;
  case 0x20: // JSR - Jump to Subroutine - Implied
    stackPush((state->pc - 1 >> 8) & 0xff); // PC high byte
    stackPush(state->pc - 1 & 0xff);        // PC low byte
    state->pc = address(state->args[0], state->args[1]);
    break;
  case 0x28: // PLP - Pull Processor Status - Implied
    setProcessorStatus(stackPop());
    break;
  case 0x30: // BMI - Branch if Minus - Relative
    if (getNegativeFlag()) {
      state->pc = relAddress(state->args[0]);
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
    stackPush(state->a);
    break;
  case 0x50: // BVC - Branch if Overflow Clear - Relative
    if (!getOverflowFlag()) {
      state->pc = relAddress(state->args[0]);
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
    state->a = stackPop();
    setArithmeticFlags(state->a);
    break;
  case 0x70: // BVS - Branch if Overflow Set - Relative
    if (getOverflowFlag()) {
      state->pc = relAddress(state->args[0]);
    }
    break;
  case 0x78: // SEI - Set Interrupt Disable - Implied
    setIrqDisableFlag();
    break;
  case 0x88: // DEY - Decrement Y Register - Implied
    state->y = --state->y & 0xff;
    setArithmeticFlags(state->y);
    break;
  case 0x8a: // TXA - Transfer X to Accumulator - Implied
    state->a = state->x;
    setArithmeticFlags(state->a);
    break;
  case 0x90: // BCC - Branch if Carry Clear - Relative
    if (!getCarryFlag()) {
      state->pc = relAddress(state->args[0]);
    }
    break;
  case 0x98: // TYA - Transfer Y to Accumulator - Implied
    state->a = state->y;
    setArithmeticFlags(state->a);
    break;
  case 0x9a: // TXS - Transfer X to Stack Pointer - Implied
    setStackPointer(state->x);
    break;
  case 0xa8: // TAY - Transfer Accumulator to Y - Implied
    state->y = state->a;
    setArithmeticFlags(state->y);
    break;
  case 0xaa: // TAX - Transfer Accumulator to X - Implied
    state->x = state->a;
    setArithmeticFlags(state->x);
    break;
  case 0xb0: // BCS - Branch if Carry Set - Relative
    if (getCarryFlag()) {
      state->pc = relAddress(state->args[0]);
    }
    break;
  case 0xb8: // CLV - Clear Overflow Flag - Implied
    clearOverflowFlag();
    break;
  case 0xba: // TSX - Transfer Stack Pointer to X - Implied
    state->x = getStackPointer();
    setArithmeticFlags(state->x);
    break;
  case 0xc8: // INY - Increment Y Register - Implied
    state->y = ++state->y & 0xff;
    setArithmeticFlags(state->y);
    break;
  case 0xca: // DEX - Decrement X Register - Implied
    state->x = --state->x & 0xff;
    setArithmeticFlags(state->x);
    break;
  case 0xd0: // BNE - Branch if Not Equal to Zero - Relative
    if (!getZeroFlag()) {
      state->pc = relAddress(state->args[0]);
    }
    break;
  case 0xd8: // CLD - Clear Decimal Mode - Implied
    clearDecimalModeFlag();
    break;
  case 0xe8: // INX - Increment X Register - Implied
    state->x = ++state->x & 0xff;
    setArithmeticFlags(state->x);
    break;
  case 0xea: // NOP
    // Do nothing.
    break;
  case 0xf0: // BEQ - Branch if Equal to Zero - Relative
    if (getZeroFlag()) {
      state->pc = relAddress(state->args[0]);
    }
    break;
  case 0xf8: // SED - Set Decimal Flag - Implied
    setDecimalModeFlag();
    break;

    /** JMP *****************************************************************/
  case 0x4c: // JMP - Absolute
    state->pc = address(state->args[0], state->args[1]);
    break;
  case 0x6c: // JMP - Indirect
    lo = address(state->args[0], state->args[1]); // Address of low byte

    if (state->args[0] == 0xff &&
	(behaviour == NMOS_WITH_INDIRECT_JMP_BUG ||
	 behaviour == NMOS_WITH_ROR_BUG)) {
      hi = address(0x00, state->args[1]);
    } else {
      hi = lo + 1;
    }

    state->pc = address(memory->peek(lo), memory->peek(hi));
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
    state->a |= state->args[0];
    setArithmeticFlags(state->a);
    break;
  case 0x01: // (Zero Page,X)
  case 0x05: // Zero Page
  case 0x0d: // Absolute
  case 0x11: // (Zero Page),Y
  case 0x15: // Zero Page,X
  case 0x19: // Absolute,Y
  case 0x1d: // Absolute,X
    state->a |= memory->peek(effectiveAddress);
    setArithmeticFlags(state->a);
    break;


    /** ASL - Arithmetic Shift Left *****************************************/
  case 0x0a: // Accumulator
    state->a = asl(state->a);
    setArithmeticFlags(state->a);
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
    setZeroFlag((state->a & tmp) == 0);
    setNegativeFlag((tmp & 0x80) != 0);
    setOverflowFlag((tmp & 0x40) != 0);
    break;


    /** AND - Logical AND ***************************************************/
  case 0x29: // #Immediate
    state->a &= state->args[0];
    setArithmeticFlags(state->a);
    break;
  case 0x21: // (Zero Page,X)
  case 0x25: // Zero Page
  case 0x2d: // Absolute
  case 0x31: // (Zero Page),Y
  case 0x35: // Zero Page,X
  case 0x39: // Absolute,Y
  case 0x3d: // Absolute,X
    state->a &= memory->peek(effectiveAddress);
    setArithmeticFlags(state->a);
    break;


    /** ROL - Rotate Left ***************************************************/
  case 0x2a: // Accumulator
    state->a = rol(state->a);
    setArithmeticFlags(state->a);
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
    state->a ^= state->args[0];
    setArithmeticFlags(state->a);
    break;
  case 0x41: // (Zero Page,X)
  case 0x45: // Zero Page
  case 0x4d: // Absolute
  case 0x51: // (Zero Page,Y)
  case 0x55: // Zero Page,X
  case 0x59: // Absolute,Y
  case 0x5d: // Absolute,X
    state->a ^= memory->peek(effectiveAddress);
    setArithmeticFlags(state->a);
    break;


    /** LSR - Logical Shift Right *******************************************/
  case 0x4a: // Accumulator
    state->a = lsr(state->a);
    setArithmeticFlags(state->a);
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
    if (state->decimalModeFlag) {
      state->a = adcDecimal(state->a, state->args[0]);
    } else {
      state->a = adc(state->a, state->args[0]);
    }
    break;
  case 0x61: // (Zero Page,X)
  case 0x65: // Zero Page
  case 0x6d: // Absolute
  case 0x71: // (Zero Page),Y
  case 0x75: // Zero Page,X
  case 0x79: // Absolute,Y
  case 0x7d: // Absolute,X
    if (state->decimalModeFlag) {
      state->a = adcDecimal(state->a, memory->peek(effectiveAddress));
    } else {
      state->a = adc(state->a, memory->peek(effectiveAddress));
    }
    break;


    /** ROR - Rotate Right **************************************************/
  case 0x6a: // Accumulator
    state->a = ror(state->a);
    setArithmeticFlags(state->a);
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
    memory->poke(effectiveAddress, state->a);
    break;


    /** STY - Store Y Register **********************************************/
  case 0x84: // Zero Page
  case 0x8c: // Absolute
  case 0x94: // Zero Page,X
    memory->poke(effectiveAddress, state->y);
    break;


    /** STX - Store X Register **********************************************/
  case 0x86: // Zero Page
  case 0x8e: // Absolute
  case 0x96: // Zero Page,Y
    memory->poke(effectiveAddress, state->x);
    break;


    /** LDY - Load Y Register ***********************************************/
  case 0xa0: // #Immediate
    state->y = state->args[0];
    setArithmeticFlags(state->y);
    break;
  case 0xa4: // Zero Page
  case 0xac: // Absolute
  case 0xb4: // Zero Page,X
  case 0xbc: // Absolute,X
    state->y = memory->peek(effectiveAddress);
    setArithmeticFlags(state->y);
    break;


    /** LDX - Load X Register ***********************************************/
  case 0xa2: // #Immediate
    state->x = state->args[0];
    setArithmeticFlags(state->x);
    break;
  case 0xa6: // Zero Page
  case 0xae: // Absolute
  case 0xb6: // Zero Page,Y
  case 0xbe: // Absolute,Y
    state->x = memory->peek(effectiveAddress);
    setArithmeticFlags(state->x);
    break;


    /** LDA - Load Accumulator **********************************************/
  case 0xa9: // #Immediate
    state->a = state->args[0];
    setArithmeticFlags(state->a);
    break;
  case 0xa1: // (Zero Page,X)
  case 0xa5: // Zero Page
  case 0xad: // Absolute
  case 0xb1: // (Zero Page),Y
  case 0xb5: // Zero Page,X
  case 0xb9: // Absolute,Y
  case 0xbd: // Absolute,X
    state->a = memory->peek(effectiveAddress);
    setArithmeticFlags(state->a);
    break;


    /** CPY - Compare Y Register ********************************************/
  case 0xc0: // #Immediate
    cmp(state->y, state->args[0]);
    break;
  case 0xc4: // Zero Page
  case 0xcc: // Absolute
    cmp(state->y, memory->peek(effectiveAddress));
    break;


    /** CMP - Compare Accumulator *******************************************/
  case 0xc9: // #Immediate
    cmp(state->a, state->args[0]);
    break;
  case 0xc1: // (Zero Page,X)
  case 0xc5: // Zero Page
  case 0xcd: // Absolute
  case 0xd1: // (Zero Page),Y
  case 0xd5: // Zero Page,X
  case 0xd9: // Absolute,Y
  case 0xdd: // Absolute,X
    cmp(state->a, memory->peek(effectiveAddress));
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
    cmp(state->x, state->args[0]);
    break;
  case 0xe4: // Zero Page
  case 0xec: // Absolute
    cmp(state->x, memory->peek(effectiveAddress));
    break;


    /** SBC - Subtract with Carry (Borrow) **********************************/
  case 0xe9: // #Immediate
    if (state->decimalModeFlag) {
      state->a = sbcDecimal(state->a, state->args[0]);
    } else {
      state->a = sbc(state->a, state->args[0]);
    }
    break;
  case 0xe1: // (Zero Page,X)
  case 0xe5: // Zero Page
  case 0xed: // Absolute
  case 0xf1: // (Zero Page),Y
  case 0xf5: // Zero Page,X
  case 0xf9: // Absolute,Y
  case 0xfd: // Absolute,X
    if (state->decimalModeFlag) {
      state->a = sbcDecimal(state->a, memory->peek(effectiveAddress));
    } else {
      state->a = sbc(state->a, memory->peek(effectiveAddress));
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

  delayLoop(state->ir);
}

void CPU::handleIrq(word returnPc)
{
  handleInterrupt(returnPc, IRQ_VECTOR_L, IRQ_VECTOR_H);
  clearIrq();
}

void CPU::handleNmi()
{
  handleInterrupt(state->pc, NMI_VECTOR_L, NMI_VECTOR_H);
  clearNmi();
}

/**
 * Handle the common behavior of BRK, /IRQ, and /NMI
 *
 * @throws MemoryAccessException
 */
void CPU::handleInterrupt(int returnPc, int vectorLow, int vectorHigh)
{
  // Set the break flag before pushing.
  setBreakFlag();

  // Push program counter + 1 onto the stack
  stackPush((returnPc >> 8) & 0xff); // PC high byte
  stackPush(returnPc & 0xff);        // PC low byte
  stackPush(state->getStatusFlag());

  // Set the Interrupt Disabled flag.  RTI will clear it.
  setIrqDisableFlag();

  // Load interrupt vector address into PC
  state->pc = memory->peekw(vectorLow);
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
int CPU::adc(int acc, int operand)
{
  int result = (operand & 0xff) + (acc & 0xff) + getCarryBit();
  int carry6 = (operand & 0x7f) + (acc & 0x7f) + getCarryBit();

  setCarryFlag((result & 0x100) != 0);
  setOverflowFlag(state->carryFlag ^ ((carry6 & 0x80) != 0));

  result &= 0xff;
  setArithmeticFlags(result);

  return result;
}

/**
 * Add with Carry (BCD).
 */

int CPU::adcDecimal(int acc, int operand) {
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
int CPU::sbc(int acc, int operand) {
  int result = adc(acc, ~operand);
  setArithmeticFlags(result);

  return result;
}

/**
 * Subtract with Carry, BCD mode.
 */
int CPU::sbcDecimal(int acc, int operand) {
  int l, h, result;

  l = (acc & 0x0f) - (operand & 0x0f) - (state->carryFlag ? 0 : 1);
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
void CPU::cmp(int reg, int operand) {
  int tmp = (reg - operand) & 0xff;

  setCarryFlag(reg >= operand);
  setZeroFlag(tmp == 0);
  setNegativeFlag((tmp & 0x80) != 0); // Negative bit set
}

/**
 * Set the Negative and Zero flags based on the current value of the
 * register operand.
 */
void CPU::setArithmeticFlags(int reg) {
  state->zeroFlag = (reg == 0);
  state->negativeFlag = (reg & 0x80) != 0;
}

/**
 * Shifts the given value left by one bit, and sets the carry
 * flag to the high bit of the initial value.
 *
 * @param m The value to shift left.
 * @return the left shifted value (m * 2).
 */
int CPU::asl(int m) {
  setCarryFlag((m & 0x80) != 0);

  return (m << 1) & 0xff;
}

/**
 * Shifts the given value right by one bit, filling with zeros,
 * and sets the carry flag to the low bit of the initial value.
 */
int CPU::lsr(int m) {
  setCarryFlag((m & 0x01) != 0);

  return (m & 0xff) >> 1;
}

/**
 * Rotates the given value left by one bit, setting bit 0 to the value
 * of the carry flag, and setting the carry flag to the original value
 * of bit 7.
 */
int CPU::rol(int m) {
  int result = ((m << 1) | getCarryBit()) & 0xff;
  setCarryFlag((m & 0x80) != 0);

  return result;
}

/**
 * Rotates the given value right by one bit, setting bit 7 to the value
 * of the carry flag, and setting the carry flag to the original value
 * of bit 1.
 */
int CPU::ror(int m) {
  int result = ((m >> 1) | (getCarryBit() << 7)) & 0xff;
  setCarryFlag((m & 0x01) != 0);

  return result;
}

bool CPU::getBreakFlag()
{
  return state->breakFlag;
}

void CPU::setBreakFlag()
{
  state->breakFlag = true;
}

void CPU::setBreakFlag(bool b)
{
  state->breakFlag = b;
}

void CPU::clearBreakFlag()
{
  state->breakFlag = false;
}

bool CPU::getOverflowFlag()
{
  return state->overflowFlag;
}

void CPU::setOverflowFlag()
{
  state->overflowFlag = true;
}

void CPU::setOverflowFlag(bool b)
{
  state->overflowFlag = b;
}

void CPU::clearOverflowFlag()
{
  state->overflowFlag = false;
}

bool CPU::getNegativeFlag()
{
  return state->negativeFlag;
}

void CPU::setNegativeFlag()
{
  state->negativeFlag = true;
}

void CPU::setNegativeFlag(bool b)
{
  state->negativeFlag = b;
}

void CPU::clearNegativeFlag()
{
  state->negativeFlag = false;
}

bool CPU::getCarryFlag()
{
  return state->carryFlag;
}

bool CPU::getCarryBit()
{
  return state->carryFlag ? 1 : 0;
}

void CPU::setCarryFlag()
{
  state->carryFlag = true;
}

void CPU::setCarryFlag(bool b)
{
  state->carryFlag = b;
}

void CPU::clearCarryFlag()
{
  state->carryFlag = false;
}

bool CPU::getZeroFlag()
{
  return state->zeroFlag;
}

void CPU::setZeroFlag()
{
  state->zeroFlag = true;
}

void CPU::setZeroFlag(bool b)
{
  state->zeroFlag = b;
}

void CPU::clearZeroFlag()
{
  state->zeroFlag = false;
}

bool CPU::getIrqDisableFlag()
{
  return state->irqDisableFlag;
}

void CPU::setIrqDisableFlag()
{
  state->irqDisableFlag = true;
}

void CPU::setIrqDisableFlag(bool b)
{
  state->irqDisableFlag = b;
}

void CPU::clearIrqDisableFlag()
{
  state->irqDisableFlag = false;
}


bool CPU::getDecimalModeFlag()
{
  return state->decimalModeFlag;
}

void CPU::setDecimalModeFlag()
{
  state->decimalModeFlag = true;
}

void CPU::setDecimalModeFlag(bool b)
{
  state->decimalModeFlag = b;
}

void CPU::clearDecimalModeFlag()
{
  state->decimalModeFlag = false;
}

int CPU::getProcessorStatus()
{
  return state->getStatusFlag();
}

void CPU::setProcessorStatus(int value) {
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

int CPU::getProgramCounter()
{
  return state->pc;
}

void CPU::setProgramCounter(int addr)
{
  state->pc = addr;
}

int CPU::getStackPointer()
{
  return state->sp;
}

void CPU::setStackPointer(int offset)
{
  state->sp = offset;
}

/**
 * Set the illegal instruction trap.
 */
void CPU::setOpTrap() {
  state->opTrap = true;
}

/**
 * Clear the illegal instruction trap.
 */
void CPU::clearOpTrap() {
  state->opTrap = false;
}


/**
 * Push an item onto the stack, and decrement the stack counter.
 * Will wrap-around if already at the bottom of the stack (This
 * is the same behavior as the real 6502)
 */
void CPU::stackPush(int data)
{
  memory->poke(0x100 + state->sp, data);

  if (state->sp == 0) {
    state->sp = 0xff;
  } else {
    state->sp--;
  }
}


/**
 * Pre-increment the stack pointer, and return the top of the stack.
 * Will wrap-around if already at the top of the stack (This
 * is the same behavior as the real 6502)
 */
int CPU::stackPop()
{
  if (state->sp == 0xff) {
    state->sp = 0x00;
  } else {
    state->sp++;
  }

  return memory->peek(0x100 + state->sp);
}

/**
 * Peek at the value currently at the top of the stack
 */
int CPU::stackPeek()
{
  int sp = state->sp;

  if (sp == 0xff) {
    sp = 0x00;
  } else {
    sp++;
  }

  return memory->peek(0x100 + state->sp);
}

/*
 * Increment the PC, rolling over if necessary.
 */
void CPU::incrementPC() {
  if (state->pc == 0xffff) {
    state->pc = 0;
  } else {
    state->pc++;
  }
}

/**
 * Simulate transition from logic-high to logic-low on the INT line.
 */
void CPU::assertIrq() {
  state->irqAsserted = true;
}

/**
 * Simulate transition from logic-low to logic-high of the INT line.
 */
void CPU::clearIrq() {
  state->irqAsserted = false;
}

/**
 * Simulate transition from logic-high to logic-low on the NMI line.
 */
void CPU::assertNmi() {
  state->nmiAsserted = true;
}

/**
 * Simulate transition from logic-low to logic-high of the NMI line.
 */
void CPU::clearNmi() {
  state->nmiAsserted = false;
}

/**
 * Given two bytes, return an address.
 */
int CPU::address(int lowByte, int hiByte)
{
  return ((hiByte << 8) | lowByte) & 0xffff;
}

/**
 * Given a hi byte and a low byte, return the Absolute,X
 * offset address.
 */
int CPU::xAddress(int lowByte, int hiByte)
{
  return (address(lowByte, hiByte) + state->x) & 0xffff;
}

/**
 * Given a hi byte and a low byte, return the Absolute,Y
 * offset address.
 */
int CPU::yAddress(int lowByte, int hiByte)
{
  return (address(lowByte, hiByte) + state->y) & 0xffff;
}

/**
 * Given a single byte, compute the Zero Page,X offset address.
 */
int CPU::zpxAddress(int zp)
{
  return (zp + state->x) & 0xff;
}

/**
 * Given a single byte, compute the offset address.
 */
int CPU::relAddress(byte offset)
{
  return (state->pc + (signed char)offset) & 0xffff;
}

/**
 * Given a single byte, compute the Zero Page,Y offset address.
 */
int CPU::zpyAddress(int zp)
{
  return (zp + state->y) & 0xff;
}

/*
 * Perform a busy-loop for CLOCK_IN_NS nanoseconds
 */
void CPU::delayLoop(int opcode)
{
  int clockSteps = Instructions::cycles(opcode & 0xff);

  // Just a precaution. This could be better.
  if (clockSteps == 0) {
    clockSteps = 1;
  }

  long opScheduledEnd = opBeginTime + clockSteps;
  long now = getNanoTicks();
  while(now < opScheduledEnd) {
    now = getNanoTicks();
  }
}

long CPU::getNanoTicks()
{
  timespec ts;

  clock_gettime(CLOCK_REALTIME, &ts);

  return ts.tv_nsec;
}

void CPU::summary()
{
  char flags[64];
  char src[512];

  state->disassembleOp(src, sizeof(src));
  printf("$%04x: %-20s ", state->lastPc, src);

  state->getStatusFlagAsString(flags, sizeof(flags));
  printf("PC: $%04x  A:$%02x X:$%02x Y:$%02x SP:$%02x Flags: %s\n",
	 state->pc, state->a, state->x, state->y, state->sp, flags);
}

CPUState *CPU::getState() {
  return state;
}

MemoryMap *CPU::getMemory() {
  return memory;
}
