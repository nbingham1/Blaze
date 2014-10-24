/*
 * material.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "material.h"
#include <dirent.h>

materialhdl::materialhdl()
{

}

materialhdl::materialhdl(const materialhdl &m)
{
	texture = m.texture;
	shader = m.shader;
}

materialhdl::materialhdl(string path)
{
	DIR *dir = opendir(path.data());
	dirent *ent;
	if (dir == NULL)
	{
		cerr << "Error: could not open " << path << "." << endl;
		return;
	}

	array<string> files;
	string vert, frag;
	while ((ent = readdir(dir)) != NULL)
	{
		if (sscanf(ent->d_name, "%*s.vx"))
			vert = ent->d_name;
		else if (sscanf(ent->d_name, "%*s.ft"))
			frag = ent->d_name;
		else if (sscanf(ent->d_name, "%*s.tga"))
			files.push_back(ent->d_name);
	}

	shader = shaderhdl(vert, frag);
	if (files.size() == 1)
		texture = texturehdl(files.front(), GL_TEXTURE_2D, GL_REPEAT, true);
	else
		texture = texturehdl(files, GL_TEXTURE_2D_ARRAY, GL_REPEAT, true);
}

materialhdl::~materialhdl()
{

}

void materialhdl::release()
{
	texture.release();
	shader.release();
}

void materialhdl::bind()
{
	shader.bind();

	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	shader.uniform("tex", texture);
}

materialhdl &materialhdl::operator=(materialhdl m)
{
	texture = m.texture;
	shader = m.shader;
	return *this;
}
