/*
 * Input.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: nbingham
 */

#include "Input.h"


char* string_to_char(string value)
{
	char *result = new char[value.length() + 1];

	value.copy(result, value.length());
	result[value.length()] = '\0';

	return result;
}

char* int_to_char(int Num)
{
	char* n = new char[20];
	sprintf(n, "%i", Num);
	return n;
}

int fsize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}
