#ifndef _LABELTABLE_H_
#define _LABELTABLE_H_

#include "ListNode.h"

class Label;

class LabelTable
{
 private:
  ListNode *labels;
  int numItems;

 public:
  LabelTable();

  void add(Label *l);
  void dump();
};

#endif
