#include <stdio.h>		// 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CLI.h"

#include "../gem.h"
#include "../Debug.h"
#include "../Machines/Machine.h"
#include "../CPUs/CPU.h"

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
  {"un*assemble",  UnassembleCmd},
  {"i*nfo",        InfoCmd}
};

CLI::CLI(Machine *m)
{
  int f = fileno(stdin);
  
  isTty = isatty(f);

  machine = m;
  radix = 16;
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

int CLI::lookupCommand(char *cmdName)
{
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
  printf("\n");
}

void CLI::handleLine(char *line)
{
  char *cmd = skipBlanks(line);
  char *argv[10];
  int argc;

  argc = makeArgs(chomp(line), argv, 10);

  if (argc> 0) {
    int cmd = lookupCommand(argv[0]);

    switch (cmd) {
    case QuitCmd:
      if (isTty) {
	printf("Bye!\n");
      }
      running = false;
      break;

    case HelpCmd:
      doHelp();
      break;

    case BaseCmd:
      doBaseCmd(argc-1, &argv[1]);
      break;

    case RegsCmd:
      doRegsCmd(argc-1, &argv[1]);
      break;

    case ResetCmd:
      doResetCmd(argc-1, &argv[1]);
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

void CLI::doHelp()
{
  printf("Commands:\n");

  for (int i=0; i<(sizeof(Commands) / sizeof(CommandEnt_t)); i++) {
    printf("  %s\n", Commands[i].cmd);
  }

  printf("\n");
}

void CLI::doBaseCmd(int argc, char **argv)
{
  if (argc != 1) {
    printf("usage: base <2|8|10|16|bin|oct|dec|hex>\n");
    return;
  }

  if (strcasecmp(argv[0], "bin") == 0) {
    radix = 2;
  }
  else if (strcasecmp(argv[0], "oct") == 0) {
    radix = 8;
  }
  else if (strcasecmp(argv[0], "dec") == 0) {
    radix = 10;
  }
  else if (strcasecmp(argv[0], "hex") == 0) {
    radix = 16;
  }
  else {
    int n = atoi(argv[0]);

    if ((n==2) || (n==8) || (n==10) || (n==16)) {
      radix = n;
    }
    else {
      printf("unsupported radix '%s'\n", argv[0]);
    }
  }
}

void CLI::doRegsCmd(int argc, char **argv)
{
}

void CLI::doResetCmd(int argc, char **argv)
{
  if (argc) {
    printf("usage: reset\n");
    return;
  }

  machine->getProcessor()->reset();

  printf("System reset.\nPC=$%04x\n\n", machine->getProcessor()->pc);
}
