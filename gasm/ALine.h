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
  void clear();
  void clearParts();
  bool isBlank(char c);
  char *nextBlank(char*str);
  char *skipBlanks(char *str);

 public:
  ALine();

  bool read(FILE *fd);
  void parse();
  const char *getComment();
  const char *getLabel();
  const char *getInstruction();
  const char *getArg();

  bool hasComment();
  bool hasLabel();
  bool hasInstruction();
  bool hasArg();
};

#endif
