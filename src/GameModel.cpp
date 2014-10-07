/*
	GameModel.cpp
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

#include "GameModel.h"
#include "GameApp.h"

Vector Transform(Bone *start, Vector v, int nextframe, GLfloat percent)
{
	Vector r = v;
	Bone *Current = start;
	while (Current != NULL)
	{
		//if (Current->NumRotationFrames != 0 && Current->NumTranslationFrames != 0)
		//{
			GLfloat a[16];
			QuaterniontoMatrix(Current->Orientation, a);
			r = (r-Current->Position);
		//}
		Current = Current->Parent;
	}
	return r;
}

ModelTriangle::ModelTriangle()
{
	Next = NULL;
	b1 = NULL;
	b2 = NULL;
	b3 = NULL;
}

ModelTriangle::ModelTriangle(short V1, short V2, short V3)
{
	Next = NULL;
	b1 = NULL;
	b2 = NULL;
	b3 = NULL;
	v1 = V1;
	v2 = V2;
	v3 = V3;
}

GLfloat CalculateVolume(ObjectGeometry *G)
{
	Vector v1, v2, v3;
	GLfloat Volume = 0.0;
	
	ModelTriangle *Curr = G->Faces;
	while (Curr != NULL)
	{
		v1 = G->Verts[Curr->v3];
		v2 = G->Verts[Curr->v2];
		v3 = G->Verts[Curr->v1];
		
		Volume += ((v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y)) * (v1.x + v2.x + v3.x);
		Curr = Curr->Next;
	}
	
	return Volume/6.0;
}

GLfloat CalculateSurfaceArea(ObjectGeometry *G)
{
	GLfloat SurfaceArea = 0.0;
	Triangle t;
	ModelTriangle *Curr = G->Faces;
	while (Curr != NULL)
	{
		t.v[0] = G->Verts[Curr->v1];
		t.v[1] = G->Verts[Curr->v2];
		t.v[2] = G->Verts[Curr->v3];
		SurfaceArea += AreaOfTriangle(t);
		Curr = Curr->Next;
	}
	
	return SurfaceArea;
}

void Update(GameModel *M, GLfloat DensityOfFluid)
{
	extern GameApp App;
	
	M->Physics.LinearAccelaration = Vector(0.0, 0.0, 0.0);
	
	M->Physics.LinearAccelaration += Vector(0.0, -9.8/App.refresh, 0.0);
	
	M->Physics.LinearVelocity += M->Physics.LinearAccelaration;
		
	// Angular Calculations
	//M->Physics.Torque = Vector(0.0, 0.0, 0.0);
	
	//M->Physics.AngularAccelaration = EulerAngle(0.0, 0.0, 0.0);
	//M->Physics.AngularAccelaration = (M->Physics.Torque/M->Physics.RotationalInertia);
	
	//M->Physics.AngularVelocity += M->Physics.AngularAccelaration;
		
	M->Physics.Position += M->Physics.LinearVelocity;
	M->Physics.Orientation += M->Physics.AngularVelocity;
}

bool isectboxtri(Vector BoxPoints[16], Triangle tri)
{
	Vector Normal = CalculateNormal(tri.v[0], tri.v[1], tri.v[2]);
	// test the x, y, and z axes
	if (!isect(BoxPoints, 16, tri.v, 3, Vector(1, 0, 0)))
		return false;
	if (!isect(BoxPoints, 16, tri.v, 3, Vector(0, 1, 0)))
		return false;
	if (!isect(BoxPoints, 16, tri.v, 3, Vector(0, 0, 1)))
		return false;
	if (!isect(BoxPoints, 16, tri.v, 3, Normal))
		return false;
	
	// test the triangle normal
	Vector triedge[] = {tri.v[1] - tri.v[0], tri.v[2] - tri.v[1], tri.v[0] - tri.v[2]};
	
	Vector boxedge[] = {BoxPoints[4]-BoxPoints[0], BoxPoints[1]-BoxPoints[0],
						BoxPoints[3]-BoxPoints[0], BoxPoints[12]-BoxPoints[8],
						BoxPoints[9]-BoxPoints[8], BoxPoints[11]-BoxPoints[8],
						BoxPoints[8]-BoxPoints[0], BoxPoints[9]-BoxPoints[1],
						BoxPoints[10]-BoxPoints[2], BoxPoints[11]-BoxPoints[3],
						BoxPoints[12]-BoxPoints[4], BoxPoints[13]-BoxPoints[5],
						BoxPoints[14]-BoxPoints[6], BoxPoints[15]-BoxPoints[7]};
	
	for (int x = 0; x < 14; x++)
		for (int y = 0; y < 3; y++)
			if (!isect(BoxPoints, 16, tri.v, 3, Cross(boxedge[x], triedge[y])))
				return false;
	
	
	return true;
	
}

bool isectboxbox(Vector BoxPoints1[16], Vector BoxPoints2[16])
{
	// test the x, y, and z axes
	if (!isect(BoxPoints1, 16, BoxPoints2, 16, Vector(1, 0, 0)))
		return false;
	if (!isect(BoxPoints1, 16, BoxPoints2, 16, Vector(0, 1, 0)))
		return false;
	if (!isect(BoxPoints1, 16, BoxPoints2, 16, Vector(0, 0, 1)))
		return false;
	
	// test the triangle normal
	
	Vector boxedge1[] = {BoxPoints1[4]-BoxPoints1[0], BoxPoints1[1]-BoxPoints1[0],
						BoxPoints1[3]-BoxPoints1[0], BoxPoints1[12]-BoxPoints1[8],
						BoxPoints1[9]-BoxPoints1[8], BoxPoints1[11]-BoxPoints1[8],
						BoxPoints1[8]-BoxPoints1[0], BoxPoints1[9]-BoxPoints1[1],
						BoxPoints1[10]-BoxPoints1[2], BoxPoints1[11]-BoxPoints1[3],
						BoxPoints1[12]-BoxPoints1[4], BoxPoints1[13]-BoxPoints1[5],
						BoxPoints1[14]-BoxPoints1[6], BoxPoints1[15]-BoxPoints1[7]};
	
	Vector boxedge2[] = {BoxPoints2[4]-BoxPoints2[0], BoxPoints2[1]-BoxPoints2[0],
						BoxPoints2[3]-BoxPoints2[0], BoxPoints2[12]-BoxPoints2[8],
						BoxPoints2[9]-BoxPoints2[8], BoxPoints2[11]-BoxPoints2[8],
						BoxPoints2[8]-BoxPoints2[0], BoxPoints2[9]-BoxPoints2[1],
						BoxPoints2[10]-BoxPoints2[2], BoxPoints2[11]-BoxPoints2[3],
						BoxPoints2[12]-BoxPoints2[4], BoxPoints2[13]-BoxPoints2[5],
						BoxPoints2[14]-BoxPoints2[6], BoxPoints2[15]-BoxPoints2[7]};
	
	for (int x = 0; x < 14; x++)
		for (int y = 0; y < 14; y++)
			if (!isect(BoxPoints1, 16, BoxPoints2, 16, Cross(boxedge1[x], boxedge2[y])))
				return false;
	
	
	return true;
}

bool BoxtoTriangleCollision(GameModel *M, Triangle t)
{
	Matrix A1, A2;
	Vertex v[16];
	EulertoMatrix(M->Physics.Orientation, A1);
	EulertoMatrix(M->Physics.Orientation + M->Physics.AngularVelocity, A2);
	for (int x = 0; x < 8; x++)
	{
		v[x] = A1*M->Physics.BoundingBox.v[x] + M->Physics.Position;
		v[x+8] = A2*M->Physics.BoundingBox.v[x] + M->Physics.Position + M->Physics.LinearVelocity;
	}
	
	bool coll = false;
	if (isectboxtri(v, t))
		coll = true;
	
	if (coll)
	{
		if (t.CollisionType == 0)
			M->CollisionFunctionGround(M, t);
		else if (t.CollisionType == 1)
		{
			coll = false;
		}
		else
		{
			coll = false;
		}
	}
	
	return coll;
}

bool BoxtoBoxCollision(GameModel *ModelOne, GameModel *ModelTwo)
{
	GLfloat A1[16], A2[16], A3[16], A4[16];
	Vector v1[16], v2[16];
	EulertoMatrix(ModelOne->Physics.Orientation, A1);
	EulertoMatrix(ModelOne->Physics.Orientation + ModelOne->Physics.AngularVelocity, A2);
	EulertoMatrix(ModelTwo->Physics.Orientation, A3);
	EulertoMatrix(ModelTwo->Physics.Orientation + ModelTwo->Physics.AngularVelocity, A4);
	for (int x = 0; x < 8; x++)
	{
		v1[x] = A1*ModelOne->Physics.BoundingBox.v[x] + ModelOne->Physics.Position;
		v1[x+8] = A2*ModelOne->Physics.BoundingBox.v[x] + ModelOne->Physics.Position + ModelOne->Physics.LinearVelocity;
		v2[x] = A3*ModelTwo->Physics.BoundingBox.v[x] + ModelTwo->Physics.Position;
		v2[x+8] = A4*ModelTwo->Physics.BoundingBox.v[x] + ModelTwo->Physics.Position + ModelTwo->Physics.LinearVelocity;
	}
	
	bool coll = false;
	if (isectboxbox(v1, v2))
		coll = true;
	
	if (coll)
	{
		GLfloat AverageElasticity = (ModelOne->Physics.Elasticity+ModelTwo->Physics.Elasticity)/2;
		
		Vector Velocity = EulertoVector(ModelOne->Physics.AngularVelocity - ModelTwo->Physics.AngularVelocity);
		Vector Numerator = -1*Vector(1, 1, 1)*(AverageElasticity+1);
		
		Vector UnitNormal = Normalize(ModelOne->Physics.Position-ModelTwo->Physics.Position);
		Vector ForceLocation2 = UnitNormal*ModelTwo->Physics.Radius;
		Vector TempVector = Cross(ForceLocation2, UnitNormal);
		
		TempVector /= ModelTwo->Physics.RotationalInertia;
		
		TempVector = Cross(TempVector, ForceLocation2);
		
		GLfloat part1 = Dot(UnitNormal, TempVector);
		
		UnitNormal *= -1;
		Vector ForceLocation1 = UnitNormal*ModelOne->Physics.Radius;
		TempVector = Cross(ForceLocation1, UnitNormal);
		
		TempVector /= ModelOne->Physics.RotationalInertia;
		
		TempVector = Cross(TempVector, ForceLocation1);
		
		GLfloat part2 = Dot(UnitNormal, TempVector);
		
		GLfloat Denominator = 1/ModelOne->Physics.Mass + 1/ModelTwo->Physics.Mass + part2 + part1;
		ModelOne->Physics.LinearVelocity -= (Numerator/Denominator)/ModelOne->Physics.Mass;
		ModelTwo->Physics.LinearVelocity += (Numerator/Denominator)/ModelTwo->Physics.Mass;
	}
	
	return coll;
}

void LoadObj(GameModel *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexScale)
{
	FILE *file;
	char oneline[255];
	char mtlname[255];
	char phsname[255];
	phsname[0] = '\0';
	short current_material = -1;
	ModelTriangle *c;
	
	GLfloat X, Y, Z;
	
	int v1, v2, v3,
		t1, t2, t3,
		n1, n2, n3;
	
	ObjectGeometry *G = (ObjectGeometry*)malloc(sizeof(ObjectGeometry));
	
	file = fopen(string_to_char(filename), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(string_to_char(filename));
		printf("\n");
		return;
	}
	
	G->NumVerts = 0;
	G->NumNorms = 0;
	G->NumTexts = 0;
	G->NumFaces = 0;
	G->NumEdges = 0;
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 225, file) == NULL)
			break;
			
		if (strncmp(oneline, "mtllib", 6) == 0)
		{
			if (sscanf(oneline, "mtllib %s", mtlname) == 1)
			{
				for (int x = 0; x < int(strlen(mtlname)); x++)
					if (mtlname[x] == '_')
						mtlname[x] = ' ';
				LoadMats(M, mtlname);
			}
		}
		
		if (strncmp(oneline, "phslib", 6) == 0)
		{
			if (sscanf(oneline, "phslib %s", phsname) == 1)
			{
				for (int x = 0; x < int(strlen(phsname)); x++)
					if (phsname[x] == '_')
						phsname[x] = ' ';
			}
		}
		
		if (strncmp(oneline, "usemtl", 6) == 0)
		{
			if ((M->Materials) && (sscanf(oneline, "usemtl %s", mtlname) == 1))
			{
				int x = 0;
				Material *CurrentMat = M->Materials->Mats;
				while (true)
				{
					if (strncmp(mtlname, CurrentMat->Name, 32) == 0)
					{
						current_material = x;
						break;
					}
					else
						current_material = -1;
					
					if (CurrentMat->Next == NULL)
						break;
					else
						CurrentMat = CurrentMat->Next;
					x++;
				}
			}
		}
		
		if (sscanf(oneline, "v %f %f %f", &X, &Y, &Z) == 3)
		{
			G->Verts[G->NumVerts] = Vector(X*sizeX, Y*sizeY, Z*sizeZ);
			G->NumVerts++;
			
			if (X*sizeX > G->Max.x)
				G->Max.x = X*sizeX;
			if (Y*sizeY > G->Max.y)
				G->Max.y = Y*sizeY;
			if (Z*sizeZ > G->Max.z)
				G->Max.z = Z*sizeZ;
			if (X*sizeX < G->Min.x)
				G->Min.x = X*sizeX;
			if (Y*sizeY < G->Min.y)
				G->Min.y = Y*sizeY;
			if (Z*sizeZ < G->Min.z)
				G->Min.z = Z*sizeZ;
		}
		
		else if (sscanf(oneline, "vn %f %f %f", &X, &Y, &Z) == 3)
		{
			G->Norms[G->NumNorms] = Vector(X, Y, Z);
			G->NumNorms++;
		}
		
		else if (sscanf(oneline, "vt %f %f %f", &X, &Y, &Z) == 3)
		{
			G->Texts[G->NumTexts] = Vector(X*TexScale, Y*TexScale, Z*TexScale);
			G->NumTexts++;
		}
		
		else if (sscanf(oneline, "vt %f %f", &X, &Y) == 2)
		{
			G->Texts[G->NumTexts] = Vector(X*TexScale, Y*TexScale, 0);
			G->NumTexts++;
		}
		
		else if (sscanf(oneline, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3) == 9)
		{
			ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (G->NumVerts + v1) : --v1;
			Face->v2 = (v2 < 0) ? (G->NumVerts + v2) : --v2;
			Face->v3 = (v3 < 0) ? (G->NumVerts + v3) : --v3;
			
			Face->t1 = (t1 < 0) ? (G->NumTexts + t1) : --t1;
			Face->t2 = (t2 < 0) ? (G->NumTexts + t2) : --t2;
			Face->t3 = (t3 < 0) ? (G->NumTexts + t3) : --t3;
			
			Face->n1 = (n1 < 0) ? (G->NumNorms + n1) : --n1;
			Face->n2 = (n2 < 0) ? (G->NumNorms + n2) : --n2;
			Face->n3 = (n3 < 0) ? (G->NumNorms + n3) : --n3;
			
			Face->b1 = NULL;
			Face->b2 = NULL;
			Face->b3 = NULL;
			
			Face->MatID = current_material;
			
			if (G->NumFaces == 0)
			{
				G->Faces = Face;
				c = G->Faces;
			}
			else
			{
				c->Next = Face;
				c = c->Next;
			}
			G->NumFaces++;
		}
		
		else if (sscanf(oneline, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3) == 6)
		{
			ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (G->NumVerts + v1) :--v1;
			Face->v2 = (v2 < 0) ? (G->NumVerts + v2) :--v2;
			Face->v3 = (v3 < 0) ? (G->NumVerts + v3) :--v3;
			
			Face->t1 = -1;
			Face->t2 = -1;
			Face->t3 = -1;
			
			Face->n1 = (n1 < 0) ? (G->NumNorms + n1) : --n1;
			Face->n2 = (n2 < 0) ? (G->NumNorms + n2) : --n2;
			Face->n3 = (n3 < 0) ? (G->NumNorms + n3) : --n3;
			
			Face->b1 = NULL;
			Face->b2 = NULL;
			Face->b3 = NULL;
			
			Face->MatID = current_material;
			if (G->NumFaces == 0)
			{
				G->Faces = Face;
				c = G->Faces;
			}
			else
			{
				c->Next = Face;
				c = c->Next;
			}
			G->NumFaces++;
		}
		
		else if (sscanf(oneline, "f %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3) == 6)
		{
			ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (G->NumVerts + v1) :--v1;
			Face->v2 = (v2 < 0) ? (G->NumVerts + v2) :--v2;
			Face->v3 = (v3 < 0) ? (G->NumVerts + v3) :--v3;
			
			Face->t1 = (t1 < 0) ? (G->NumTexts + t1) : --t1;
			Face->t2 = (t2 < 0) ? (G->NumTexts + t2) : --t2;
			Face->t3 = (t3 < 0) ? (G->NumTexts + t3) : --t3;
			
			Face->n1 = -1;
			Face->n2 = -1;
			Face->n3 = -1;
			
			Face->b1 = NULL;
			Face->b2 = NULL;
			Face->b3 = NULL;
			
			Face->MatID = current_material;
			if (G->NumFaces == 0)
			{
				G->Faces = Face;
				c = G->Faces;
			}
			else
			{
				c->Next = Face;
				c = c->Next;
			}
			G->NumFaces++;
		}
		
		else if (sscanf(oneline, "f %d %d %d", &v1, &v2, &v3) == 3)
		{
			ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (G->NumVerts + v1) :--v1;
			Face->v2 = (v2 < 0) ? (G->NumVerts + v2) :--v2;
			Face->v3 = (v3 < 0) ? (G->NumVerts + v3) :--v3;
			
			Face->t1 = -1;
			Face->t2 = -1;
			Face->t3 = -1;
			
			Face->n1 = -1;
			Face->n2 = -1;
			Face->n3 = -1;
			
			Face->b1 = NULL;
			Face->b2 = NULL;
			Face->b3 = NULL;
			
			Face->MatID = current_material;
			if (G->NumFaces == 0)
			{
				G->Faces = Face;
				c = G->Faces;
			}
			else
			{
				c->Next = Face;
				c = c->Next;
			}
			G->NumFaces++;
		}
	}
	fclose(file);
	
	Vector m = MidPoint(G->Max, G->Min);
	
	if (fabs(G->Max.x-m.x) > G->Radius)
		G->Radius = fabs(G->Max.x-m.x);
	if (fabs(G->Max.y-m.y) > G->Radius)
		G->Radius = fabs(G->Max.y-m.y);
	if (fabs(G->Max.z-m.z) > G->Radius)
		G->Radius = fabs(G->Max.z-m.z);
	if (fabs(G->Min.x+m.x) > G->Radius)
		G->Radius = fabs(G->Max.x+m.x);
	if (fabs(G->Min.y+m.y) > G->Radius)
		G->Radius = fabs(G->Max.y+m.y);
	if (fabs(G->Min.z+m.z) > G->Radius)
		G->Radius = fabs(G->Max.z+m.z);
	
	M->Geometry = G;
	if (phsname[0] != '\0')
		LoadPhysics(M, phsname);
	cout << "done loading" << endl;
}

void LoadWRL(GameModel *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexSize)
{
	FILE *file;
	char oneline[255];
	char phsname[255];
	phsname[0] = '\0';
	short current_material = 0;
	ModelTriangle *c;
	
	GLfloat X, Y, Z;
	int v1, v2, v3,
		t1, t2, t3,
		n1, n2, n3;
	
	ObjectGeometry *G = (ObjectGeometry*)malloc(sizeof(ObjectGeometry));
	ObjectAnimation *A = (ObjectAnimation*)malloc(sizeof(ObjectAnimation));
	
	file = fopen(string_to_char(filename), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(string_to_char(filename));
		printf("\n");
		return;
	}
	
	G->NumVerts = 0;
	G->NumNorms = 0;
	G->NumTexts = 0;
	G->NumFaces = 0;
	G->NumEdges = 0;
	
	M->Animation = A;
	
	string input = "";
	string previnput;
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 225, file) == NULL)
			break;
		
		if (strncmp(oneline, string_to_char(input + "{"), (input+"{").length()) == 0 || strncmp(oneline, string_to_char(input + "["), (input+"[").length()) == 0)
			input += " ";
		
		previnput = input;
		input = "";
		if (previnput.length() > 0)
		{
			for (unsigned int x = 0; x < previnput.length()-1; x++)
				input += previnput[x];
			if (strncmp(oneline, string_to_char(input+"}"), (input+"}").length()) == 0 || strncmp(oneline, string_to_char(input+"]"), (input+"]").length()) == 0)
				previnput = input;
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, string_to_char(input + "vertex ["), (input + "vertex [").length()) == 0)
		{
			while (strncmp(oneline, string_to_char(previnput + "]"), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, string_to_char(input + " %f %f %f,"), &X, &Y, &Z) == 3)
				{
					G->Verts[G->NumVerts] = Vector(X*sizeX, Y*sizeY, Z*sizeZ);
					G->NumVerts++;
					G->NumNorms++;
					
					if (X*sizeX > G->Max.x)
						G->Max.x = X*sizeX;
					if (Y*sizeY > G->Max.y)
						G->Max.y = Y*sizeY;
					if (Z*sizeZ > G->Max.z)
						G->Max.z = Z*sizeZ;
					if (X*sizeX < G->Min.x)
						G->Min.x = X*sizeX;
					if (Y*sizeY < G->Min.y)
						G->Min.y = Y*sizeY;
					if (Z*sizeZ < G->Min.z)
						G->Min.z = Z*sizeZ;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, string_to_char(input + "coordIndex ["), (input + "coordIndex [").length()) == 0)
		{	
			input = previnput;
			input += " %d %d %d -1,";
			while (strncmp(oneline, string_to_char(previnput + "]"), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, string_to_char(input), &v1, &v2, &v3) == 3)
				{
					ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
					Face->Next = NULL;

					Face->v1 = v1;
					Face->v2 = v2;
					Face->v3 = v3;

					Face->t1 = -1;
					Face->t2 = -1;
					Face->t3 = -1;
					
					Face->n1 = -1;
					Face->n2 = -1;
					Face->n3 = -1;
					
					Face->b1 = NULL;
					Face->b2 = NULL;
					Face->b3 = NULL;
					
					Face->MatID = current_material;
					if (G->NumFaces == 0)
					{
						G->Faces = Face;
						c = G->Faces;
					}
					else
					{
						c->Next = Face;
						c = c->Next;
					}
					G->NumFaces++;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, string_to_char(input + "normal ["), (input + "normal [").length()) == 0)
		{	
			while (strncmp(oneline, string_to_char(previnput + "]"), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, string_to_char(input + " %f %f %f,"), &X, &Y, &Z) == 3)
				{
					G->Norms[G->NumNorms] = Vector(X, Y, Z);
					G->NumNorms++;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, string_to_char(input + "normalIndex ["), (input + "normalIndex [").length()) == 0)
		{
			ModelTriangle *Current = G->Faces;
			while (Current != NULL)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, string_to_char(input + " %d %d %d -1,"), &n1, &n2, &n3) == 3)
				{
					Current->n1 = n1;
					Current->n2 = n2;
					Current->n3 = n3;
				}
				Current = Current->Next;
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, string_to_char(input + "texCoord ["), (input + "texCoord [").length()) == 0)
		{
			while (strncmp(oneline, string_to_char(previnput + "]"), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, string_to_char(input + " %f %f,"), &X, &Y) == 2)
				{
					G->Texts[G->NumTexts] = Vector(X, Y, 0);
					G->NumTexts++;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, string_to_char(input + "texCoordIndex ["), (input + "texCoordIndex [").length()) == 0)
		{
			ModelTriangle *Current = G->Faces;
			while (Current != NULL)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, string_to_char(input + " %d %d %d -1,"), &t1, &t2, &t3) == 3)
				{
					Current->t1 = t1;
					Current->t2 = t2;
					Current->t3 = t3;
				}
				Current = Current->Next;
			}
		}
		input = previnput;
		
		if (strncmp(oneline, "Group", 5) == 0)
		{
			M->Geometry = G;
			LoadBones(M, file);
		}
	}
	fclose(file);
	
	Vector m = MidPoint(G->Max, G->Min);
	
	if (fabs(G->Max.x-m.x) > G->Radius)
		G->Radius = fabs(G->Max.x-m.x);
	if (fabs(G->Max.y-m.y) > G->Radius)
		G->Radius = fabs(G->Max.y-m.y);
	if (fabs(G->Max.z-m.z) > G->Radius)
		G->Radius = fabs(G->Max.z-m.z);
	if (fabs(G->Min.x+m.x) > G->Radius)
		G->Radius = fabs(G->Max.x+m.x);
	if (fabs(G->Min.y+m.y) > G->Radius)
		G->Radius = fabs(G->Max.y+m.y);
	if (fabs(G->Min.z+m.z) > G->Radius)
		G->Radius = fabs(G->Max.z+m.z);
	
	if (phsname[0] != '\0')
		LoadPhysics(M, phsname);
	cout << "done loading" << endl;
}

void LoadHeightMap(GameModel *M, string heightmap, string materials, string physics, GLfloat size, GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, GLfloat tex_scale)
{
	unsigned char data[int(size*size)];
	
	FILE *file;
	ModelTriangle *c;
	
	
	file = fopen(string_to_char(heightmap), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(string_to_char(heightmap));
		printf("\n");
		return;
	}
	
	fread(data, 1, int(size*size), file);
	
	fclose(file);
	
	ObjectGeometry *G = (ObjectGeometry*)malloc(sizeof(ObjectGeometry));
	G->NumVerts = 0;
	G->NumTexts = 0;
	G->NumFaces = 0;
	G->NumNorms = 0;
	
	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			G->Verts[z*int(size) + x] = Vector(x*x_scale, -GLfloat(data[z*int(size) + x])*y_scale, z*z_scale);
			
			G->NumVerts++;
			G->Texts[z*int(size) + x] = Vector(x*tex_scale, z*tex_scale, 0.0);
			G->NumTexts++;
			G->NumNorms++;
		}
	}
	G->Max.x = size*x_scale + x_scale;
	G->Min.x = -x_scale;
	G->Max.y = 256*y_scale;
	G->Min.y = -y_scale;
	G->Max.z = size*z_scale + z_scale;
	G->Min.z = -z_scale;
	
	Vector Mid = MidPoint(G->Max, G->Min);
	G->Min.x -= Mid.x;
	G->Min.z -= Mid.z;
	G->Max.x -= Mid.x;
	G->Max.z -= Mid.z;
	
	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			G->Verts[z*int(size) + x] -= Vector(Mid.x, 0.0, Mid.z);
			G->Texts[z*int(size) + x].z = (G->Max.y - G->Min.y)/(G->Max.y - GLfloat(data[z*int(size) + x])*y_scale);
		}
	}
	
	for (int z = 0; z < size-1; z++)
	{
		for (int x = 0; x < size-1; x++)
		{
			ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face->Next = NULL;
			
			Face->v1 = z*int(size) + x;
			Face->v2 = (z+1)*int(size) + x;
			Face->v3 = z*int(size) + (x+1);
			
			Face->t1 = z*int(size) + x;
			Face->t2 = (z+1)*int(size) + x;
			Face->t3 = z*int(size) + (x+1);
			
			Face->n1 = z*int(size) + x;
			Face->n2 = (z+1)*int(size) + x;
			Face->n3 = z*int(size) + (x+1);
			
			Face->b1 = NULL;
			Face->b2 = NULL;
			Face->b3 = NULL;
			
			Face->MatID = 0;
			if (G->NumFaces == 0)
			{
				G->Faces = Face;
				c = G->Faces;
			}
			else
			{
				c->Next = Face;
				c = c->Next;
			}
			G->NumFaces++;
			
			ModelTriangle *Face2 = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face2->Next = NULL;
			
			Face2->v1 = (z+1)*int(size) + x;
			Face2->v2 = (z+1)*int(size) + (x+1);
			Face2->v3 = z*int(size) + (x+1);
			
			Face2->t1 = (z+1)*int(size) + x;
			Face2->t2 = (z+1)*int(size) + (x+1);
			Face2->t3 = z*int(size) + (x+1);
			
			Face2->n1 = (z+1)*int(size) + x;
			Face2->n2 = (z+1)*int(size) + (x+1);
			Face2->n3 = z*int(size) + (x+1);
			
			Face2->b1 = NULL;
			Face2->b2 = NULL;
			Face2->b3 = NULL;
			
			Face2->MatID = 0;
			if (G->NumFaces == 0)
			{
				G->Faces = Face2;
				c = G->Faces;
			}
			else
			{
				c->Next = Face2;
				c = c->Next;
			}
			G->NumFaces++;
		}
	}
	
	M->Geometry = G;
	LoadPhysics(M, string_to_char(physics));
	LoadMats(M, string_to_char(materials));
}

void LoadMats(GameModel *M, char *filename)
{
	FILE *file;
	char oneline[255];
	ObjectMaterials *Mat = (ObjectMaterials*)malloc(sizeof(ObjectMaterials));
	Mat->NumMats = 0;
	GLfloat v1, v2, v3;
	
	file = fopen(filename, "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename);
		printf("\n");
		return;
	}
	Material *Current;
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "newmtl", 6) == 0)
		{
			Mat->NumMats++;
			Material *M = (Material*)malloc(sizeof(Material));
			M->Next = NULL;
			sscanf(oneline, "newmtl %s", M->Name);
			M->hasTexts = false;
			M->hasDetail = false;
			M->hasFur = false;
			M->Opacity = 1.0;
			M->Reflect = 0.0;
			Current = M;
			if (Mat->NumMats-1 == 0)
				Mat->Mats = M;
			else
			{
				Material *C = Mat->Mats;
				while (C->Next != NULL)
					C = C->Next;
				C->Next = M;
			}
		}
		
		if (strncmp(oneline, "Am", 2) == 0)
		{
			sscanf(oneline, "Am %f %f %f", &v1, &v2, &v3);
			Current->Ambient.x = v1;
			Current->Ambient.y = v2;
			Current->Ambient.z = v3;
		}
		
		if (strncmp(oneline, "Di", 2) == 0)
		{
			sscanf(oneline, "Di %f %f %f", &v1, &v2, &v3);
			Current->Diffuse.x = v1;
			Current->Diffuse.y = v2;
			Current->Diffuse.z = v3;
		}
		
		if (strncmp(oneline, "Sp", 2) == 0)
		{
			sscanf(oneline, "Sp %f %f %f", &v1, &v2, &v3);
			Current->Specular.x = v1;
			Current->Specular.y = v2;
			Current->Specular.z = v3;
		}
		
		if (strncmp(oneline, "Em", 2) == 0)
		{
			sscanf(oneline, "Em %f %f %f", &v1, &v2, &v3);
			Current->Emission.x = v1;
			Current->Emission.y = v2;
			Current->Emission.z = v3;
		}
		
		if (strncmp(oneline, "Op", 2) == 0)
		{
			sscanf(oneline, "Op %f", &v1);
			Current->Opacity = v1;
		}
		
		if (strncmp(oneline, "Re", 2) == 0)
		{
			sscanf(oneline, "Re %f", &v1);
			Current->Reflect = v1;
		}
		
		if (strncmp(oneline, "TextMap2D", 9) == 0)
		{
			Current->TextDimension = Second;
			char LOD[5];
			sscanf(oneline, "TextMap2D %s %s", Current->TextName, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Current->TextLOD = true;
			else
				Current->TextLOD = false;
		}
		else if (strncmp(oneline, "TextMap3D", 9) == 0)
		{
			Current->TextDimension = Third;
			char LOD[5];
			sscanf(oneline, "TextMap3D %d %s %s %s", &Current->TextureDepth, Current->TextName, Current->TextFileType, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Current->TextLOD = true;
			else
				Current->TextLOD = false;
		}
			
		if (strncmp(oneline, "DetailMap2D", 11) == 0)
		{
			Current->DetailDimension = Second;
			char LOD[5];
			sscanf(oneline, "DetailMap2D %s %s", Current->DetailName, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Current->DetailLOD = true;
			else
				Current->DetailLOD = false;
		}
		
		else if (strncmp(oneline, "DetailMap3D", 11) == 0)
		{
			Current->DetailDimension = Third;
			char LOD[5];
			sscanf(oneline, "DetailMap3D %d %s %s %s", &Current->DetailDepth, Current->DetailName, Current->DetailFileType, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Current->DetailLOD = true;
			else
				Current->DetailLOD = false;
		}
		if (strncmp(oneline, "FurMap", 6) == 0)
		{
			int a, b, c;
			Vector v;
			sscanf(oneline, "FurMap %f %f %f %d %d %d", &v.x, &v.y, &v.z, &a, &b, &c);
			Current->GenerateFurTextures(a, b, c, v);
		}
	}
	fclose(file);
	
	M->Materials = Mat;
}

void LoadPhysics(GameModel *M, char *filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	
	M->Physics.Mass = 1.0;
	M->Physics.Elasticity = 1.0;
	
	file = fopen(filename, "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename);
		printf("\n");
		return;
	}
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "Ma", 2) == 0)
		{
			sscanf(oneline, "Ma %f", &v1);
			M->Physics.Mass = v1;
		}
		
		if (strncmp(oneline, "El", 2) == 0)
		{
			sscanf(oneline, "El %f", &v1);
			M->Physics.Elasticity = v1;
		}
		
		if (strncmp(oneline, "Po", 2) == 0)
		{
			sscanf(oneline, "Po %f %f %f", &v1, &v2, &v3);
			M->Physics.Position.x = v1;
			M->Physics.Position.y = v2;
			M->Physics.Position.z = v3;
		}
		
		if (strncmp(oneline, "Or", 2) == 0)
		{
			sscanf(oneline, "Or %f %f %f", &v1, &v2, &v3);
			M->Physics.Orientation.x = v1;
			M->Physics.Orientation.y = v2;
			M->Physics.Orientation.z = v3;
		}
	}
	
	CreateBox(M->Geometry->Max, M->Geometry->Min, &M->Physics.BoundingBox);
	
	M->Physics.Radius = M->Geometry->Radius;
	M->Physics.Volume = pi*sqr(M->Physics.Radius);
	M->Physics.Density = M->Physics.Mass/M->Physics.Volume;
	M->Physics.SurfaceArea = CalculateSurfaceArea(M->Geometry);
	M->Physics.RotationalInertia = RotationalInertiaEquation(M->Physics.Mass, M->Geometry->Verts, M->Geometry->NumVerts);
	
	fclose(file);
}

void LoadBones(GameModel *M, FILE *file)
{
	string input = "";
	string previnput;
	char oneline[255];
	
	char LastBone[11];
	LastBone[0] = '\0';
	Bone *LastBoneLink = NULL;
	char CurrentBone[11];
	CurrentBone[0] = '\0';
	EulerAngle v1;
	int AnimationSize = 0;
	
	Bone *NextInList = NULL;
	
	Bone *Skeleton = NULL;
	
	GLfloat q1, q2, q3, q4;
	
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, string_to_char(input + "{"), (input+"{").length()) == 0 || strncmp(oneline, string_to_char(input + "["), (input+"[").length()) == 0)
			input += " ";
		
		previnput = input;
		input = "";
		if (previnput.length() > 0)
		{
			for (unsigned int x = 0; x < previnput.length()-1; x++)
				input += previnput[x];
			
			if (strncmp(oneline, string_to_char(input+"}"), (input+"}").length()) == 0)
				previnput = input;
			
			else if (strncmp(oneline, string_to_char(input+"]"), (input+"]").length()) == 0)
			{
				previnput = input;
				strncpy(CurrentBone, LastBone, 11);
				
				Bone *C = Skeleton;
				while (C != NULL)
				{
					if (strncmp(CurrentBone, C->BoneID, 11) == 0)
					{
						strncpy(LastBone, C->ParentID, 11);
						LastBoneLink = C->Parent;
						C = NULL;
						break;
					}
					C = C->Next;
				}
			}
		}
		input = previnput;
		
		if (strncmp(oneline, string_to_char(input+"cycleInterval"), (input+"cycleInterval").length()) == 0)
			sscanf(oneline, string_to_char(input+"cycleInterval	%d"), &AnimationSize);
		
		if (strncmp(oneline, string_to_char(input + "DEF"), (input + "DEF").length()) == 0)
		{
			char test[50];
			char ID[50];
			sscanf(oneline, string_to_char(input + "DEF %s %s"), ID, test);
			if (strncmp(test, "Transform", 9) == 0)
			{
				Bone *NewBone = (Bone*)malloc(sizeof(Bone));
				NewBone->Rotation = NULL;
				NewBone->Translation = NULL;
				NewBone->Parent = NULL;
				NewBone->Next = NULL;
				NewBone->NumRotationFrames = 0;
				NewBone->NumTranslationFrames = 0;
				
				strncpy(NewBone->BoneID, ID, 11);
				strncpy(NewBone->ParentID, LastBone, 11);
				NewBone->Parent = LastBoneLink;
				strncpy(LastBone, NewBone->BoneID, 11);
				LastBoneLink = NewBone;
				NewBone->Next = NULL;
				if (Skeleton == NULL)
				{
					Skeleton = NewBone;
					NextInList = Skeleton;
				}
				else
				{
					NextInList->Next = NewBone;
					NextInList = NextInList->Next;
				}
			}
			
			if (strncmp(test, "OrientationInterpolator", 23) == 0)
			{
				Bone *C = Skeleton;
				while (C != NULL)
				{
					if (strncmp(ID, C->BoneID, strlen(C->BoneID)) == 0)
					{
						C->Rotation = new RotationFrame[AnimationSize];
						C->NumRotationFrames = AnimationSize;
						int frame = 0;
						while (strncmp(oneline, string_to_char(input+"}"), input.length()+1) != 0)
						{
							if (better_fgets(oneline, 255, file) == NULL)
								break;
							if (sscanf(oneline, string_to_char(input+"  %f %f %f %f"), &q1, &q2, &q3, &q4) == 4)
								C->Rotation[frame++].Orientation = Quaternion(q1, q2, q3, q4);
						}
						break;
					}
					C = C->Next;
				}
			}
			
			if (strncmp(test, "PositionInterpolator", 20) == 0)
			{
				Bone *C = Skeleton;
				while (C != NULL)
				{
					if (strncmp(ID, C->BoneID, strlen(C->BoneID)) == 0)
					{
						C->Translation = new TranslationFrame[AnimationSize];
						C->NumTranslationFrames = AnimationSize;
						int frame = 0;
						while (strncmp(oneline, string_to_char(input+"}"), input.length()+1) != 0)
						{
							if (better_fgets(oneline, 255, file) == NULL)
								break;
							if (sscanf(oneline, string_to_char(input+"  %f %f %f"), &q1, &q2, &q3) == 3)
								C->Translation[frame++].Position = Vector(q1, q2, q3);
						}
						break;
					}
					C = C->Next;
				}
			}
		}
		
		if (strncmp(oneline, string_to_char(input + "translation"), (input + "translation").length()) == 0)
			sscanf(oneline, string_to_char(input + "translation		%f %f %f"), &NextInList->Position.x, &NextInList->Position.y, &NextInList->Position.z);
		if (strncmp(oneline, string_to_char(input + "rotation"), (input + "rotation").length()) == 0)
		{
			sscanf(oneline, string_to_char(input + "rotation		%f %f %f %f"), &q1, &q2, &q3, &q4);
			NextInList->Orientation = Quaternion(q1, q2, q3, q4);
		}
	}
	
	GLfloat OldDistance = 0;
	GLfloat NewDistance = 0;

	ModelTriangle *Current = M->Geometry->Faces;
	while (Current != NULL)
	{
		OldDistance = 1000000000;
		NextInList = Skeleton;
		while (NextInList != NULL)
		{
			NewDistance = Distance(NextInList->Position, M->Geometry->Verts[Current->v1]);
			if (NewDistance < OldDistance)
			{
				OldDistance = NewDistance;
				Current->b1 = NextInList;
			}
			NextInList = NextInList->Next;
		}
		
		OldDistance = 1000000000;
		NextInList = Skeleton;
		while (NextInList != NULL)
		{
			NewDistance = Distance(NextInList->Position, M->Geometry->Verts[Current->v2]);
			if (NewDistance < OldDistance)
			{
				OldDistance = NewDistance;
				Current->b2 = NextInList;
			}
			NextInList = NextInList->Next;
		}
		
		OldDistance = 1000000000;
		NextInList = Skeleton;
		while (NextInList != NULL)
		{
			NewDistance = Distance(NextInList->Position, M->Geometry->Verts[Current->v3]);
			if (NewDistance < OldDistance)
			{
				OldDistance = NewDistance;
				Current->b3 = NextInList;
			}
			NextInList = NextInList->Next;
		}
		Current = Current->Next;
	}
	
	M->Animation->FrameIndices.begin = 0;
	M->Animation->FrameIndices.end = AnimationSize;
	M->Animation->FrameIndices.next = 1;
	M->Animation->FrameIndices.percent = 0.0;
	
	M->Animation->Skeleton = Skeleton;
}

void GenNormals(GameModel *M)
{
	Vector Normal;
	
	for (int x = 0; x < 75000; x++)
	{
		M->Geometry->Norms[x] = Vector();
	}
	ModelTriangle *Current = M->Geometry->Faces;
	while (Current != NULL)
	{
		Normal = CalculateNormal(M->Geometry->Verts[Current->v1], M->Geometry->Verts[Current->v2], M->Geometry->Verts[Current->v3]);
		M->Geometry->Norms[Current->v1] += Normal;
		M->Geometry->Norms[Current->v2] += Normal;
		M->Geometry->Norms[Current->v3] += Normal;
		Current->n1 = Current->v1;
		Current->n2 = Current->v2;
		Current->n3 = Current->v3;
		
		Current = Current->Next;
	}
	
	for (int x = 0; x < 75000; x++)
		M->Geometry->Norms[x] = Normalize(M->Geometry->Norms[x]);
}

void GenDisplayLists(GameModel *M)
{
	Material *CurrentMat = M->Materials->Mats;
	while (true)
	{
		if (CurrentMat->TextName[0] != '\0')
		{
			if (CurrentMat->TextDimension == Second)
			{
				string a = CurrentMat->TextName;
				CurrentMat->TextMap = Load2DTexture(a, CurrentMat->TextLOD);
				CurrentMat->hasTexts = true;
			}
			else if (CurrentMat->TextDimension == Third)
			{
				string a[CurrentMat->TextureDepth];
				for (int y = 0; y < CurrentMat->TextureDepth; y++)
					a[y] = string(CurrentMat->TextName) + "0" + string(int_to_char(y)) + string(CurrentMat->TextFileType);
				CurrentMat->TextMap = Load3DTexture(a, CurrentMat->TextureDepth, CurrentMat->TextLOD);
				CurrentMat->hasTexts = true;
			}
		}
		if (CurrentMat->DetailName[0] != '\0')
		{
			if (CurrentMat->DetailDimension == Second)
			{
				string a = CurrentMat->DetailName;
				CurrentMat->DetailMap = Load2DTexture(a, CurrentMat->DetailLOD);
				CurrentMat->hasDetail = true;
			}
			else if (CurrentMat->DetailDimension == Third)
			{
				string a[CurrentMat->DetailDepth];
				for (int y = 0; y < CurrentMat->DetailDepth; y++)
					a[y] = string(CurrentMat->DetailName) + "0" + string(int_to_char(y)) + string(CurrentMat->DetailFileType);
				CurrentMat->DetailMap = Load3DTexture(a, CurrentMat->DetailDepth, CurrentMat->DetailLOD);
				CurrentMat->hasDetail = true;
			}
		}
		
		CurrentMat->NumVertices = 0;
		
		if (CurrentMat->Next == NULL)
			break;
		else
			CurrentMat = CurrentMat->Next;
	}
		
	ModelTriangle *Current;
	if (M->Geometry->NumFaces > 0)
	{
		Current = M->Geometry->Faces;
		while (true)
		{
			CurrentMat = M->Materials->Mats;
			for (int x = 0; x < Current->MatID; x++)
				CurrentMat = CurrentMat->Next;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+0] = M->Geometry->Verts[Current->v1].x;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+1] = M->Geometry->Verts[Current->v1].y;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+2] = M->Geometry->Verts[Current->v1].z;
			
			CurrentMat->normals[(CurrentMat->NumVertices*3)+0] = M->Geometry->Norms[Current->n1].x;
			CurrentMat->normals[(CurrentMat->NumVertices*3)+1] = M->Geometry->Norms[Current->n1].y;
			CurrentMat->normals[(CurrentMat->NumVertices*3)+2] = M->Geometry->Norms[Current->n1].z;
			
			if (CurrentMat->hasTexts)
			{
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t1].x;
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t1].y;
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t1].z;
			}
			
			if (CurrentMat->hasDetail)
			{
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t1].x*4;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t1].y*4;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t1].z*4;
			}
			CurrentMat->NumVertices++;
			
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+0] = M->Geometry->Verts[Current->v2].x;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+1] = M->Geometry->Verts[Current->v2].y;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+2] = M->Geometry->Verts[Current->v2].z;
			
			CurrentMat->normals[(CurrentMat->NumVertices*3)+0] = M->Geometry->Norms[Current->n2].x;
			CurrentMat->normals[(CurrentMat->NumVertices*3)+1] = M->Geometry->Norms[Current->n2].y;
			CurrentMat->normals[(CurrentMat->NumVertices*3)+2] = M->Geometry->Norms[Current->n2].z;
			
			if (CurrentMat->hasTexts)
			{
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t2].x;
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t2].y;
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t2].z;
			}
			
			if (CurrentMat->hasDetail)
			{
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t2].x*4;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t2].y*4;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t2].z*4;
			}
			CurrentMat->NumVertices++;
			
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+0] = M->Geometry->Verts[Current->v3].x;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+1] = M->Geometry->Verts[Current->v3].y;
			CurrentMat->vertices[(CurrentMat->NumVertices*3)+2] = M->Geometry->Verts[Current->v3].z;
			
			CurrentMat->normals[(CurrentMat->NumVertices*3)+0] = M->Geometry->Norms[Current->n3].x;
			CurrentMat->normals[(CurrentMat->NumVertices*3)+1] = M->Geometry->Norms[Current->n3].y;
			CurrentMat->normals[(CurrentMat->NumVertices*3)+2] = M->Geometry->Norms[Current->n3].z;
			
			if (CurrentMat->hasTexts)
			{
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t3].x;
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t3].y;
				CurrentMat->texcoords0[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t3].z;
			}
			
			if (CurrentMat->hasDetail)
			{
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t3].x*4;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t3].y*4;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t3].z*4;
			}
			CurrentMat->NumVertices++;
			
			if (Current->Next == NULL)
				break;
			else
				Current = Current->Next;
		}
		
		CurrentMat = M->Materials->Mats;
		while (true)
		{
			CurrentMat->GenerateList();
			if (CurrentMat->Next == NULL)
				break;
			else
				CurrentMat = CurrentMat->Next;
		}
		
		M->DisplayList = glGenLists(1);
		glNewList(M->DisplayList, GL_COMPILE);
		
		CurrentMat = M->Materials->Mats;
		while (true)
		{
			CurrentMat->RenderMaterial();
			if (CurrentMat->hasFur)
			{
				cout << CurrentMat->FurDepth << endl;
				glEnable(GL_TEXTURE_2D);
				for (int y = 0; y < CurrentMat->FurDepth; y++)
				{
					glBindTexture(GL_TEXTURE_2D, CurrentMat->FurMap[y]);
					glTranslatef(0, 0.25, 0);
					glCallList(CurrentMat->List);
				}
				glDisable(GL_TEXTURE_2D);
			}
			if (CurrentMat->Next == NULL)
				break;
			else
				CurrentMat = CurrentMat->Next;
		}
		
		glEndList();
	}
}

void RenderModel(GameModel *M, bool list)
{
	glPushMatrix();
	glTranslatef(M->Physics.Position.x, M->Physics.Position.y, M->Physics.Position.z);
	glRotatef(M->Physics.Orientation.x, 1, 0, 0);
    glRotatef(M->Physics.Orientation.y, 0, 1, 0);
    glRotatef(M->Physics.Orientation.z, 0, 0, 1);
	
	if (list)
		glCallList(M->DisplayList);
	else
	{
		/*M->Animation->FrameIndices.percent += .1;
		if (M->Animation->FrameIndices.percent >= 1.0)
		{
			M->Animation->FrameIndices.percent -= 1.0;
			M->Animation->FrameIndices.next += 1;
			if (M->Animation->FrameIndices.next == M->Animation->FrameIndices.end)
				M->Animation->FrameIndices.next = 1;
		}
		
		ModelTriangle *Current = M->Geometry->Faces;
		int matid = Current->MatID;
		Material *CurrentMat = M->Materials->Mats;
		for (int x = 0; x < Current->MatID; x++)
			CurrentMat = CurrentMat->Next;
		glTranslatef(0.0, 0.0, 25.0);
		Vector v;
		CurrentMat->BindMaterial();
		glBegin(GL_TRIANGLES);
		while (Current != NULL)
		{
			if (Current->MatID != matid)
			{
				CurrentMat = M->Materials->Mats;
				for (int x = 0; x < Current->MatID; x++)
					CurrentMat = CurrentMat->Next;
				matid = Current->MatID;
				glEnd();
				CurrentMat->BindMaterial();
				glBegin(GL_TRIANGLES);
			}
			v = Transform(Current->b1, M->Geometry->Verts[Current->v1], M->Animation->FrameIndices.next, M->Animation->FrameIndices.percent);
			glMultiTexCoord3f(GL_TEXTURE0, M->Geometry->Texts[Current->t1].x, M->Geometry->Texts[Current->t1].y, M->Geometry->Texts[Current->t1].z);
			glMultiTexCoord3f(GL_TEXTURE1, M->Geometry->Texts[Current->t1].x, M->Geometry->Texts[Current->t1].y, M->Geometry->Texts[Current->t1].z);
			glNormal3f(M->Geometry->Norms[Current->n1].x, M->Geometry->Norms[Current->n1].y, M->Geometry->Norms[Current->n1].z);
			glVertex3f(v.x, v.y, v.z);
			
			v = Transform(Current->b1, M->Geometry->Verts[Current->v2], M->Animation->FrameIndices.next, M->Animation->FrameIndices.percent);
			glMultiTexCoord3f(GL_TEXTURE0, M->Geometry->Texts[Current->t2].x, M->Geometry->Texts[Current->t2].y, M->Geometry->Texts[Current->t2].z);
			glMultiTexCoord3f(GL_TEXTURE1, M->Geometry->Texts[Current->t2].x, M->Geometry->Texts[Current->t2].y, M->Geometry->Texts[Current->t2].z);
			glNormal3f(M->Geometry->Norms[Current->n2].x, M->Geometry->Norms[Current->n2].y, M->Geometry->Norms[Current->n2].z);
			glVertex3f(v.x, v.y, v.z);
			
			v = Transform(Current->b1, M->Geometry->Verts[Current->v3], M->Animation->FrameIndices.next, M->Animation->FrameIndices.percent);
			glMultiTexCoord3f(GL_TEXTURE0, M->Geometry->Texts[Current->t3].x, M->Geometry->Texts[Current->t3].y, M->Geometry->Texts[Current->t3].z);
			glMultiTexCoord3f(GL_TEXTURE1, M->Geometry->Texts[Current->t3].x, M->Geometry->Texts[Current->t3].y, M->Geometry->Texts[Current->t3].z);
			glNormal3f(M->Geometry->Norms[Current->n3].x, M->Geometry->Norms[Current->n3].y, M->Geometry->Norms[Current->n3].z);
			glVertex3f(v.x, v.y, v.z);
			
			Current = Current->Next;
		}
		glEnd();
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		
		Bone *Current = M->Animation->Skeleton;
		glPointSize(20);
		glTranslatef(0.0, 0.0, 100.0);
		
		Vertex FinalTranslation;
		Quaternion FinalRotation;
		Vertex v;
		
		glBegin(GL_POINTS);
		while (Current != NULL)
		{
			GLfloat g[16];
			
			v = Current->Position;
			
			Bone *c = Current;
			while (c != NULL)
			{
				QuaterniontoMatrix(c->Orientation, g);
				
				v = g*(v - c->Position) + c->Position;
				
				c = c->Parent;
			}
			
			glVertex3f(v.x, v.y, v.z);
			Current = Current->Next;
		}
		glEnd();*/
		
		Material *CurrentMat = M->Materials->Mats;
		while (true)
		{
			CurrentMat->RenderMaterial();
			if (CurrentMat->Next == NULL)
				break;
			else
				CurrentMat = CurrentMat->Next;
		}
	}
	glPopMatrix();
}

