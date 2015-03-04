#include <stdio.h>
#include <time.h>

#include "i8080.h"

i8080::i8080(MemoryMap *mem) : Processor(mem) {
  state = new i8080State(memory);
}

i8080::~i8080() {
}

void i8080::reset()
{
  state->reset();
}

void i8080::step()
{
}

i8080State *i8080::getState() {
  return state;
}

MemoryMap *i8080::getMemory() {
  return memory;
}

int i8080::disassemble(int addr, char*str, int len)
{
  strncpy(str, "???", len);

  return 1;
}
