/*
 * BreakpointTable.h
 *
 *  Created on: 17 Mar 2015
 *      Author: bob
 */

#ifndef _BREAKPOINTTABLE_H_
#define _BREAKPOINTTABLE_H_

#include "AddressHash.h"

class PointTable : public AddressHash {
private:
	int numPoints;

public:
	PointTable();
	~PointTable();

	void clearAllPoints();
	void clearPoint(int addr);
	void setPoint(int addr);
	bool exists(int addr);
	int getNumberOfPoints() { return numPoints; };
};

#endif
