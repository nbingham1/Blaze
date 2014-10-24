/*
 * controller.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on December 7, 2011.
 * Modified by Ned Bingham on December 7, 2011.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller.h"
#include "../common.h"

controllerhdl::controllerhdl()
{
	next = NULL;
	prev = NULL;

	device = NULL;
	handle = NULL;

	type = CNTRL_UNKNOWN;
	interface_idx = 0;
	configuration_idx = 0;
	altsetting_idx = 0;

	report = NULL;
}

controllerhdl::controllerhdl(libusb_device *dev, libusb_config_descriptor *config, int c, int i, int s)
{
	next = NULL;
	prev = NULL;

	device = dev;
	handle = NULL;
	report = NULL;

	type = CNTRL_UNKNOWN;
	interface_idx = i;
	configuration_idx = c;
	altsetting_idx = s;

	if (libusb_open(device, &handle) != 0)
		printf("Error: Unable to open handle for USB device.\n");
	if (libusb_get_device_descriptor(device, &device_descriptor) != 0)
		printf("Error: Unable to get device descriptor for USB device.\n");

	config_descriptor = config;
	if (config_descriptor != NULL)
		interface = (libusb_interface*)&config_descriptor->interface[i];
	if (interface != NULL)
		interface_descriptor = (libusb_interface_descriptor*)&interface->altsetting[altsetting_idx];

	const libusb_endpoint_descriptor *endpoint = &interface_descriptor->endpoint[0];
	report = new uint8_t[endpoint->wMaxPacketSize];

	libusb_set_configuration(handle, configuration_idx);
	libusb_claim_interface(handle, interface_idx);
	libusb_set_interface_alt_setting(handle, interface_idx, altsetting_idx);

	libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN | LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_GET_DESCRIPTOR, (LIBUSB_DT_REPORT << 8) | interface_idx, 0, report, endpoint->wMaxPacketSize, 5000);

	for (int x = 0; report[x] != 0xC0; x++)
		printf("%X ", report[x]);
}

controllerhdl::~controllerhdl()
{
	if (handle != NULL)
		libusb_release_interface(handle, interface_idx);

	if (config_descriptor != NULL)
	libusb_free_config_descriptor(config_descriptor);

	if (handle != NULL)
		libusb_close(handle);

	if (report != NULL)
		delete [] report;

	report = NULL;

	next = NULL;
	prev = NULL;

	device = NULL;
	handle = NULL;

	type = CNTRL_UNKNOWN;
	interface_idx = 0;
	configuration_idx = 0;
	altsetting_idx = 0;
}

void controllerhdl::update()
{

}

void *controllerhdl::operator new(size_t size, void *v)
{
	return v;
}
