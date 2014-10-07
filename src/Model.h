/*
 *  Model.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Model_h
#define Model_h

#include "OpenGLIncludes.h"
#include "CoreAnimation.h"
#include "CoreMathematics.h"
#include "Material.h"
#include "CorePhysics.h"

struct ModelFace
{
	ModelFace()
	{
		Next = NULL;
	}
	~ModelFace()
	{
		Next = NULL;
	}

	int v1, v2, v3;
	int n1, n2, n3;
	int t1, t2, t3;
	short MatID;
	
	ModelFace *Next;
};

struct ModelEdge
{
	ModelEdge()
	{
		Next = NULL;
	}
	~ModelEdge()
	{
		Next = NULL;
	}
	int v1, v2;
	double RestDistance;
	
	ModelEdge *Next;
};

inline bool operator ==(ModelEdge e1, ModelEdge e2)
{
	if ((e1.v1 == e2.v1 && e1.v2 == e2.v2) || (e1.v1 == e2.v2 && e1.v2 == e2.v1))
		return true;
	else
		return false;
}

struct Model
{
	Model()
	{
		Edges = NULL;
		Faces = NULL;
		Current = NULL;
		Materials = NULL;
	}
	~Model()
	{
	}

	GLfloat		Verts[2250000];
	GLfloat		Norms[2250000];
	GLfloat		Texts[2250000];
	ModelEdge *Edges;
	ModelFace *Faces;
	
	ModelFace *Current;
	
	Vector		Min, Max;
	GLfloat		Radius;
	
	Material   *Materials;
	
	ModelPhysics Physics;
	
	int NumVerts;
	int NumNorms;
	int NumTexts;
	int NumEdges;
	int NumFaces;
	int NumMats;
	
	void AddVert(GLfloat x, GLfloat y, GLfloat z);
	void AddNorm(GLfloat x, GLfloat y, GLfloat z);
	void AddText(GLfloat x, GLfloat y, GLfloat z);
	void AddEdge(ModelEdge *Edge);
	void AddFace(ModelFace *Face);
	
	void Move(double x, double y, double z);
};

void LoadObj(Model *mdl, char *filename);

void LoadMaterials(Model *mdl, char *filename);
void LoadPhysics(Model *mdl, char *filename);
void GenerateNormals(Model *mdl);

void RenderMdl(Model *mdl);
void ReleaseMdl(Model *mdl);

#endif
