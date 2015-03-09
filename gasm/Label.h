#ifndef _LABEL_H_
#define _LABEL_H_

class Label
{
 public:
  enum Access {RW, RO, WO};
  
 private:
  char *name;
  int value;
  Access access;

 public:
  Label(const char *labName, int val) {
    Label(labName, val, RW);
  };

  Label(Label &l) {
    Label(l.getName(), l.getValue(), l.getAccess());
  }

  Label() {
    Label("", -1, RW);
  }

  Label(const char *labName, int val, Access acc);

  ~Label();

  const char *getName();
  void setName(const char *str);

  int getValue();
  void setValue(int val);

  Access getAccess();
  void setAccess(Access acc);

  void setRW() { access = RW; };
  void setRO() { access = RO; };
  void setWO() { access = WO; };
};

#endif
