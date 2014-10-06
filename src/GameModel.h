/*
	GameModel.h
	Blaze Game Engine 0.01

	Created by Ned Bingham on 12/28/05.
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
#include "GameTexture.h"
#include "GameMisc.h"

#ifndef GameModel_h
#define GameModel_h

struct Triangle
{
	short v[3];
	short t[3];
	short n[3];
		
	Triangle *Next;
};

struct CTriangle
{
	Vector  Vertices[3];
	Vector  Normal;
	GLfloat Distance;
	
	CTriangle *Next;
	
	CTriangle &operator =(CTriangle t);
};

int intersect_RayPlane(Vector R1, Vector R2, CTriangle T, Vector *I);
void AddCTriangle(CTriangle *List, CTriangle *Next);
void DeleteCTriangle(CTriangle *List);

struct Groups
{
	Triangle *Faces;
	
	int Material;
};

struct Material
{
	char Name[32];
	
	Vector Emission;
	
	GLfloat Reflect;
	GLfloat Specular;
	GLfloat Opacity;
	
	char TextName[255];
	char BumpName[255];
	
	texture_t TextMap;
	texture_t BumpMap;
	GLuint	  ReflectMap[6];
	
	bool hasTexts;
	bool hasBumps;
};

struct MeshData
{
	Vector   Verts[75000];
	Vector   Texts[75000];
	Vector   Norms[75000];
	Groups   Group[64];
	Material Mtrls[64];
	CTriangle CData[150000];
	
	short NumVerts;
	short NumTexts;
	short NumNorms;
	short NumFaces;
	short NumGroup;
	
	Vector Max;
	Vector Min;
	GLfloat Radius;
	
	GLfloat Scale;
	
	GLuint *DispList;
	
	void InsertVert(Vector Vert);
	void InsertNorm(Vector Norm);
	void InsertText(Vector Text);
	void InsertFace(Triangle *Face, int GroupNum);
	void DeleteFace(int GroupNum);
	
	void Load(string filename, GLfloat scale);
	void LoadMesh(string filename, GLfloat scale);
	void LoadMats(char *filename);
	void LoadCFaces();
	
	void GenNorms();
	void GenList();
	
	void Render(bool list);
	
	void Release();
};

class Model
{
	public:
		Model();
		~Model();
		
		MeshData *Mesh;
		
		Vector Box[8];
		
		Vector Position;
		Vector Orientation;
		
		GLfloat Mass;
		GLfloat Radius;
		
		Vector LinearAccelaration;
		Vector LinearVelocity;
		
		Vector AngularAccelaration;
		Vector AngularVelocity;
		
		Vector Torque;
		Vector RotationalInertia;
		
		Vector Force;
		Vector ApplicationPoint;
		
		bool jump;
		
		void Initialize(MeshData *m);
		void Update();
		void UpdatePos();
		void Render(bool list);
		
		Model *Next;
};

#endif
