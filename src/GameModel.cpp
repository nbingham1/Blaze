/*
	GameModel.cpp
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
#include "GameModel.h"
#include "GameApp.h"

extern GameApp App;

CTriangle &CTriangle::operator =(CTriangle t)
{
	this->Vertices[0] = t.Vertices[0];
	this->Vertices[1] = t.Vertices[1];
	this->Vertices[2] = t.Vertices[2];
	this->Normal = t.Normal;
	this->Distance = t.Distance;
	
	return (*this);
}

void AddCTriangle(CTriangle *List, CTriangle *Next)
{
	CTriangle *Current = List;
	while (Current->Next != NULL)
		Current = Current->Next;
	
	Current->Next = Next;
}

void DeleteCTriangle(CTriangle *List)
{
	CTriangle *Current = List, *Previous;
	while (Current != NULL)
	{
		Previous = Current;
		Current = Current->Next;
		free(Previous);
	}
}

int intersect_RayPlane(Vector R1, Vector R2, CTriangle T, Vector *I)
{
	Vector    u, v, n;             // triangle vectors
    Vector    dir, w0, w;          // ray vectors
    float     r, a, b;             // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = T.Vertices[1] - T.Vertices[0];
    v = T.Vertices[2] - T.Vertices[0];
    n = u.Cross(v);             // cross product

    if (n == Vector(0, 0, 0))            // triangle is degenerate
        return -1;                 // do not deal with this case

    dir = R2 - R1;             // ray direction vector
    w0 = R1 - T.Vertices[0];
    a = -n.Dot(w0);
    b = n.Dot(dir);
    if (fabs(b) < 0.00000001) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
	if (r > 1.0)
		return 0;
    // for a segment, also test if (r > 1.0) => no intersect

    *I = R1 + dir * r;           // intersect point of ray and plane
	return 1;
}

void MeshData::InsertVert(Vector Vert)
{
	Verts[NumVerts] = Vert;
	NumVerts++;
	
	if (Vert.x > Max.x)
		Max.x = Vert.x;
	if (Vert.y > Max.y)
		Max.y = Vert.y;
	if (Vert.z > Max.z)
		Max.z = Vert.z;
	if (Vert.x < Min.x)
		Min.x = Vert.x;
	if (Vert.y < Min.y)
		Min.y = Vert.y;
	if (Vert.z < Min.z)
		Min.z = Vert.z;
}

void MeshData::InsertNorm(Vector Norm)
{
	Norms[NumNorms] = Norm;
	NumNorms++;
}

void MeshData::InsertText(Vector Text)
{
	Texts[NumTexts] = Text;
	NumTexts++;
}

void MeshData::InsertFace(Triangle *Face, int GroupNum)
{
	Triangle *Current = Group[GroupNum].Faces;
	while (Current->Next != NULL)
		Current = Current->Next;
	
	Current->Next = Face;
	NumFaces++;
}

void MeshData::DeleteFace(int GroupNum)
{
	Triangle *Current = Group[GroupNum].Faces, *Previous;
	while (Current != NULL)
	{
		Previous = Current;
		Current = Current->Next;
		free(Previous);
	}
}

void MeshData::Load(string filename, GLfloat scale)
{
	LoadMesh(filename, scale);
	GenNorms();
	GenList();
	LoadCFaces();
}

void MeshData::LoadMesh(string filename, GLfloat scale)
{
	FILE *file;
	char oneline[255];
	char mtlname[255];
	int CurrentGroup;
	GLfloat x, y, z;
	int v1, t1, n1, v2, t2, n2, v3, t3, n3;
	
	file = fopen(string_to_char(filename), "rt");
	if (file == NULL)
	{
		printf("error, file not found: %s\n", filename.c_str());
		exit(0);
	}
	NumVerts = 0;
	NumTexts = 0;
	NumNorms = 0;
	NumFaces = 0;
	NumGroup = 0;
	Scale = scale;
	
	for (int m = 0; m < 64; m++)
	{
		Group[m].Faces = NULL;
		Group[m].Material = -1;
		
		Mtrls[m].Name[0] = '\0';
		Mtrls[m].Emission.x = 0.0;
		Mtrls[m].Emission.y = 0.0;
		Mtrls[m].Emission.z = 0.0;
		Mtrls[m].Reflect = 1.0;
		Mtrls[m].Specular = 1.0;
		Mtrls[m].Opacity = 1.0;
		Mtrls[m].TextName[0] = '\0';
		Mtrls[m].BumpName[0] = '\0';
		Mtrls[m].hasTexts = false;
		Mtrls[m].hasBumps = false;
	}
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "mtllib", 6) == 0)
		{
			if (sscanf(oneline, "mtllib %s", mtlname) == 1)
			{
				for (int x = 0; x < int(strlen(mtlname)); x++)
					if (mtlname[x] == '_')
						mtlname[x] = ' ';
				LoadMats(mtlname);
			}
		}
		
		if (strncmp(oneline, "usemtl", 6) == 0)
		{
			int matID = 0;

			if ((sscanf(oneline, "usemtl %s", mtlname) == 1) && (NumGroup != 0))
			{
				while (strncmp(mtlname, Mtrls[matID].Name, 32) != 0)
				{
					matID++;
					if (matID == NumGroup)
						break;
				}
			}
			CurrentGroup = matID;
			Group[CurrentGroup].Material = matID;
		}
		
		if (sscanf(oneline, "v %f %f %f", &x, &y, &z) == 3)
			InsertVert(Vector(x*Scale, y*Scale, z*Scale));
		
		else if (sscanf(oneline, "vt %f %f %f", &x, &y, &z) == 3)
			InsertText(Vector(x, y, z));
		
		else if (sscanf(oneline, "vt %f %f", &x, &y) == 2)
			InsertText(Vector(x, y, 0));
		
		else if (sscanf(oneline, "vn %f %f %f", &x, &y, &z) == 3)
			InsertNorm(Vector(x, y, z));
		
		else if (sscanf(oneline, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3) == 9)
		{
			Triangle *Face = (Triangle*)malloc(sizeof(Triangle));
			Face->Next = NULL;
			
			Face->v[0] = (v1 < 0) ? (NumVerts + v1) : --v1;
			Face->v[1] = (v2 < 0) ? (NumVerts + v2) : --v2;
			Face->v[2] = (v3 < 0) ? (NumVerts + v3) : --v3;
			
			Face->t[0] = (t1 < 0) ? (NumTexts + t1) : --t1;
			Face->t[1] = (t2 < 0) ? (NumTexts + t2) : --t2;
			Face->t[2] = (t3 < 0) ? (NumTexts + t3) : --t3;
			
			Face->n[0] = (n1 < 0) ? (NumNorms + n1) : --n1;
			Face->n[1] = (n2 < 0) ? (NumNorms + n2) : --n2;
			Face->n[2] = (n3 < 0) ? (NumNorms + n3) : --n3;
			
			if (CurrentGroup == -1)
				CurrentGroup = NumGroup;
			if (Group[CurrentGroup].Faces == NULL)
				Group[CurrentGroup].Faces = Face;
			else
				InsertFace(Face, CurrentGroup);
		}
		
		else if (sscanf(oneline, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3) == 6)
		{
			Triangle *Face = (Triangle*)malloc(sizeof(Triangle));
			Face->Next = NULL;
			
			Face->v[0] = (v1 < 0) ? (NumVerts + v1) : --v1;
			Face->v[1] = (v2 < 0) ? (NumVerts + v2) : --v2;
			Face->v[2] = (v3 < 0) ? (NumVerts + v3) : --v3;
			
			Face->t[0] = -1;
			Face->t[1] = -1;
			Face->t[2] = -1;
			
			Face->n[0] = (n1 < 0) ? (NumNorms + n1) : --n1;
			Face->n[1] = (n2 < 0) ? (NumNorms + n2) : --n2;
			Face->n[2] = (n3 < 0) ? (NumNorms + n3) : --n3;
			
			if (CurrentGroup == -1)
				CurrentGroup = NumGroup;
			if (Group[CurrentGroup].Faces == NULL)
				Group[CurrentGroup].Faces = Face;
			else
				InsertFace(Face, CurrentGroup);
		}
		
		else if (sscanf(oneline, "f %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3) == 6)
		{
			Triangle *Face = (Triangle*)malloc(sizeof(Triangle));
			Face->Next = NULL;
			
			Face->v[0] = (v1 < 0) ? (NumVerts + v1) : --v1;
			Face->v[1] = (v2 < 0) ? (NumVerts + v2) : --v2;
			Face->v[2] = (v3 < 0) ? (NumVerts + v3) : --v3;
			
			Face->t[0] = (t1 < 0) ? (NumTexts + t1) : --t1;
			Face->t[1] = (t2 < 0) ? (NumTexts + t2) : --t2;
			Face->t[2] = (t3 < 0) ? (NumTexts + t3) : --t3;
			
			Face->n[0] = -1;
			Face->n[1] = -1;
			Face->n[2] = -1;
			
			if (CurrentGroup == -1)
				CurrentGroup = NumGroup;
			if (Group[CurrentGroup].Faces == NULL)
				Group[CurrentGroup].Faces = Face;
			else
				InsertFace(Face, CurrentGroup);
		}
		
		else if (sscanf(oneline, "f %d %d %d", &v1, &v2, &v3) == 3)
		{
			Triangle *Face = (Triangle*)malloc(sizeof(Triangle));
			Face->Next = NULL;
			
			Face->v[0] = (v1 < 0) ? (NumVerts + v1) : --v1;
			Face->v[1] = (v2 < 0) ? (NumVerts + v2) : --v2;
			Face->v[2] = (v3 < 0) ? (NumVerts + v3) : --v3;
			
			Face->t[0] = -1;
			Face->t[1] = -1;
			Face->t[2] = -1;
			
			Face->n[0] = -1;
			Face->n[1] = -1;
			Face->n[2] = -1;
			
			if (CurrentGroup == -1)
				CurrentGroup = NumGroup;
			if (Group[CurrentGroup].Faces == NULL)
				Group[CurrentGroup].Faces = Face;
			else
				InsertFace(Face, CurrentGroup);
		}
	}
	fclose(file);
	
	if (NumGroup == 0)
		NumGroup++;
	
	if (fabs(Max.x) > Radius)
		Radius = fabs(Max.x);
	if (fabs(Max.y) > Radius)
		Radius = fabs(Max.y);
	if (fabs(Max.z) > Radius)
		Radius = fabs(Max.z);
	if (fabs(Min.x) > Radius)
		Radius = fabs(Max.x);
	if (fabs(Min.y) > Radius)
		Radius = fabs(Max.y);
	if (fabs(Min.z) > Radius)
		Radius = fabs(Max.z);
	
	cout << NumFaces <<endl;
}

void MeshData::LoadMats(char *filename)
{
	FILE	*file;
	char	 oneline[255];
	int    CurrentMat = -1;
	GLfloat  v1, v2, v3;
	
	file = fopen(filename, "rt");
	if (file == NULL)
	{
		printf("error, file not found: %s\n", filename);
		exit(1);
	}
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "newmtl", 6) == 0)
		{
			CurrentMat++;
			sscanf(oneline, "newmtl %s", Mtrls[CurrentMat].Name);
		}
		
		if (strncmp(oneline, "Em", 2) == 0)
		{
			sscanf(oneline, "Em %f %f %f", &v1, &v2, &v3);
			Mtrls[CurrentMat].Emission.x = v1;
			Mtrls[CurrentMat].Emission.y = v2;
			Mtrls[CurrentMat].Emission.z = v3;
		}
		
		if (strncmp(oneline, "Op", 2) == 0)
		{
			sscanf(oneline, "Op %f", &v1);
			Mtrls[CurrentMat].Opacity = v1;
		}
		
		if (strncmp(oneline, "Re", 2) == 0)
		{
			sscanf(oneline, "Re %f", &v1);
			Mtrls[CurrentMat].Reflect = v1;
		}
		
		if (strncmp(oneline, "Sr", 2) == 0)
		{
			sscanf(oneline, "Sr %f", &v1);
			Mtrls[CurrentMat].Specular = v1;
		}
		
		if (strncmp(oneline, "TextMap", 7) == 0)
			sscanf(oneline, "TextMap %s", Mtrls[CurrentMat].TextName);
		
		if (strncmp(oneline, "BumpMap", 7) == 0)
			sscanf(oneline, "BumpMap %s", Mtrls[CurrentMat].BumpName);
	}
	fclose(file);
	
	NumGroup = CurrentMat;
}

void MeshData::LoadCFaces()
{
	int x = 0;
	for (int i = 0; i < NumGroup; i++)
	{
		Triangle *Current;
		
		Current = Group[i].Faces;
		while (Current != NULL)
		{
			CData[x].Vertices[0] = Verts[Current->v[0]];
			CData[x].Vertices[1] = Verts[Current->v[1]];
			CData[x].Vertices[2] = Verts[Current->v[2]];
			CData[x].Normal = Norms[Current->n[0]];
			CData[x].Distance = -(CData[x].Normal.Dot(CData[x].Vertices[0]));
			Current = Current->Next;
			x++;
		}
	}
}
	

void MeshData::GenNorms()
{
	Vector Normal;
	
	for (int i = 0; i < 75000; i++)
	{
		Norms[i].x = 0.0f;
		Norms[i].y = 0.0f;
		Norms[i].z = 0.0f;
	}
	
	for (int i = 0; i < NumGroup; i++)
	{
		Triangle *Current;
		
		Current = Group[i].Faces;
		while (Current != NULL)
		{
			CalculateNormal(Verts[Current->v[0]], Verts[Current->v[1]], Verts[Current->v[2]], &Normal);
			Norms[Current->v[0]] = Normal;
			Norms[Current->v[1]] = Normal;
			Norms[Current->v[2]] = Normal;
			Current->n[0] = Current->v[0];
			Current->n[1] = Current->v[1];
			Current->n[2] = Current->v[2];
			
			Current = Current->Next;
		}
	}
	
	for (int i = 0; i < 75000; i++)
	{
		Norms[i].Normalize();
	}
}

void MeshData::GenList()
{
	Triangle *Current;
	DispList = new GLuint[NumGroup];
	for (int i = 0; i < NumGroup; i++)
	{
		if (Mtrls[i].TextName[0] != '\0')
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			Mtrls[i].TextMap = LoadTexture(Mtrls[i].TextName, GL_FALSE, GL_RGBA, GL_RGBA, 1);
			Mtrls[i].hasTexts = true;
		}
		
		if (Mtrls[i].BumpName[0] != '\0')
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			Mtrls[i].BumpMap = LoadTexture(Mtrls[i].BumpName, GL_FALSE, GL_RGBA, GL_RGBA, 1);
			Mtrls[i].hasBumps = true;
		}
		
		DispList[i] = glGenLists(1);
		glNewList(DispList[i], GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Mtrls[i].TextMap.Image);
		glBegin(GL_TRIANGLES);
		Current = Group[i].Faces;
		while (Current != NULL)
		{
			for (int num = 0; num < 3; num++)
			{
				if (Current->n[num] != -1)
					glNormal3f(Norms[Current->n[num]].x, Norms[Current->n[num]].y, Norms[Current->n[num]].z);
				if (Current->t[num] != -1)
					glTexCoord2f(Texts[Current->t[num]].x, Texts[Current->t[num]].y);
				glVertex3f(Verts[Current->v[num]].x, Verts[Current->v[num]].y, Verts[Current->v[num]].z);
			}
			Current = Current->Next;
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();
	}
}

void MeshData::Render(bool list)
{
	Triangle *Current;
	for (int i = 0; i < NumGroup; i++)
	{
		if (!list)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Mtrls[i].TextMap.Image);
			glBegin(GL_TRIANGLES);
			Current = Group[i].Faces;
			while (Current != NULL)
			{
				for (int num = 0; num < 3; num++)
				{
					if (Current->n[num] != -1)
						glNormal3f(Norms[Current->n[num]].x, Norms[Current->n[num]].y, Norms[Current->n[num]].z);
					if (Current->t[num] != -1)
						glTexCoord2f(Texts[Current->t[num]].x, Texts[Current->t[num]].y);
					glVertex3f(Verts[Current->v[num]].x, Verts[Current->v[num]].y, Verts[Current->v[num]].z);
				}
				Current = Current->Next;
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		else
			glCallList(DispList[i]);
	}
}

void MeshData::Release()
{
	for (int i = 0; i < NumGroup; i++)
	{
		DeleteFace(i);
		Group[i].Faces = NULL;
		glDeleteLists(DispList[i], 1);
		glDeleteTextures(1, &Mtrls[i].TextMap.Image);
		glDeleteTextures(1, &Mtrls[i].BumpMap.Image);
	}
	delete DispList;
}

Model::Model()
{
	Mass = 1;
}

Model::~Model()
{
}

void Model::Initialize(MeshData *m)
{
	Mesh = m;
	Radius = Mesh->Radius;
	Mass = 1;
	Box[0] = Mesh->Min;
	Box[1] = Vector(Mesh->Min.x, Mesh->Min.y, Mesh->Max.z);
	Box[2] = Vector(Mesh->Max.x, Mesh->Min.y, Mesh->Max.z);
	Box[3] = Vector(Mesh->Max.x, Mesh->Min.y, Mesh->Min.z);
	
	Box[4] = Vector(Mesh->Min.x, Mesh->Max.y, Mesh->Min.z);
	Box[5] = Vector(Mesh->Min.x, Mesh->Max.y, Mesh->Max.z);
	Box[6] = Mesh->Max;
	Box[7] = Vector(Mesh->Max.x, Mesh->Max.y, Mesh->Min.z);
}

void Model::Update()
{
	LinearAccelaration = Vector(0, 0, 0);
	if (Mass != 0)
		LinearAccelaration = Force/Mass;
		
	LinearAccelaration.y -= 9.8/App.refresh;
	
	LinearVelocity += LinearAccelaration;
	
	Torque = ApplicationPoint.Cross(Force);
	
	AngularAccelaration = Torque/RotationalInertia;
	AngularVelocity += AngularAccelaration;
}	

void Model::UpdatePos()
{
	Position += LinearVelocity;
	Orientation += AngularVelocity;
}	

void Model::Render(bool list)
{
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glTranslated(Position.x, Position.y, Position.z);
	glRotated(Orientation.x, 0, 0, 1);
	glRotated(Orientation.y, 0, 1, 0);
	glRotated(Orientation.x, 1, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	
	Mesh->Render(list);
	
	glPopMatrix();
}
