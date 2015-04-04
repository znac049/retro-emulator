#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "gem.h"
#include "Debug.h"
#include "ADMachine.h"
#include "CLI.h"
#include "Misc/Radix.h"

void startGameWithConsole(const char *game)
{
  ADMachine m;
  CLI *cli;

  m.construct();

  cli = new CLI(&m);;

  m.printSummary();
  cli->go();
}

void startGame(const char *game)
{
  ADMachine m;
  
  m.construct();

  m.printSummary();
}

int main(int argc, char **argv)
{
  int opt;
  bool console = false;
  char gameName[MAXSTR];
  int val;

  strncpy(gameName, "AD-v3", MAXSTR);

  static struct option long_options[] = {
    {"console", no_argument, NULL, 'C'},
    {"defs",    optional_argument, NULL, 'd'},
    {"debug",   optional_argument, NULL, 'D'},
    {"game",    required_argument, NULL, 'g'},
    {NULL, 0, NULL, 0}
  };

  printf("Starting GEM\n");
  while ((opt = getopt_long(argc, argv, "Cg:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'C':
      console = true;
      break;

    case 'D':
      val = Radix::convert(optarg, 10);
      if (val == -1) {
	printf("Bad debug option: %s\n",  optarg);
	exit(1);
      }
      else {
	Debug::setLevel(val);
      }
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
