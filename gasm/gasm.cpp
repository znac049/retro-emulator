#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>

#include "gasm.h"
#include "Assembler.h"

void usage()
{
  printf("Blargh!\n");
  exit(1);
}

int main(int argc, char **argv)
{
  int opt;
  bool verbose = false;
  char sourceFile[MAXSTR];
  char processor[MAXSTR];
  int pass = 1;
  FILE *fd;
  Assembler ass;

  strcpy(processor, "");

  static struct option long_options[] = {
    {"verbose", no_argument, NULL, 'v'},
    {"cpu",     required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0}
  };

  printf("GASM Assembler v1.0\n");
  while ((opt = getopt_long(argc, argv, "Cg:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'v':
      verbose = true;
      break;

    case 'p':
      strncpy(processor, optarg, MAXSTR);
      break;
      
    default:
      printf("Bad args!\n");
      exit(1);
      break;
    }
  }

  if ((optind+1) == argc) {
    strncpy(sourceFile, argv[optind], MAXSTR);

    if (!processor[0]) {
    }
  }
  else {
    usage();
  }

  if (processor[0] == '\0') {
    char *bn = basename(sourceFile);
    char *dot = strrchr(bn, '.');

    if (dot) {
      dot++;

      printf("File extension: '%s'\n", dot);

      if (strcasecmp(dot, "a09") == 0) {
	strncpy(processor, "6809", MAXSTR);
      }
      else if (strcasecmp(dot, "a65") == 0) {
	strncpy(processor, "6502", MAXSTR);
      }
      else if (strcasecmp(dot, "a80") == 0) {
	strncpy(processor, "8080", MAXSTR);
      }
      else if (strcasecmp(dot, "a68") == 0) {
	strncpy(processor, "6800", MAXSTR);
      }
    }

    if (processor[0] == '\0') {
      printf("No processor specified and can't infer one from the source file name.\n");
      usage();
    }
  }

  printf("Source file: '%s'\n", sourceFile);
  printf("Processor:   '%s'\n", processor);

  fd = fopen(sourceFile, "r");
  if (!fd) {
    printf("Couldn't open source file '%s'\n", sourceFile);
    exit(1);
  }

  ass.doPass(pass, fd);
  fclose(fd);
}
