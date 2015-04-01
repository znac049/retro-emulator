#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "gem.h"
#include "Debug.h"
#include "Machine.h"
#include "ADMachine.h"
#include "CLI.h"

void startGameWithConsole(const char *game)
{
  ADMachine m;
  CLI cli(&m);;

  m.printSummary();
  cli.go();
}

void startGame(const char *game)
{
  Machine m;

  m.printSummary();
}

int main(int argc, char **argv)
{
  int opt;
  bool console = false;
  char gameName[MAXSTR];

  Debug::setLevel(20);

  strncpy(gameName, "AD-v3", MAXSTR);

  static struct option long_options[] = {
    {"console", no_argument, NULL, 'C'},
    {"game",    required_argument, NULL, 'g'},
    {"defs",    optional_argument, NULL, 'd'},
    {NULL, 0, NULL, 0}
  };

  printf("Starting GEM\n");
  while ((opt = getopt_long(argc, argv, "Cg:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'C':
      console = true;
      break;

    case 'g':
      strncpy(gameName, optarg, MAXSTR);
      break;
      
    default:
      printf("Bad args!\n");
      exit(1);
      break;
    }
  }

  if (console) {
    startGameWithConsole(gameName);
  }
  else {
    startGame(gameName);
  }
}
