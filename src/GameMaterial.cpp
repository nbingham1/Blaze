/*
	GameMaterial.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 7/20/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "GameMaterial.h"

void Material::RenderMaterial()
{
	GLfloat f[4];
	
	f[0] = Ambient.x;
	f[1] = Ambient.y;
	f[2] = Ambient.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_AMBIENT, f);
	f[0] = Diffuse.x;
	f[1] = Diffuse.y;
	f[2] = Diffuse.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, f);
	f[0] = Specular.x;
	f[1] = Specular.y;
	f[2] = Specular.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_SPECULAR, f);
	f[0] = Emission.x;
	f[1] = Emission.y;
	f[2] = Emission.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_EMISSION, f);
	glMaterialf(GL_FRONT, GL_SHININESS, Reflect);
	if (hasTexts)
	{
		glActiveTexture(GL_TEXTURE0);
		if (TextDimension == Second)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TextMap);
		}
		else if (TextDimension == Third)
		{
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, TextMap);
		}
		if (hasDetail)
		{
			glActiveTexture(GL_TEXTURE1);
			if (DetailDimension == Second)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, DetailMap);
			}
			else if (DetailDimension == Third)
			{
				glEnable(GL_TEXTURE_3D);
				glBindTexture(GL_TEXTURE_3D, DetailMap);
			}
		}
	}
	
	glCallList(List);
	
	if (hasDetail)
	{
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
	}
	if (hasTexts)
	{
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
	}
}

void Material::BindMaterial()
{
	GLfloat f[4];
	
	f[0] = Ambient.x;
	f[1] = Ambient.y;
	f[2] = Ambient.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_AMBIENT, f);
	f[0] = Diffuse.x;
	f[1] = Diffuse.y;
	f[2] = Diffuse.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, f);
	f[0] = Specular.x;
	f[1] = Specular.y;
	f[2] = Specular.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_SPECULAR, f);
	f[0] = Emission.x;
	f[1] = Emission.y;
	f[2] = Emission.z;
	f[3] = Opacity;
	glMaterialfv(GL_FRONT, GL_EMISSION, f);
	glMaterialf(GL_FRONT, GL_SHININESS, Reflect);
	if (hasTexts)
	{
		glActiveTexture(GL_TEXTURE0);
		if (TextDimension == Second)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TextMap);
		}
		else if (TextDimension == Third)
		{
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, TextMap);
		}
		if (hasDetail)
		{
			glActiveTexture(GL_TEXTURE1);
			if (DetailDimension == Second)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, DetailMap);
			}
			else if (DetailDimension == Third)
			{
				glEnable(GL_TEXTURE_3D);
				glBindTexture(GL_TEXTURE_3D, DetailMap);
			}
		}
	}
}

void Material::GenerateList()
{
	for (int x = 1; x < NumVertices*3; x+=3)
	{
		depth[(x-1)/3] = 20*(-18000-vertices[x]);
		if ((-18000-vertices[x]) < 0)
			depth[(x-1)/3] = 0;
	}
	
	if (hasTexts || hasFur)
	{
		glActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, texcoords0);
	}
	if (hasDetail)
	{
		glActiveTexture(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, texcoords1);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_FOG_COORDINATE_ARRAY);
	
	glNormalPointer(GL_FLOAT, 0, normals);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glFogCoordPointer(GL_FLOAT, 0, depth);
	
	List = glGenLists(1);
	glNewList(List, GL_COMPILE);
	
	glLockArraysEXT(0, NumVertices);
	
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	
	glUnlockArraysEXT();
	
	glEndList();
	
	glDisableClientState(GL_FOG_COORDINATE_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	if (hasDetail)
	{
		glActiveTexture(GL_TEXTURE1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	if (hasTexts || hasFur)
	{
		glActiveTexture(GL_TEXTURE0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Material::GenerateFurTextures(int Depth, int Size, int Density, Vector Color)
{
	hasFur = true;
	FurDepth = Depth;
	rgba_t Datargba[sqr(Size)];
	int r = 0;
	for (int x = 0; x < sqr(Size); x++)
	{
		Datargba[x].a = 0;
		Datargba[x].r = 0;
		Datargba[x].g = 0;
		Datargba[x].b = 0;
	}
	
	glGenTextures(Depth, FurMap);
	
	for (int x = Depth-1; x >= 0; x--)
	{
		for (int y = 0; y < Density; y++)
		{
			r = rand() % (sqr(Size));
			Datargba[r].a = 255;
			Datargba[r].r = rand() % ((int)Color.x % 256);
			Datargba[r].g = rand() % ((int)Color.y % 256);
			Datargba[r].b = rand() % ((int)Color.z % 256);
		}
		
		glBindTexture(GL_TEXTURE_2D, FurMap[x]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, Size, Size, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)Datargba);
	}
}

void Material::Release()
{
	if (hasTexts)
		glDeleteTextures(1, &TextMap);
	if (hasDetail)
		glDeleteTextures(1, &DetailMap);
	if (hasFur)
		glDeleteTextures(FurDepth, FurMap);
			
	glDeleteLists(List, 1);
}
