#ifndef _NODE_H
#define _NODE_H

#include <iostream>

using namespace std;

class Label;

class ListNode
{
 private:
  ListNode *nextNode;

 public:
  Label *label;

 public:
  ListNode(Label *item, ListNode *nextItem);

  ListNode *next();
};

#endif
