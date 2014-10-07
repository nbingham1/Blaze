/*
	Material.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/13/06.
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
#include "Texture.h"

#ifndef Material_h
#define Material_h

enum TextureDimension
{
	Second,
	Third
};

struct Material
{
	char Name[32];
	
	char TextName[255];
	char DetailName[255];
	
	char TextFileType[6];
	char DetailFileType[6];
	
	int TextureDepth;
	int DetailDepth;
	
	Vector Ambient;
	Vector Diffuse;
	Vector Specular;
	Vector Emission;
			
	GLfloat Reflect;
	GLfloat Opacity;
		
	GLuint TextMap;
	GLuint DetailMap;
	
	TextureDimension TextDimension;
	TextureDimension DetailDimension;
	
	GLfloat vertices[2250000];
	GLfloat texcoords0[2250000];
	GLfloat texcoords1[2250000];
	GLfloat normals[2250000];
	int NumVertices;
	
	bool hasTexts;
	bool TextLOD;
	bool hasDetail;
	bool DetailLOD;
	
	GLuint List;
	
	void RenderMaterial();
	void BindMaterial();
	void GenerateList();
	void Release();
	
	Material *Next;
};

#endif
