/*
	GameMisc.h
	Blaze Game Engine 0.01

	Created by Ned Bingham on 12/18/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Main.h"
#include "GameMatrix.h"

#ifndef GameMisc_h
#define GameMisc_h

char* string_to_char(string value);
char* int_to_char(int Num);
char* better_fgets(char *line, int len, FILE *in_file);
void CalculateNormal(Vector p1, Vector p2, Vector p3, Vector *Normal);
GLfloat Det(GLfloat a[4*4]);
GLfloat VolumeOfTetrahedron(GLfloat tetra[3*4]);
GLfloat AreaOfTriangle(Vector v0, Vector v1, Vector v2);
int intersect_RayTriangle(Vector R1, Vector R2, Vector T1, Vector T2, Vector T3, Vector *I, Vector *N);
GLfloat DistanceFormula(Vector V1, Vector V2);
int IntersectRayVolume(Vector R1, Vector R2, Vector *Vol, Vector VolNormal, int NumVol);

#endif
