#include "../common.h"
#include "../bge_math.h"

#ifndef texture_h
#define texture_h

struct texturehdl
{
	texturehdl(){}
	virtual ~texturehdl(){}

	GLuint identity;
	int size;

	virtual void render();
	void release();
};

struct texture1hdl : texturehdl
{
	texture1hdl(){}
	virtual ~texture1hdl(){}

	virtual void initialize(unsigned char *data, int s, bool lod, GLenum repeat, GLenum format);
	virtual void render();
};

struct texture2hdl : texturehdl
{
	texture2hdl(){}
	virtual ~texture2hdl(){}

	virtual void initialize(unsigned char *data, int s, bool lod, GLenum repeat, GLenum format);
	virtual void render();
};

struct texture3hdl : texturehdl
{
	texture3hdl(){}
	virtual ~texture3hdl(){}

	int depth;

	virtual void initialize(unsigned char *data, int s, int d, bool lod, GLenum repeat, GLenum format);
	virtual void render();
};

struct textureahdl : texture3hdl
{
	textureahdl(){}
	virtual ~textureahdl(){}

	void initialize(unsigned char *data, int s, int d, bool lod, GLenum repeat, GLenum format);
	void render();
};

unsigned char *rgb_tga(const char *filename, int *w, int *h);
unsigned char *rgba_tga(char *rgb, char *a, int *w, int *h);
unsigned char *rgba_tga(char *rgb, int *w, int *h);
unsigned char *rgb_solid(vec rgb, int w, int h);
unsigned char *rgba_solid(vec rgba, int w, int h);

void start_rtt(texture2hdl *tex, bool depth_test);
void end_rrt();

#endif
