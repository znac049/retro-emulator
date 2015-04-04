#ifndef _TIMER_STAMP_H_
#define _TIMER_STAMP_H_

#include <time.h>

class TimeStamp {
 private:
  timespec ts;

 public:
  TimeStamp();

  void now();
  void add(long nSecs);
  void wait();
};

#endif
