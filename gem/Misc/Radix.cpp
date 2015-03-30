#include <stdio.h>
#include <string.h>

#include "Radix.h"
#include "../Debug.h"

char Radix::pool[PoolSize];
int Radix::radix = 16;
int Radix::ind = 0;

void Radix::set(int newRadix)
{
  if ((newRadix == 2) || (newRadix == 8) || (newRadix == 10) || (newRadix == 16)) {
    radix = newRadix;
  }
}

int Radix::get()
{
  return radix;
}

char *Radix::allocateBytes(int nBytes)
{
  char *bytes = &pool[ind];

  ind += nBytes;
  
  // Is there enough room?
  if (ind >= PoolSize) {
    ind = nBytes;
    bytes = &pool[0];
  }

  return bytes;
}

char *Radix::toBinString(int val, int bits)
{
  int digits = 0;
  char *s, *cp; 

  for (int i=0; i<16; i++) {
    if (val & (1<<i)) {
      digits = i;
    }
  }
  digits++;
  
  if (bits) {
    if (bits > digits) {
      digits = bits;
    }
  }

  cp = s = allocateBytes(digits + 3);

  *cp++ = '%';
  *cp++ = 'b';
  for (int i=digits-1; i>=0; i--) {
    *cp++ = (val & (1<<i))?'1':'0';
  }
  *cp = '\0';

  return s;
}

char *Radix::toOctString(int val, int bits)
{
  int nBytes = 11;
  char *s;

  val &= 0xffff;

  if (bits == 0) {
    s = allocateBytes(nBytes);

    snprintf(s, nBytes, "0o%o", val);
  }
  else {
    char fmt[10];
    int places = (bits / 3) + 1;

    sprintf(fmt, "0o%%0%do", places);

    nBytes = places + 3;
    s = allocateBytes(nBytes);
    snprintf(s, nBytes, fmt, val);
  }

  return s;
}

char *Radix::toDecString(int val, int bits)
{
  int nBytes = 11;
  char *s;

  val &= 0xffff;

  if (bits == 0) {
    s = allocateBytes(nBytes);

    snprintf(s, nBytes, "%d", val);
  }
  else {
    char fmt[10];
    int places = 1;
    int n = 1 << bits;;

    while (n > 10) {
      places++;
      n = n / 10;
    }

    sprintf(fmt, "%%0%dd", places);

    nBytes = places + 1;
    s = allocateBytes(nBytes);
    snprintf(s, nBytes, fmt, val);
  }

  return s;
}

char *Radix::toHexString(int val, int bits)
{
  int nBytes = 7;
  char *s;

  val &= 0xffff;

  if (bits == 0) {
    s = allocateBytes(nBytes);

    snprintf(s, nBytes, "$%x", val);
  }
  else {
    char fmt[10];
    sprintf(fmt, "$%%0%dx", bits >> 2);

    nBytes = (bits >> 2) + 3;
    s = allocateBytes(nBytes);
    snprintf(s, nBytes, fmt, val);
  }

  return s;
}

int Radix::convert(const char *str, int base)
{
  char *digit = (char *)str;
  int res = 0;

  while (*digit) {
    char ch = *digit;
    int unit;

    if ((ch >= '0') && (ch <= '9')) {
      unit = ch - '0';
    }
    else if ((ch >= 'a') && (ch <= 'f')) {
      unit = ch - 'a' + 10;
    }
    else if ((ch >= 'A') && (ch <= 'F')) {
      unit = ch - 'F' + 10;
    }
    else {
      return -1;
    }

    res = (res * base) + unit;
    digit++;
  }
  
  return res;
}

int Radix::convert(const char *str)
{
  int base = radix;

  /*
   * Does it have any of the recognised prefixes:
   *  0x 0b 0o %h %b %o
   */
  if ((strncmp(str, "0x", 2) == 0) || (strncmp(str, "%h", 2) == 0)) {
    str+=2;
    base = 16;
  }
  else if ((strncmp(str, "0b", 2) == 0) || (strncmp(str, "%b", 2) == 0)) {
    str+=2;
    base = 2;
  }
  else if ((strncmp(str, "0o", 2) == 0) || (strncmp(str, "%o", 2) == 0)) {
    str+=2;
    base = 8;
  }
  
  return convert(str, base);
}

char *Radix::toString(int val)
{
  return toString(val, 0);
}

char *Radix::toString(int val, int bits)
{
  switch (radix) {
  case 2:
    return toBinString(val, bits);

  case 8:
    return toOctString(val, bits);

  case 10:
    return toDecString(val, bits);

  case 16:
    return toHexString(val, bits);

    return NULL;
  }
}

