#ifndef _DEBUG_H_
#define _DEBUG_H_

class Debug
{
 private:
  static int debugLevel;

 public:
  static int setLevel(int level);
  static int getLevel() { return debugLevel; };
  static void logf(int level, const char *fmt, ...);
};

#endif
