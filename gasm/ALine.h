#ifndef _ALINE_H_
#define _ALINE_H_

#include <stdio.h>
#include <string.h>

class ALine {
 private:
  char *label;
  char *instruction;
  char *args;
  char *comment;

  char *line;

 private:
  static const int MAXLINE;

 private:

 public:
  ALine();

  bool read(FILE *fd);
  bool parse();
  const char *getInstruction();
};

#endif
