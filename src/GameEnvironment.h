/*
	GameEnvironment.h
	Blaze Game Engine 0.02

	Created by Ned Bingham on 6/6/06.
	Copyright 2006 Sol Union. All rights reserved.

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


#include "Main.h"
#include "GeometricalTypes.h"
#include "PhysicsTypes.h"
#include "GameTexture.h"
#include "GameModel.h"

#ifndef GameEnvironment_h
#define GameEnvironment_h

struct SkyPlane
{
	GLfloat vertices[225000];
	GLfloat texcoord[150000];
	GLubyte indices[225000];
	
	int NumVertices;
	
	GLuint SkyTexture;
	
	Vector Position;
	
	void Init(string filename, int division, GLfloat Radius, GLfloat peakheight);
	void Render(Vector Translation);
	void Release();
};

struct SkyBox
{
	GLfloat Size;
	Vector *Position;
	
	GLuint DispList;
	GLuint rt;
	GLuint lt;
	GLuint ft;
	GLuint bk;
	GLuint up;
	GLuint dn;
	
	void Init(string filename, string filetype, GLfloat size);
	void Render();
	void Release();
};

struct SkyDome
{
	GLfloat vertices[225000];
	GLfloat texcoord[150000];
	GLfloat norms[225000];
	unsigned short indices[225000];
	
	GLuint SkyTexture;
	
	Vector Position;
	
	int NumSides;
	int NumSlices;
	
	void Init(string filename, int slices, GLfloat Radius, GLfloat dampener);
	void Render(Vector Animation);
	void Release();
};

struct Water
{
	GLuint Reflect;
	
	GLfloat vertices[750000];
	GLfloat normals[750000];
	GLfloat texcoord[750000];
	GLfloat colors[750000];
	
	GLfloat forces[750000];
	GLfloat velocity[750000];
	unsigned short indices[750000];
	
	int NumDivs;
	int NumVerts;
		
	void Init(string filename, int divs, Vector Min, Vector Max);
	void CalculateNormals();
	void Update();
	void Render();
	void Release();
};

#define NumTreeTypes 1

#define Deciduous 0

struct TreeType
{
	GLuint TreeTypes[NumTreeTypes];
	
	void Init(string filenames[NumTreeTypes]);
	void Release();
};

struct Tree
{
	GLuint TreeType;
	GLfloat YXRatio;
	
	GameModel TreeModel;
	Vector *PlayerPos;
	
	void Init(GLfloat ratio, GLuint type, Vector pos, Vector *Player);
	void Render();
};

struct GameEnvironment
{
	SkyPlane plane;
	SkyBox box;
	SkyDome dome;
	Water WaterLevel;
	GameModel Map;
	TreeType Trees;
	Tree t;
	
	Vector *PlayerPos;
	
	int sky;
	
	void InitWithSkyPlane(string filename, int size, GLfloat height, GLfloat scale, string mapname, Vector *PlayerPos);
	void InitWithSkyBox(string filename, string filetype, GLfloat size, string mapname, Vector *PlayerPos);
	void InitWithSkyDome(string filename, int slices, GLfloat Radius, GLfloat damper, string mapname, Vector *PlayerPos);
	void Render(Vector Animation);
	void Release();
};

#endif
