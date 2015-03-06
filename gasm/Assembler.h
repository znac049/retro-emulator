#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdio.h>

class ALine;

class Assembler 
{
 potected:
  enum directrives {};
  struct pseudoOp {
    const char *op;
    int directive;
    int org = 0;
  };

 public:
  bool DoPass(int passNumber);
  void processDirective(ALine &line);
  void assemble(ALine &line);
};

#endif
