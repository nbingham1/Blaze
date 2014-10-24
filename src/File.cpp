/*
 *  File.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "File.h"

void CoreFile::Open(const char *filename)
{
	file = fopen(filename, "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename);
		exit(1);
	}
}

void CoreFile::Close()
{
	fclose(file);
}

char* better_fgets(char *line, int len, FILE *in_file)
{
   char *temp = line;
   int   val;

   if (--len < 0)
      return NULL;

   if (len)
   {
      do
      {
         val = getc(in_file);

         if (val == EOF)
         {
            if (feof(in_file) && temp != line)
               break;
            else
            {
               line = NULL;
               return NULL;
            }
         }
         *temp++ = val;
      } while (val != '\r' && val != '\n' && --len);
   }
   *temp = '\0';

   return line;
}

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