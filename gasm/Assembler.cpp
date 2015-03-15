#include <stdio.h>
#include <string.h>

#include <exception>

#include "Assembler.h"
#include "ALine.h"
#include "LabelTable.h"
#include "Label.h"

#include "UnknownInstruction.h"

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
  throw UnknownInstruction("Ooops!");
}

bool Assembler::isDirective(const char *op)
{
  int res = lookupDirective(op);

  return (res == -1) ? false : true;
}

int Assembler::lookupDirective(const char *op)
{
  //printf("Lookup directive '%s'\n", op);

  for (int i=0; i<(sizeof(pseudoOps) / sizeof(struct pseudoOp)); i++) {
    if (strcasecmp(op, pseudoOps[i].op) == 0) {
      return pseudoOps[i].directive;
    }
  }
   
  return -1;
}

void Assembler::processDirective(ALine &line)
{
  int op = lookupDirective(line.getInstruction());

  line.dump();

  switch (op) {
  case EQU:
    if (line.hasLabel()) {
      Label *l = new Label(line.getLabel(), 42);

      labels.add(l);
    }
    else {
      printf("EQU without label makes no sense!\n");
    }
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

  printf("Pass %d\n", passNumber);

  try {
    while (!feof(fd)) {
      if (line.read(fd)) {
	line.parse();

	if (line.hasLabel()) {
	  printf("Label: '%s'\n", line.getLabel());
	}

	if (line.hasInstruction()) {
	  printf("Instruction: '%s'\n", line.getInstruction());

	  if (isDirective(line.getInstruction())) {
	    processDirective(line);
	  }
	  else {
	    assemble(line);
	  }
	}
      }
    }
  }
  catch (UnknownInstruction &e) {
    printf("Unknown Instruction in pass %d: '%s'\n", passNumber, e.what());
    return false;
  }
  catch (std::exception &e) {
    printf("Caught an exception of some sort: '%s'\n", e.what());
    return false;
  }

  labels.dump();

  return stable;
}

void Assembler::complainAndThrow(const char *msg, ALine &line)
{
}
