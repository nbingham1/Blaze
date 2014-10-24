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
	KeyState[key] = true;
    if (key == 'w')
        Renderer.view.Move(Vector(0.0, 0.0, 1.0));
    
    if (key == 'a')
        Renderer.view.Move(Vector(-1.0, 0.0, 0.0));
    
    if (key == 's')
        Renderer.view.Move(Vector(0.0, 0.0, -1.0));
    
    if (key == 'd')
		Renderer.view.Move(Vector(1.0, 0.0, 0.0));
    
    if (key == ' ')
    	Renderer.view.Move(Vector(0.0, 0.5, 0.0));

    if (key == 'z')
    	Renderer.view.Host->Physics.LinearVelocity = Vector(0.0, 0.0, 0.0);

    if (key == ',')
    	Renderer.view.move_mult *= .5;
    
    if (key == '.')
    	Renderer.view.move_mult *= 2.0;

    return true;
}

void Keyboard::HandleKeyStillDown()
{
    if (KeyState['w'])
    	Renderer.view.Move(Vector(0.0, 0.0, 1.0));
    
    if (KeyState['a'])
    	Renderer.view.Move(Vector(-1.0, 0.0, 0.0));
    
    if (KeyState['s'])
    	Renderer.view.Move(Vector(0.0, 0.0, -1.0));
    
    if (KeyState['d'])
    	Renderer.view.Move(Vector(1.0, 0.0, 0.0));

    if (KeyState[' '])
       	Renderer.view.Move(Vector(0.0, 0.5, 0.0));
}

bool Keyboard::HandleKeyUp(unsigned char key)
{
	KeyState[key] = false;

    return true;
}
