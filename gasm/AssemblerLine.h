/*
 * AssemblerLine.h
 *
 *  Created on: 5 Mar 2015
 *      Author: bob
 */

#ifndef _GASM_ASSEMBLERLINE_H_
#define _GASM_ASSEMBLERLINE_H_

#include <stdio.h>
#include <string.h>

class AssemblerLine {
private:
	char *label;
	char *instruction;
	char *args;
	char *comment;

public:
	static const int MAXLINE;

public:
	AssemblerLine();

	bool readLine(FILE *fd);
};

#endif
