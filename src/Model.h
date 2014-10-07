/*
	Model.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/13/06.
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

#include "Mesh.h"
#include "CorePhysics.h"
#include "Material.h"
#include "Lighting.h"
#include "CoreMathematics.h"

#ifndef Model_h
#define Model_h

struct ObjectMaterials
{
	Material *Mats;
	
	short NumMats;
};

struct Model
{
	PhysicsController	*Physics;
	Mesh				*Geometry;
	ObjectMaterials		*Materials;
			
	GLuint DisplayList;
	
	void (*CollisionFunctionGround)(Model *M, Triangle t);
	
	Model *Next;
};

bool BoxtoTriangleCollision(Model *M, Triangle t);
bool BoxtoBoxCollision(Model *ModelOne, Model *ModelTwo);

void LoadObj(Model *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexSize);
void LoadWRL(Model *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexSize);
void LoadHeightMap(Model *M, string heightmap, string materials, string physics, int size, int x_offset, int z_offset, int loadsize, GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, GLfloat tex_scale);

void LoadMats(Model *M, const char *filename);
void LoadPhysics(Model *M, const char *filename);

void GenNormals(Model *M);
void GenDisplayLists(Model *M);
void RenderModel(Model *M);
void ReleaseModel(Model *M);
void SetCollisionGroundFunction(Model *M, void (*pFunc)(Model *M, Triangle t));

void AnimateObjectGroundCollision(Model *M, Triangle t);
void InAnimateObjectGroundCollision(Model *M, Triangle t);

#endif
