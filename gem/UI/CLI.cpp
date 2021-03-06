#include <stdio.h>		// 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CLI.h"

#include "../gem.h"
#include "../MemoryMap.h"
#include "../Debug.h"
#include "../Machines/Machine.h"
#include "../CPUs/CPU.h"
#include "../Misc/Radix.h"

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
  {"i*nfo",        InfoCmd},
  {"de*bug",       DebugCmd}
};

CLI::CLI(Machine *m)
{
  int f = fileno(stdin);
  
  isTty = isatty(f);

  machine = m;

  Radix::set(16);

  dataAddr = 0x6000;
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

    case DebugCmd:
      doDebugCmd(argc-1, &argv[1]);
      break;

    case HelpCmd:
      doHelp();
      break;

    case BaseCmd:
      doBaseCmd(argc-1, &argv[1]);
      break;

    case DumpBytesCmd:
      doDumpBytesCmd(argc-1, &argv[1]);
      break;

    case RegsCmd:
      doRegsCmd(argc-1, &argv[1]);
      break;

    case ResetCmd:
      doResetCmd(argc-1, &argv[1]);
      break;

    case StepCmd:
      doStepCmd(argc-1, &argv[1]);
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
  if (argc == 0) {
    printf("current base is %d (dec), 0x%02x\n", Radix::get(), Radix::get());
    return;
  }

  if (argc != 1) {
    printf("usage: base <2|8|10|16|bin|oct|dec|hex>\n");
    return;
  }

  if (strcasecmp(argv[0], "bin") == 0) {
    Radix::set(2);
  }
  else if (strcasecmp(argv[0], "oct") == 0) {
    Radix::set(8);
  }
  else if (strcasecmp(argv[0], "dec") == 0) {
    Radix::set(10);
  }
  else if (strcasecmp(argv[0], "hex") == 0) {
    Radix::set(16);
  }
  else {
    int n = atoi(argv[0]);

    if ((n==2) || (n==8) || (n==10) || (n==16)) {
      Radix::set(n);
    }
    else {
      printf("unsupported radix '%s'\n", argv[0]);
    }
  }
}

void CLI::doRegsCmd(int argc, char **argv)
{
  CPU *cpu = machine->getProcessor();

  if (argc == 0) {
    cpu->printRegisters();
  }
  else if ((argc == 2) || (argc == 3)) {
    char *reg = argv[0];
    char *val = argv[1];
    int bits;

    if (argc == 3) {
      if (strcmp(val, "=") == 0) {
	val = argv[2];
      }
      else {
	printf("usage: r*egister [<register> [ = ] <value>]\n");
	return;
      }
    }

    printf("Set register %s to %s\n", reg, val);
    bits = cpu->sizeOfRegister(reg);
    if (bits) {
      int v = Radix::convert(val);
      cpu->setRegister(reg, v);
      printf("%s-> %s\n", reg, Radix::toString(v, bits));
    }
    else {
      printf("usage: r*egister [<register> [ = ] <value>]\n");
    }
  }
  else if (argc == 1) {
    char *reg = argv[0];
    char *eq = strchr(reg, '=');

    if (eq == NULL) {
      int bits = cpu->sizeOfRegister(reg);

      if (bits) {
	printf("%s: %s\n", reg, Radix::toString(cpu->getRegister(reg), bits));
      }
      else {
	printf("usage: r*egister [<register> [ = ] <value>]\n");
      }
    }
    else {
      int bits;

      *eq++ = '\0';
      bits = cpu->sizeOfRegister(reg);
      
      if (bits) {
	int v = Radix::convert(eq);
	cpu->setRegister(reg, v);
	printf("%s-> %s\n", reg, Radix::toString(v, bits));
      }
      else {
	printf("usage: r*egister [<register> [ = ] <value>]\n");
      }
    }
  }
  else {
    printf("usage: r*egister [<register> [ = ] <value>]\n");
  }
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

void CLI::doDebugCmd(int argc, char **argv)
{
  int lvl;

  if ((argc < 0) || (argc > 1)) {
    printf("usage: debug [ all | off | <level> ]\n");
    return;
  }

  if (argc) {
    char *level = argv[0];

    if (strcasecmp(level, "off") == 0) {
      Debug::setLevel(0);
    }
    else if (strcasecmp(level, "all") == 0) {
      Debug::setLevel(99);
    }
    else {
      lvl = Radix::convert(level);

      if (lvl == -1) {
	printf("usage: debug [ all | off | <level> ]\n");
	return;
      }

      Debug::setLevel(lvl);
    }
  }

  lvl = Debug::getLevel();

  if (lvl) {
    printf("Debug level is %s\n", Radix::toString(lvl));
  }
  else {
    printf("All debugging is turned off\n");
  }
}

void CLI::doStepCmd(int argc, char **argv)
{
  CPU *cpu = machine->getProcessor();
  char srcCode[200];

  cpu->disassembleOp(srcCode, sizeof(srcCode));
  printf("$%04x: %s\n", cpu->pc, srcCode);

  cpu->step();
}

void CLI::doDumpBytesCmd(int argc, char **argv)
{
  int nCols = 8;
  MemoryMap *mm = machine->getMemoryMap();

  switch (Radix::get()) {
  case 2:
    nCols = 4;
    break;

  case 8:
  case 10:
  case 16:
    nCols = 16;
    break;
  }

  if (argc > 1) {
    printf("usage: db*ytes [ <start addr> ]\n");
    return;
  }
  else if (argc == 1) {
    int addr = Radix::convert(argv[0]);

    if (addr == -1) {
      printf("usage: db*ytes [ <start addr> ]\n");
      return;
    }

    dataAddr = addr;
  }

  for (int lines=0; lines<16; lines++) {
    printf("%s:", Radix::toString(dataAddr, 16));

    for (int col=0; col<nCols; col++) {
      printf(" %s", Radix::toString(mm->peek(dataAddr+col), 8));
    }

    printf("    ");
    for (int col=0; col<nCols; col++) {
      char ch = mm->peek(dataAddr+col);

      if ((ch < ' ') || (ch >= 127)) {
	putchar('.');
      }
      else {
	putchar(ch);
      }
    }

    dataAddr += nCols;

    printf("\n");
  }
}
