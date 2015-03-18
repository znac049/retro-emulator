#include "PointTable.h"

PointTable::PointTable() : AddressHash()
{
  numPoints = 0;
}

void PointTable::clearAllPoints()
{
  clearAll();
}

void PointTable::clearPoint(int addr)
{
  set(addr, false);
}

void PointTable::setPoint(int addr)
{
  set(addr, true);
}

bool PointTable::exists(int addr)
{
  return get(addr);
}
