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
#include "Main.h"

#ifndef GameInput_h
#define GameInput_h

class GameInput
{
	public:
		GameInput();
        ~GameInput();
	
		bool KeyState[256];
		int previous_mouse_x, previous_mouse_y;
		int current_mouse_x, current_mouse_y;

        bool HandleMouseDown(int button, int state, int x, int y);
        bool HandleMouseMoved(int h, int v);
        bool HandleKeyDown(unsigned char key);
		void HandleKeyStillDown();
        bool HandleKeyUp(unsigned char key);
};

#endif
