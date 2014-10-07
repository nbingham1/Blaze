/*
	Keyboard.cpp
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

#include "Keyboard.h"
#include "CoreGraphics.h"

extern CoreGraphics Renderer;

bool Keyboard::HandleKeyDown(unsigned char key)
{
    if (key == 'w')
    {
        KeyState[(int)'w'] = true;
		Renderer.ThePlayer.MoveForward(1.0);
    }
    
    if (key == 't')
    {
        KeyState[(int)'t'] = true;
		Renderer.TheCamera.Rotate(1.0, 0.0, 0.0);
    }
    
    if (key == 'a')
    {
        KeyState[(int)'a'] = true;
		Renderer.ThePlayer.MoveSideways(-1.0);
    }
    
    if (key == 's')
    {
        KeyState[(int)'s'] = true;
		Renderer.ThePlayer.MoveForward(-1.0);
    }
    
    if (key == 'd')
    {
        KeyState[(int)'d'] = true;
		Renderer.ThePlayer.MoveSideways(1.0);
    }
    
    if (key == 'f')
    {
        KeyState[(int)'f'] = true;
		Renderer.TheCamera.Rotate(0.0, -1.0, 0.0);
    }
    
    if (key == 'g')
    {
        KeyState[(int)'g'] = true;
		Renderer.TheCamera.Rotate(-1.0, 0.0, 0.0);
    }
    
    if (key == 'h')
    {
        KeyState[(int)'h'] = true;
		Renderer.TheCamera.Rotate(0.0, 1.0, 0.0);
    }
    
    return true;
}

void Keyboard::HandleKeyStillDown()
{
    if (KeyState[(int)'w'])
    {
		Renderer.ThePlayer.MoveForward(1.0);
    }
    
    if (KeyState[(int)'t'])
    {
		Renderer.TheCamera.Rotate(1.0, 0.0, 0.0);
    }
    
    if (KeyState[(int)'a'])
    {
		Renderer.ThePlayer.MoveSideways(-1.0);
    }
    
    if (KeyState[(int)'s'])
    {
		Renderer.ThePlayer.MoveForward(-1.0);
    }
    
    if (KeyState[(int)'d'])
    {
		Renderer.ThePlayer.MoveSideways(1.0);
    }
    
    if (KeyState[(int)'f'])
    {
		Renderer.TheCamera.Rotate(0.0, -1.0, 0.0);
    }
    
    if (KeyState[(int)'g'])
    {
		Renderer.TheCamera.Rotate(-1.0, 0.0, 0.0);
    }
    
    if (KeyState[(int)'h'])
    {
		Renderer.TheCamera.Rotate(0.0, 1.0, 0.0);
    }
}

bool Keyboard::HandleKeyUp(unsigned char key)
{
    if (key == 'w')
    {
        KeyState[(int)'w'] = false;
		Renderer.ThePlayer.MoveForward(0.0);
    }
    
    if (key == 'a')
    {
        KeyState[(int)'a'] = false;
		Renderer.ThePlayer.MoveSideways(0.0);
    }
    
    if (key == 's')
    {
        KeyState[(int)'s'] = false;
		Renderer.ThePlayer.MoveForward(0.0);
    }
    
    if (key == 'd')
    {
        KeyState[(int)'d'] = false;
		Renderer.ThePlayer.MoveSideways(0.0);
    }

    return true;
}
