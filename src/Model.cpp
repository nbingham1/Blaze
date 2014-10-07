/*
 *  Model.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Model.h"
#include "Input.h"

void Model::AddVert(GLfloat x, GLfloat y, GLfloat z)
{
	if (NumVerts < 750000)
	{
		Verts[NumVerts*3 + 0] = x;
		Verts[NumVerts*3 + 1] = y;
		Verts[NumVerts*3 + 2] = z;
		NumVerts++;
		
		if (x > Max.x)
			Max.x = x;
		else if (x < Min.x)
			Min.x = x;
		if (y > Max.y)
			Max.y = y;
		else if (x < Min.y)
			Min.y = y;
		if (z > Max.z)
			Max.z = z;
		else if (z < Min.z)
			Min.z = z;
	}
	else
		printf("Too many vertices.\n");
}

void Model::AddNorm(GLfloat x, GLfloat y, GLfloat z)
{
	if (NumNorms < 750000)
	{
		Norms[NumNorms*3 + 0] = x;
		Norms[NumNorms*3 + 1] = y;
		Norms[NumNorms*3 + 2] = z;
		NumNorms++;
	}
	else
		printf("Too many normals.\n");
}

void Model::AddText(GLfloat x, GLfloat y, GLfloat z)
{
	if (NumTexts < 750000)
	{
		Texts[NumTexts*3 + 0] = x;
		Texts[NumTexts*3 + 1] = y;
		Texts[NumTexts*3 + 2] = z;
		NumTexts++;
	}
	else
		printf("Too many texture coordinates.\n");
}

void Model::AddEdge(ModelEdge *Edge)
{
	if (Edges == NULL)
	{
		Edges = Edge;
		NumEdges = 1;
	}
	else
	{
		bool Duplicate = false;
		ModelEdge *Current = Edges;
		while (Current != NULL && Current->Next != NULL)
		{
			if ((*Edge) == (*Current))
			{
				Duplicate = true;
				free(Edge);
				break;
			}

			Current = Current->Next;
		}
		
		if (!Duplicate)
		{
			Edge->RestDistance = Distance(Vector(Verts[Edge->v1*3 + 0], Verts[Edge->v1*3 + 1], Verts[Edge->v1*3 + 2]), Vector(Verts[Edge->v2*3 + 0], Verts[Edge->v2*3 + 1], Verts[Edge->v2*3 + 2]));
			Edge->Next = NULL;
			Current->Next = Edge;
			NumEdges++;
		}
	}
}

void Model::AddFace(ModelFace *Face)
{
	if (Faces == NULL)
	{
		Faces = Face;
		Current = Faces;
		NumFaces = 1;
	}
	else
	{
		Current->Next = Face;
		Current = Current->Next;
		NumFaces++;
	}
}

void Model::Move(double x, double y, double z)
{
	Physics.Position += Vector(x, y, z);
}

void LoadObj(Model *mdl, char *filename)
{
	FILE *file;
	char oneline[255];
	char mtlname[255];
	char phsname[255];
	
	int CurrentMaterial = -1;
	int v1, v2, v3, v4;
	int n1, n2, n3, n4;
	int t1, t2, t3, t4;
	GLfloat x, y, z;
	
	file = fopen(filename, "r");
	
	mdl->NumVerts = 0;
	mdl->NumNorms = 0;
	mdl->NumTexts = 0;
	mdl->NumFaces = 0;
	mdl->NumEdges = 0;
	mdl->NumMats = 0;
	mdl->Edges = NULL;
	mdl->Faces = NULL;
	mdl->Materials = NULL;	
	
	while (!feof(file))
	{
		if (fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "mtllib", 6) == 0)
		{
			if (sscanf(oneline, "mtllib %s", mtlname) == 1)
			{
				for (int x = 0; x < int(strlen(mtlname)); x++)
					if (mtlname[x] == '_')
						mtlname[x] = ' ';
				LoadMaterials(mdl, mtlname);
			}
		}
		
		if (strncmp(oneline, "usemtl", 6) == 0)
		{
			if ((mdl->Materials) && (sscanf(oneline, "usemtl %s", mtlname) == 1))
			{
				int x = 0;
				Material *CurrentMat = mdl->Materials;
				while (true)
				{
					if (strncmp(mtlname, CurrentMat->Name, 32) == 0)
					{
						CurrentMaterial = x;
						break;
					}
					else
						CurrentMaterial = -1;
					
					if (CurrentMat->Next == NULL)
						break;
					else
						CurrentMat = CurrentMat->Next;
					x++;
				}
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
		
		if (sscanf(oneline, "v %f %f %f", &x, &y, &z) == 3)
			mdl->AddVert(x, y, z);
		
		else if (sscanf(oneline, "vn %f %f %f", &x, &y, &z) == 3)
			mdl->AddNorm(x, y, z);
		
		else if (sscanf(oneline, "vt %f %f %f", &x, &y, &z) == 3)
			mdl->AddText(x, y, z);
		
		else if (sscanf(oneline, "vt %f %f", &x, &y) == 2)
			mdl->AddText(x, y, 0.0);
		
		else if (sscanf(oneline, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3, &v4, &n4, &t4) == 12)
		{
			ModelFace *Face1 = (ModelFace*)malloc(sizeof(ModelFace));
			Face1->Next = NULL;
			
			Face1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face1->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face1->t1 = (t1 < 0) ? (mdl->NumTexts + t1) : (t1 - 1);
			Face1->t2 = (t2 < 0) ? (mdl->NumTexts + t2) : (t2 - 1);
			Face1->t3 = (t3 < 0) ? (mdl->NumTexts + t3) : (t3 - 1);
			
			Face1->n1 = (n1 < 0) ? (mdl->NumNorms + n1) : (n1 - 1);
			Face1->n2 = (n2 < 0) ? (mdl->NumNorms + n2) : (n2 - 1);
			Face1->n3 = (n3 < 0) ? (mdl->NumNorms + n3) : (n3 - 1);
						
			Face1->MatID = CurrentMaterial;
			
			mdl->AddFace(Face1);
			
			ModelFace *Face2 = (ModelFace*)malloc(sizeof(ModelFace));
			Face2->Next = NULL;
			
			Face2->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Face2->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Face2->v3 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			
			Face2->t1 = (t3 < 0) ? (mdl->NumTexts + t3) : (t3 - 1);
			Face2->t2 = (t4 < 0) ? (mdl->NumTexts + t4) : (t4 - 1);
			Face2->t3 = (t1 < 0) ? (mdl->NumTexts + t1) : (t1 - 1);
			
			Face2->n1 = (n3 < 0) ? (mdl->NumNorms + n3) : (n3 - 1);
			Face2->n2 = (n4 < 0) ? (mdl->NumNorms + n4) : (n4 - 1);
			Face2->n3 = (n1 < 0) ? (mdl->NumNorms + n1) : (n1 - 1);
						
			Face2->MatID = CurrentMaterial;
			
			mdl->AddFace(Face2);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
			
			ModelEdge *Edge4 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge4->v1 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge4->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge4->Next = NULL;
			mdl->AddEdge(Edge4);
			
			ModelEdge *Edge5 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge5->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge5->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge5->Next = NULL;
			mdl->AddEdge(Edge5);
		}
		
		else if (sscanf(oneline, "f %d//%d %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3, &v4, &n4) == 8)
		{
			ModelFace *Face1 = (ModelFace*)malloc(sizeof(ModelFace));
			Face1->Next = NULL;
			
			Face1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face1->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face1->t1 = -1;
			Face1->t2 = -1;
			Face1->t3 = -1;
			
			Face1->n1 = (n1 < 0) ? (mdl->NumNorms + n1) : (n1 - 1);
			Face1->n2 = (n2 < 0) ? (mdl->NumNorms + n2) : (n2 - 1);
			Face1->n3 = (n3 < 0) ? (mdl->NumNorms + n3) : (n3 - 1);
			
			Face1->MatID = CurrentMaterial;
			
			mdl->AddFace(Face1);
			
			ModelFace *Face2 = (ModelFace*)malloc(sizeof(ModelFace));
			Face2->Next = NULL;
			
			Face2->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Face2->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Face2->v3 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			
			Face2->t1 = -1;
			Face2->t2 = -1;
			Face2->t3 = -1;
			
			Face2->n1 = (n3 < 0) ? (mdl->NumNorms + n3) : (n3 - 1);
			Face2->n2 = (n4 < 0) ? (mdl->NumNorms + n4) : (n4 - 1);
			Face2->n3 = (n1 < 0) ? (mdl->NumNorms + n1) : (n1 - 1);
			
			Face2->MatID = CurrentMaterial;
			
			mdl->AddFace(Face2);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
			
			ModelEdge *Edge4 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge4->v1 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge4->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge4->Next = NULL;
			mdl->AddEdge(Edge4);
			
			ModelEdge *Edge5 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge5->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge5->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge5->Next = NULL;
			mdl->AddEdge(Edge5);
		}
		
		else if (sscanf(oneline, "f %d/%d %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3, &v3, &t4) == 8)
		{
			ModelFace *Face1 = (ModelFace*)malloc(sizeof(ModelFace));
			Face1->Next = NULL;
			
			Face1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face1->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face1->t1 = (t1 < 0) ? (mdl->NumTexts + t1) : (t1 - 1);
			Face1->t2 = (t2 < 0) ? (mdl->NumTexts + t2) : (t2 - 1);
			Face1->t3 = (t3 < 0) ? (mdl->NumTexts + t3) : (t3 - 1);
			
			Face1->n1 = -1;
			Face1->n2 = -1;
			Face1->n3 = -1;
			
			Face1->MatID = CurrentMaterial;
			
			mdl->AddFace(Face1);
			
			ModelFace *Face2 = (ModelFace*)malloc(sizeof(ModelFace));
			Face2->Next = NULL;
			
			Face2->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Face2->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Face2->v3 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			
			Face2->t1 = (t3 < 0) ? (mdl->NumTexts + t3) : (t3 - 1);
			Face2->t2 = (t4 < 0) ? (mdl->NumTexts + t4) : (t4 - 1);
			Face2->t3 = (t1 < 0) ? (mdl->NumTexts + t1) : (t1 - 1);
			
			Face2->n1 = -1;
			Face2->n2 = -1;
			Face2->n3 = -1;
			
			Face2->MatID = CurrentMaterial;
			
			mdl->AddFace(Face2);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
			
			ModelEdge *Edge4 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge4->v1 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge4->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge4->Next = NULL;
			mdl->AddEdge(Edge4);
			
			ModelEdge *Edge5 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge5->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge5->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge5->Next = NULL;
			mdl->AddEdge(Edge5);
		}
		
		else if (sscanf(oneline, "f %d %d %d %d", &v1, &v2, &v3, &v4) == 4)
		{
			ModelFace *Face1 = (ModelFace*)malloc(sizeof(ModelFace));
			Face1->Next = NULL;
			
			Face1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face1->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face1->t1 = -1;
			Face1->t2 = -1;
			Face1->t3 = -1;
			
			Face1->n1 = -1;
			Face1->n2 = -1;
			Face1->n3 = -1;
			
			Face1->MatID = CurrentMaterial;
			
			mdl->AddFace(Face1);
			
			ModelFace *Face2 = (ModelFace*)malloc(sizeof(ModelFace));
			Face2->Next = NULL;
			
			Face2->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Face2->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Face2->v3 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			
			Face2->t1 = -1;
			Face2->t2 = -1;
			Face2->t3 = -1;
			
			Face2->n1 = -1;
			Face2->n2 = -1;
			Face2->n3 = -1;
			
			Face2->MatID = CurrentMaterial;
			
			mdl->AddFace(Face2);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
			
			ModelEdge *Edge4 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge4->v1 = (v4 < 0) ? (mdl->NumVerts + v4) : (v4 - 1);
			Edge4->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge4->Next = NULL;
			mdl->AddEdge(Edge4);
			
			ModelEdge *Edge5 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge5->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge5->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge5->Next = NULL;
			mdl->AddEdge(Edge5);
		}
		
		else if (sscanf(oneline, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3) == 9)
		{
			ModelFace *Face = (ModelFace*)malloc(sizeof(ModelFace));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face->t1 = (t1 < 0) ? (mdl->NumTexts + t1) : (t1 - 1);
			Face->t2 = (t2 < 0) ? (mdl->NumTexts + t2) : (t2 - 1);
			Face->t3 = (t3 < 0) ? (mdl->NumTexts + t3) : (t3 - 1);
			
			Face->n1 = (n1 < 0) ? (mdl->NumNorms + n1) : (n1 - 1);
			Face->n2 = (n2 < 0) ? (mdl->NumNorms + n2) : (n2 - 1);
			Face->n3 = (n3 < 0) ? (mdl->NumNorms + n3) : (n3 - 1);
						
			Face->MatID = CurrentMaterial;
			
			mdl->AddFace(Face);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
		}
		
		else if (sscanf(oneline, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3) == 6)
		{
			ModelFace *Face = (ModelFace*)malloc(sizeof(ModelFace));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face->t1 = -1;
			Face->t2 = -1;
			Face->t3 = -1;
			
			Face->n1 = (n1 < 0) ? (mdl->NumNorms + n1) : (n1 - 1);
			Face->n2 = (n2 < 0) ? (mdl->NumNorms + n2) : (n2 - 1);
			Face->n3 = (n3 < 0) ? (mdl->NumNorms + n3) : (n3 - 1);
			
			Face->MatID = CurrentMaterial;
			
			mdl->AddFace(Face);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
		}
		
		else if (sscanf(oneline, "f %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3) == 6)
		{
			ModelFace *Face = (ModelFace*)malloc(sizeof(ModelFace));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face->t1 = (t1 < 0) ? (mdl->NumTexts + t1) : (t1 - 1);
			Face->t2 = (t2 < 0) ? (mdl->NumTexts + t2) : (t2 - 1);
			Face->t3 = (t3 < 0) ? (mdl->NumTexts + t3) : (t3 - 1);
			
			Face->n1 = -1;
			Face->n2 = -1;
			Face->n3 = -1;
			
			Face->MatID = CurrentMaterial;
			
			mdl->AddFace(Face);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
		}
		
		else if (sscanf(oneline, "f %d %d %d", &v1, &v2, &v3) == 3)
		{
			ModelFace *Face = (ModelFace*)malloc(sizeof(ModelFace));
			Face->Next = NULL;
			
			Face->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Face->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Face->v3 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			
			Face->t1 = -1;
			Face->t2 = -1;
			Face->t3 = -1;
			
			Face->n1 = -1;
			Face->n2 = -1;
			Face->n3 = -1;
			
			Face->MatID = CurrentMaterial;
			
			mdl->AddFace(Face);
			
			ModelEdge *Edge1 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge1->v1 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge1->v2 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge1->Next = NULL;
			mdl->AddEdge(Edge1);
			
			ModelEdge *Edge2 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge2->v1 = (v2 < 0) ? (mdl->NumVerts + v2) : (v2 - 1);
			Edge2->v2 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge2->Next = NULL;
			mdl->AddEdge(Edge2);
			
			ModelEdge *Edge3 = (ModelEdge*)malloc(sizeof(ModelEdge));
			Edge3->v1 = (v3 < 0) ? (mdl->NumVerts + v3) : (v3 - 1);
			Edge3->v2 = (v1 < 0) ? (mdl->NumVerts + v1) : (v1 - 1);
			Edge3->Next = NULL;
			mdl->AddEdge(Edge3);
		}
	}

	fclose(file);
		
	if (phsname[0] != '\0')
		LoadPhysics(mdl, phsname);
	
	GenerateNormals(mdl);
	
	mdl->Radius = Magnitude(mdl->Max);
	GLfloat mag = Magnitude(mdl->Min);
	if (mag > mdl->Radius)
		mdl->Radius = mag;
	
	printf("Done Loading Model %s\n", filename);
}

void LoadMaterials(Model *mdl, char *filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	
	file = fopen(filename, "r");
	
	Material *Current;
	while (!feof(file))
	{
		if (fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "newmtl", 6) == 0)
		{
			mdl->NumMats++;
			Material *material = (Material*)malloc(sizeof(Material));
			sscanf(oneline, "newmtl %s", material->Name);
			material->Initialize();
			if (mdl->NumMats-1 == 0)
			{
				mdl->Materials = material;
				Current = mdl->Materials;
			}
			else
			{
				Current->Next = material;
				Current = Current->Next;
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
			Current->Shininess = v1;
		}
		
		if (strncmp(oneline, "TextMap2D", 9) == 0)
		{
			char LOD[5];
			sscanf(oneline, "TextMap2D %s %s", Current->TextName, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Current->TextMap = Load2DTexture(string(Current->TextName), true);
			else
				Current->TextMap = Load2DTexture(string(Current->TextName), false);
		}
			
		if (strncmp(oneline, "DetailMap2D", 11) == 0)
		{
			char LOD[5];
			sscanf(oneline, "DetailMap2D %s %s", Current->DetailName, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Current->DetailMap = Load2DTexture(string(Current->DetailName), true);
			else
				Current->DetailMap = Load2DTexture(string(Current->DetailName), false);
		}
	}
	fclose(file);
}

void LoadPhysics(Model *mdl, char *filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	
	mdl->Physics.Mass = 1.0;
	mdl->Physics.Elasticity = 1.0;
	
	file = fopen(filename, "r");
	
	while (!feof(file))
	{
		if (fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "Ma", 2) == 0)
		{
			sscanf(oneline, "Ma %f", &v1);
			mdl->Physics.Mass = v1;
		}
		
		if (strncmp(oneline, "El", 2) == 0)
		{
			sscanf(oneline, "El %f", &v1);
			mdl->Physics.Elasticity = v1;
		}
		
		if (strncmp(oneline, "Po", 2) == 0)
		{
			sscanf(oneline, "Po %f %f %f", &v1, &v2, &v3);
			mdl->Physics.Position.x = v1;
			mdl->Physics.Position.y = v2;
			mdl->Physics.Position.z = v3;
		}
		
		if (strncmp(oneline, "Or", 2) == 0)
		{
			sscanf(oneline, "Or %f %f %f", &v1, &v2, &v3);
			mdl->Physics.Orientation.x = v1;
			mdl->Physics.Orientation.y = v2;
			mdl->Physics.Orientation.z = v3;
		}
	}

	fclose(file);
	
	mdl->Physics.Volume = (mdl->Max.x-mdl->Min.x)*(mdl->Max.y-mdl->Min.y)*(mdl->Max.z-mdl->Min.z);
	mdl->Physics.Density = mdl->Physics.Mass/mdl->Physics.Volume;
	mdl->Physics.RotationalInertia = CalculateRotationalInertia(mdl->Physics.Mass, (Vector*)mdl->Verts, mdl->NumVerts);
}

void GenerateNormals(Model *mdl)
{
	Vector Normal;
	
	for (int x = 0; x < 750000; x++)
	{
		mdl->Norms[x*3 + 0] = 0.0;
		mdl->Norms[x*3 + 1] = 0.0;
		mdl->Norms[x*3 + 2] = 0.0;
	}

	ModelFace *Current = mdl->Faces;
	while (Current != NULL)
	{
		Normal = CalculateNormal(Vector(mdl->Verts[Current->v1*3 + 0], mdl->Verts[Current->v1*3 + 1], mdl->Verts[Current->v1*3 + 2]), 
								 Vector(mdl->Verts[Current->v2*3 + 0], mdl->Verts[Current->v2*3 + 1], mdl->Verts[Current->v2*3 + 2]), 
								 Vector(mdl->Verts[Current->v3*3 + 0], mdl->Verts[Current->v3*3 + 1], mdl->Verts[Current->v3*3 + 2]));
		mdl->Norms[Current->v1*3 + 0] += Normal.x;
		mdl->Norms[Current->v1*3 + 1] += Normal.y;
		mdl->Norms[Current->v1*3 + 2] += Normal.z;
		
		mdl->Norms[Current->v2*3 + 0] += Normal.x;
		mdl->Norms[Current->v2*3 + 1] += Normal.y;
		mdl->Norms[Current->v2*3 + 2] += Normal.z;
		
		mdl->Norms[Current->v3*3 + 0] += Normal.x;
		mdl->Norms[Current->v3*3 + 1] += Normal.y;
		mdl->Norms[Current->v3*3 + 2] += Normal.z;
		
		Current->n1 = Current->v1;
		Current->n2 = Current->v2;
		Current->n3 = Current->v3;
		
		Current = Current->Next;
	}
	
	for (int x = 0; x < 750000; x++)
	{
		Normal = Normalize(Vector(mdl->Norms[x*3 + 0], mdl->Norms[x*3 + 1], mdl->Norms[x*3 + 2]));
		mdl->Norms[x*3 + 0] = Normal.x;
		mdl->Norms[x*3 + 1] = Normal.y;
		mdl->Norms[x*3 + 2] = Normal.z;
	}
}

void RenderMdl(Model *mdl)
{
	ModelFace *Current = mdl->Faces;
	
	mdl->Materials->Enable();
	glBegin(GL_TRIANGLES);
	while (Current != NULL)
	{
		if (Current->t1 != -1)
			glTexCoord3fv(&mdl->Texts[Current->t1*3]);
		if (Current->n1 != -1)
			glNormal3fv(&mdl->Norms[Current->n1*3]);
		if (Current->v1 != -1)
			glVertex3fv(&mdl->Verts[Current->v1*3]);
		
		if (Current->t2 != -1)
			glTexCoord3fv(&mdl->Texts[Current->t2*3]);
		if (Current->n2 != -1)
			glNormal3fv(&mdl->Norms[Current->n2*3]);
		if (Current->v2 != -1)
			glVertex3fv(&mdl->Verts[Current->v2*3]);
		
		if (Current->t3 != -1)
			glTexCoord3fv(&mdl->Texts[Current->t3*3]);
		if (Current->n3 != -1)
			glNormal3fv(&mdl->Norms[Current->n3*3]);
		if (Current->v3 != -1)
			glVertex3fv(&mdl->Verts[Current->v3*3]);
		
		Current = Current->Next;
	}
	glEnd();
	mdl->Materials->Disable();
}

void ReleaseMdl(Model *mdl)
{
	ModelEdge *CurrentEdge = mdl->Edges, *PreviousEdge;
	while (CurrentEdge != NULL)
	{
		PreviousEdge = CurrentEdge;
		CurrentEdge = CurrentEdge->Next;
		free(PreviousEdge);
		PreviousEdge = NULL;
	}
	
	ModelFace *CurrentFace = mdl->Faces, *PreviousFace;
	while (CurrentFace != NULL)
	{
		PreviousFace = CurrentFace;
		CurrentFace = CurrentFace->Next;
		free(PreviousFace);
		PreviousFace = NULL;
	}
	
	Material *CurrentMat = mdl->Materials, *PreviousMat;
	while (CurrentMat != NULL)
	{
		PreviousMat = CurrentMat;
		CurrentMat = CurrentMat->Next;
		PreviousMat->Release();
		free(PreviousMat);
		PreviousMat = NULL;
	}
}
