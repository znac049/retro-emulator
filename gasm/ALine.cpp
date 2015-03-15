#include "ALine.h"

const int ALine::MAXLINE = 512;

ALine::ALine()
{
  label = new char[MAXLINE];
  instruction = new char[MAXLINE];
  args = new char[MAXLINE];
  comment = new char[MAXLINE];

  line = new char[MAXLINE];
  
  clear();
}

void ALine::clearParts()
{
  strcpy(label, "");
  strcpy(instruction, "");
  strcpy(args, "");
  strcpy(comment, "");
}

void ALine::clear()
{
  clearParts();

  strcpy(line, "");
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

  if ((len > 0) && (line[len-1] == '\n')) {
    len--;

    if ((len > 0) && (line[len-1] == '\r')) {
      len--;
    }

    line[len] = '\0';
  }

  return true;
}

/**
 */
void ALine::parse()
{
  char *cp;
  char *rest = line;

  clearParts();

  //printf("Parsing '%s'\n", line);

  // Remove any comments
  cp = strchr(line, ';');
  if (cp) {
    strncpy(comment, cp, MAXLINE);
    *cp = '\0';
  }

  // Do we have a label?
  if ((line[0]) && (!isBlank(line[0]))) {
    int len;

    // Label
    rest = nextBlank(line);
    if (rest == 0) {
      // Nothing after the label
      strncpy(label, line, MAXLINE);
      len = strlen(label);
      if (label[len-1] == ':') {
	label[len-1] = '\0';
      }
      return;
    }
    else {
      *rest++ = '\0';
      strncpy(label, line, MAXLINE);
      len = strlen(label);
      if (label[len-1] == ':') {
	label[len-1] = '\0';
      }
    }

    rest = skipBlanks(rest);
  }

  // Label (if present, has been dealt with
  rest = skipBlanks(rest);

  cp = nextBlank(rest);
  if (cp == 0) {
    // Just an instruction
    strncpy(instruction, rest, MAXLINE);
    return;
  }

  if (*cp) {
    *cp++ = '\0';
  }

  strncpy(instruction, rest, MAXLINE);

  strncpy(args, skipBlanks(cp), MAXLINE);

  return;
}

bool ALine::isBlank(char c)
{
  if ((c == ' ') || (c == '\t')) {
    return true;
  }

  return false;
}

char *ALine::skipBlanks(char *str)
{
  while (isBlank(*str)) {
    str++;
  }

  return str;
}

char *ALine::nextBlank(char *str)
{
  while (*str) {
    if (isBlank(*str)) {
      return str;
    }
    str++;
  }

  return 0;
}

const char *ALine::getComment()
{
  return (const char *) comment;
}

const char *ALine::getLabel()
{
  return (const char *) label;
}

const char *ALine::getInstruction()
{
  return (const char *) instruction;
}

const char *ALine::getArg()
{
  return (const char *) args;
}

bool ALine::hasComment()
{
  return (comment[0] != 0) ? true : false;
}

bool ALine::hasLabel()
{
  return (label[0] != 0) ? true : false;
}

bool ALine::hasInstruction()
{
  return (instruction[0] != 0) ? true : false;
}

bool ALine::hasArg()
{
  return (args[0] != 0) ? true : false;
}

void ALine::dump()
{
  printf("Dumping line:\n");

  if (hasLabel()) {
    printf("Label: %s\n", getLabel());
  }

  if (hasInstruction()) {
    printf("Instruction: %s\n", getInstruction());
  }

  if (hasArg()) {
    printf("Arg: %s\n", getArg());
  }

  if (hasComment()) {
    printf("Comment: %s\n", getComment());
  }
}
