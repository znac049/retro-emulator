#include "ALine.h"

const int ALine::MAXLINE = 512;

ALine::ALine() {
  label = new char[MAXLINE];
  instruction = new char[MAXLINE];
  args = new char[MAXLINE];
  comment = new char[MAXLINE];

  line = new char[MAXLINE];
  line[0] = 0;
}

bool ALine::read(FILE *fd)
{
  char *res = fgets(line, MAXLINE, fd);
  int len;

  if (res == 0) {
    line[0] = 0;
    return false;
  }

  len = strlen(line);

  if (line[len-1] == '\n') {
    len--;

    if (line[len-1] == '\r') {
      len--;
    }

    line[len] = 0;
  }

  return true;
}

/**
 */
bool ALine::parse()
{
  return false;
}

const char *ALine::getInstruction()
{
  return (const char *) instruction;
}
