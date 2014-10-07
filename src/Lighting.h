/*
	Lighting.h
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

#include "CoreMathematics.h"
#include "BasicDefinitions.h"

#ifndef Lighting_h
#define Lighting_h

struct ObjectEmissionLight
{
	ObjectEmissionLight()
	{
		DynamicPosition = NULL;
	}
	
	Vector *DynamicPosition;
	Vector Position;
	Vector Diffuse;
	Vector Specular;
	int lightnum;
	
	void Set(GLfloat dx, GLfloat dy, GLfloat dz, GLfloat sx, GLfloat sy, GLfloat sz);
	void Enable();
	void AttachLight(Vector *pos);
	void Render();
};

struct DirectionalLight
{
	DirectionalLight()
	{
		DynamicPosition = NULL;
	}
	
	Vector *DynamicPosition;
	Vector Position;
	Vector Diffuse;
	Vector Specular;
	int lightnum;
	
	void Set(GLfloat dx, GLfloat dy, GLfloat dz, GLfloat sx, GLfloat sy, GLfloat sz);
	void Enable();
	void AttachLight(Vector *pos);
	void Render();
};

#endif
