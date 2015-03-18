#ifndef _ADDRESSHASH_H_
#define _ADDRESSHASH_H_

class AddressHash {
 private:
  static const int HashEntrySize = 256;
  int **hashRoot;

 protected:
  int hits;
  int misses;

 private:
  int *newEntry();

 public:
  AddressHash();
  ~AddressHash();

  void clearAll();
  void set(int addr, int val);
  int get(int addr);
};

#endif
