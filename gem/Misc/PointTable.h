#ifndef _POINTTABLE_H_
#define _POINTTABLE_H_

#include "AddressHash.h"

class PointTable : public AddressHash {
 private:
  int numPoints;

 public:
  PointTable();

  void clearAllPoints();
  void clearPoint(int addr);
  void setPoint(int addr);
  bool exists(int addr);
  int getNumberOfPoints() { return numPoints; };
};

#endif
