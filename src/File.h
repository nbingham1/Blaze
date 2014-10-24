/*
 *  File.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef File_h
#define File_h

#include "graphics.h"
#include "CoreMathematics.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

struct CoreFile
{
	FILE *file;

	void Open(const char *filename);
	void Close();
};

char* better_fgets(char *line, int len, FILE *in_file);
char* string_to_char(string value);
char* int_to_char(int Num);
int fsize(FILE *file);

#endif
