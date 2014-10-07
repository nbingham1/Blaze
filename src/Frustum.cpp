/*
	Frustm.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/8/06.
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

#include "Frustum.h"

void Frustum::SetFrustum(GLfloat right, GLfloat left, GLfloat front, GLfloat back, GLfloat top, GLfloat bottom)
{
	Right = right;
	Left = left;
	Front = front;
	Back = back;
	Top = top;
	Bottom = bottom;
}

void Frustum::RenderFrustum()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(Left, Right, Bottom, Top, Front, Back);
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
}

