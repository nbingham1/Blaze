/*
 * camera.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on December 7, 2011.
 * Modified by Ned Bingham on December 7, 2011.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "camera.h"
#include "../graphics.h"

camerahdl::camerahdl()
{
	scale = vec4f(1.0, 1.0, 1.0, 1.0);
}

camerahdl::~camerahdl()
{
}

void camerahdl::setup(frustum3f f)
{
	frustum = f;
}

void camerahdl::projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(frustum[0][0], frustum[0][1], frustum[1][0], frustum[1][1], frustum[2][0], frustum[2][1]);
	glMatrixMode(GL_MODELVIEW);
}

void camerahdl::ortho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(frustum[0][0], frustum[0][1], frustum[1][0], frustum[1][1], frustum[2][0], frustum[2][1]);
	glMatrixMode(GL_MODELVIEW);
}

void camerahdl::frame()
{
	glLoadIdentity();
	glScalef(scale[0], scale[1], scale[2]);
	glRotated(-orientation[0], 1.0, 0.0, 0.0);
	glRotated(-orientation[1], 0.0, 1.0, 0.0);
	glRotated(-orientation[2], 0.0, 0.0, 1.0);
	glTranslatef(-position[0], -position[1], -position[2]);
}

void camerahdl::render(double t)
{
	vec4f p = position;
	vec4f o = orientation;
	vec4f s = scale;

	glPushMatrix();
	glTranslatef(p[0], p[1], p[2]);
	glRotated(o[2], 0.0, 0.0, 1.0);
	glRotated(o[1], 0.0, 1.0, 0.0);
	glRotated(o[0], 1.0, 0.0, 0.0);
	glScalef(s[0], s[1], s[2]);
	glutSolidSphere(1.0, 8, 8);
	glScalef((frustum[0][1]-frustum[0][0])*2.5, (frustum[1][1]-frustum[1][0])*2.5, frustum[2][0]*2.5);
	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	glutWireCone(1.0, 1.0, 4, 1);
	glPopMatrix();
}
