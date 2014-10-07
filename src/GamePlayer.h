/*
	GamePlayer.h
	Blaze Game Engine 0.02

	Created by Ned Bingham on 6/3/06.
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

#include "Main.h"
#include "GameCam.h"
#include "GameModel.h"

#ifndef GamePlayer_h
#define GamePlayer_h

struct GamePlayer
{
	GamePlayer();
	
	GameCam Camera;
	GameModel PlayerModel;
	
	GLfloat cx, cz;
	bool jumping;
	
	void Init(string filename);
	void Prepare();
	void RenderCamera();
	void Render();
	void MoveX(GLfloat x);
	void MoveZ(GLfloat z);
	void Release();
};

#endif
