#ifndef CODEWINDOW_H_
#define CODEWINDOW_H_

#include <ncurses.h>

#include "Window.h"

class R6502;
class ProcessorState;
class MemoryMap;

class CodeWindow : public Window {
 private:
  R6502 *proc;
  ProcessorState *state;
  MemoryMap *mem;

  int *addresses;

 private:
  bool addressVisible(int addr);
  void highlightAddress(int addr);

 public:
  CodeWindow(int rows, int cols, int atRow, int atCol, R6502 *cpu);
  ~CodeWindow();

  void display(int addr);
};

#endif /* CODEWINDOW_H_ */
