#ifndef _REGISTER_H_
#define _REGISTER_H_

class Register
{
 private:
  int numBits;
  int mask;
  int val;
  char *name;

 public:
  Register(const char *reg, int bits);
  ~Register();

  void set(int newVal) { val = newVal & mask; };
  int get() { return val; };

  Register operator+(int newVal) { val = ((val + newVal) & mask); };
  Register operator++(int) { val = ((val + 1) & mask); };
  Register operator=(const int newVal) { val = newVal & mask; };
  Register operator=(const Register &reg) { val = reg.val & mask; };
  bool operator==(const int comp) { return val == comp; };
  bool operator==(const Register &reg) { return val == reg.val; };
};

#endif
