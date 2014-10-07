/*
	Material.cpp
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

#include "Material.h"

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
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
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
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1);
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
	if (hasTexts)
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
	
	glNormalPointer(GL_FLOAT, 0, normals);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	List = glGenLists(1);
	glNewList(List, GL_COMPILE);
	
	glLockArraysEXT(0, NumVertices);
	
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	
	glUnlockArraysEXT();
	
	glEndList();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	if (hasDetail)
	{
		glActiveTexture(GL_TEXTURE1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	if (hasTexts)
	{
		glActiveTexture(GL_TEXTURE0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Material::Release()
{
	if (hasTexts)
		glDeleteTextures(1, &TextMap);
	if (hasDetail)
		glDeleteTextures(1, &DetailMap);
			
	glDeleteLists(List, 1);
}

