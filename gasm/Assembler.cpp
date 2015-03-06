#include <stdio.h>
#include <string.h>

#include <exception>

#include "Assembler.h"
#include "ALine.h"

const Assembler::pseudoOp Assembler::pseudoOps[] = {
  {"=",     EQU},
  {"equ",   EQU},
  {"dc.b",  BYTE},
  {".byte", BYTE},
  {"dc.w",  WORD},
  {".word", WORD},
  {"org",   ORG}
};

Assembler::Assembler()
{
  org = 0;
}

void Assembler::assemble(ALine &line)
{
}

bool Assembler::isDirective(const char *op)
{
  int res = lookupDirective(op);

  return (res == -1) ? false : true;
}

int Assembler::lookupDirective(const char *op)
{
  for (int i=0; i<sizeof(pseudoOps); i++) {
    if (strcasecmp(op, pseudoOps[i].op) == 0) {
      return pseudoOps[i].directive;
    }
  }
   
  return -1;
}

void Assembler::processDirective(ALine &line)
{
  int op = lookupDirective(line.getInstruction());

  switch (op) {
  case EQU:
    break;

  case BYTE:
    break;

  case WORD:
    break;

  case ORG:
    break;

  default:
    complainAndThrow("Bother", line);
    break;
  }
}

bool Assembler::doPass(int passNumber, FILE *fd)
{
  bool stable = true;
  ALine line;

  rewind(fd);

  try {
    while (!feof(fd)) {
      if (line.read(fd)) {
	line.parse();
	if (isDirective(line.getInstruction())) {
	  processDirective(line);
	}
	else {
	  assemble(line);
	}
      }
    }
  }
  catch (std::exception& e) {
  }

  return stable;
}

void Assembler::complainAndThrow(const char *msg, ALine &line)
{
}
