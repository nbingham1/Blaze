/*
 * bus.cpp
 *
 *  Created on: Jul 7, 2012
 *      Author: Ned Bingham
 */

#include "bus.h"

bushdl::bushdl()
{
	context			= NULL;
	device_list		= NULL;
	device_count	= 0;
	controller_s	= NULL;
	controller_e	= NULL;
}

bushdl::~bushdl()
{
	release();
}

bool bushdl::init()
{
	if (libusb_init(&context) < 0)
	{
		printf("Error: libusb failed to initialize.\n");
		return false;
	}

	libusb_set_debug(context, 3);

	device_count = libusb_get_device_list(context, &device_list);
	if (device_count < 0)
	{
		printf("Error: libusb could not get the device list.\n");
		return false;
	}

	libusb_set_debug(context, 3);

	print();

	return true;
}

void bushdl::release()
{
	controllerhdl *curr_controller = controller_s, *prev_controller;
	while (curr_controller != NULL)
	{
		prev_controller = curr_controller;
		curr_controller = curr_controller->next;
		delete prev_controller;
		prev_controller = NULL;
	}

	if (device_list != NULL)
		libusb_free_device_list(device_list, 1);

	if (context != NULL)
		libusb_exit(context);

	context			= NULL;
	device_list		= NULL;
	device_count	= 0;
	controller_s	= NULL;
	controller_e	= NULL;
}

controllerhdl *bushdl::new_cntrl(int cclass, int csubclass, int cprotocol, int size)
{
	libusb_device_descriptor desc;
	libusb_config_descriptor *config;
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;

	controllerhdl *ret = NULL;

	int i, j, k, l;
	bool found = false;

	for (i = 0; i < device_count && !found; i++)
	{
		if (libusb_get_device_descriptor(device_list[i], &desc) == 0)
		{
			if (desc.bDeviceClass == cclass && desc.bDeviceSubClass == csubclass && desc.bDeviceProtocol == cprotocol && !check(device_list[i]))
				found = true;
			else if (desc.bDeviceClass == 0)
			{
				for (j = 0; j < desc.bNumConfigurations; j++)
				{
					config = NULL;
					if (libusb_get_config_descriptor(device_list[i], j, &config) == 0 && config != NULL)
					{
						for(k = 0; k < config->bNumInterfaces && !found; k++)
						{
							inter = &config->interface[k];
							for(l = 0; l < inter->num_altsetting && !found; l++)
							{
								interdesc = &inter->altsetting[l];
								if (interdesc->bInterfaceClass == cclass && interdesc->bInterfaceSubClass == csubclass && interdesc->bInterfaceProtocol == cprotocol && !check(device_list[i]))
									found = true;
							}
						}
					}
					if (!found)
					{
						libusb_free_config_descriptor(config);
						config = NULL;
					}
				}
			}
		}
	}

	if (found)
	{
		void *ptr = malloc(size);
		if (ptr == NULL)
		{
			printf("Error: Unable to allocate USB device controller for %X:%X:%X\n", cclass, csubclass, cprotocol);
			return ret;
		}

		ret = new (ptr) controllerhdl(device_list[i], config, j, k, l);

		add(ret);
	}
	else
	{
		printf("Error: Unable to find an attached USB device matching %X:%X:%X\n", cclass, csubclass, cprotocol);
		printf("Info : Reverting to GLUT hardware device.\n");
		void *ptr = malloc(size);
		if (ptr == NULL)
		{
			printf("Error: Unable to allocate USB device controller for %X:%X:%X\n", cclass, csubclass, cprotocol);
			return ret;
		}

		ret = new (ptr) controllerhdl();

		add(ret);
	}

	return ret;
}

mousehdl *bushdl::new_mouse()
{
	mousehdl *ret = (mousehdl*)new_cntrl(0x03, 0x00, 0x02, sizeof(mousehdl));
	if (ret == NULL)
		ret = (mousehdl*)new_cntrl(0x03, 0x01, 0x02, sizeof(mousehdl));

	if (ret != NULL)
		ret->init();

	return ret;
}

