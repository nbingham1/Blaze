/*
 * file.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "file.h"

int fsize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}
