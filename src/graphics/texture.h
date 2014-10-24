#include "../math.h"
#include "opengl.h"

#ifndef texture_h
#define texture_h

typedef vec<float, 3> uvw;
typedef vec<float, 2> uv;

struct texturehdl
{
	texturehdl();
	texturehdl(const texturehdl &t);
	texturehdl(string filename, GLuint type, GLenum repeat, bool lod);
	texturehdl(array<string> filenames, GLuint type, GLenum repeat, bool lod);
	texturehdl(int width, int height);
	~texturehdl();

	GLuint type, identity;
	int bpp, width, height, depth;

	string load_tga(string filename);

	void release();

	void bind();

	void begin_render(bool depth_test);
	void end_render();

	texturehdl &operator=(const texturehdl &t);
};

#endif
