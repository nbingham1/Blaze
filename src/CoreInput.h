/*
 *  CoreInput.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef CoreInput_h
#define CoreInput_h

#if defined(__APPLE__) || defined(MACOSX)

#include "CarbonKeyboard.h"
#include "CarbonMouse.h"

struct CarbonInputController
{
	CarbonKeyboard keyboard;
	CarbonMouse	   mouse;
};

#else

#include "WindowsKeyboard.h"
#include "WindowsMouse.h"

struct WindowsInputController
{
	WindowsKeyboard	keyboard;
	WindowsMouse	mouse;
};

#endif

#endif