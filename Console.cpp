#include <ncurses.h>

#include "AtariEm.h"
#include "Console.h"

Console::Console(CPU *cpu)
{
  proc = cpu;

  initScreen();
}

Console::~Console()
{
  endwin();
}

void Console::initScreen() {
  initscr();
  colorScreen = has_colors();

  if (colorScreen) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
  }

  raw();
  keypad(stdscr, true);
  noecho();

  hexWin =    newwin(34, 60, 0,  0);
  statusWin = newwin(12, 21, 0,  62);
  codeWin =   newwin(12, 61, 13, 62);

  updateScreen();
}

void Console::updateScreen()
{
  updateStatus();
  updateHex();
  updateCode();

  refresh();

  wrefresh(hexWin);
  wrefresh(statusWin);
  wrefresh(codeWin);
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
  CPUState *state = proc->getState();
  MemoryMap *mem = proc->getMemory();
  CPUState cs(mem);

  int maxX, maxY;
  int pc = state->pc;
  int size;

  char code[256];

  wclear(codeWin);
  setTitle(codeWin, "Code");

  getmaxyx(codeWin, maxY, maxX);

  mvwaddstr(codeWin, 0, 2, "Code");

  for (int y = 1; y < maxY-1; y++) {
    size = cs.load(pc);
    mvwprintw(codeWin, y, 3, "$%04X: ", pc);

    for (int i = 0; i < size; i++) {
      wprintw(codeWin, "%02X ", mem->peek(pc+i));
    }

    for (int i = size; i < 3; i++) {
      waddstr(codeWin, "   ");
    }

    cs.disassembleOp(code, sizeof(code));
    wprintw(codeWin, "  %s", code);

    pc += size;
  }
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
