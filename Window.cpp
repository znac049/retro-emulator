#include <ncurses.h>

#include "Window.h"

Window::Window(int nrows, int ncols, int atRow, int atCol)
{
  height = nrows;
  width = ncols;

  win = newwin(nrows, ncols, atRow, atCol);
  box(win, 0, 0);
}

Window::~Window() {
}

void Window::refresh()
{
  wrefresh(win);
}

void Window::setTitle(const char *title)
{
  box(win, 0, 0);
  wattron(win, COLOR_PAIR(2));
  mvwaddstr(win, 0, 2, title);
  wattroff(win, COLOR_PAIR(2));
}
