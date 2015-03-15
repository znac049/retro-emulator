#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <exception>

#include "Label.h"

Label::Label(const char *labName, int val, Access acc)
{
  printf("MAKE LABEL: '%s'\n", labName);

  name = strdup(labName);
  value = val;
  access = acc;
}

Label::~Label()
{
  free(name);
}

const char *Label::getName()
{
  return name;
}

void Label::setName(const char *newName)
{
  free(name);
  name = strdup(newName);
}

int Label::getValue()
{
  return value;
}

void Label::setValue(int val)
{
  value = val;
}

Label::Access Label::getAccess()
{
  return access;
}
