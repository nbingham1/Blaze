/*
 *  Camera.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/29/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "CoreMathematics.h"
#include "Model.h"

#ifndef Camera_h
#define Camera_h

struct Camera
{
	Camera()
	{
		viewscale = 1.0;
		movementtype = 3;
		move_mult = pow(2, 0);
	}

	Model *Host;
	Vector DistanceFromHost;
	GLdouble CameraDistance;
	bool	Control;
	GLdouble viewscale;
	int movementtype;
	GLdouble move_mult;

	Vector Position;
	Vector Orientation;

	void SetScale(GLdouble s);
	void SetMoveType(int m);

	void Move(Vector v);

	void Rotate(GLdouble x, GLdouble y, GLdouble z);
	void Rotate(Vector v);
	void Translate(GLdouble x, GLdouble y, GLdouble z);
	void AttachCamera(Model *HostAdress, Vector Dist, bool control);
	void DetachCamera();
	void Render();
};

#endif
