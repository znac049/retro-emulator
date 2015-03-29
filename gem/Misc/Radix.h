#ifndef _RADIX_H_
#define _RADIX_H_

class Radix
{
 private:
  static const int PoolSize = 500;
  static char pool[PoolSize];
  static int ind;
  static int radix;

 private:
  static char *allocateBytes(int nBytes);

  static char *toBinString(int val, int bits);
  static char *toOctString(int val, int bits);
  static char *toDecString(int val, int bits);
  static char *toHexString(int val, int bits);

 public:
  static void set(int newRadix);
  static int get();

  static int convert(const char *str);
  static int convert(const char *str, int base);

  static char *toString(int val);
  static char *toString(int val, int bits);
};

#endif
