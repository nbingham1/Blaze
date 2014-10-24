/*
 * bus.cpp
 *
 *  Created on: Jul 7, 2012
 *      Author: Ned Bingham
 */

#include "bus.h"

bushdl::bushdl()
{
}

bushdl::~bushdl()
{
}

mousehdl *bushdl::new_mouse()
{
	mousehdl *result = (mousehdl*)push_back(mousehdl());
	result->init();
	return result;
}

keyboardhdl *bushdl::new_keyboard()
{
	keyboardhdl *result = (keyboardhdl*)push_back(keyboardhdl());
	result->init();
	return result;
}
