/*
 *  Material.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/2/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Material.h"

void Material::Initialize()
{
	Name[0] = '\0';
	TextName[0] = '\0';
	DetailName[0] = '\0';
	Shininess = 0.0;
	Opacity = 0.0;
	
	TextDimension = 2;
	DetailDimension = 2;
	DetailScale = 2;
	
	Next = NULL;
}

void Material::Enable(GLdouble *texts)
{
	GLfloat f[4];
	f[3] = Opacity;
	
	f[0] = Ambient.x;
	f[1] = Ambient.y;
	f[2] = Ambient.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, f);
	f[0] = Diffuse.x;
	f[1] = Diffuse.y;
	f[2] = Diffuse.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, f);
	f[0] = Specular.x;
	f[1] = Specular.y;
	f[2] = Specular.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f);
	f[0] = Emission.x;
	f[1] = Emission.y;
	f[2] = Emission.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, f);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_DOUBLE, 0, texts);
	glClientActiveTexture(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_DOUBLE, 0, texts);
	glClientActiveTexture(GL_TEXTURE0);
	
	if (TextName[0] != '\0')
	{
		glActiveTexture(GL_TEXTURE0);
		if (TextDimension == 2)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TextMap);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, TextMap);
		}
		
		if (DetailName[0] != '\0')
		{
			glActiveTexture(GL_TEXTURE1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glMatrixMode(GL_TEXTURE);
			glScalef(DetailScale, DetailScale, 1.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, DetailMap);
		}
	}
}

void Material::Enable()
{
	GLfloat f[4];
	f[3] = Opacity;
	
	f[0] = Ambient.x;
	f[1] = Ambient.y;
	f[2] = Ambient.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, f);
	f[0] = Diffuse.x;
	f[1] = Diffuse.y;
	f[2] = Diffuse.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, f);
	f[0] = Specular.x;
	f[1] = Specular.y;
	f[2] = Specular.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f);
	f[0] = Emission.x;
	f[1] = Emission.y;
	f[2] = Emission.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, f);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
	
	if (TextName[0] != '\0')
	{
		glActiveTexture(GL_TEXTURE0);
		if (TextDimension == 2)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TextMap);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, TextMap);
		}
		
		if (DetailName[0] != '\0')
		{
			glActiveTexture(GL_TEXTURE1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, DetailScale);
			glMatrixMode(GL_TEXTURE);
			glScaled(DetailScale, DetailScale, 1.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, DetailMap);
		}
	}
}

void Material::Disable()
{
	if (DetailName[0] != '\0')
	{
		glActiveTexture(GL_TEXTURE1);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glDisable(GL_TEXTURE_2D);
	}
	if (TextName[0] != '\0')
	{
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
	}
}

void Material::Release()
{
	glDeleteTextures(1, &TextMap);
	glDeleteTextures(1, &DetailMap);
}