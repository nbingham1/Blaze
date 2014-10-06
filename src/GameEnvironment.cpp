/*
	GameEnvironment.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 12/26/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameEnvironment.h"
#include "GameMisc.h"

Skybox::Skybox()
{
}

void Skybox::Init(string filename, string filetype, GLfloat size)
{
	Size = size;
	Vector v1(size, size, size);
	Vector v2(-size, size, size);
	Vector v3(-size, -size, size);
	Vector v4(size, -size, size);
	Vector v5(size, size, -size);
	Vector v6(-size, size, -size);
	Vector v7(-size, -size, -size);
	Vector v8(size, -size, -size);
	
	string file = filename + "_rt." + filetype;
	
	rt = LoadTexture(string_to_char(file), GL_FALSE, GL_RGBA, GL_RGBA, 1);
	file = filename + "_lt." + filetype;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	lt = LoadTexture(string_to_char(file), GL_FALSE, GL_RGBA, GL_RGBA, 1);
	file = filename + "_ft." + filetype;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	ft = LoadTexture(string_to_char(file), GL_FALSE, GL_RGBA, GL_RGBA, 1);
	file = filename + "_bk." + filetype;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	bk = LoadTexture(string_to_char(file), GL_FALSE, GL_RGBA, GL_RGBA, 1);
	file = filename + "_up." + filetype;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	up = LoadTexture(string_to_char(file), GL_FALSE, GL_RGBA, GL_RGBA, 1);
	file = filename + "_dn." + filetype;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	dn = LoadTexture(string_to_char(file), GL_FALSE, GL_RGBA, GL_RGBA, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	DispList = glGenLists(1);
	glNewList(DispList, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, rt.Image);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(1, 1);
		glVertex3f(v5.x, v5.y, v5.z);
		glTexCoord2f(1, 0);
		glVertex3f(v8.x, v8.y, v8.z);
		glTexCoord2f(0, 0);
		glVertex3f(v4.x, v4.y, v4.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, lt.Image);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v6.x, v6.y, v6.z);
		glTexCoord2f(1, 1);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(1, 0);
		glVertex3f(v3.x, v3.y, v3.z);
		glTexCoord2f(0, 0);
		glVertex3f(v7.x, v7.y, v7.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, bk.Image);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v5.x, v5.y, v5.z);
		glTexCoord2f(1, 1);
		glVertex3f(v6.x, v6.y, v6.z);
		glTexCoord2f(1, 0);
		glVertex3f(v7.x, v7.y, v7.z);
		glTexCoord2f(0, 0);
		glVertex3f(v8.x, v8.y, v8.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, ft.Image);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(1, 1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(1, 0);
		glVertex3f(v4.x, v4.y, v4.z);
		glTexCoord2f(0, 0);
		glVertex3f(v3.x, v3.y, v3.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, up.Image);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(1, 1);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(1, 0);
		glVertex3f(v6.x, v6.y, v6.z);
		glTexCoord2f(0, 0);
		glVertex3f(v5.x, v5.y, v5.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, dn.Image);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v3.x, v3.y, v3.z);
		glTexCoord2f(1, 1);
		glVertex3f(v4.x, v4.y, v4.z);
		glTexCoord2f(1, 0);
		glVertex3f(v8.x, v8.y, v8.z);
		glTexCoord2f(0, 0);
		glVertex3f(v7.x, v7.y, v7.z);
	glEnd();
	glEndList();
}

void Skybox::Render()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslated(-Position.x, -Position.y, -Position.z);
	glMatrixMode(GL_MODELVIEW);
	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glCallList(DispList);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Skybox::Release()
{
	glDeleteLists(DispList, 1);
	glDeleteTextures(1, &rt.Image);
	glDeleteTextures(1, &lt.Image);
	glDeleteTextures(1, &ft.Image);
	glDeleteTextures(1, &bk.Image);
	glDeleteTextures(1, &up.Image);
	glDeleteTextures(1, &dn.Image);
}

GameEnvironment::GameEnvironment()
{
}
	
void GameEnvironment::Init(MeshData *Mesh, string filename, string filetype, GLfloat size)
{
	Map.Initialize(Mesh);
	Background.Init(filename, filetype, size);
}

void GameEnvironment::Render()
{
	Background.Render();
	Map.Render(true);
}

void GameEnvironment::Release()
{
	Background.Release();
}
