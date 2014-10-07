/*
 *  Mouse.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 10/6/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Mouse_h
#define Mouse_h

struct Mouse
{
	int current_mouse_x;
	int current_mouse_y;
	
	bool HandleMouseMoved(int x, int y);					// handles mouse movement
	bool HandleMouseDown(int button, int x, int y);		// handles a mouse click
	bool HandleMouseUp(int botton, int x, int y);			// handles a mouse release
};

#endif
