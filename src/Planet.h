/*
 *  Planet.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Planet_h
#define Planet_h

#include "BasicAstronomy.h"
#include "Material.h"

// Verts per node
#define vpn 32
// Pixels per node
#define ppn 128
// vpn/ppn
#define ppn_into_vpn GLdouble(vpn-1)/GLdouble(ppn)

enum terra_type
{
	height,
	three_shift
};

struct QuadTreeNode
{
	GLdouble *verts;
	terra_type *type;
	
	GLdouble *liquid_verts;
	
	Material mat;
	
	GLdouble x_size, longitude, // rads
			 y_size, latitude; // rads
	
	QuadTreeNode *data_start;
	GLdouble *planet_minimum;
	GLdouble *planet_maximum;
	
	Vector half_vec;
	
	GLdouble minimum;
	GLdouble maximum;
	GLdouble average;
	
	bool rend;
	
	GLdouble radius; //  meters
	
	QuadTreeNode *children;
	QuadTreeNode *parent;
	
	void Load(int seed, GLdouble rad, terra_type *t);
	
	void LODCheck(camera_reference *info);
	void Split();
	void Merge();
	void MakeTexture();
	
	void Render(GLdouble *texts, unsigned int *indices, camera_reference *info);
	void Release();
	
	GLdouble GetGroundHeight(GLdouble longitude, GLdouble latitude);
};

struct Planet
{
	string Name;
	
	QuadTreeNode *vert_data;
	terra_type type;
	
	PhsHandle Physics; // Relative to the center of the Solar System
	GLdouble Radius; // meters
	
	GLdouble *node_texts;
	unsigned int *node_indices;
	
	camera_reference player_info;
	
	void Load(string name, Camera *view);
	void Prepare();
	void Render();
	void Release();
};

#endif