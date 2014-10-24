#include "color.h"
#include "opengl.h"
#include "../base.h"
#include "texture.h"

#ifndef shader_h
#define shader_h

struct shaderhdl
{
	shaderhdl();
	shaderhdl(const shaderhdl &s);
	shaderhdl(string v, string f, bool raw = false);
	~shaderhdl();

	GLuint vertex, fragment, program;

	GLuint load_file(string filename, GLenum type);
	GLuint load_source(string source, GLenum type);

	void release();

	void bind();

	void uniform(string location, float data);
	void uniform(string location, vec1f data);
	void uniform(string location, vec2f data);
	void uniform(string location, vec3f data);
	void uniform(string location, vec4f data);

	void uniform(string location, int data);
	void uniform(string location, vec1i data);
	void uniform(string location, vec2i data);
	void uniform(string location, vec3i data);
	void uniform(string location, vec4i data);

	void uniform(string location, texturehdl data);

	shaderhdl &operator=(shaderhdl s);
};

#endif
