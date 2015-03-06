#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdio.h>

class ALine;

class Assembler 
{
 protected:
  enum Op {EQU, BYTE, WORD, ORG};
  
  struct pseudoOp {
    const char *op;
    int directive;
  };

  static const pseudoOp pseudoOps[];
  
  int org;

 private:
  int lookupDirective(const char *op);
  bool isDirective(const char *op);
  void complainAndThrow(const char *msg, ALine &line);

 public:
  Assembler();

  bool doPass(int passNumber, FILE *fd);
  void processDirective(ALine &line);
  void assemble(ALine &line);

};

#endif
