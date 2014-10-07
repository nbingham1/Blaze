/*
	Player.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/11/06.
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

#include "Player.h"

void Player::Init()
{
	LoadObj(&PlayerModel, "res/Player/Player.obj", .000125, .000125, .000125, 1);
	GenNormals(&PlayerModel);
	GenDisplayLists(&PlayerModel);
	SetCollisionGroundFunction(&PlayerModel, &AnimateObjectGroundCollision);
}

void Player::Prepare(Camera *TheCam)
{
	//if (PlayerModel.Physics->Grounded)
	//{
	//	PlayerModel.Physics->LinearVelocity.x += MoveX*cos(pi/180 * TheCam->Orientation.y) + MoveZ*cos(pi/180 * (TheCam->Orientation.y-90));
	//	PlayerModel.Physics->LinearVelocity.z += MoveX*sin(pi/180 * TheCam->Orientation.y) + MoveZ*sin(pi/180 * (TheCam->Orientation.y-90));
	//}
	if (!PlayerModel.Physics->Grounded)
	{
		PlayerModel.Physics->LinearVelocity += 1*MoveZ*Vector(cos(pi/180 * (TheCam->Orientation.y-90))*cos(pi/180 * TheCam->Orientation.x),
														   -sin(pi/180 * TheCam->Orientation.x),
														   sin(pi/180 * (TheCam->Orientation.y-90))*cos(pi/180 * TheCam->Orientation.x));
	}
}

void Player::MoveForward(GLfloat z)
{
	MoveZ = -z*.001;
}

void Player::MoveSideways(GLfloat x)
{
	MoveX = -x*.001;
}

void Player::Render()
{
	RenderModel(&PlayerModel);
}

void Player::Release()
{
	ReleaseModel(&PlayerModel);
}
