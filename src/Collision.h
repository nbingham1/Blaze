/*
 *  Collision.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 4/23/07.
 *  Copyright 2007 Sol Union. All rights reserved.
 *
 */

#include "OpenGLIncludes.h"
#include "CoreMathematics.h"

#ifndef Collision_h
#define Collision_h

struct bsp_node
{
	bsp_node()
	{
		static_data = NULL;
		dynamic_data = NULL;
		children = NULL;
	}
	
	Plane Splitter;
	
	unsigned short *static_data;
	void *dynamic_data;
	
	void *children;
};

struct bsp_tree
{
	bsp_tree()
	{
		base = NULL;
	}
	
	bsp_node *base;
	
	GLfloat *static_data;
	void	*dynamic_data;
	
	
	void create_node(bsp_node *n, unsigned short *indices);
	void destroy_node(bsp_node *n);
	
	void swap(unsigned short i1, unsigned short i2);
	void binary_sort(unsigned short *indices);
};

#endif
