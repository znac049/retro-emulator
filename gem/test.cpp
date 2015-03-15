#include <stdio.h>

class Inst {
private:
  static const char *names[];

protected:
  const char **tab;

public:
  Inst();
  const char *getName(int ind);
};



class r6502 : public Inst
{
private:
  static const char *names[];

public:
  r6502();
};

const char *Inst::names[2] = {"Even", "Odd"};
const char *r6502::names[]= {"Zero", "One"};


Inst::Inst()
{
  tab = names;
}

r6502::r6502() : Inst()
{
  tab = names;
}

const char *Inst::getName(int ind)
{
  return tab[ind&1];
}

int main()
{
  Inst inst;
  r6502 r6;

  printf("Testing Inst...\n");
  for (int i=0; i<10; i++) {
    printf("%d: %s\n", i, inst.getName(i));
  }

  printf("Testing 6502...\n");
  for (int i=0; i<10; i++) {
    printf("%d: %s\n", i, r6.getName(i));
  }

  return 0;
}
