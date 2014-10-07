/*
 *  Player.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 1/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Player.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void Player::Init(char *mn, int mt)
{
	printf("loading\n");
	LoadObj(&PlayerModel, mn);
	printf("attaching\n");
	Viewpoint.AttachCamera(&PlayerModel, Vector(0.0, -20.0/8.0, -3.0/8.0), true);
	MovementType = mt;
	Jump = false;
	on_ground = false;
	GridX = -1;
	GridY = -1;
	Planet = -1;
	System = 1;
	Galaxy = 1;
	scale = 1.0;
	distance = 1000000000;
	mult = 10000;
}

void Player::MoveForward(GLdouble z)
{
	Move.z = z;
}

void Player::MoveSideways(GLdouble x)
{
	Move.x = x;
}

void Player::MoveVertically(GLdouble y)
{
	if (on_ground)
		Move.y = 50*y;
}

void Player::Render()
{
	GLdouble s;
	if ((distance-6380) <= 30.0)
		s = 1.0/max((distance-6380.0), .2);
	else
		s = 1.0/(distance-6380);
	glScaled(s, s, s);
	distance = 1000000000;
	
	Vector p = pi/180 * PlayerModel.Physics.Orientation;
	Vector v = -pi/180 * Viewpoint.Orientation;
	
	if (on_ground)
		MovementType = 2;
	if (GridX == -1)
		MovementType = 3;
	
	if (MovementType == 2 && on_ground)
	{
		if (Move.y > 0)
		{
			Move.x *= 500;
			Move.z *= 500;
		}
		
		PlayerModel.Physics.SumForces.Force += 1000*1.8333*(Move.z*RotateY(RotateZ(RotateZ(RotateY(Vector(0.0, 0.0, -1.0), v.y), v.z), p.z), p.y) +
													   Move.x*RotateY(RotateZ(RotateZ(RotateY(Vector(1.0, 0.0, 0.0), v.y), v.z), p.z), p.y) +
													   Move.y*RotateY(RotateZ(RotateZ(RotateY(Vector(0.0, 1.0, 0.0), v.y), v.z), p.z), p.y));
		Move.y = 0;
	}
	if (MovementType == 3)
	{
		PlayerModel.Physics.SumForces.Force += mult*(Move.z*RotateX(RotateY(RotateZ(RotateZ(RotateY(RotateX(Vector(0.0, 0.0, -1.0), v.x), v.y), v.z), p.z), p.y), p.x) +
											   Move.x*RotateX(RotateY(RotateZ(RotateZ(RotateY(RotateX(Vector(1.0, 0.0, 0.0), v.x), v.y), v.z), p.z), p.y), p.x));
	}
	
	Viewpoint.Render();
	
	GridX = -1;
	GridY = -1;
	Planet = -1;
	//System = -1;
	//Galaxy = -1;
}

void Player::Release()
{
	ReleaseMdl(&PlayerModel);
}
