/*
	Mesh.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/12/06.
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

#include "BasicDefinitions.h"
#include "CoreMathematics.h"

#ifndef Mesh_h
#define Mesh_h

struct ModelTriangle
{
	short v1, v2, v3;
	short t1, t2, t3;
	short n1, n2, n3;
	short MatID;
	
	ModelTriangle *Next;
};

typedef Vector Box[16];

struct Mesh
{
	Vector		  Verts[750000];
	Vector		  Norms[750000];
	Vector		  Texts[750000];
	ModelTriangle *Faces;
	
	short NumVerts;
	short NumNorms;
	short NumTexts;
	short NumFaces;
	
	Box		BoundingBox;
	GLfloat Radius;
};

void MakeBox(Vector Min, Vector Max, Box TheBox);
GLfloat getmin(Vector *points, int NumPoints, Vector axis);
GLfloat getmax(Vector *points, int NumPoints, Vector axis);
bool isect(Vector *P1, int PNum1, Vector *P2, int PNum2, Vector axis);
bool isectboxbox(Vector Box1[16], Vector Box2[16]);
bool isectboxtri(Vector BoxPoints[16], Triangle TrianglePoints);

#endif
