/*
	GameInput.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 8/8/05.
	Copyright 2005 Sol Union. All rights reserved.

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

#include "GameInput.h"
#include "GameApp.h"

extern GameApp App;

GameInput::GameInput()
{
    for (int x = 0; x < 103; x++)
    {
        KeyState[x] = false;
    }
    
    previous_mouse_x = 0;
    previous_mouse_y = 0;
}

GameInput::~GameInput()
{
}

bool GameInput::HandleMouseDown(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
		}
		else
			return false;
	}
	else if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

bool GameInput::HandleMouseMoved(int h, int v)
{
    current_mouse_x = h;
    current_mouse_y = v;

    if (!warp)
    {
		float a = h - previous_mouse_x;
		float b = v - previous_mouse_y;

		float x, y, z;

		x = -b/5;
		y = a/5;
		z = 0;
		App.Player.Camera.Rotate(x, y, z);

		previous_mouse_x = current_mouse_x;
		previous_mouse_y = current_mouse_y;

		if (current_mouse_x >= 1300)
		{
			current_mouse_x = 700;
			glutWarpPointer(current_mouse_x, current_mouse_y);
			warp = true;
		}

		else if (current_mouse_x <= 100)
		{
			current_mouse_x = 700;
			glutWarpPointer(current_mouse_x, current_mouse_y);
			warp = true;
		}
	
		if (current_mouse_y >= 800)
		{
			current_mouse_y = 450;
			glutWarpPointer(current_mouse_x, current_mouse_y);
			warp = true;
		}

		else if (current_mouse_y <= 100)
		{
			current_mouse_y = 450;
			glutWarpPointer(current_mouse_x, current_mouse_y);
			warp = true;
		}
    }
    else if (current_mouse_y > 100 && current_mouse_y < 800 && current_mouse_x > 100 && current_mouse_x < 1300)
	{
    	previous_mouse_x = current_mouse_x;
    	previous_mouse_y = current_mouse_y;
    	warp = false;
	}

    return true;
}

bool GameInput::HandleKeyDown(unsigned char key)
{
	KeyState[key] = true;
    
	if (key == '1')
		App.Particles.Rain(500, App.Environment.Map.Geometry->Min, App.Environment.Map.Geometry->Max);
	else if (key == '2')
		App.Particles.Snow(3, App.Environment.Map.Geometry->Min, App.Environment.Map.Geometry->Max);
	else if (key == '3')
		App.Particles.ExplodeFire(1000, Vector(0, 0, 0));
	else if (key == '4')
		App.Particles.ExplodeDirt(500, Vector(0, 0, 0));
	else if (key == '5')
		App.Particles.ExplodeGrass(500, Vector(0, 0, 0));
	else if (key == '6')
		App.Particles.Spark(1, Vector(0, 0, 0));
	else if (key == '7')
		App.Particles.Emitters[6].Emit(10000, App.Environment.Map.Geometry->Min + Vector(5000.0, 0.0, 5000.0), App.Environment.Map.Geometry->Max - Vector(5000.0, 0000.0, 5000.0), Vector(0.0, 0.0, 0.0), 1.0);
	else if (key == 'w')
		App.Player.MoveZ(-.2);
    else if (key == 'a')
		App.Player.MoveX(.2);
    else if (key == 's')
		App.Player.MoveZ(.2);
    else if (key == 'd')
		App.Player.MoveX(-.2);
    else if (key == ' ')
    	App.Player.jumping = true;
    
    return true;
}

void GameInput::HandleKeyStillDown()
{
    if (KeyState[(int)'w'])
		App.Player.MoveZ(-.2);
    if (KeyState[(int)'a'])
		App.Player.MoveX(.2);
    if (KeyState[(int)'s'])
		App.Player.MoveZ(.2);
    if (KeyState[(int)'d'])
		App.Player.MoveX(-.2);
}

bool GameInput::HandleKeyUp(unsigned char key)
{
	KeyState[key] = false;

    if (key == 'w')
		App.Player.MoveZ(0);
    else if (key == 'a')
		App.Player.MoveX(0);
    else if (key == 's')
		App.Player.MoveZ(0);
    else if (key == 'd')
		App.Player.MoveX(0);
    
    return true;
}
