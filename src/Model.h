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

#include "graphics.h"
#include <string.h>
#include "CoreMathematics.h"
#include "CorePhysics.h"
#include "Material.h"
#include "File.h"

struct ModelFace
{
	int v1, v2, v3;
	int n1, n2, n3;
	int t1, t2, t3;
	short MatID;

	ModelFace *Next;
};

struct ModelEdge
{
	int v1, v2;
	GLdouble RestDistance;

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
	GLdouble		Verts[2250000];
	GLdouble		Norms[2250000];
	GLdouble		Texts[2250000];
	ModelEdge *Edges;
	ModelFace *Faces;

	ModelFace *Current;

	Vector		Min, Max;
	GLdouble		Radius;

	Material   *Materials;

	PhsHandle Physics;

	int NumVerts;
	int NumNorms;
	int NumTexts;
	int NumEdges;
	int NumFaces;
	int NumMats;

	void AddVert(GLdouble x, GLdouble y, GLdouble z);
	void AddNorm(GLdouble x, GLdouble y, GLdouble z);
	void AddText(GLdouble x, GLdouble y, GLdouble z);
	void AddEdge(ModelEdge *Edge);
	void AddFace(ModelFace *Face);

	void Move(GLdouble x, GLdouble y, GLdouble z);

	void Render();
};

void LoadObj(Model *mdl, char *filename);

void LoadMaterials(Model *mdl, char *filename);
void LoadPhysics(Model *mdl, char *filename);
void GenerateNormals(Model *mdl);

void RenderMdl(Model *mdl);
void ReleaseMdl(Model *mdl);

#endif
