/*
 *  Keyboard.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 10/6/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Keyboard.h"
#include "CoreGraphics.h"

extern CoreGraphics Renderer;

bool Keyboard::HandleKeyDown(unsigned char key)
{
    if (key == 'w')
    {
        KeyState['w'] = true;
		Renderer.ThePlayer.MoveForward(1.0);
    }
    
    if (key == 'a')
    {
        KeyState['a'] = true;
		Renderer.ThePlayer.MoveSideways(-1.0);
    }
    
    if (key == 's')
    {
        KeyState['s'] = true;
		Renderer.ThePlayer.MoveForward(-1.0);
    }
    
    if (key == 'd')
    {
        KeyState['d'] = true;
		Renderer.ThePlayer.MoveSideways(1.0);
    }
    
    if (key == ' ')
    {
        KeyState[' '] = true;
        Renderer.ThePlayer.MoveVertically(0.5);
    }
    
    return true;
}

void Keyboard::HandleKeyStillDown()
{
    if (KeyState['w'])
    {
		Renderer.ThePlayer.MoveForward(1.0);
    }
    
    if (KeyState['a'])
    {
		Renderer.ThePlayer.MoveSideways(-1.0);
    }
    
    if (KeyState['s'])
    {
		Renderer.ThePlayer.MoveForward(-1.0);
    }
    
    if (KeyState['d'])
    {
		Renderer.ThePlayer.MoveSideways(1.0);
    }
}

bool Keyboard::HandleKeyUp(unsigned char key)
{
    if (key == 'w')
    {
        KeyState['w'] = false;
		Renderer.ThePlayer.MoveForward(0.0);
    }
    
    if (key == 'a')
    {
        KeyState['a'] = false;
		Renderer.ThePlayer.MoveSideways(0.0);
    }
    
    if (key == 's')
    {
        KeyState['s'] = false;
		Renderer.ThePlayer.MoveForward(0.0);
    }
    
    if (key == 'd')
    {
        KeyState['d'] = false;
		Renderer.ThePlayer.MoveSideways(0.0);
    }

    if (key == ' ')
	{
		KeyState[' '] = false;
	}

    return true;
}
