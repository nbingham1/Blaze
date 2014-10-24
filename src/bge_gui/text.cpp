#include "text.h"
#include "../common.h"

void texthdl::init(char *n, char *v)
{
	strcpy(name, n);
	strcpy(value, v);
}

void texthdl::render(fonthdl *font)
{
	renderstring2d(font, name, 1.0, 1.0, 1.0, 0.0, -0.0275);
	renderstring2d(font, value, 1.0, 1.0, 1.0, 0.0, -0.0625);
}

void texthdl::release()
{

}
