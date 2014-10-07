/*
 *  Camera.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 1/6/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Camera.h"

void Camera::Rotate(GLfloat x, GLfloat y, GLfloat z)
{
	Orientation.x += x;
	Orientation.y += y;
	Orientation.z += z;
	if (Orientation.x >= 90)
		Orientation.x = 90;
	
	else if (Orientation.x <= -80)
		Orientation.x = -80;
}

void Camera::Rotate(Vector v)
{
	Orientation.x += v.x;
	Orientation.y += v.y;
	Orientation.z += v.z;
	if (Orientation.x >= 90)
		Orientation.x = 90;
	
	else if (Orientation.x <= -80)
		Orientation.x = -80;
}

void Camera::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	Position.y += y;

    Position.x += x*cos(pi/180 * Orientation.y) + z*cos(pi/180 * (Orientation.y-90));
	Position.z += x*sin(pi/180 * Orientation.y) + z*sin(pi/180 * (Orientation.y-90));
	
	if (Host != NULL && Control)
		Host->Move(x*cos(pi/180 * Orientation.y) + z*cos(pi/180 * (Orientation.y-90)), y, x*sin(pi/180 * Orientation.y) + z*sin(pi/180 * (Orientation.y-90)));
}

void Camera::AttachCamera(Model *HostAdress, Vector Dist, bool control)
{
	Control = control;
	Host = HostAdress;
	DistanceFromHost = Dist;
	CameraDistance = 0.0;
}

void Camera::DetachCamera()
{
	Host = NULL;
	Control = false;
	DistanceFromHost = Vector();
	CameraDistance = 0.0;
}

void Camera::Render()
{
	glTranslatef(0.0, 0.0, CameraDistance);
    glRotated(Orientation.x, 1, 0, 0);
    glRotated(Orientation.y, 0, 1, 0);
    glRotated(Orientation.z, 0, 0, 1);
	glTranslated(DistanceFromHost.x, DistanceFromHost.y, DistanceFromHost.z);
	
	glScalef(.125, .125, .125);
	RenderMdl(Host);
	glScalef(8.0, 8.0, 8.0);
	
	if (CameraDistance == 0.0 && Host != NULL)
	{
		glRotated(Host->Physics.Orientation.z, 0, 0, -1);
		glRotated(Host->Physics.Orientation.y, 0, -1, 0);
		glRotated(Host->Physics.Orientation.x, -1, 0, 0);
	}
	if (Host != NULL)
		glTranslated(-Host->Physics.Position.x, -Host->Physics.Position.y, -Host->Physics.Position.z);
	else
		glTranslated(Position.x, Position.y, Position.z);
	
}
