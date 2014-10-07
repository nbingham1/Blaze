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
	}
	
	Model *Host;
	Vector DistanceFromHost;
	GLfloat CameraDistance;	
	bool	Control;
	
	Vector Position;
	Vector Orientation;
	
	void Rotate(GLfloat x, GLfloat y, GLfloat z);
	void Rotate(Vector v);
	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void AttachCamera(Model *HostAdress, Vector Dist, bool control);
	void DetachCamera();
	void Render();
};

#endif
