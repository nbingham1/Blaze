/*
	GameCam.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 8/8/05.
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

#include "GameCam.h"

GameCam::GameCam()
{
}

GameCam::~GameCam()
{
}

void GameCam::Rotate(float x, float y, float z)
{
    rotate.x += x;
	rotate.y += y;
	rotate.z += z;
	if (rotate.x >= 90)
		rotate.x = 90;
	
	else if (rotate.x <= -80)
		rotate.x = -80;
}

void GameCam::Translate(float x, float y, float z)
{
	translate.y += y;

    translate.x += x*cos(pi/180 * rotate.y) + z*cos(pi/180 * (rotate.y-90));
	translate.z += x*sin(pi/180 * rotate.y) + z*sin(pi/180 * (rotate.y-90));
}

void GameCam::Render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1.0, 1.0, 2.0, 1000000000);
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
    glRotated(rotate.x, 1, 0, 0);
    glRotated(rotate.y, 0, 1, 0);
    glRotated(rotate.z, 0, 0, 1);
    glTranslated(translate.x, translate.y, translate.z);
    glMatrixMode(GL_MODELVIEW);
}
