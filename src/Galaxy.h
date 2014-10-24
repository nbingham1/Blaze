/*
 *  Galaxy.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Galaxy_h
#define Galaxy_h

#include "SolarSystem.h"

struct Galaxy
{
	string Name;

	PhsHandle Physics; // Relative to the center of the Universe
};

#endif
