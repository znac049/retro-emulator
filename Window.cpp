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

void Window::redraw()
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

void Window::printf(const char *fmt, ...)
{
  va_list args;
 
  va_start(args, fmt);
  //assert(fmt != NULL);
  //assert(args != NULL);
  vwprintw(win, fmt, args);
  va_end(args);

  wrefresh(win);
}

void Window::mvprintf(int row, int col, const char *fmt, ...)
{
  va_list args;

  wmove(win, row, col);
 
  va_start(args, fmt);
  //assert(fmt != NULL);
  //assert(args != NULL);
  vwprintw(win, fmt, args);
  va_end(args);

  wrefresh(win);
}

void Window::clear()
{
  wclear(win);
}