void ReleaseModel(GameModel *M)
{
	if (M->Animation != NULL)
	{
		Bone *CurrentBone = M->Animation->Skeleton, *PreviousBone;
		while (CurrentBone != NULL)
		{
			PreviousBone = CurrentBone;
			CurrentBone = CurrentBone->Next;
			if (PreviousBone->Rotation != NULL)
				delete [] PreviousBone->Rotation;
			PreviousBone->Rotation = NULL;
			if (PreviousBone->Translation != NULL)
				delete [] PreviousBone->Translation;
			PreviousBone->Translation = NULL;
			free(PreviousBone);
		}
		free(M->Animation);
	}
	
	ModelTriangle *CurrentTri = M->Geometry->Faces, *PreviousTri;
	while (CurrentTri != NULL)
	{
		PreviousTri = CurrentTri;
		CurrentTri = CurrentTri->Next;
		PreviousTri->b1 = NULL;
		PreviousTri->b2 = NULL;
		PreviousTri->b3 = NULL;
		free(PreviousTri);
	}
	
	Material *CurrentMat = M->Materials->Mats;
	if (CurrentMat != NULL)
	{
		while (true)
		{
			CurrentMat->Release();
			
			if (CurrentMat->Next != NULL)
				CurrentMat = CurrentMat->Next;
			else
				break;
		}
	}
	
	Material *Previous;
	CurrentMat = M->Materials->Mats;
	while (CurrentMat != NULL)
	{
		Previous = CurrentMat;
		CurrentMat = CurrentMat->Next;
		free(Previous);
	}
	
	free(M->Geometry);
	free(M->Materials);
	glDeleteLists(M->DisplayList, 1);
}

