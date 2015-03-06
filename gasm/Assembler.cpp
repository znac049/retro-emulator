#include "Assembler.h"

Assembler::pseudoOps = {
  "=".     Op.EQU,
  "equ",   Op.EQU,
  "dc.b",  Op.BYTE,
  ".byte", Op.BYTE,
  "dc.w",  Op.WORD,
  ".word", Op.WORD,
  "org",   Op.ORG
};

void processDirective(ALine &line)
{
  int op = lookupDirective(line.instruction);

  switch (op) {
  case Op.EQU:
    break;

  case Op.BYTE:
    break;

  case Op.WORD:
    break;

  case Op.ORG:
    break;

  default:
    complainAndThrow("Bother", line);
    break;
  }
}

bool Assembler::DoPass(int passNumber)
{
  bool stable = true;
  ALine line;

  frewind(fd);

  try {
    while (!feof(fd)) {
      line.read(fd);
      line.parse();
      if (line.ok()) {
	if (isDirective(line)) {
	  processDirective(line);
	}
	else {
	  assemble(line);
	}
      }
    }
  }
  catch {
  }

  return stable;
}
