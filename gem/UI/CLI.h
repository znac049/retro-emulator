#ifndef _CLI_H_
#define _CLI_H_

class Machine;

class CLI
{
 private:
  static const int MaxLine = 200;

  bool isTty;
  Machine *machine;
  
 protected:
  void handleLine(char *line);
  bool isBlank(char c);
  char *skipWord(char *line);
  char *skipBlanks(char *line);
  int makeArgs(char *line, char **argv, int maxArgs);

 public:
  CLI(Machine *m);
  
  void go();
};

#endif
