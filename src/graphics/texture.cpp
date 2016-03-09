/*
 * texture.cpp
 *
 *  Created on: May 22, 2015
 *      Author: nbingham
 */

#include "texture.h"
#include "opengl.h"

texturehdl::texturehdl()
{
	type = GL_NONE;
	identity = -1;
}

texturehdl::texturehdl(unsigned int type, unsigned int identity)
{
	this->type = type;
	this->identity = identity;
}

texturehdl::~texturehdl()
{
}

bool texturehdl::load(unsigned int type, const imagehdl &image)
{
	this->type = type;

	GLuint format;
	if (image.bpp == 4)
		format = GL_RGBA;
	else if (image.bpp == 3)
		format = GL_RGB;
	else if (image.bpp == 2)
		format = GL_LUMINANCE_ALPHA;
	else if (image.bpp == 1)
		format = GL_LUMINANCE;

	glGenTextures(1, &identity);
	glBindTexture(type, identity);
	glTexParameterf(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameterf(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	if (type == GL_TEXTURE_1D)
		glTexImage1D(type, 0, format, image.width, 0, format, GL_UNSIGNED_BYTE, image.data.data);
	else if (type == GL_TEXTURE_2D)
		glTexImage2D(type, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.data.data);
	else if (type == GL_TEXTURE_3D || type == GL_TEXTURE_2D_ARRAY)
		glTexImage3D(type, 0, format, image.width, image.height, image.depth, 0, format, GL_UNSIGNED_BYTE, image.data.data);
	glGenerateMipmap(type);

	return true;
}

void texturehdl::bind()
{
	glEnable(type);
	glBindTexture(type, identity);
}

void texturehdl::release()
{
	if (identity != -1)
		glDeleteTextures(1, &identity);
	identity = -1;
}


void texturehdl::frame(int width, int height, bool color, bool depth)
{
	GLuint FBO;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, identity, 0);

	if (depth)
	{
		GLuint RB;
		glGenRenderbuffers(1, &RB);
		glBindRenderbuffer(GL_RENDERBUFFER, RB);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RB);
	}

	glViewport(0, 0, width, height);
}

void texturehdl::unframe(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glViewport(0, 0, width, height);
}

bool operator<(texturehdl s0, texturehdl s1)
{
	return (s0.type < s1.type) ||
		   (s0.type == s1.type && s0.identity < s1.identity);
}

bool operator>(texturehdl s0, texturehdl s1)
{
	return (s0.type > s1.type) ||
		   (s0.type == s1.type && s0.identity > s1.identity);
}

bool operator<=(texturehdl s0, texturehdl s1)
{
	return (s0.type <= s1.type) ||
		   (s0.type == s1.type && s0.identity <= s1.identity);
}

bool operator>=(texturehdl s0, texturehdl s1)
{
	return (s0.type >= s1.type) ||
		   (s0.type == s1.type && s0.identity >= s1.identity);
}

bool operator==(texturehdl s0, texturehdl s1)
{
	return (s0.type == s1.type && s0.identity == s1.identity);
}

bool operator!=(texturehdl s0, texturehdl s1)
{
	return (s0.type != s1.type || s0.identity != s1.identity);
}
