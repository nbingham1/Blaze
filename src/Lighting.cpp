/*
	Lighting.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/9/06.
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

#include "Lighting.h"

void ObjectEmissionLight::Set(GLfloat dx, GLfloat dy, GLfloat dz, GLfloat sx, GLfloat sy, GLfloat sz)
{
	Diffuse = Vector(dx, dy, dz);
	Specular = Vector(sx, sy, sz);
}

void ObjectEmissionLight::Enable()
{
	glEnable(GL_LIGHTING);
	GLfloat p[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat d[] = {Diffuse.x, Diffuse.y, Diffuse.z, 1.0};
	GLfloat s[] = {Specular.x, Specular.y, Specular.z, 1.0};
	int x = 0x4000;
	while (true)
	{
		if (!glIsEnabled(x))
		{
			glEnable(x);
			glLightfv(x, GL_POSITION, p);
			glLightfv(x, GL_DIFFUSE, d);
			glLightfv(x, GL_SPECULAR, s);
			glLightf(x, GL_CONSTANT_ATTENUATION, -1);
			glLightf(x, GL_LINEAR_ATTENUATION, -1);
			glLightf(x, GL_QUADRATIC_ATTENUATION, -1);
			lightnum = x;
			break;
		}
		x++;
	}
}

void ObjectEmissionLight::AttachLight(Vector *pos)
{
	DynamicPosition = pos;
}

void ObjectEmissionLight::Render()
{
	if (DynamicPosition != NULL)
	{
		GLfloat p[] = {DynamicPosition->x, DynamicPosition->y, DynamicPosition->z, 1.0};
		glLightfv(lightnum, GL_POSITION, p);
	}
	else
	{
		GLfloat p[] = {Position.x, Position.y, Position.z, 1.0};
		glLightfv(lightnum, GL_POSITION, p);
	}
}

void DirectionalLight::Set(GLfloat dx, GLfloat dy, GLfloat dz, GLfloat sx, GLfloat sy, GLfloat sz)
{
	Diffuse = Vector(dx, dy, dz);
	Specular = Vector(sx, sy, sz);
}

void DirectionalLight::Enable()
{
	glEnable(GL_LIGHTING);
	GLfloat p[] = {0.0, 0.0, 0.0, 0.0};
	GLfloat d[] = {Diffuse.x, Diffuse.y, Diffuse.z, 1.0};
	GLfloat s[] = {Specular.x, Specular.y, Specular.z, 1.0};
	int x = 0x4000;
	while (true)
	{
		if (!glIsEnabled(x))
		{
			glEnable(x);
			glLightfv(x, GL_POSITION, p);
			glLightfv(x, GL_DIFFUSE, d);
			glLightfv(x, GL_SPECULAR, s);
			glLightf(x, GL_CONSTANT_ATTENUATION, -1);
			glLightf(x, GL_LINEAR_ATTENUATION, -1);
			glLightf(x, GL_QUADRATIC_ATTENUATION, -1);
			lightnum = x;
			break;
		}
		x++;
	}
}

void DirectionalLight::AttachLight(Vector *pos)
{
	DynamicPosition = pos;
}

void DirectionalLight::Render()
{
	if (DynamicPosition != NULL)
	{
		GLfloat p[] = {DynamicPosition->x, DynamicPosition->y, DynamicPosition->z, 0.0};
		glLightfv(lightnum, GL_POSITION, p);
	}
	else
	{
		GLfloat p[] = {Position.x, Position.y, Position.z, 0.0};
		glLightfv(lightnum, GL_POSITION, p);
	}
}

