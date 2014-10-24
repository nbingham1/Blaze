/*
 * bus.h
 *
 *  Created on: Jul 7, 2012
 *      Author: Ned Bingham
 */

#include "../common.h"
#include "controller.h"
#include "mouse.h"
#include "keyboard.h"

#ifndef bus_h
#define bus_h

struct bushdl
{
	bushdl();
	~bushdl();

	libusb_context	*context;
	libusb_device	**device_list;
	ssize_t			device_count;

	controllerhdl *controller_s;
	controllerhdl *controller_e;

	bool init();
	void release();

	controllerhdl *new_cntrl(int cclass, int csubclass, int cprotocol, int size);
	mousehdl *new_mouse();
	keyboardhdl *new_keyboard();

	bool check(libusb_device *dev);
	void add(controllerhdl *c);
	void remove(controllerhdl *c);

	void print(FILE *file);
	void print();
};

#endif
