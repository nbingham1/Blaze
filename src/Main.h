/*
	Main.h
	Blaze Game Engine 0.02

	Created by Ned Bingham on 12/11/05.
	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef Main_h
#define Main_h

// C++ Libraries
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define GLEW_STATIC

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#define pi 3.1415926535898

#ifndef sqr
#define sqr(n) n*n
#endif

#ifndef DegToRad
#define DegToRad(d) (pi/180)*d
#endif

#ifndef RadToDeg
#define RadToDeg(r) (180/pi)*r
#endif

#ifndef Clamp
#define Clamp(x, min, max)	((x = (x < min ? min : x < max ? x : max)))
#endif

using namespace std;

char* string_to_char(string value);
char* int_to_char(int Num);
char* better_fgets(char *line, int len, FILE *in_file);

#endif