void SetCollisionGroundFunction(GameModel *M, void (*pFunc)(GameModel *M, Triangle t))
{
	M->CollisionFunctionGround = pFunc;
}

void AnimateObjectGroundCollision(GameModel *M, Triangle t)
{
	Vector n = CalculateNormal(t.v[0], t.v[1], t.v[2]);
	GLfloat D1 = Dot(M->Physics.Position + MidPoint(M->Geometry->Min, M->Geometry->Max), n) - Dot(n, t.v[0]), D2 = Dot(M->Physics.Position + MidPoint(M->Geometry->Min, M->Geometry->Max) + M->Physics.LinearVelocity, n) - Dot(n, t.v[0]);
	GLfloat D;
	if (D1 < 0)
		D = -D1;
	else
		D = D1;
		
	if (n.y <= 0)
	{
		Vector v = M->Physics.LinearVelocity;
		Vector a = EulertoVector(M->Geometry->Radius*M->Physics.AngularVelocity);
		
		if (Dot(v, n) < Dot(-1*v, n))
			M->Physics.LinearVelocity = (v+a - 2*Dot(v+a, n)*n)*M->Physics.Elasticity;
	}
	else
	{
		if (M->Physics.LinearVelocity.x != 0 || M->Physics.LinearVelocity.z != 0)
		{
			if (Dot(M->Physics.LinearVelocity, n) < Dot(-1*M->Physics.LinearVelocity, n))
				M->Physics.LinearVelocity += (D1-D2)*n;
		}
		else
			M->Physics.LinearVelocity.y = 9.8/60;
		
		if (D <= M->Geometry->Radius*0.8)
			M->Physics.Position += n*(M->Geometry->Radius*0.8 - D);
	}
}

void InAnimateObjectGroundCollision(GameModel *M, Triangle t)
{
	Vector n = CalculateNormal(t.v[0], t.v[1], t.v[2]);
	Vector v = M->Physics.LinearVelocity;
	
	if (Dot(v, n) < Dot(-1*v, n))
	{
		M->Physics.LinearVelocity = (v - 2*Dot(v, n)*n)*M->Physics.Elasticity;
		M->Physics.AngularVelocity = M->Physics.Elasticity*((v - 2*Dot(v, n)*n)/M->Geometry->Radius)/M->Physics.RotationalInertia;
	}
	M->Physics.LinearVelocity.y += 9.8/60;
}
