/*
 *  Mouse.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 10/6/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
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
	Renderer.view.Rotate(double(y - current_mouse_y)/5, double(x - current_mouse_x)/5, 0.0);

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
