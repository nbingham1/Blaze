/*
 *  Camera.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 1/6/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Camera.h"

void Camera::SetScale(GLdouble s)
{
	viewscale = s;
}

void Camera::SetMoveType(int m)
{
	movementtype = m;
}

void Camera::Move(Vector v)
{
	Vector p = pi/180 * Host->Physics.Orientation;
	Vector v1 = -pi/180 * Orientation;
	if (Host)
	{
		if (movementtype == 2)
			Host->Physics.SumForces += move_mult*(v.z*RotateY(RotateZ(RotateZ(RotateY(Vector(0.0, 0.0, -1.0), v1.y), v1.z), p.z), p.y) + 
										v.x*RotateY(RotateZ(RotateZ(RotateY(Vector(1.0, 0.0, 0.0), v1.y), v1.z), p.z), p.y) + 
										v.y*RotateY(RotateZ(RotateZ(RotateY(Vector(0.0, 1.0, 0.0), v1.y), v1.z), p.z), p.y));
		else if (movementtype == 3)
			Host->Physics.SumForces += move_mult*(v.z*RotateX(RotateY(RotateZ(RotateZ(RotateY(RotateX(Vector(0.0, 0.0, -1.0), v1.x), v1.y), v1.z), p.z), p.y), p.x) + 
										v.x*RotateX(RotateY(RotateZ(RotateZ(RotateY(RotateX(Vector(1.0, 0.0, 0.0), v1.x), v1.y), v1.z), p.z), p.y), p.x));
	}
	else
		Translate(move_mult*v.x, move_mult*v.y, move_mult*v.z);
}

void Camera::Rotate(GLdouble x, GLdouble y, GLdouble z)
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

void Camera::Translate(GLdouble x, GLdouble y, GLdouble z)
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
	glScalef(viewscale, viewscale, viewscale);
	glTranslatef(0.0, 0.0, -CameraDistance);
    glRotated(Orientation.x, 1, 0, 0);
    glRotated(Orientation.y, 0, 1, 0);
    glRotated(Orientation.z, 0, 0, 1);
	glTranslated(-DistanceFromHost.x, -DistanceFromHost.y, -DistanceFromHost.z);
	
	glScalef(.1, .1, .1);
	if (Host != NULL)
		RenderMdl(Host);
	glScalef(10.0, 10.0, 10.0);
	
	if (Host != NULL)
	{
		glRotated(Host->Physics.Orientation.z, 0, 0, -1);
		glRotated(Host->Physics.Orientation.y, 0, -1, 0);
		glRotated(Host->Physics.Orientation.x, -1, 0, 0);
	}
	//if (Host != NULL)
	//	glTranslated(-Host->Physics.Position.x, -Host->Physics.Position.y, -Host->Physics.Position.z);
	if (Host == NULL)
		glTranslated(Position.x, Position.y, Position.z);
	viewscale = 0.0;
}
