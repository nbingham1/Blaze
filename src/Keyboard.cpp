#include "keyboard.h"

keyboardhdl::keyboardhdl()
{
	for (int x = 0; x < 256; x++)
		keystates[x] = false;
}

void keyboardhdl::keydown(unsigned char key)
{
	keystates[key] = true;
}

void keyboardhdl::keyup(unsigned char key)
{
	keystates[key] = false;
}