#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "TimeStamp.h"

TimeStamp::TimeStamp()
{
  now();
}

void TimeStamp::now()
{
  clock_gettime(CLOCK_REALTIME, &ts);
}

void TimeStamp::add(long nSecs)
{
  ts.tv_nsec += nSecs;
  if (ts.tv_nsec >= 1000000000) {
    ts.tv_nsec -= 1000000000;
    ts.tv_sec++;
  }
}

void TimeStamp::wait()
{
  timespec now;
  timespec then;
  timespec rem;
  int res;

  clock_gettime(CLOCK_REALTIME, &now);

  then.tv_sec = 0;
  then.tv_nsec = 42;

  res = nanosleep(&then, &rem);
  while (res != 0) {
    then.tv_sec = rem.tv_sec;
    then.tv_nsec = rem.tv_nsec;

    res = nanosleep(&then, &rem);
  }
}
