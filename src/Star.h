/*
 *  Star.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Star_h
#define Star_h

#include "BasicAstronomy.h"

struct Star
{
	string Name;
	
	Material Coloring;
	PhsHandle Physics; // Relative to the center of the Solar System
	GLdouble Radius;
	GLdouble LightRadius;
	
	// temperary variables (optimization)
	GLdouble RadiusSquared;
	GLdouble LightRadiusSquared;
	GLdouble BackProjectionDistance;
	
	camera_reference reference;
	
	void Load(string name, Camera *r);
	void Prepare();
	void Render();
	void Release();
};

#endif