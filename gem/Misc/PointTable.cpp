/*
 * BreakpointTable.cpp
 *
 *  Created on: 17 Mar 2015
 *      Author: bob
 */

#include "PointTable.h"

PointTable::PointTable() : AddressHash()
{
	numPoints = 0;
}

PointTable::~PointTable() : ~AddressHash()
{
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
