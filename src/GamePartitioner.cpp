/*
	GamePartitioner.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 8/14/06.
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

#include "GamePartitioner.h"

void BuildBSPTree(BSPNode *n, Triangle *Data, int DataSize, Vector Min, Vector Max, int splitnum)
{
	if (DataSize == 0)
	{
		n->Data = Data;
		n->Min = Min;
		n->Max = Max;
	}
	else if (DataSize <= BSP_MAXTRIS)
	{
		n->Data = Data;
		n->Min = Min;
		n->Max = Max;
	}
	else
	{
		Vector Mid = MidPoint(Min, Max);
		
		Triangle *FrontList = NULL, *BackList = NULL;
		int FrontSize = 0, BackSize = 0;
		BSPNode *FrontNode = (BSPNode*)malloc(sizeof(BSPNode));
		BSPNode *BackNode = (BSPNode*)malloc(sizeof(BSPNode));
		FrontNode->Initialize();
		BackNode->Initialize();
		
		if (splitnum == 0)
		{
			n->Splitter.Normal = Vector(1, 0, 0);
			n->Splitter.Distance = -(Dot(n->Splitter.Normal, Vector(Mid.x, Max.y, Max.z)));
			FrontNode->Min = Vector(Mid.x, Min.y, Min.z);
			FrontNode->Max = Max;
			BackNode->Min = Min;
			BackNode->Max = Vector(Mid.x, Max.y, Max.z);
		}
		
		else if (splitnum == 1)
		{
			n->Splitter.Normal = Vector(0, -1, 0);
			n->Splitter.Distance = -(Dot(n->Splitter.Normal, Vector(Min.x, Mid.y, Max.z)));
			FrontNode->Min = Vector(Min.x, Mid.y, Min.z);
			FrontNode->Max = Max;
			BackNode->Min = Min;
			BackNode->Max = Vector(Max.x, Mid.y, Max.z);
		}
		
		else if (splitnum == 2)
		{
			n->Splitter.Normal = Vector(0, 0, 1);
			n->Splitter.Distance = -(Dot(n->Splitter.Normal, Vector(Min.x, Max.y, Mid.z)));
			FrontNode->Min = Vector(Min.x, Min.y, Mid.z);
			FrontNode->Max = Max;
			BackNode->Min = Min;
			BackNode->Max = Vector(Max.x, Max.y, Mid.z);
		}
		
		Triangle *c1, *c2;
		
		Triangle *Current, *Previous;
		Current = Data;
		while (Current != NULL)
		{
			Previous = Current;
			Current = Current->Next;
			Previous->Next = NULL;
			
			switch (ClassifyTriangletoPlane(n->Splitter, Previous))
			{
				case 0:
					if (FrontList == NULL)
					{
						FrontList = Previous;
						c1 = FrontList;
					}
					else
					{
						c1->Next = Previous;
						c1 = c1->Next;
					}
					FrontSize++;
					break;
				case 1:
					if (BackList == NULL)
					{
						BackList = Previous;
						c2 = BackList;
					}
					else
					{
						c2->Next = Previous;
						c2 = c2->Next;
					}
					BackSize++;
					break;
				case 2:
					if (FrontList == NULL)
					{
						FrontList = Previous;
						c1 = FrontList;
					}
					else
					{
						c1->Next = Previous;
						c1 = c1->Next;
					}
					FrontSize++;
					break;
				case 3:
					Triangle *FrontPrev = (Triangle*)malloc(sizeof(Triangle));
					Triangle *BackPrev = (Triangle*)malloc(sizeof(Triangle));
					*FrontPrev = *Previous;
					*BackPrev = *Previous;
					
					if (FrontList == NULL)
					{
						FrontList = FrontPrev;
						c1 = FrontList;
					}
					else
					{
						c1->Next = FrontPrev;
						c1 = c1->Next;
					}
					FrontSize++;

					if (BackList == NULL)
					{
						BackList = BackPrev;
						c2 = BackList;
					}
					else
					{
						c2->Next = BackPrev;
						c2 = c2->Next;
					}
					BackSize++;
					free(Previous);
					break;
			}
		}
		BuildBSPTree(FrontNode, FrontList, FrontSize, FrontNode->Min, FrontNode->Max, (splitnum+1)%3);
		BuildBSPTree(BackNode, BackList, BackSize, BackNode->Min, BackNode->Max, (splitnum+1)%3);
		n->Front = FrontNode;
		n->Back = BackNode;
	}
}

BSPNode *GetNode(BSPNode *n, Vector ID)
{
	if (n->Leaf())
		return n;
	else
	{
		switch (ClassifyVertextoPlane(n->Splitter, ID))
		{
			case 0:
				return GetNode(n->Front, ID);
				break;
			case 1:
				return GetNode(n->Back, ID);
				break;
			case 2:
				return GetNode(n->Front, ID);
				break;
			default:
				return NULL;
				break;
		}
	}
}

void FreeBSPTree(BSPNode *n)
{
	if (n->Leaf())
	{
		ReleaseTriangles(n->Data);
		free(n);
	}
	else
	{
		FreeBSPTree(n->Front);
		FreeBSPTree(n->Back);
		free(n);
	}
}

void BSPTree::Initialize()
{
	DataSize = 0;
	Data = NULL;
	Start = (BSPNode*)malloc(sizeof(BSPNode));
	ModelList = NULL;
}

void BSPTree::Build()
{
	BuildBSPTree(Start, Data, DataSize, Min, Max, 0);
}

void BSPTree::CheckForCollisions()
{
	BSPNode *CurrentNode;
	Triangle *CurrentTri;
	GameModel *Current = ModelList;
	GameModel *Current2;
	bool ground = false;
	while (Current != NULL)
	{
		ground = false;
		CurrentNode = GetNode(Start, Current->Physics.Position);
		CurrentTri = CurrentNode->Data;
		while (CurrentTri != NULL)
		{
			if (BoxtoTriangleCollision(Current, *CurrentTri))
			{
				ground = true;
			}
			CurrentTri = CurrentTri->Next;
		}
		
		CurrentNode = GetNode(Start, Current->Physics.Position+Current->Physics.LinearVelocity);
		CurrentTri = CurrentNode->Data;
		while (CurrentTri != NULL)
		{
			if (BoxtoTriangleCollision(Current, *CurrentTri))
			{
				ground = true;
			}
			CurrentTri = CurrentTri->Next;
		}
		
		Current->collidewithground = ground;
		Current = Current->Next;
	}

	Current = ModelList;
	while (Current != NULL)
	{
		Current2 = Current;
		while (Current2 != NULL)
		{
			if (Current != Current2)
			{
				if (BoxtoBoxCollision(Current, Current2))
				{
				}
			}
			Current2 = Current2->Next;
		}
		Current = Current->Next;
	}
}

void BSPTree::Release()
{
	FreeBSPTree(Start);
}

void BSPTree::AddModelToList(GameModel *M)
{
	M->Next = NULL;
	if (ModelList == NULL)
		ModelList = M;
	else
	{
		GameModel *Current = ModelList;
		while (Current->Next != NULL)
			Current = Current->Next;
		Current->Next = M;
	}
}

void BSPTree::AddRenderListToData(GLfloat *v, unsigned short *indices, int NumIndices, int CollisionType)
{
	
	Triangle *c = NULL;
	for (int x = 0; x < NumIndices; x += 3)
	{
		Triangle *face = (Triangle*)malloc(sizeof(Triangle));
		face->Next = NULL;
		face->v[0] = Vector(v[indices[x+0]*3 + 0], v[indices[x+0]*3 + 1], v[indices[x+0]*3 + 2]);
		face->v[1] = Vector(v[indices[x+1]*3 + 0], v[indices[x+1]*3 + 1], v[indices[x+1]*3 + 2]);
		face->v[2] = Vector(v[indices[x+2]*3 + 0], v[indices[x+2]*3 + 1], v[indices[x+2]*3 + 2]);
		
		for (int y = 0; y < 3; y++)
		{
			if (face->v[y].x > Max.x)
				Max.x = face->v[y].x;
			if (face->v[y].y > Max.y)
				Max.y = face->v[y].y;
			if (face->v[y].z > Max.z)
				Max.z = face->v[y].z;
			if (face->v[y].x < Min.x)
				Min.x = face->v[y].x;
			if (face->v[y].y < Min.y)
				Min.y = face->v[y].y;
			if (face->v[y].z < Min.z)
				Min.z = face->v[y].z;
		}
				
		face->CollisionType = CollisionType;
		
		if (Data == NULL)
		{
			Data = face;
			c = Data;
		}
		else
		{
			if (c == NULL)
			{
				c = Data;
				while (c->Next != NULL)
					c = c->Next;
			}
			
			c->Next = face;
			c = c->Next;
		}
		DataSize++;
	}
}

void BSPTree::AddModelToData(GameModel *M)
{
	Triangle *c;
	ModelTriangle *Current = M->Geometry->Faces;
	while (Current != NULL)
	{
		Triangle *face = (Triangle*)malloc(sizeof(Triangle));
		face->Next = NULL;
		
		face->v[0] = M->Geometry->Verts[Current->v1];
		face->v[1] = M->Geometry->Verts[Current->v2];
		face->v[2] = M->Geometry->Verts[Current->v3];
		
		for (int x = 0; x < 3; x++)
		{
			if (face->v[x].x > Max.x)
				Max.x = face->v[x].x;
			if (face->v[x].y > Max.y)
				Max.y = face->v[x].y;
			if (face->v[x].z > Max.z)
				Max.z = face->v[x].z;
			if (face->v[x].x < Min.x)
				Min.x = face->v[x].x;
			if (face->v[x].y < Min.y)
				Min.y = face->v[x].y;
			if (face->v[x].z < Min.z)
				Min.z = face->v[x].z;
		}
			
		face->CollisionType = 0;
		if (Data == NULL)
		{
			Data = face;
			c = Data;
		}
		else
		{
			c->Next = face;
			c = c->Next;
		}
		DataSize++;
		Current = Current->Next;
	}
}
