#include <stdlib.h>

#include "ListNode.h"
#include "Label.h"

ListNode::ListNode(Label *item, ListNode *nextItem)
{
  label = item;
  nextNode = nextItem;
}

ListNode *ListNode::next()
{
  return nextNode;
}

