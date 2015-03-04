#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <ncurses.h>

#include "R6502.h"
#include "CodeWindow.h"
#include "TimerListener.h"

class Console : public TimerListener
{
 private:
  WINDOW *hexWin;
  WINDOW *statusWin;
  CodeWindow *codeWin;
  R6502 *proc;
  bool colorScreen;

  void setTitle(WINDOW *w, const char *str);

 public:
  Console(R6502 *proc);
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
