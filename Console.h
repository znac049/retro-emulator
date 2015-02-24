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
  CPU *proc;
  bool colorScreen;

 public:
  Console(CPU *proc);
  ~Console();

  void initScreen();
  void updateScreen();

  void updateHex();
  void updateStatus();

  void commandLoop();
};

#endif
