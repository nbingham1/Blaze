/*
 * material.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "core/string.h"
#include "material.h"
#include "opengl.h"

using namespace core;

materialhdl::materialhdl()
{
	program = -1;
}

materialhdl::~materialhdl()
{

}

void materialhdl::bind()
{
	glUseProgram(program);

	int j = 0;
	for (map<string, texturehdl>::iterator tex = textures.begin(); tex != textures.end(); tex++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		tex->value.bind();
		glUniform1i(glGetUniformLocation(program, tex->key.c_str()), j);
		j++;
	}

	for (map<string, array<float> >::iterator value = values.begin(); value != values.end(); value++)
	{
		switch (value->value.size())
		{
		case 1: glUniform1fv(glGetUniformLocation(program, value->key.c_str()), 1, value->value.data); break;
		case 2: glUniform2fv(glGetUniformLocation(program, value->key.c_str()), 1, value->value.data); break;
		case 3: glUniform3fv(glGetUniformLocation(program, value->key.c_str()), 1, value->value.data); break;
		case 4: glUniform4fv(glGetUniformLocation(program, value->key.c_str()), 1, value->value.data); break;
		case 9: glUniformMatrix3fv(glGetUniformLocation(program, value->key.c_str()), 1, false, value->value.data); break;
		case 16: glUniformMatrix4fv(glGetUniformLocation(program, value->key.c_str()), 1, false, value->value.data); break;
		}
	}
}
