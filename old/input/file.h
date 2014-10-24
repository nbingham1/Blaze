/*
 * ios.h
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

#ifndef ios_h
#define ios_h

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <dirent.h>

int fsize(FILE *file);
int ftype(char *type, char *filename, int length);
int ftype(char *type, char *filename);
int fname(char *name, char *filename, int length);
int fname(char *name, char *filename);
int fpath(char *path, char *filename, int length);
int fpath(char *path, char *filename);

void tolower(char *dest, char *src, int length);
void tolower(char *dest, char *src);
void toupper(char *dest, char *src, int length);
void toupper(char *dest, char *src);

#endif
