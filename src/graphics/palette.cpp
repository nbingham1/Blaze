/*
 * palette.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "palette.h"
#include "opengl.h"
#include <std/file.h>

palettehdl::palettehdl()
{
}

palettehdl::~palettehdl()
{
}

unsigned int palettehdl::program(string vertex, string fragment)
{
	map<string, shaderhdl>::iterator vtloc = shaders.find(vertex);
	if (vtloc == shaders.end())
	{
		shaderhdl shader;
		shader.load(GL_VERTEX_SHADER, vertex);
		vtloc = shaders.insert(vertex, shader);
	}

	map<string, shaderhdl>::iterator ftloc = shaders.find(fragment);
	if (ftloc == shaders.end())
	{
		shaderhdl shader;
		shader.load(GL_FRAGMENT_SHADER, fragment);
		ftloc = shaders.insert(fragment, shader);
	}

	array<shaderhdl> index;
	index.push_back(vtloc->value);
	index.push_back(ftloc->value);

	map<array<shaderhdl>, unsigned int>::iterator ploc = programs.find(index);
	if (ploc == programs.end())
	{
		unsigned int prog = glCreateProgram();
		glAttachShader(prog, vtloc->value.identity);
		glAttachShader(prog, ftloc->value.identity);
		glBindFragDataLocation(prog, 0, "frag_color");
		glLinkProgram(prog);
		ploc = programs.insert(index, prog);
	}

	return ploc->value;
}

unsigned int palettehdl::program(string vertex, string geometry, string fragment)
{
	map<string, shaderhdl>::iterator vtloc = shaders.find(vertex);
	if (vtloc == shaders.end())
	{
		shaderhdl shader;
		shader.load(GL_VERTEX_SHADER, vertex);
		vtloc = shaders.insert(vertex, shader);
	}

	map<string, shaderhdl>::iterator gmloc = shaders.find(geometry);
	if (gmloc == shaders.end())
	{
		shaderhdl shader;
		shader.load(GL_GEOMETRY_SHADER, geometry);
		gmloc = shaders.insert(geometry, shader);
	}

	map<string, shaderhdl>::iterator ftloc = shaders.find(fragment);
	if (ftloc == shaders.end())
	{
		shaderhdl shader;
		shader.load(GL_FRAGMENT_SHADER, fragment);
		ftloc = shaders.insert(fragment, shader);
	}

	array<shaderhdl> index;
	index.push_back(vtloc->value);
	index.push_back(ftloc->value);
	index.push_back(gmloc->value);

	map<array<shaderhdl>, unsigned int>::iterator ploc = programs.find(index);
	if (ploc == programs.end())
	{
		unsigned int prog = glCreateProgram();
		glAttachShader(prog, vtloc->value.identity);
		glAttachShader(prog, gmloc->value.identity);
		glAttachShader(prog, ftloc->value.identity);
		glBindFragDataLocation(prog, 0, "frag_color");
		glLinkProgram(prog);
		ploc = programs.insert(index, prog);
	}

	return ploc->value;
}

texturehdl palettehdl::texture(unsigned int type, string filename)
{
	map<string, texturehdl>::iterator loc = textures.find(filename);
	if (loc != textures.end())
		return loc->value;
	else
	{
		imagehdl image;
		texturehdl tex;

		if (image.load(filename))
		{
			tex.load(type, image);
			loc = textures.insert(filename, tex);
		}
		else
			return tex;
	}

	return loc->value;
}


