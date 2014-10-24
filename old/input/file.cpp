/*
 * ios.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on January 20, 2012.
 * Modified by Ned Bingham on January 20, 2012.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "file.h"

int fsize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

int ftype(char *type, char *filename, int length)
{
	if (filename == NULL || type == NULL || length == 0)
		return 0;

	int i = length-1;
	for (i = length-1; i >= 0 && filename[i] != '.'; i--);

	tolower(type, filename + i, length-i);

	return length - i;
}

int ftype(char *type, char *filename)
{
	return ftype(type, filename, strlen(filename));
}

int fname(char *name, char *filename, int length)
{
	if (filename == NULL || name == NULL || length == 0)
		return 0;

	int i = length-1, j = length-1;
	for (i = length-1; i >= 0 && filename[i] != '/' && filename[i] != '\\'; i--)
		if (filename[i] == '.')
			j = i;

	strncpy(name, filename + i, j - i);

	return j-i;
}

int fname(char *name, char *filename)
{
	return fname(name, filename, strlen(filename));
}

int fpath(char *path, char *filename, int length)
{
	if (filename == NULL || path == NULL || length == 0)
		return 0;

	int i = length-1;
	for (i = length-1; i >= 0 && filename[i] != '/'; i--);

	strncpy(path, filename, i);

	return i;
}

int fpath(char *path, char *filename)
{
	return fpath(path, filename, strlen(filename));
}

void tolower(char *dest, char *src, int length)
{
	for (int i = 0; i < length; i++)
		dest[i] = tolower(src[i]);
}

void tolower(char *dest, char *src)
{
	tolower(dest, src, strlen(src));
}

void toupper(char *dest, char *src, int length)
{
	for (int i = 0; i < length; i++)
		dest[i] = toupper(src[i]);
}

void toupper(char *dest, char *src)
{
	toupper(dest, src, strlen(src));
}
