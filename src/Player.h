/*
 *  Player.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 1/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Player_h
#define Player_h

#include "CoreMathematics.h"
#include "OpenGLIncludes.h"

#include "Model.h"
#include "Camera.h"

struct Player
{
	Camera Viewpoint;
	Model  PlayerModel;
	
	int	   MovementType;
	Vector Move;
	bool   Jump;
	bool   on_ground;
	double scale;
	
	int GridX, GridY;
	int Planet;
	int System;
	int Galaxy;
	
	void Init(char *mn, int mt);
	void MoveForward(double z);
	void MoveSideways(double x);
	void MoveVertically(double y);
	void Render();
	void Release();
};

#endif
