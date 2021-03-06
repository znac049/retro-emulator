#include <ncurses.h>

#include "Console.h"
#include "../Timer.h"
#include "../CPUs/CPU.h"
#include "../UI/CodeWindow.h"

Console::Console(CPU *cpu)
{
  proc = cpu;

  initScreen();
  Timer::addListener(this, 999999);
}

Console::~Console()
{
  Timer::stop();
  endwin();
}

void Console::initScreen() {
  initscr();
  colorScreen = has_colors();

  if (colorScreen) {
    start_color();
    init_pair(1, COLOR_RED,   COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE,  COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
  }

  raw();
  keypad(stdscr, true);
  noecho();

  //hexWin =    newwin(34, 60, 0,  0);
  statusWin = newwin(12, 21, 0,  62);
  codeWin =   new CodeWindow(30, 61, 13, 62, proc);

  updateScreen();
}

void Console::updateScreen()
{
  updateStatus();
  //updateHex();
  updateCode();

  refresh();

  //wrefresh(hexWin);
  wrefresh(statusWin);
  codeWin->redraw();
}

void Console::updateHex()
{
  int nBytes = 32*16;
  int y = 1;
  MemoryMap *mem = proc->getMemory();

  setTitle(hexWin, "Memory");

  for (int i=0; i<nBytes; i++) {
    if ((i % 16) == 0) {
      mvwprintw(hexWin, y, 2, "$%04X:", i);
      y++;
    }

    wprintw(hexWin, " %02X", mem->peek(i));
  }
}

void Console::updateStatus()
{
  CPUState *state = proc->getState();
  char flags[30];

  state->getStatusFlagAsString(flags, sizeof(flags));

  setTitle(statusWin, "CPU Registers");

  mvwprintw(statusWin, 2, 6,     "A: %02X", state->a);
  mvwprintw(statusWin, 3, 6,     "X: %02X", state->x);
  mvwprintw(statusWin, 4, 6,     "Y: %02X", state->y);
  mvwprintw(statusWin, 5, 5,    "SP: %02X", state->sp);
  mvwprintw(statusWin, 6, 2, "Flags: %s",   flags);
  mvwprintw(statusWin, 8, 5,    "PC: %04X", state->pc);
  mvwprintw(statusWin, 9, 5,    "IR: %02X", state->ir);
}

void Console::updateCode()
{
  int pc = proc->getState()->pc;

  codeWin->display(pc);
}

void Console::commandLoop()
{
  int ch;

  Timer::start();
  updateScreen();

  ch = getch();

  while (ch != 'q') {
    switch (ch) {
    case 's':
    case ' ':
      proc->step();
      break;

    case 'g':
      proc->run();
      break;
    }

    updateScreen();

    ch = getch();
  }

  Timer::stop();
}

void Console::setTitle(WINDOW *w, const char *str)
{
  box(w, 0, 0);

  if (colorScreen) {
    attron(COLOR_PAIR(1));
  }

  mvwaddstr(w, 0, 2, str);
  if (colorScreen) {
    waddstr(w, " c");
  }
  else {
    waddstr(w, " m");
  }

  if (colorScreen) {
    attroff(COLOR_PAIR(1));
  }
}

void Console::timerFired()
{
  static const char *fish = "/-\\|";
  static int fishInd = 0;

  mvaddch(0, 0, fish[fishInd++]);
  fishInd &= 0x03;

  refresh();
}
