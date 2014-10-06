/*
	GamePartitioner.h
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

#include "GeometricalTypes.h"
#include "GameModel.h"

#ifndef GamePartitioner_h
#define GamePartitioner_h

#define BSP_MAXTRIS 40

struct BSPNode
{
	Plane Splitter;
	
	Triangle *Data;
	
	Vector Min, Max;
		
	BSPNode *Front;
	BSPNode *Back;
	
	void Initialize()
	{
		Data = NULL;
		Front = NULL;
		Back = NULL;
	}
	
	bool Leaf()
	{
		if (Front == NULL && Back == NULL)
			return true;
		else
			return false;
	}
};

void BuildBSPTree(BSPNode *n, Triangle *Data, int DataSize, Vector Min, Vector Max, int splitnum);
BSPNode *GetNode(BSPNode *n, Vector ID);
void FreeBSPTree(BSPNode *n);

struct BSPTree
{
	BSPNode *Start;
	
	Triangle *Data;
	int DataSize;
	
	Vector Min, Max;
	
	GameModel *ModelList;
	
	void Initialize();
	void Build();
	void CheckForCollisions();
	void Release();
	
	void AddModelToList(GameModel *M);
	
	void AddRenderListToData(GLfloat *v, unsigned short *indices, int NumIndices, int CollisionType);
	void AddModelToData(GameModel *M);
};

#endif
