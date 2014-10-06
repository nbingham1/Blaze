/*
	GameInput.cpp
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
    
    float a = h - previous_mouse_x;
    float b = v - previous_mouse_y;
	
	float x, y, z;
	
    x = -b/5;
    y = a/5;
    z = 0;
	App.Player.Camera.Rotate(x, y, z);
	
	if (current_mouse_x >= 712)
	{
		current_mouse_x = 512;
		glutWarpPointer(current_mouse_x, current_mouse_y);
	}
	
	else if (current_mouse_x <= 312)
	{
		current_mouse_x = 512;
		glutWarpPointer(current_mouse_x, current_mouse_y);
	}
	
	if (current_mouse_y >= 584)
	{
		current_mouse_y = 384;
		glutWarpPointer(current_mouse_x, current_mouse_y);
	}
	
	else if (current_mouse_y <= 184)
	{
		current_mouse_y = 384;
		glutWarpPointer(current_mouse_x, current_mouse_y);
	}
	
	previous_mouse_x = current_mouse_x;
	previous_mouse_y = current_mouse_y;
    
    return true;
}

bool GameInput::HandleKeyDown(unsigned char key)
{
	KeyState[key] = true;
    
    if (key == 'w')
		App.Player.MoveZ(2);
    else if (key == 'a')
		App.Player.MoveX(-2);
    else if (key == 's')
		App.Player.MoveZ(-2);
    else if (key == 'd')
		App.Player.MoveX(2);
    else if (key == ' ')
		App.Player.PlayerModel.LinearVelocity.y = 2;
    
    return true;
}

void GameInput::HandleKeyStillDown()
{
    if (KeyState[(int)'w'])
		App.Player.MoveZ(2);
    if (KeyState[(int)'a'])
		App.Player.MoveX(-2);
    if (KeyState[(int)'s'])
		App.Player.MoveZ(-2);
    if (KeyState[(int)'d'])
		App.Player.MoveX(2);
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
