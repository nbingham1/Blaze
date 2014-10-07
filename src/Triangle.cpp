/*
	Triangle.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/18/06.
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

#include "Triangle.h"

Triangle &Triangle::operator =(Triangle t)
{
	this->v[0] = t.v[0];
	this->v[1] = t.v[1];
	this->v[2] = t.v[2];
	this->n = t.n;
	
	this->CollisionType = t.CollisionType;
	this->Next = t.Next;
	return *this;
}

void Triangle::Split(Plane Splitter, Triangle *FrontList, Triangle *BackList)
{
	int points[3];
	int ploc[3] = {0, 0, 0};
	for (int x = 0; x < 3; x++)
	{
		points[x] = ClassifyVertextoPlane(Splitter, v[x]);
		ploc[ClassifyVertextoPlane(Splitter, v[x])]++;
	}
	
	if (ploc[0] == 2)
	{
		if (points[0] == 1)
		{
			Vector p1, p2;
			intersect_RayPlane(v[0], v[1], Splitter, &p1);
			intersect_RayPlane(v[0], v[2], Splitter, &p2);
			
			BackList->v[0] = v[0];
			BackList->v[1] = p1;
			BackList->v[2] = p2;
			BackList->Next = NULL;
			BackList->CollisionType = CollisionType;
			BackList->n = n;
			
			Triangle *Front = (Triangle*)malloc(sizeof(Triangle));
			
			FrontList->v[0] = p1;
			FrontList->v[1] = v[1];
			FrontList->v[2] = v[2];
			
			Front->v[0] = p2;
			Front->v[1] = p1;
			Front->v[2] = v[2];
			
			FrontList->Next = Front;
			FrontList->CollisionType = CollisionType;
			FrontList->n = n;
			Front->Next = NULL;
			Front->CollisionType = CollisionType;
			Front->n = n;
		}
		else if (points[1] == 1)
		{
			Vector p1, p2;
			intersect_RayPlane(v[1], v[2], Splitter, &p1);
			intersect_RayPlane(v[1], v[1], Splitter, &p2);
			
			BackList->v[0] = v[1];
			BackList->v[1] = p1;
			BackList->v[2] = p2;
			BackList->Next = NULL;
			BackList->CollisionType = CollisionType;
			BackList->n = n;
			
			Triangle *Front = (Triangle*)malloc(sizeof(Triangle));
			
			FrontList->v[0] = p1;
			FrontList->v[1] = v[2];
			FrontList->v[2] = v[0];
			
			Front->v[0] = p2;
			Front->v[1] = p1;
			Front->v[2] = v[0];
			
			FrontList->Next = Front;
			FrontList->CollisionType = CollisionType;
			FrontList->n = n;
			Front->Next = NULL;
			Front->CollisionType = CollisionType;
			Front->n = n;
		}
		else if (points[2] == 1)
		{
			Vector p1, p2;
			intersect_RayPlane(v[2], v[0], Splitter, &p1);
			intersect_RayPlane(v[2], v[1], Splitter, &p2);
			
			BackList->v[0] = v[2];
			BackList->v[1] = p1;
			BackList->v[2] = p2;
			BackList->Next = NULL;
			BackList->CollisionType = CollisionType;
			BackList->n = n;
			
			Triangle *Front = (Triangle*)malloc(sizeof(Triangle));
			
			FrontList->v[0] = p1;
			FrontList->v[1] = v[0];
			FrontList->v[2] = v[1];
			
			Front->v[0] = p2;
			Front->v[1] = p1;
			Front->v[2] = v[1];
			
			FrontList->Next = Front;
			FrontList->CollisionType = CollisionType;
			FrontList->n = n;
			Front->Next = NULL;
			Front->CollisionType = CollisionType;
			Front->n = n;
		}
	}
	else if (ploc[1] == 2)
	{
		if (points[0] == 0)
		{
			Vector p1, p2;
			intersect_RayPlane(v[0], v[1], Splitter, &p1);
			intersect_RayPlane(v[0], v[2], Splitter, &p2);
			
			FrontList->v[0] = v[0];
			FrontList->v[1] = p1;
			FrontList->v[2] = p2;
			FrontList->Next = NULL;
			FrontList->CollisionType = CollisionType;
			FrontList->n = n;
			
			Triangle *Back = (Triangle*)malloc(sizeof(Triangle));
			
			BackList->v[0] = p1;
			BackList->v[1] = v[1];
			BackList->v[2] = v[2];
			
			Back->v[0] = p2;
			Back->v[1] = p1;
			Back->v[2] = v[2];
			
			BackList->Next = Back;
			BackList->CollisionType = CollisionType;
			BackList->n = n;
			Back->Next = NULL;
			Back->CollisionType = CollisionType;
			Back->n = n;
		}
		else if (points[1] == 0)
		{
			Vector p1, p2;
			intersect_RayPlane(v[1], v[2], Splitter, &p1);
			intersect_RayPlane(v[1], v[1], Splitter, &p2);
			
			FrontList->v[0] = v[1];
			FrontList->v[1] = p1;
			FrontList->v[2] = p2;
			FrontList->Next = NULL;
			FrontList->CollisionType = CollisionType;
			FrontList->n = n;
			
			Triangle *Back = (Triangle*)malloc(sizeof(Triangle));
			
			BackList->v[0] = p1;
			BackList->v[1] = v[2];
			BackList->v[2] = v[0];
			
			Back->v[0] = p2;
			Back->v[1] = p1;
			Back->v[2] = v[0];
			
			BackList->Next = Back;
			BackList->CollisionType = CollisionType;
			BackList->n = n;
			Back->Next = NULL;
			Back->CollisionType = CollisionType;
			Back->n = n;
		}
		else if (points[2] == 0)
		{
			Vector p1, p2;
			intersect_RayPlane(v[2], v[0], Splitter, &p1);
			intersect_RayPlane(v[2], v[1], Splitter, &p2);
			
			FrontList->v[0] = v[2];
			FrontList->v[1] = p1;
			FrontList->v[2] = p2;
			FrontList->Next = NULL;
			FrontList->CollisionType = CollisionType;
			FrontList->n = n;
			
			Triangle *Back = (Triangle*)malloc(sizeof(Triangle));
			
			BackList->v[0] = p1;
			BackList->v[1] = v[0];
			BackList->v[2] = v[1];
			
			Back->v[0] = p2;
			Back->v[1] = p1;
			Back->v[2] = v[1];
			
			BackList->Next = Back;
			BackList->CollisionType = CollisionType;
			BackList->n = n;
			Back->Next = NULL;
			Back->CollisionType = CollisionType;
			Back->n = n;
		}
	}
	else if (ploc[2] == 1)
	{
		if (points[0] == 2)
		{
			if (points[1] == 0)
			{
				Vector p;
				intersect_RayPlane(v[1], v[2], Splitter, &p);
								
				FrontList->v[0] = v[0];
				FrontList->v[1] = v[1];
				FrontList->v[2] = p;
				
				FrontList->Next = NULL;
				FrontList->CollisionType = CollisionType;
				FrontList->n = n;
				
				BackList->v[0] = v[0];
				BackList->v[1] = p;
				BackList->v[2] = v[2];
				
				BackList->Next = NULL;
				BackList->CollisionType = CollisionType;
				BackList->n = n;
			}
			else if (points[2] == 0)
			{
				Vector p;
				intersect_RayPlane(v[1], v[2], Splitter, &p);
								
				BackList->v[0] = v[0];
				BackList->v[1] = v[1];
				BackList->v[2] = p;
				
				BackList->Next = NULL;
				BackList->CollisionType = CollisionType;
				BackList->n = n;
				
				FrontList->v[0] = v[0];
				FrontList->v[1] = p;
				FrontList->v[2] = v[2];
				
				FrontList->Next = NULL;
				FrontList->CollisionType = CollisionType;
				FrontList->n = n;
			}
		}
		else if (points[1] == 2)
		{
			if (points[0] == 0)
			{
				Vector p;
				intersect_RayPlane(v[0], v[2], Splitter, &p);
								
				FrontList->v[0] = v[0];
				FrontList->v[1] = v[1];
				FrontList->v[2] = p;
				
				FrontList->Next = NULL;
				FrontList->CollisionType = CollisionType;
				FrontList->n = n;
				
				BackList->v[0] = p;
				BackList->v[1] = v[1];
				BackList->v[2] = v[2];
				
				BackList->Next = NULL;
				BackList->CollisionType = CollisionType;
				BackList->n = n;
			}
			else if (points[2] == 0)
			{
				Vector p;
				intersect_RayPlane(v[0], v[2], Splitter, &p);
				
				BackList->v[0] = v[0];
				BackList->v[1] = v[1];
				BackList->v[2] = p;
				
				BackList->Next = NULL;
				BackList->CollisionType = CollisionType;
				BackList->n = n;
				
				FrontList->v[0] = p;
				FrontList->v[1] = v[1];
				FrontList->v[2] = v[2];
				
				FrontList->Next = NULL;
				FrontList->CollisionType = CollisionType;
				FrontList->n = n;
			}
		}
		else if (points[2] == 2)
		{
			if (points[0] == 0)
			{
				Vector p;
				intersect_RayPlane(v[0], v[1], Splitter, &p);
				
				FrontList->v[0] = v[0];
				FrontList->v[1] = p;
				FrontList->v[2] = v[2];
				
				FrontList->Next = NULL;
				FrontList->CollisionType = CollisionType;
				FrontList->n = n;
				
				BackList->v[0] = p;
				BackList->v[1] = v[1];
				BackList->v[2] = v[2];
				
				BackList->Next = NULL;
				BackList->CollisionType = CollisionType;
				BackList->n = n;
			}
			else if (points[1] == 0)
			{
				Vector p;
				intersect_RayPlane(v[0], v[1], Splitter, &p);
				
				BackList->v[0] = v[0];
				BackList->v[1] = p;
				BackList->v[2] = v[2];
				
				BackList->Next = NULL;
				BackList->CollisionType = CollisionType;
				BackList->n = n;
				
				FrontList->v[0] = p;
				FrontList->v[1] = v[1];
				FrontList->v[2] = v[2];
				
				FrontList->Next = NULL;
				FrontList->CollisionType = CollisionType;
				FrontList->n = n;
			}
		}
	}
}

int ClassifyTriangletoPlane(Plane p, Triangle *t)
{
	int points[] = {0, 0, 0};
	for (int x = 0; x < 3; x++)
		points[ClassifyVertextoPlane(p, t->v[x])]++;
	
	if (points[1] == 0)
		return 0;
	else if (points[0] == 0)
		return 1;
	else if (points[2] == 3)
		return 2;
	else
		return 3;
}

