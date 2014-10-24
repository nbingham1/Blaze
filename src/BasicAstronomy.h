/*
 *  BasicAstronomy.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BasicAstronomy_h
#define BasicAstronomy_h

#include "OpenGLIncludes.h"
#include "CorePhysics.h"
#include "Material.h"
#include "Camera.h"

enum body_type
{
	galaxy,
	solar_system,
	planet,
	star,
	asteroid
};

struct camera_reference
{
	Camera *cam;
	Vector pvec, npvec;
	GLdouble longitude, latitude,
			 nlongitude, nlatitude;
	GLdouble distance, ndistance;
	GLdouble scale;
	GLdouble visualangle;
	GLdouble ground_height;
	
	Vector vp_vis;
	GLdouble vp_dist;
	
	PhsHandle *body_physics;
	
	void Initialize(Camera *c, PhsHandle *phs);
	void Update();
	void UpdateScale();
	void CheckCollision(PhsHandle *phs);
};

#endif