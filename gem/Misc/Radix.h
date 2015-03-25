#ifndef _RADIX_H_
#define _RADIX_H_

class Radix
{
 private:
  static const int MaxTmpStr = 500;
  static char pool[MaxTmpStr];
  static int ind;
  static int radix;

 private:
  static void resetPool();
  static char *getPool();
  static void allocBytes(int nBytes);
  static int getFreeBytes();

 public:
  static const int RESET = 1;

 public:
  static void set(int newRadix);
  static int get();
  static char *toRadix(int val, int places);
  static char *toRadix(int val, int places, int reset);
};

#endif