keyboardhdl *bushdl::new_keyboard()
{
	keyboardhdl *ret = (keyboardhdl*)new_cntrl(LIBUSB_CLASS_HID, 0x00, 0x01, sizeof(keyboardhdl));
	if (ret == NULL)
		ret = (keyboardhdl*)new_cntrl(LIBUSB_CLASS_HID, 0x01, 0x01, sizeof(keyboardhdl));

	if (ret != NULL)
		ret->init();

	return ret;
}

bool bushdl::check(libusb_device *dev)
{
	controllerhdl *curr_controller = controller_s;
	while (curr_controller != NULL)
	{
		if (curr_controller->device == dev)
			return true;

		curr_controller = curr_controller->next;
	}

	return false;
}

void bushdl::add(controllerhdl *c)
{
	if (controller_s == NULL || controller_e == NULL)
	{
		controller_s = c;
		controller_e = c;
		controller_e->next = NULL;
		controller_e->prev = NULL;
	}
	else
	{
		controller_e->next = c;
		c->prev = controller_e;
		controller_e = controller_e->next;
	}
}

void bushdl::remove(controllerhdl *c)
{
	controllerhdl *curr = controller_s, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		if (prev == c)
		{
			if (controller_s == prev)
				controller_s = prev->next;
			if (controller_e == prev)
				controller_e = prev->prev;
			if (prev->next != NULL)
				prev->next->prev = prev->prev;
			if (prev->prev != NULL)
				prev->prev->next = prev->next;

			delete prev;
			prev = NULL;
		}
	}
}

void bushdl::print(FILE *file)
{
	libusb_device_descriptor desc;
	libusb_config_descriptor *config;
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;
	int i, j, k, l;

	fprintf(file, "%d Devices Found\n", device_count);
	for (i = 0; i < device_count; i++)
	{
		if (libusb_get_device_descriptor(device_list[i], &desc) < 0)
			fprintf(file, "Unable to get device descriptor for device %d\n\n", i);
		else
		{
			fprintf(file, "Vendor ID:        0x%X\n", desc.idVendor);
			fprintf(file, "Product ID:       0x%X\n", desc.iProduct);
			fprintf(file, "Device Class:     0x%X\n", desc.bDeviceClass);
			fprintf(file, "Device Sub Class: 0x%X\n", desc.bDeviceSubClass);
			fprintf(file, "Device Protocol:  0x%X\n", desc.bDeviceProtocol);

			config = NULL;
			if (libusb_get_config_descriptor(device_list[i], 0, &config) != 0 || config == NULL)
				fprintf(file, "\tNULL config\n");
			else
			{
				fprintf(file, "\tInterfaces: %d\n", config->bNumInterfaces);
				for(j = 0; j < config->bNumInterfaces; j++)
				{
					inter = &config->interface[j];
					fprintf(file, "\t\t%d Alternate settings\n", inter->num_altsetting);
					for(k = 0; k < inter->num_altsetting; k++)
					{
						interdesc = &inter->altsetting[k];
						fprintf(file, "\t\t\tInterface Number: %d\n", interdesc->bInterfaceNumber);
						fprintf(file, "\t\t\tInterface Class: 0x%X\n", interdesc->bInterfaceClass);
						fprintf(file, "\t\t\tInterface Sub Class: 0x%X\n", interdesc->bInterfaceSubClass);
						fprintf(file, "\t\t\tInterface Protocol: 0x%X\n", interdesc->bInterfaceProtocol);
						fprintf(file, "\t\t\tNumber of endpoints: %d\n", interdesc->bNumEndpoints);
						for(l = 0; l < interdesc->bNumEndpoints; l++)
						{
							epdesc = &interdesc->endpoint[l];
							fprintf(file, "\t\t\t\tDescriptor Type: 0x%X\n", epdesc->bDescriptorType);
							fprintf(file, "\t\t\t\tEP Address: 0x%X\n", epdesc->bEndpointAddress);
						}
					}
				}
			}
			libusb_free_config_descriptor(config);
			fprintf(file, "\n");
		}
	}
}

void bushdl::print()
{
	print(stdout);
}
