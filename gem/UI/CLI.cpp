#include <stdio.h>
#include <unistd.h>

#include "CLI.h"

#include "../gem.h"
#include "../Debug.h"
#include "../Machines/Machine.h"

CLI::CLI(Machine *m)
{
  int f = fileno(stdin);
  
  isTty = isatty(f);

  machine = m;
}

bool CLI::isBlank(char c)
{
  if ((c == ' ') || (c == '\t')) {
    return true;
  }

  return false;
}

char *CLI::skipWord(char *line)
{
  while (!isBlank(*line)) {
    line++;
  }

  return line;
}

char *CLI::skipBlanks(char *line)
{
  while (isBlank(*line)) {
    line++;
  }

  return line;
}

int CLI::makeArgs(char *line, char **argv, int maxArgs)
{
  int argc = 0;

  while (argc < maxArgs) {
    char *word = skipBlanks(line);
    char *blank = skipWord(word);
  }

  return argc;
}

void CLI::handleLine(char *line)
{
  char *cmd = skipBlanks(line);

  line = skipWord(line);
}

void CLI::go()
{
  char *line = new char[MaxLine];
  char *s = line;

  if (isTty) {
    printf("Command line monitor.\n");
  }

  while (s != NULL) {
    if (isTty) {
      printf("> ");
      fflush(stdout);
    }

    s = fgets(line, MaxLine, stdin);

    if (s) {
      handleLine(line);
    }
  }
}
