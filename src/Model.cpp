/*
	Model.cpp
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

#include "Model.h"

bool BoxtoTriangleCollision(Model *M, Triangle t)
{
	Matrix A1, A2;
	Vector v[16];
	EulertoMatrix(M->Physics->Orientation, A1);
	EulertoMatrix(M->Physics->Orientation + M->Physics->AngularVelocity, A2);
	for (int x = 0; x < 8; x++)
	{
		v[x] = A1*M->Geometry->BoundingBox[x] + M->Physics->Position;
		v[x+8] = A2*M->Geometry->BoundingBox[x] + M->Physics->Position + M->Physics->LinearVelocity;
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

bool BoxtoBoxCollision(Model *ModelOne, Model *ModelTwo)
{
	GLfloat A1[16], A2[16], A3[16], A4[16];
	Vector v1[16], v2[16];
	EulertoMatrix(ModelOne->Physics->Orientation, A1);
	EulertoMatrix(ModelOne->Physics->Orientation + ModelOne->Physics->AngularVelocity, A2);
	EulertoMatrix(ModelTwo->Physics->Orientation, A3);
	EulertoMatrix(ModelTwo->Physics->Orientation + ModelTwo->Physics->AngularVelocity, A4);
	for (int x = 0; x < 8; x++)
	{
		v1[x] = A1*ModelOne->Geometry->BoundingBox[x] + ModelOne->Physics->Position;
		v1[x+8] = A2*ModelOne->Geometry->BoundingBox[x] + ModelOne->Physics->Position + ModelOne->Physics->LinearVelocity;
		v2[x] = A3*ModelTwo->Geometry->BoundingBox[x] + ModelTwo->Physics->Position;
		v2[x+8] = A4*ModelTwo->Geometry->BoundingBox[x] + ModelTwo->Physics->Position + ModelTwo->Physics->LinearVelocity;
	}
	
	bool coll = false;
	if (isectboxbox(v1, v2))
		coll = true;
	
	if (coll)
	{
		GLfloat AverageElasticity = (ModelOne->Physics->Elasticity+ModelTwo->Physics->Elasticity)/2;
		
		Vector Velocity = ModelOne->Physics->AngularVelocity - ModelTwo->Physics->AngularVelocity;
		Vector Numerator = -1*Vector(1, 1, 1)*(AverageElasticity+1);
		
		Vector UnitNormal = Normalize(ModelOne->Physics->Position-ModelTwo->Physics->Position);
		Vector ForceLocation2 = UnitNormal*ModelTwo->Geometry->Radius;
		Vector TempVector = Cross(ForceLocation2, UnitNormal);
		
		TempVector /= ModelTwo->Physics->RotationalInertia;
		
		TempVector = Cross(TempVector, ForceLocation2);
		
		GLfloat part1 = Dot(UnitNormal, TempVector);
		
		UnitNormal *= -1;
		Vector ForceLocation1 = UnitNormal*ModelOne->Geometry->Radius;
		TempVector = Cross(ForceLocation1, UnitNormal);
		
		TempVector /= ModelOne->Physics->RotationalInertia;
		
		TempVector = Cross(TempVector, ForceLocation1);
		
		GLfloat part2 = Dot(UnitNormal, TempVector);
		
		GLfloat Denominator = 1/ModelOne->Physics->Mass + 1/ModelTwo->Physics->Mass + part2 + part1;
		ModelOne->Physics->LinearVelocity += (Numerator/Denominator)/ModelOne->Physics->Mass;
		ModelTwo->Physics->LinearVelocity -= (Numerator/Denominator)/ModelTwo->Physics->Mass;
	}
	
	return coll;
}

void LoadObj(Model *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexScale)
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
	
	Mesh *G = (Mesh*)malloc(sizeof(Mesh));
	
	file = fopen(filename.c_str(), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename.c_str());
		return;
	}
	
	G->NumVerts = 0;
	G->NumNorms = 0;
	G->NumTexts = 0;
	G->NumFaces = 0;
	
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
			
			if (X*sizeX > G->BoundingBox[6].x)
				G->BoundingBox[6].x = X*sizeX;
			if (Y*sizeY > G->BoundingBox[6].y)
				G->BoundingBox[6].y = Y*sizeY;
			if (Z*sizeZ > G->BoundingBox[6].z)
				G->BoundingBox[6].z = Z*sizeZ;
			if (X*sizeX < G->BoundingBox[0].x)
				G->BoundingBox[0].x = X*sizeX;
			if (Y*sizeY < G->BoundingBox[0].y)
				G->BoundingBox[0].y = Y*sizeY;
			if (Z*sizeZ < G->BoundingBox[0].z)
				G->BoundingBox[0].z = Z*sizeZ;
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
			
			Face->v1 = (v1 < 0) ? (G->NumVerts + v1) :v1 - 1;
			Face->v2 = (v2 < 0) ? (G->NumVerts + v2) :v2 - 1;
			Face->v3 = (v3 < 0) ? (G->NumVerts + v3) :v3 - 1;
			
			Face->t1 = (t1 < 0) ? (G->NumTexts + t1) : t1 - 1;
			Face->t2 = (t2 < 0) ? (G->NumTexts + t2) : t2 - 1;
			Face->t3 = (t3 < 0) ? (G->NumTexts + t3) : t3 - 1;
			
			Face->n1 = -1;
			Face->n2 = -1;
			Face->n3 = -1;
			
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
	
	Vector m = MidPoint(G->BoundingBox[6], G->BoundingBox[0]);
	
	if (fabs(G->BoundingBox[6].x-m.x) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].x-m.x);
	if (fabs(G->BoundingBox[6].y-m.y) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].y-m.y);
	if (fabs(G->BoundingBox[6].z-m.z) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].z-m.z);
	if (fabs(G->BoundingBox[0].x+m.x) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].x+m.x);
	if (fabs(G->BoundingBox[0].y+m.y) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].y+m.y);
	if (fabs(G->BoundingBox[0].z+m.z) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].z+m.z);
	
	M->Geometry = G;
	if (phsname[0] != '\0')
		LoadPhysics(M, phsname);
	cout << "done loading" << endl;
}

void LoadWRL(Model *M, string filename, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ, GLfloat TexSize)
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
	
	Mesh *G = (Mesh*)malloc(sizeof(Mesh));
	
	file = fopen(filename.c_str(), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename.c_str());
		return;
	}
	
	G->NumVerts = 0;
	G->NumNorms = 0;
	G->NumTexts = 0;
	G->NumFaces = 0;
		
	string input = "";
	string previnput;
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 225, file) == NULL)
			break;
		
		if (strncmp(oneline, (input + "{").c_str(), (input+"{").length()) == 0 || strncmp(oneline, (input + "[").c_str(), (input+"[").length()) == 0)
			input += " ";
		
		previnput = input;
		input = "";
		if (previnput.length() > 0)
		{
			for (unsigned int x = 0; x < previnput.length()-1; x++)
				input += previnput[x];
			if (strncmp(oneline, (input+"}").c_str(), (input+"}").length()) == 0 || strncmp(oneline, (input+"]").c_str(), (input+"]").length()) == 0)
				previnput = input;
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, (input + "vertex [").c_str(), (input + "vertex [").length()) == 0)
		{
			while (strncmp(oneline, (previnput + "]").c_str(), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, (input + " %f %f %f,").c_str(), &X, &Y, &Z) == 3)
				{
					G->Verts[G->NumVerts] = Vector(X*sizeX, Y*sizeY, Z*sizeZ);
					G->NumVerts++;
					G->NumNorms++;
					
					if (X*sizeX > G->BoundingBox[6].x)
						G->BoundingBox[6].x = X*sizeX;
					if (Y*sizeY > G->BoundingBox[6].y)
						G->BoundingBox[6].y = Y*sizeY;
					if (Z*sizeZ > G->BoundingBox[6].z)
						G->BoundingBox[6].z = Z*sizeZ;
					if (X*sizeX < G->BoundingBox[0].x)
						G->BoundingBox[0].x = X*sizeX;
					if (Y*sizeY < G->BoundingBox[0].y)
						G->BoundingBox[0].y = Y*sizeY;
					if (Z*sizeZ < G->BoundingBox[0].z)
						G->BoundingBox[0].z = Z*sizeZ;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, (input + "coordIndex [").c_str(), (input + "coordIndex [").length()) == 0)
		{	
			input = previnput;
			input += " %d %d %d -1,";
			while (strncmp(oneline, (previnput + "]").c_str(), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, input.c_str(), &v1, &v2, &v3) == 3)
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
		if (strncmp(oneline, (input + "normal [").c_str(), (input + "normal [").length()) == 0)
		{	
			while (strncmp(oneline, (previnput + "]").c_str(), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, (input + " %f %f %f,").c_str(), &X, &Y, &Z) == 3)
				{
					G->Norms[G->NumNorms] = Vector(X, Y, Z);
					G->NumNorms++;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, (input + "normalIndex [").c_str(), (input + "normalIndex [").length()) == 0)
		{
			ModelTriangle *Current = G->Faces;
			while (Current != NULL)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, (input + " %d %d %d -1,").c_str(), &n1, &n2, &n3) == 3)
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
		if (strncmp(oneline, (input + "texCoord [").c_str(), (input + "texCoord [").length()) == 0)
		{
			while (strncmp(oneline, (previnput + "]").c_str(), (previnput + "]").length()) != 0)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, (input + " %f %f,").c_str(), &X, &Y) == 2)
				{
					G->Texts[G->NumTexts] = Vector(X, Y, 0);
					G->NumTexts++;
				}
			}
		}
		input = previnput;
		
		previnput = input;
		if (strncmp(oneline, (input + "texCoordIndex [").c_str(), (input + "texCoordIndex [").length()) == 0)
		{
			ModelTriangle *Current = G->Faces;
			while (Current != NULL)
			{
				if (better_fgets(oneline, 225, file) == NULL)
					break;
				
				if (sscanf(oneline, (input + " %d %d %d -1,").c_str(), &t1, &t2, &t3) == 3)
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
		}
	}
	fclose(file);
	
	Vector m = MidPoint(G->BoundingBox[6], G->BoundingBox[0]);
	
	if (fabs(G->BoundingBox[6].x-m.x) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].x-m.x);
	if (fabs(G->BoundingBox[6].y-m.y) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].y-m.y);
	if (fabs(G->BoundingBox[6].z-m.z) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].z-m.z);
	if (fabs(G->BoundingBox[0].x+m.x) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].x+m.x);
	if (fabs(G->BoundingBox[0].y+m.y) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].y+m.y);
	if (fabs(G->BoundingBox[0].z+m.z) > G->Radius)
		G->Radius = fabs(G->BoundingBox[6].z+m.z);
	
	if (phsname[0] != '\0')
		LoadPhysics(M, phsname);
	cout << "done loading" << endl;
}

void LoadHeightMap(Model *M, string heightmap, string materials, string physics, int size, int x_offset, int z_offset, int loadsize, GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, GLfloat tex_scale)
{
	unsigned char data[size*size];
	
	FILE *file;
	ModelTriangle *c;
	
	
	file = fopen(heightmap.c_str(), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(heightmap.c_str());
		return;
	}
	
	fread(data, 1, size*size, file);
	
	fclose(file);
	
	Mesh *G = (Mesh*)malloc(sizeof(Mesh));
	G->NumVerts = 0;
	G->NumTexts = 0;
	G->NumFaces = 0;
	G->NumNorms = 0;
	
	for (int z = z_offset; z < loadsize+z_offset; z++)
	{
		for (int x = x_offset; x < loadsize+x_offset; x++)
		{
			G->Verts[(z-z_offset)*loadsize + (x-x_offset)] = Vector((x-x_offset)*x_scale, GLfloat(data[(z%size)*size + (x%size)])*y_scale, (z-z_offset)*z_scale);
			if ((x-x_offset)*x_scale > G->BoundingBox[6].x)
				G->BoundingBox[6].x = (x-x_offset)*x_scale;
			if (-GLfloat(data[(z%size)*size + (x%size)])*y_scale > G->BoundingBox[6].y)
				G->BoundingBox[6].y = -GLfloat(data[(z%size)*size + (x%size)])*y_scale;
			if ((z-z_offset)*z_scale > G->BoundingBox[6].z)
				G->BoundingBox[6].z = (z-z_offset)*z_scale;
			if ((x-x_offset)*x_scale < G->BoundingBox[0].x)
				G->BoundingBox[0].x = (x-x_offset)*x_scale;
			if (-GLfloat(data[(z%size)*size + (x%size)])*y_scale < G->BoundingBox[0].y)
				G->BoundingBox[0].y = -GLfloat(data[(z%size)*size + (x%size)])*y_scale;
			if ((z-z_offset)*z_scale < G->BoundingBox[0].z)
				G->BoundingBox[0].z = (z-z_offset)*z_scale;
			
			G->NumVerts++;
			G->Texts[(z-z_offset)*loadsize + (x-x_offset)] = Vector(x*tex_scale, z*tex_scale, 0.0);
			G->NumTexts++;
			G->NumNorms++;
		}
	}
	G->BoundingBox[6].y = 255*y_scale;
	G->BoundingBox[0].y = 0;
	Vector Mid = MidPoint(G->BoundingBox[6], G->BoundingBox[0]);
	Mid.y = 0;
	G->BoundingBox[0] -= Mid;
	G->BoundingBox[6] -= Mid;
	
	for (int z = z_offset; z < loadsize+z_offset; z++)
	{
		for (int x = x_offset; x < loadsize+x_offset; x++)
		{
			G->Verts[(z-z_offset)*loadsize + (x-x_offset)] -= Mid;
			G->Texts[(z-z_offset)*loadsize + (x-x_offset)].z = (G->BoundingBox[6].y - G->BoundingBox[0].y)/(G->BoundingBox[6].y - GLfloat(data[(z%size)*size + (x%size)])*y_scale);
		}
	}
	
	for (int z = 0; z < loadsize-1; z++)
	{
		for (int x = 0; x < loadsize-1; x++)
		{
			ModelTriangle *Face = (ModelTriangle*)malloc(sizeof(ModelTriangle));
			Face->Next = NULL;
			
			Face->v1 = z*loadsize + x;
			Face->v2 = (z+1)*loadsize + x;
			Face->v3 = z*loadsize + (x+1);
			
			Face->t1 = z*loadsize + x;
			Face->t2 = (z+1)*loadsize + x;
			Face->t3 = z*loadsize + (x+1);
			
			Face->n1 = z*loadsize + x;
			Face->n2 = (z+1)*loadsize + x;
			Face->n3 = z*loadsize + (x+1);
			
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
			
			Face2->v1 = (z+1)*loadsize + x;
			Face2->v2 = (z+1)*loadsize + (x+1);
			Face2->v3 = z*loadsize + (x+1);
			
			Face2->t1 = (z+1)*loadsize + x;
			Face2->t2 = (z+1)*loadsize + (x+1);
			Face2->t3 = z*loadsize + (x+1);
			
			Face2->n1 = (z+1)*loadsize + x;
			Face2->n2 = (z+1)*loadsize + (x+1);
			Face2->n3 = z*loadsize + (x+1);
			
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
	LoadPhysics(M, physics.c_str());
	LoadMats(M, materials.c_str());
}

void LoadMats(Model *M, const char *filename)
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
	}
	fclose(file);
	
	M->Materials = Mat;
}

void LoadPhysics(Model *M, const char *filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	
	M->Physics = (PhysicsController*)malloc(sizeof(PhysicsController));
	
	M->Physics->Mass = 1.0;
	M->Physics->Elasticity = 1.0;
	
	file = fopen(filename, "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename);
		return;
	}
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "Ma", 2) == 0)
		{
			sscanf(oneline, "Ma %f", &v1);
			M->Physics->Mass = v1;
		}
		
		if (strncmp(oneline, "El", 2) == 0)
		{
			sscanf(oneline, "El %f", &v1);
			M->Physics->Elasticity = v1;
		}
		
		if (strncmp(oneline, "Po", 2) == 0)
		{
			sscanf(oneline, "Po %f %f %f", &v1, &v2, &v3);
			M->Physics->Position.x = v1;
			M->Physics->Position.y = v2;
			M->Physics->Position.z = v3;
		}
		
		if (strncmp(oneline, "Or", 2) == 0)
		{
			sscanf(oneline, "Or %f %f %f", &v1, &v2, &v3);
			M->Physics->Orientation.x = v1;
			M->Physics->Orientation.y = v2;
			M->Physics->Orientation.z = v3;
		}
	}
	
	MakeBox(M->Geometry->BoundingBox[6], M->Geometry->BoundingBox[0], M->Geometry->BoundingBox);
	
	M->Physics->Volume = pi*Square(M->Geometry->Radius);
	M->Physics->RotationalInertia = CalculateRotationalInertia(M->Physics->Mass, M->Geometry->Verts, M->Geometry->NumVerts);
	
	fclose(file);
}

void GenNormals(Model *M)
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

void GenDisplayLists(Model *M)
{
	Material *CurrentMat;
	CurrentMat = M->Materials->Mats;
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
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t1].x;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t1].y;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t1].z;
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
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t2].x;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t2].y;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t2].z;
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
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+0] = M->Geometry->Texts[Current->t3].x;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+1] = M->Geometry->Texts[Current->t3].y;
				CurrentMat->texcoords1[(CurrentMat->NumVertices*3)+2] = M->Geometry->Texts[Current->t3].z;
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
		while (CurrentMat != NULL)
		{
			CurrentMat->RenderMaterial();
			CurrentMat = CurrentMat->Next;
		}
		
		glEndList();
	}
}

void RenderModel(Model *M)
{
	glPushMatrix();
	M->Physics->Apply();
	glCallList(M->DisplayList);
	glPopMatrix();
}

void ReleaseModel(Model *M)
{	
	ModelTriangle *CurrentTri = M->Geometry->Faces, *PreviousTri;
	while (CurrentTri != NULL)
	{
		PreviousTri = CurrentTri;
		CurrentTri = CurrentTri->Next;
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
	free(M->Physics);
	M->CollisionFunctionGround = NULL;
	glDeleteLists(M->DisplayList, 1);
}

void SetCollisionGroundFunction(Model *M, void (*pFunc)(Model *M, Triangle t))
{
	M->CollisionFunctionGround = pFunc;
}

void AnimateObjectGroundCollision(Model *M, Triangle t)
{
	//M->Physics->LinearVelocity.x += 0.001;
	/*Vector n = CalculateNormal(t.v[0], t.v[1], t.v[2]);
	GLfloat D1 = Dot(M->Physics->Position + MidPoint(M->Geometry->BoundingBox[0], M->Geometry->BoundingBox[6]), n) - Dot(n, t.v[0]), D2 = Dot(M->Physics->Position + MidPoint(M->Geometry->BoundingBox[0], M->Geometry->BoundingBox[6]) + M->Physics->LinearVelocity, n) - Dot(n, t.v[0]);
	GLfloat D;
	if (D1 < 0)
		D = -D1;
	else
		D = D1;*/
		
	/*if (n.y <= 0)
	{
		Vector v = M->Physics->LinearVelocity;
		Vector a = M->Geometry->Radius*M->Physics->AngularVelocity;
		
		if (Dot(v, n) < Dot(-1*v, n))
			M->Physics->LinearVelocity = (v+a - 2*Dot(v+a, n)*n)*M->Physics->Elasticity;
	}
	else
	{*/
		//if (M->Physics->LinearVelocity.x != 0 || M->Physics->LinearVelocity.z != 0)
		//{
		//	if (Dot(M->Physics->LinearVelocity, n) < Dot(-1*M->Physics->LinearVelocity, n))
		//		M->Physics->LinearVelocity += (D1-D2)*n;
		//}
		//else
		//	M->Physics->LinearVelocity.y = 9.8/60;
		
		//if (D <= M->Geometry->Radius*0.8)
		//			M->Physics->Position += n*(M->Geometry->Radius*0.8 - D);
	//}
}

void InAnimateObjectGroundCollision(Model *M, Triangle t)
{
	Vector n = CalculateNormal(t.v[0], t.v[1], t.v[2]);
	Vector v = M->Physics->LinearVelocity;
	
	if (Dot(v, n) < Dot(-1*v, n))
	{
		M->Physics->LinearVelocity = (v - 2*Dot(v, n)*n)*M->Physics->Elasticity;
		M->Physics->AngularVelocity = M->Physics->Elasticity*((v - 2*Dot(v, n)*n)/M->Geometry->Radius)/M->Physics->RotationalInertia;
	}
	//M->Physics->LinearVelocity.y += 9.8/60;
}
