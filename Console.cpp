#include <ncurses.h>

#include "AtariEm.h"
#include "Console.h"

Console::Console(CPU *cpu)
{
  proc = cpu;

  //colorScreen = has_colors();
  colorScreen = false;
  initScreen();
}

Console::~Console()
{
  endwin();
}

void Console::initScreen() {
  initscr();
  raw();
  keypad(stdscr, true);
  noecho();

  if (colorScreen) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
  }

  hexWin = newwin(34, 60, 0, 0);
  box(hexWin, 0, 0);

  statusWin = newwin(12, 21, 0, 62);
  box(statusWin, 0, 0);

  updateScreen();
}

void Console::updateScreen()
{
  updateStatus();
  updateHex();

  refresh();

  wrefresh(hexWin);
  wrefresh(statusWin);
}

void Console::updateHex()
{
  int nBytes = 32*16;
  int y = 1;
  MemoryMap *mem = proc->getMemory();

  mvwprintw(hexWin, 0, 2, "%s", "Memory");

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

  if (colorScreen) {
    attron(COLOR_PAIR(1));
  }

  mvwprintw(statusWin, 0, 2, "%s", "CPU Registers");

  if (colorScreen) {
    attroff(COLOR_PAIR(1));
  }

  mvwprintw(statusWin, 2, 6,     "A: %02X", state->a);
  mvwprintw(statusWin, 3, 6,     "X: %02X", state->x);
  mvwprintw(statusWin, 4, 6,     "Y: %02X", state->y);
  mvwprintw(statusWin, 5, 5,    "SP: %02X", state->sp);
  mvwprintw(statusWin, 6, 2, "Flags: %s",   flags);
  mvwprintw(statusWin, 8, 5,    "PC: %04X", state->pc);
  mvwprintw(statusWin, 9, 5,    "IR: %02X", state->ir);
}

void Console::commandLoop()
{
  int ch = getch();

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
}
