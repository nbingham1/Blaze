#include "text.h"

texthdl::texthdl()
{

}

texthdl::texthdl(const texthdl &t)
{
	name = t.name;
	value = t.value;
}

texthdl::texthdl(string name)
{
	this->name = name;
}

texthdl::texthdl(string name, string value)
{
	this->name = name;
	this->value = value;
}

texthdl::~texthdl()
{

}

void texthdl::render(fonthdl *font)
{
	font->print(name, rgb_hdr(1.0, 1.0, 1.0), vec2f(0.0, -0.0275));
	font->print(value, rgb_hdr(1.0, 1.0, 1.0), vec2f(0.0, -0.0625));
}

texthdl &texthdl::operator=(texthdl t)
{
	name = t.name;
	value = t.value;
	return *this;
}
