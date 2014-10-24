/*
 *  Planet.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/24/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "graphics.h"
#include "OffscreenRenderer.h"
#include "CoreMathematics.h"
#include "CorePhysics.h"
#include "Shader.h"
#include "Material.h"
#include "Model.h"
#include "Camera.h"

#ifndef Planet_h
#define Planet_h

#define node_size 32
#define tex_size 64
#define node_tex_radio GLdouble(node_size-1)/GLdouble(tex_size)

struct ReferenceInformation
{
	Camera *ReferencePoint;
	Vector vec1, vec2;
	GLdouble x_angle, y_angle,
			 nx_angle, ny_angle;
	GLdouble distance;
	GLdouble ndistance;
	GLdouble visualangle, visualangle1;
	void *planet;

	void Update(void *planet);
};

struct planet_node
{
	int seed;
	GLdouble *heights; // meters
	Material material;
	GLfloat percent_loaded;
	GLdouble foglimit;

	GLdouble x_size, x_angle, // rads
			 y_size, y_angle; // rads

	GLdouble radius, //  meters
			 average, // meters
			 minimum, // meters
			 maximum; // meters

	planet_node *parent;
	planet_node *children;

	void load(int sd, ReferenceInformation *reference);
	void destroy();

	void split(ReferenceInformation *reference);
	void merge();

	void determine_lod(ReferenceInformation *reference);
	void renderheights(ReferenceInformation *reference);
	void renderwater(ReferenceInformation *reference);
	void rendersky(ReferenceInformation *reference, GLdouble scale, GLdouble angle);
	GLdouble get_height(ReferenceInformation *reference);
	GLdouble get_minimum(GLdouble curr);
	GLdouble get_maximum(GLdouble curr);
	void CreateTextureMap(int a, int b, ReferenceInformation *reference);
};

struct Planet
{
	string Name;
	planet_node data;
	PhsHandle Physics;
	Material Water;
 	Material Sky;
	Material Shell;

	GLdouble sky_angle;

	GLdouble AtmosphereScale;
	GLfloat AtmosphereDensity;
	Vector AtmosphereColor;

	Vector OceanColor;

	GLdouble maximum;
	GLdouble radius;
	GLdouble minimum;

	ReferenceInformation referencepoint;

	void Load(string Name, Camera *reference);
	void Prepare();
	void Render();
	void Release();
};

#endif
