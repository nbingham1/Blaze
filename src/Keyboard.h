#ifndef keyboard_h
#define keyboard_h

struct keyboardhdl
{
	keyboardhdl();
	
	bool keystates[256];
	
	void keydown(unsigned char key);
	void keyup(unsigned char key);
};

#endif