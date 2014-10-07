/*
	Mouse.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/6/06.
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

#include "Mouse.h"
#include "CoreGraphics.h"

extern CoreGraphics Renderer;

bool Mouse::HandleMouseDown(int button, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
    	break;
    case GLUT_RIGHT_BUTTON:
    	break;
    case GLUT_MIDDLE_BUTTON:
    	break;
    }

    return true;
}

bool Mouse::HandleMouseUp(int button, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
    	break;
    case GLUT_RIGHT_BUTTON:
    	break;
    case GLUT_MIDDLE_BUTTON:
    	break;
    }

    return true;
}

bool Mouse::HandleMouseMoved(int x, int y)
{
	Renderer.TheCamera.Rotate(-GLfloat(y - current_mouse_y)/5, GLfloat(x - current_mouse_x)/5, 0.0); // rotates the camera depending on the mouse movement

	current_mouse_x = x;
	current_mouse_y = y;
	
	if (current_mouse_x > 1200 || current_mouse_x < 200 || current_mouse_y > 700 || current_mouse_y < 200)
	{
		glutWarpPointer(700, 450);
		current_mouse_x = 700;
		current_mouse_y = 450;
	}
	
    return true;
}
