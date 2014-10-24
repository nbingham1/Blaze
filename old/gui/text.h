#include "font.h"

#ifndef text_h
#define text_h

struct texthdl
{
	texthdl();
	texthdl(const texthdl &t);
	texthdl(string name);
	texthdl(string name, string value);
	~texthdl();

	string name;
	string value;

	void render(fonthdl *font);

	texthdl &operator=(texthdl t);
};

texthdl &operator<<(texthdl &fout, const char *str);
texthdl &operator<<(texthdl &fout, string str);
texthdl &operator<<(texthdl &fout, char i);
texthdl &operator<<(texthdl &fout, unsigned char i);
texthdl &operator<<(texthdl &fout, bool i);
texthdl &operator<<(texthdl &fout, int i);
texthdl &operator<<(texthdl &fout, short i);
texthdl &operator<<(texthdl &fout, long i);
texthdl &operator<<(texthdl &fout, unsigned int i);
texthdl &operator<<(texthdl &fout, unsigned short i);
texthdl &operator<<(texthdl &fout, unsigned long i);
texthdl &operator<<(texthdl &fout, float i);
texthdl &operator<<(texthdl &fout, double i);

#endif
