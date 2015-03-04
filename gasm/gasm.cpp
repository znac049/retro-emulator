#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "gasm.h"

int main(int argc, char **argv)
{
  int opt;
  bool console = false;
  char gameName[MAXSTR];

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
}
