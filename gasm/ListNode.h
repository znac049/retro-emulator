#ifndef _NODE_H
#define _NODE_H

#include <iostream>

using namespace std;

template<class T>
class ListNode
{
 private:
  ListNode<T> *nextNode;

 public:
  T data;

 public:
  ListNode(const T *item, ListNode<T>* nextItem = NULL);

  ListNode<T> *next();
};

#endif
