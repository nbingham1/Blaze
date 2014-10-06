/*
	GameEnvironment.h
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

#include "Main.h"
#include "GameMatrix.h"
#include "GameTexture.h"
#include "GameModel.h"

#ifndef GameEnvironment_h
#define GameEnvironment_h

struct Skybox
{
	Skybox();
	
	GLfloat Size;
	Vector Position;
	
	GLuint DispList;
	texture_t rt;
	texture_t lt;
	texture_t ft;
	texture_t bk;
	texture_t up;
	texture_t dn;
	
	void Init(string filename, string filetype, GLfloat size);
	void Render();
	void Release();
};

struct GameEnvironment
{
	GameEnvironment();
	GLfloat Size;
	
	Skybox Background;
	Model  Map;
	
	void Init(MeshData *Mesh, string filename, string filetype, GLfloat size);
	void Render();
	void Release();
};

#endif
