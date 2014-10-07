/*
 *  Collision.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 4/23/07.
 *  Copyright 2007 Sol Union. All rights reserved.
 *
 */

#include "Collision.h"

void bsp_tree::create_node(bsp_node *n, unsigned short *indices)
{
	bsp_node *children = (bsp_node*)malloc(sizeof(bsp_node)*2);
	Vector normal;
	Vector v1, v2, v3, v4;
	
	for (int x = 0; x < sizeof(indices)/(sizeof(unsigned short)*4); x+=4)
	{
		v1 = Vector(indices[x+0], indices[x+1], indices[x+2]);
		v2 = Vector(indices[x+3], indices[x+4], indices[x+5]);
		v3 = Vector(indices[x+6], indices[x+7], indices[x+8]);
		v4 = Vector(indices[x+9], indices[x+10], indices[x+11]);
		normal = CalculateNormal(v1, v2, v3);
		
	}
}

void bsp_tree::destroy_node(bsp_node *n)
{
}

void bsp_tree::binary_sort(unsigned short *indices)
{
	int low = 0;
	int high = sizeof(indices)/(sizeof(unsigned short)*3);
	int pivot;
	unsigned short temp;
		
	while (low != high)
	{
		
	}
}
