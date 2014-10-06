/*
	GamePlayer.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 1/17/06.
  	Copyright 2006 Sol Union. All rights reserved.

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

#include "GamePlayer.h"

GamePlayer::GamePlayer()
{
	cx = 0;
	cz = 0;
}

void GamePlayer::Init(MeshData *Data)
{
	PlayerModel.Initialize(Data);
}

void GamePlayer::Prepare()
{
	PlayerModel.Update();
}

void GamePlayer::RenderCamera()
{
	PlayerModel.UpdatePos();
	Camera.translate = PlayerModel.Position*-1;
	Camera.translate.y -= PlayerModel.Mesh->Max.y + 5;
	PlayerModel.Orientation.y = -Camera.rotate.y;
	Camera.Render();
}

void GamePlayer::RenderModel()
{
	PlayerModel.Render(false);
}

void GamePlayer::MoveX(GLfloat x)
{
	cx = -x;
	PlayerModel.LinearVelocity.x = cx*cos(pi/180 * Camera.rotate.y) + cz*cos(pi/180 * (Camera.rotate.y-90));
	PlayerModel.LinearVelocity.z = cx*sin(pi/180 * Camera.rotate.y) + cz*sin(pi/180 * (Camera.rotate.y-90));
}

void GamePlayer::MoveZ(GLfloat z)
{
	cz = -z;
	PlayerModel.LinearVelocity.x = cx*cos(pi/180 * Camera.rotate.y) + cz*cos(pi/180 * (Camera.rotate.y-90));
	PlayerModel.LinearVelocity.z = cx*sin(pi/180 * Camera.rotate.y) + cz*sin(pi/180 * (Camera.rotate.y-90));
}
