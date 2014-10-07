/*
	Camera.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/7/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
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

void Camera::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	Position.y += y;

    Position.x += x*cos(pi/180 * Orientation.y) + z*cos(pi/180 * (Orientation.y-90));
	Position.z += x*sin(pi/180 * Orientation.y) + z*sin(pi/180 * (Orientation.y-90));
}

void Camera::AttachCamera(Vector dist)
{
	DistanceFromCenter = dist;
}

void Camera::Render(Model *M)
{
	if (thirdperson)
		glTranslatef(0.0, 0.0, CameraDistance);
    glRotated(Orientation.x, 1, 0, 0);
    glRotated(Orientation.y, 0, 1, 0);
    glRotated(Orientation.z, 0, 0, 1);
	glTranslated(DistanceFromCenter.x, DistanceFromCenter.y, DistanceFromCenter.z);
	
	//glCallList(M->DisplayList);
	
	if (!thirdperson && M != NULL)
	{
		glRotated(M->Physics->Orientation.z, 0, 0, -1);
		glRotated(M->Physics->Orientation.y, 0, -1, 0);
		glRotated(M->Physics->Orientation.x, -1, 0, 0);
	}
	if (M != NULL)
		glTranslated(-M->Physics->Position.x, -M->Physics->Position.y, -M->Physics->Position.z);
	else
		glTranslated(Position.x, Position.y, Position.z);
}
