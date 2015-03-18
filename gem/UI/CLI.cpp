#include <stdio.h>		// 
#include <string.h>
#include <unistd.h>

#include "CLI.h"

#include "../gem.h"
#include "../Debug.h"
#include "../Machines/Machine.h"

const CLI::CommandEnt_t CLI::Commands[] = {
  {"q*uit",        QuitCmd},
  {"h*elp",        HelpCmd},
  {"g*o",          GoCmd},
  {"s*tep",        StepCmd},
  {"bp",           AddBreakpointCmd},
  {"bl*ist",       ListBreakpointsCmd},
  {"bd*elete",     DelBreakpointCmd},
  {"bc*lear",      ClearBreakpointsCmd},
  {"wp",           AddWatchpointCmd},
  {"wl*ist",       ListWatchpointsCmd},
  {"wd*elete",     DelWatchpointCmd},
  {"wc*lear",      ClearWatchpointsCmd},
  {"r*egister",    RegsCmd},
  {"res*et",       ResetCmd},
  {"db*ytes",      DumpBytesCmd},
  {"dw*ords",      DumpWordsCmd},
  {"ba*se",        BaseCmd},
  {"dis*assemble", UnassembleCmd},
  {"un*assemble",  UnassembleCmd}
};

CLI::CLI(Machine *m)
{
  int f = fileno(stdin);
  
  isTty = isatty(f);

  machine = m;
}

int CLI::compareCommand(char *str, char *command)
{
  int len = strlen(str);
  bool partial = false;

  for (int i=0; i<len; i++) {
    if (*command == '\0') {
      return 1;
    }

    if (*command == '*') {
      partial = true;
      command++;
    }

    if (str[i] < *command) {
      return -1;
    }
    else if (str[i] > *command) {
      return 1;
    }

    command++;
  }

  if (partial || (*command == '*') || (*command == '\0')) {
    return 0;
  }

  return -1;
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
  while (!isBlank(*line) && (*line != '\0')) {
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

char *CLI::chomp(char *line)
{
  int len = strlen(line);

  if (len && line[len-1] == '\n') {
    len--;

    if (len && line[len-1] == '\r') {
      len--;
    }

    line[len] = '\0';
  }

  return line;
}

int CLI::makeArgs(char *line, char **argv, int maxArgs)
{
  int argc = 0;
  char *word = line;
  char *blank;

  while (argc < maxArgs) {
    word = skipBlanks(word);
    blank = skipWord(word);

    if (*word == '\0') {
      return argc;
    }

    argv[argc++] = word;

    if (*blank == '\0') {
      return argc;
    }

    *blank++ = '\0';

    word = blank;
  }

  return argc;
}

void CLI::handleLine(char *line)
{
  char *cmd = skipBlanks(line);
  char *argv[10];
  int argc;

  argc = makeArgs(chomp(line), argv, 10);

  printf("Broke line up into %d parts\n", argc);
  for (int i=0; i<argc; i++) {
    printf("  %d: '%s'\n", i, argv[i]);
  }

  if (argc> 0) {
    int cmd = lookupCommand(argv[0]);

    switch (cmd) {
    case QuitCmd:
      if (isTty) {
	printf("Bye!\n");
      }
      running = false;
      break;

    case NoCmd:
      printf("Unknown command: '%s'. Type 'help' for list of commands.\n", argv[0]);
      break;

    default:
      printf("Unhandled command %d\n", cmd);
      break;
    }
  }
}

int CLI::lookupCommand(char *cmdName)
{
  printf("Looking for command '%s'\n", cmdName);

  for (int i=0; i<(sizeof(Commands) / sizeof(CommandEnt_t)); i++) {
    if (compareCommand(cmdName, (char *)Commands[i].cmd) == 0) {
      return Commands[i].val;
    }
  }

  return -1;
}

void CLI::go()
{
  char *line = new char[MaxLine];
  char *s = line;

  if (isTty) {
    printf("Command line monitor.\n");
  }

  running = true;

  while (running && s != NULL) {
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
