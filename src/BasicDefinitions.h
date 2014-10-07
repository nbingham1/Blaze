/*
	BasicDefinitions.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/26/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef BasicDefinitions_h
#define BasicDefinitions_h

#define GLEW_STATIC

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <time.h>

using namespace std;

char* better_fgets(char *line, int len, FILE *in_file);
char* int_to_char(int Num);

#endif
