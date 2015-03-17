#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>

#include "Timer.h"
#include "Window.h"

TimerListener *Timer::listeners[MAX_TIMER_LISTENERS];
useconds_t Timer::intervals[MAX_TIMER_LISTENERS];
useconds_t Timer::tocks[MAX_TIMER_LISTENERS];

int Timer::nListeners = 0;
int Timer::ticksPerTock = 1;

Window *Timer::win;

void Timer::start()
{
  signal(SIGALRM, timerInterruptHandler);
  ualarm(ticksPerTock, ticksPerTock);
}

void Timer::stop()
{
  signal(SIGALRM, timerInterruptHandler);
  ualarm(0, 0);
}

void Timer::timerInterruptHandler(int sig)
{
  static long bilbo = 0;

  bilbo++;

  win->mvprintf(3, 2, "Bilbo: %d", bilbo);
  win->redraw();

  for (int i=0; i<nListeners; i++) {
    if (tocks[i] <= 0) {
      listeners[i]->timerFired();

      tocks[i] = intervals[i] / ticksPerTock;
    }

    tocks[i]--;
  }
}

void Timer::addListener(TimerListener *lstnr, useconds_t usec)
{
  if (nListeners >= MAX_TIMER_LISTENERS) {
    throw "Too many timer listeners";
  }

  listeners[nListeners] = lstnr;
  intervals[nListeners] = usec;
  tocks[nListeners] = usec / ticksPerTock;

  nListeners++;

  Timer::optimizeTocks();
}

void Timer::optimizeTocks()
{
  if (nListeners == 0) {
    ticksPerTock = 0;
  }
  else if (nListeners == 1) {
    ticksPerTock = intervals[0];
  }
  else {
    useconds_t min = intervals[0];
    useconds_t max = min;
    useconds_t best[MAX_TIMER_JITTER];

    for (int i=1; i<nListeners; i++) {
      if (intervals[i] < min){
	min = intervals[i];
      }

      if (intervals[i] > max) {
	max = intervals[i];
      }
    }

    for (int i=0; i<MAX_TIMER_JITTER; i++) {
      best[i] = max+1;
    }

    for (long div=1; div<=min; div++) {
      long maxJitter = 0;

      for (long i=0; i<nListeners; i++) {
	long res = (intervals[i] / div);
	long jitter = intervals[i] - (res * div);

	if (jitter > maxJitter) {
	  maxJitter = jitter;
	}
      }

      if (maxJitter < MAX_TIMER_JITTER) {
	best[maxJitter] = div;
      }
    }

    for (int i=0; i<MAX_TIMER_JITTER; i++) {
      if (best[i] != max+1) {
	ticksPerTock = best[i];
	break;
      }
    }
  } 

  for (int i=0; i<nListeners; i++) {
    tocks[i] = intervals[i] / ticksPerTock;
  }

  if (!win) {
    win = new Window(10, 30, 0, 0);
  }

  win->setTitle("Timer");
  win->mvprintf(1, 2, "nListeners: %d", nListeners);
  win->mvprintf(2, 2, "ticksPerTock: %d", ticksPerTock);

  for (int i=0; i<nListeners; i++) {
    win->mvprintf(4 + i, 2, "%d: %d %d", i, intervals[i], tocks[i]);
  }
  win->redraw();
}

long int Timer::getNanoTicks()
{
	// TODO: write the code!

	return 0;
}
