/*
	GameModel.h
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
#include "GeometricalTypes.h"
#include "PhysicsTypes.h"
#include "GameTexture.h"
#include "GameMaterial.h"

#ifndef GameModel_h
#define GameModel_h

struct RotationFrame
{
	Quaternion Orientation;
};

struct TranslationFrame
{
	Vector Position;
};

struct Bone
{
	char BoneID[11];
	char ParentID[11];
	
	Quaternion Orientation;
	Quaternion AngularVelocity;
	Quaternion AngularAccelaration;
	
	Vector Position;
	Vector LinearVelocity;
	Vector LinearAccelaration;
	
	int NumRotationFrames;
	int NumTranslationFrames;
	
	RotationFrame *Rotation;
	TranslationFrame *Translation;
	Bone *Parent;
	Bone *Next;
};

Vector Transform(Bone *start, Vector v, int nextframe, GLfloat percent);

struct ModelTriangle
{
	ModelTriangle();
	ModelTriangle(short V1, short V2, short V3);
	
	short v1, v2, v3;
	short t1, t2, t3;
	short n1, n2, n3;
	Bone *b1, *b2, *b3;
	short MatID;
	
	ModelTriangle *Next;
};

void CheckEquality(ModelTriangle t1, ModelTriangle t2, int indices[3], int *count);

struct ObjectGeometry
{
	Vertex		  Verts[750000];
	Vertex		  Norms[750000];
	Vertex		  Texts[750000];
	short		  Edges[75000][2];
	ModelTriangle *Faces;
	
	short NumVerts;
	short NumNorms;
	short NumTexts;
	short NumFaces;
	short NumEdges;
	
	Vertex Max;
	Vertex Min;
	
	GLfloat Radius;
};

void OrderCounterclockwise(Vertex Center, Vertex v1, Vertex v2, Vertex v3, int indices[3]);

GLfloat CalculateVolume(ObjectGeometry *G);
GLfloat CalculateSurfaceArea(ObjectGeometry *G);

struct ObjectMaterials
{
	Material *Mats;
	
	short NumMats;
};

struct ObjectPhysics
{
	Box BoundingBox;
	
	Vertex		Position;
	EulerAngle	Orientation;
	
	GLfloat		Mass;
	GLfloat		Density;
	GLfloat		Volume;
	GLfloat		SurfaceArea;
	GLfloat		Elasticity;
	GLfloat		Radius;
	
	Vector		LinearAccelaration;
	Vector		LinearVelocity;
	Vector		Momentum;
	
	EulerAngle	AngularAccelaration;
	EulerAngle	AngularVelocity;
	
	Vector		RotationalInertia;
	Vector		Torque;
};

struct Animation
{
	string name;
	int begin;
	int next;
	GLfloat percent;
	int end;
};

struct ObjectAnimation
{
	int NumBones;
	int NumFrameIndices;
	
	Bone *Skeleton;
	
	Animation FrameIndices;
};

struct GameModel
{
	ObjectPhysics		Physics;
	ObjectGeometry		*Geometry;
	ObjectMaterials		*Materials;
	ObjectAnimation		*Animation;
	int CurrentNode;
	int NextNode;
	
	GLuint DisplayList;
	void (*CollisionFunctionGround)(GameModel *M, Triangle t);
	
	bool collidewithground;
	
	GameModel *Next;
};

void	Update(GameModel *M, GLfloat DensityOfFluid);

inline GLfloat getmin(Vector *points, int NumPoints, Vector axis)
{
	GLfloat min = 9999999999999.0;
	
	for (int ctr = 0; ctr < NumPoints; ctr++)
    {
		GLfloat dotprod = Dot(points[ctr], axis);
		if (dotprod < min)
			min = dotprod;
    }
	return min;
}

inline GLfloat getmax(Vector *points, int NumPoints, Vector axis)
{
	GLfloat max = -9999999999999.0;
	
	for (int ctr = 0; ctr < NumPoints; ctr++)
    {
		GLfloat dotprod = Dot(points[ctr], axis);
		if (dotprod > max)
			max = dotprod;
    }
	return max;
}

inline bool isect(Vector *P1, int PNum1, Vector *P2, int PNum2, Vector axis)
{
	if (getmin(P1, PNum1, axis) > getmax(P2, PNum2, axis))
		return false;
	if (getmax(P1, PNum1, axis) < getmin(P2, PNum2, axis))
		return false;
		
	return true;     
}

bool isectboxtri(Vector BoxPoints[16], Triangle tri);
bool isectboxbox(Vector BoxPoints1[16], Vector BoxPoints2[16]);
bool BoxtoTriangleCollision(GameModel *M, Triangle t);
bool BoxtoBoxCollision(GameModel *ModelOne, GameModel *ModelTwo);

void LoadObj(GameModel *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexSize);
void LoadWRL(GameModel *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexSize);
void LoadHeightMap(GameModel *M, string heightmap, string materials, string physics, GLfloat size, GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, GLfloat tex_scale);

void LoadMats(GameModel *M, char *filename);
void LoadPhysics(GameModel *M, char *filename);
void LoadBones(GameModel *M, FILE *file);

void GenNormals(GameModel *M);
void GenDisplayLists(GameModel *M);
void RenderModel(GameModel *M, bool list);
void ReleaseModel(GameModel *M);
void SetCollisionGroundFunction(GameModel *M, void (*pFunc)(GameModel *M, Triangle t));

void AnimateObjectGroundCollision(GameModel *M, Triangle t);
void InAnimateObjectGroundCollision(GameModel *M, Triangle t);

#endif
