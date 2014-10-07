/*
 *  Keyboard.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 10/6/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Keyboard_h
#define Keyboard_h

struct Keyboard
{
	Keyboard()
	{
		for (int x = 0; x < 256; x++)
		{
			KeyState[x] = false;
		}
	}
	
	bool KeyState[256];
	
	bool HandleKeyDown(unsigned char key);				// handles key down events
	bool HandleKeyUp(unsigned char key);				// handles key up events
	void	 HandleKeyStillDown();						// handles key held down events
};

#endif
