#ifndef _CLI_H_
#define _CLI_H_

class Machine;

class CLI
{
 private:
  typedef struct CommandEnt {
    const char *cmd;
    int val;
  } CommandEnt_t;

  static const int MaxLine = 200;
  static const CommandEnt_t Commands[];
  
  enum Cmds {NoCmd=-1,            QuitCmd=1,           
	     HelpCmd,             GoCmd,            
	     StepCmd,             AddBreakpointCmd, 
	     ListBreakpointsCmd,  DelBreakpointCmd, 
	     ClearBreakpointsCmd, AddWatchpointCmd, 
	     ListWatchpointsCmd,  DelWatchpointCmd,
	     ClearWatchpointsCmd, RegsCmd,
	     ResetCmd,            DumpBytesCmd,
	     DumpWordsCmd,        BaseCmd,
	     UnassembleCmd,       InfoCmd
  };

  bool isTty;
  Machine *machine;
  bool running;
  int radix;
  
 protected:
  bool isBlank(char c);
  char *skipWord(char *line);
  char *skipBlanks(char *line);
  char *chomp(char *line);

  int makeArgs(char *line, char **argv, int maxArgs);
  void handleLine(char *line);
  int lookupCommand(char *cmdName);
  int compareCommand(char *str, char *command);

  void doHelp();
  void doBaseCmd(int argc, char **argv);
  void doRegsCmd(int argc, char **argv);
  void doResetCmd(int argc, char **argv);
  void doStepCmd(int argc, char **argv);

 public:
  CLI(Machine *m);
  
  void go();
};

#endif
