/*
 * Atari Watchdog device.
 */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <ncurses.h>

#include "CPU.h"

class Console
{
 private:
  WINDOW *hexWin;
  WINDOW *statusWin;
  WINDOW *codeWin;
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
};

#endif
