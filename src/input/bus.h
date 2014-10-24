/*
 * bus.h
 *
 *  Created on: Jul 7, 2012
 *      Author: Ned Bingham
 */

#include "common.h"
#include "controller.h"
#include "mouse.h"
#include "keyboard.h"
#include "../base.h"

#ifndef bus_h
#define bus_h

struct bushdl : list<controllerhdl>
{
	bushdl();
	~bushdl();

	mousehdl *new_mouse();
	keyboardhdl *new_keyboard();
};

#endif
