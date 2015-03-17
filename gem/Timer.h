#ifndef _TIMER_H_
#define _TIMER_H_

#include "TimerListener.h"
#include "Window.h"

#define MAX_TIMER_LISTENERS 10
#define MAX_TIMER_JITTER 4

class Timer {
 private:
  static TimerListener *listeners[];
  static useconds_t intervals[];
  static useconds_t tocks[];

  static int nListeners;
  static int ticksPerTock;

  static Window *win;

 private:
  static void optimizeTocks();

 protected:

 public:
  Timer();
  ~Timer();

  static void timerInterruptHandler(int sig);
  static void addListener(TimerListener *lstnr, useconds_t usec);
  static void start();
  static void stop();

  static long int getNanoTicks();
};

#endif
