#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <ncurses.h>

class Window {
 private:

 protected:
  WINDOW *win;

  int height;
  int width;

 private:

 public:
  Window(int rows, int cols, int atRow, int atCol);
  ~Window();

  void refresh();
  void setTitle(const char *title);
};

#endif
