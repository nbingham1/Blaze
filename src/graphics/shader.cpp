/*
 * shader.cpp
 *
 *  Created on: May 22, 2015
 *      Author: nbingham
 */

#include "shader.h"
#include "opengl.h"
#include "core/file.h"

shaderhdl::shaderhdl()
{
	type = GL_NONE;
	identity = 0xFFFFFFFF;
}

shaderhdl::shaderhdl(unsigned int type, unsigned int identity)
{
	this->type = type;
	this->identity = identity;
}

shaderhdl::~shaderhdl()
{
}

bool shaderhdl::load(unsigned int type, string filename)
{
	this->type = type;
	file fin(filename, "rt");
	if (!fin.is_open())
	{
		cerr << "Error: Could not find file: " << filename << endl;
		return false;
	}
	string source = fin.read_file();
	const char* data = source.c_str();
	int length = source.size();
	fin.close();

	identity = glCreateShader(type);
	glShaderSource(identity, 1, &data, &length);
	glCompileShader(identity);

	char temp[1024];
	length = 1023;
	glGetShaderInfoLog(identity, 1023, &length, temp);
	if (length > 0)
	{
		cerr << temp;
		return false;
	}

	return true;
}

bool shaderhdl::compile(unsigned int type, string source)
{
	this->type = type;

	const char* data = source.c_str();
	GLint length = source.size();

	if (length == 0)
		return false;

	identity = glCreateShader(type);
	glShaderSource(identity, 1, &data, &length);
	glCompileShader(identity);

	char temp[1024];
	length = 1023;
	glGetShaderInfoLog(identity, 1023, &length, temp);
	if (length > 0)
	{
		cerr << temp;
		return false;
	}

	return true;
}

bool operator<(shaderhdl s0, shaderhdl s1)
{
	return (s0.type < s1.type) ||
		   (s0.type == s1.type && s0.identity < s1.identity);
}

bool operator>(shaderhdl s0, shaderhdl s1)
{
	return (s0.type > s1.type) ||
		   (s0.type == s1.type && s0.identity > s1.identity);
}

bool operator<=(shaderhdl s0, shaderhdl s1)
{
	return (s0.type <= s1.type) ||
		   (s0.type == s1.type && s0.identity <= s1.identity);
}

bool operator>=(shaderhdl s0, shaderhdl s1)
{
	return (s0.type >= s1.type) ||
		   (s0.type == s1.type && s0.identity >= s1.identity);
}

bool operator==(shaderhdl s0, shaderhdl s1)
{
	return (s0.type == s1.type && s0.identity == s1.identity);
}

bool operator!=(shaderhdl s0, shaderhdl s1)
{
	return (s0.type != s1.type || s0.identity != s1.identity);
}

