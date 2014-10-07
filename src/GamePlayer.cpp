/*
	GamePlayer.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 6/03/06.
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

#include "GamePlayer.h"

GamePlayer::GamePlayer()
{
	cx = 0;
	cz = 0;
}

void GamePlayer::Init(string filename)
{
	LoadObj(&PlayerModel, filename, 1, 1, 1, 1);
	//LoadWRL(&PlayerModel, filename, 1, 1, 1, 1);
	LoadMats(&PlayerModel, "Player/Player.mtl");
	LoadPhysics(&PlayerModel, "Player/Player.phs");
	GenNormals(&PlayerModel);
	GenDisplayLists(&PlayerModel);
	SetCollisionGroundFunction(&PlayerModel, &AnimateObjectGroundCollision);
}

void GamePlayer::Prepare()
{
	if (jumping && PlayerModel.collidewithground)
	{
		PlayerModel.Physics.LinearVelocity.y += 10;
		PlayerModel.Physics.Position.y += 2;
		PlayerModel.collidewithground = false;
		if (jumping)
		{
			PlayerModel.Physics.LinearVelocity.x *= 2;
			PlayerModel.Physics.LinearVelocity.z *= 2;
		}
	}
	Update(&PlayerModel, .0013);
	if (PlayerModel.collidewithground)
	{
		PlayerModel.Physics.LinearVelocity.x = (cx*cos(pi/180 * Camera.rotate.y) + cz*cos(pi/180 * (Camera.rotate.y-90)));
		PlayerModel.Physics.LinearVelocity.z = (cx*sin(pi/180 * Camera.rotate.y) + cz*sin(pi/180 * (Camera.rotate.y-90)));
	}
	jumping = false;
}

void GamePlayer::RenderCamera()
{
	//glTranslatef(0.0, 0.0, -20.0);
	Camera.Render();
	glTranslatef(0.0, -(PlayerModel.Physics.BoundingBox.v[6].y+3), 0.0);
	glRotatef(PlayerModel.Physics.Orientation.z, 0.0, 0.0, -1.0);
	glRotatef(PlayerModel.Physics.Orientation.y, 0.0, -1.0, 0.0);
	glRotatef(PlayerModel.Physics.Orientation.x, -1.0, 0.0, 0.0);
	glTranslatef(-PlayerModel.Physics.Position.x, -PlayerModel.Physics.Position.y, -PlayerModel.Physics.Position.z);
}

void GamePlayer::Render()
{
	RenderModel(&PlayerModel, true);
}

void GamePlayer::MoveX(GLfloat x)
{
	cx = x*10;
}

void GamePlayer::MoveZ(GLfloat z)
{
	cz = z*10;
}

void GamePlayer::Release()
{
	ReleaseModel(&PlayerModel);
}
