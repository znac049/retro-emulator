#ifndef CODEWINDOW_H_
#define CODEWINDOW_H_

#include <ncurses.h>

#include "CPU.h"
#include "MemoryMap.h"
#include "CPUState.h"
#include "Window.h"

class CodeWindow : public Window {
 private:
  CPU *proc;
  CPUState *state;
  MemoryMap *mem;

  int *addresses;

 private:
  bool addressVisible(int addr);
  void highlightAddress(int addr);

 public:
  CodeWindow(int rows, int cols, int atRow, int atCol, CPU *cpu);
  ~CodeWindow();

  void display(int addr);
};

#endif /* CODEWINDOW_H_ */
