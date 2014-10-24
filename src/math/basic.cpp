/*
 * common.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: nbingham
 */

#include "basic.h"

bool is_even(int i)
{
	return ((i & 0xFFFFFFFE) == (unsigned int)i);
}

bool is_odd(int i)
{
	return ((i & 0xFFFFFFFE) != (unsigned int)i);
}

int log2i(int i)
{
	int r = 0;
	while (i >>= 1)
		r++;
	return r;
}


