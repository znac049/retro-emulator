/*
 * AssemblerLine.cpp
 *
 *  Created on: 5 Mar 2015
 *      Author: bob
 */

#include "AssemblerLine.h"

const int AssemblerLine::MAXLINE = 512;

AssemblerLine::AssemblerLine() {
	label = new char[MAXLINE];
	instruction = new char[MAXLINE];
	args = new char[MAXLINE];
	comment = new char[MAXLINE];
}

bool AssemblerLine::readLine(FILE *fd)
{
	char *line = new char[MAXLINE];
	char *cp;

	if (!fgets(line, MAXLINE, fd)) {
		return false;
	}

	*label = 0;
	*instruction = 0;
	*args = 0;
	*comment = 0;

	// pull the line apart

	// do we have any comments?
	cp = strchr(line, ';');
	if (cp) {
		strcpy(comment, cp);
		*cp = 0;
	}


}

