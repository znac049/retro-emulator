#include <ncurses.h>

#include "CodeWindow.h"
#include "../MemoryMap.h"
#include "../CPUs/R6502.h"
#include "../CPUs/ProcessorState.h"

CodeWindow::CodeWindow(int nrows, int ncols, int atRow, int atCol, R6502 *cpu) :
  Window(nrows, ncols, atRow, atCol)
{
  height = nrows;
  width = ncols;

  proc = cpu;
  mem = cpu->getMemory();
  state = new CPUState(mem);

  addresses = new int[nrows-2];
  for  (int i=0; i<nrows-2; i++) {
    addresses[i] = -1;
  }

  win = newwin(nrows, ncols, atRow, atCol);
  box(win, 0, 0);
  setTitle("Code");
}

CodeWindow::~CodeWindow() {
}

void CodeWindow::display(int addr)
{
  // is that address already visible in the window?
  box(win, 0, 0);
  setTitle("Code");
  
  if (!addressVisible(addr)) {
    int pc = proc->getState()->pc;
    int nBytes;
    char code[100];

    clear();

    // Not on the window,need to redraw the contents to include it
    for (int y= 1; y < height-1; y++) {
      addresses[y-1] = -1;

      if (pc <= 0xffff) {
	addresses[y-1] = pc;

	nBytes = state->load(pc);
	mvwprintw(win, y, 3, "$%04x: ", pc);

	for (int i = 0; i < nBytes; i++) {
	  wprintw(win, "%02X ", mem->peek(pc+i));
	}

	for (int i = nBytes; i < 3; i++) {
	  waddstr(win, "   ");
	}

	state->disassembleOp(code, sizeof(code));
	wprintw(win, "  %s", code);
	
	pc += nBytes;
      }
    }
  }

  highlightAddress(addr);

  wrefresh(win);
}

void CodeWindow::highlightAddress(int addr)
{
  for (int y=0; y<height-2; y++) {
    wmove(win,  y+1, 2);

    if (addresses[y] == addr) {
      waddstr(win, ">");
      wchgat(win, width-4, COLOR_PAIR(4), 1, NULL);
    }
    else {
      wchgat(win, width-4, COLOR_PAIR(3), 1, NULL);
      waddstr(win, " ");
    }
  }

  wrefresh(win);
}

bool CodeWindow::addressVisible(int addr)
{
  if (addr < addresses[0]) {
    return false;
  }

  for (int y=0; y<height-2; y++) {
    if (addr <= addresses[y]){
      return true;
    }
    else if (addresses[y] == -1) {
      return false;
    }
  }

  return false;
}
