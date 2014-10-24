/*
 *  SolarSystem.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SolarSystem_h
#define SolarSystem_h

#include "Star.h"
#include "Planet.h"
#include "Asteroid.h"

struct SolarSystem
{
	string Name;

	PhsHandle Physics; // Relative to the center of the Galaxy
};

#endif
