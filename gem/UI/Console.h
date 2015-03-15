#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "../gem.h"

#include <ncurses.h>

class CodeWindow;
class CPU;

#include "../TimerListener.h"

class Console : public TimerListener
{
 private:
  WINDOW *hexWin;
  WINDOW *statusWin;
  CodeWindow *codeWin;
  CPU *proc;
  bool colorScreen;

  void setTitle(WINDOW *w, const char *str);

 public:
  Console(CPU *proc);
  ~Console();

  void initScreen();
  void updateScreen();

  void updateHex();
  void updateStatus();
  void updateCode();

  void commandLoop();

  void timerFired();
};

#endif
