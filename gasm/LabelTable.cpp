#include <stdio.h>

#include <vector>
#include <exception>

#include "LabelTable.h"
#include "ListNode.h"
#include "Label.h"

LabelTable::LabelTable()
{
  labels = NULL;
  numItems = 0;
}

void LabelTable::add(Label *l)
{
  labels = new ListNode<Label *>(l, labels);
  numItems++;
}

void LabelTable::dump()
{
  ListNode<Label *> *node = labels;

  printf("Label table (%d entries):\n", numItems);

  for (int i=0; i<numItems; i++) {
    Label *lab = node->data;

    printf("%-25s = $%04x (%d) ", lab->getName(), lab->getValue(), lab->getValue());

    switch (lab->getAccess()) {
    case Label::RW:
      printf("RW");
      break;

    case Label::RO:
      printf("RO");
      break;

    case Label::WO:
      printf("WO");
      break;

    default:
      printf("??");
      break;
    }

    putchar('\n');

    node = node->next();
  }
}
