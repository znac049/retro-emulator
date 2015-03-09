#include <stdlib.h>

#include "ListNode.h"

template<class T> 
ListNode<T>::ListNode(const T *item, ListNode<T>* nextItem)
{
  this->data = item;
  this->nextNode = nextItem;
}

template<class T>
ListNode<T> *ListNode<T>::next()
{
  return this->nextNode;
}

