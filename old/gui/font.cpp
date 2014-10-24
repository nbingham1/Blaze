/*
 * font.cpp
 *
 *  Created on: Aug 3, 2011
 *      Author: Ned Bingham
 */

#include "font.h"

fonthdl::fonthdl()
{
}

fonthdl::~fonthdl()
{
	release();
}

void fonthdl::print(string str, rgba_hdr color, vec2f pos)
{
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

	bind();
	uv coord;
	float w = 9.0;
	float h = 15.0;

	for (int i = 0; i < str.size(); i++)
	{
		coord = uv((float)(str[i]%16)/16.0, 1.0 - (float)(str[i]/16 + 1)/16.0);
		glBegin(GL_QUADS);
			glColor4fv(color.data);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)i, pos[1]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1] + 0.026);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)i, pos[1] + 0.026);
		glEnd();
	}

	glUseProgram(current_program);
}

void fonthdl::print(string str, rgba_hdr color, vec3f pos)
{
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

	bind();
	uv coord;
	float w = 9.0;
	float h = 15.0;

	for (int i = 0; i < str.size(); i++)
	{
		coord = uv((float)(str[i]%16)/16.0, 1.0 - (float)(str[i]/16 + 1)/16.0);
		glBegin(GL_QUADS);
			glColor4fv(color.data);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)i, pos[1], pos[2]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1], pos[2]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1] + 0.026, pos[2]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)i, pos[1] + 0.026, pos[2]);
		glEnd();
	}

	glUseProgram(current_program);
}

void fonthdl::print(string str, rgb_hdr color, vec2f pos)
{
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

	bind();
	uv coord;
	float w = 9.0;
	float h = 15.0;

	for (int i = 0; i < str.size(); i++)
	{
		coord = uv((float)(str[i]%16)/16.0, 1.0 - (float)(str[i]/16 + 1)/16.0);
		glBegin(GL_QUADS);
			glColor4f(color[0], color[1], color[2], 1.0);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)i, pos[1]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1] + 0.026);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex2f(pos[0] + 0.026*(w/h)*(float)i, pos[1] + 0.026);
		glEnd();
	}

	glUseProgram(current_program);
}

void fonthdl::print(string str, rgb_hdr color, vec3f pos)
{
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

	bind();
	uv coord;
	float w = 9.0;
	float h = 15.0;

	for (int i = 0; i < str.size(); i++)
	{
		coord = uv((float)(str[i]%16)/16.0, 1.0 - (float)(str[i]/16 + 1)/16.0);
		glBegin(GL_QUADS);
			glColor4f(color[0], color[1], color[2], 1.0);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)i, pos[1], pos[2]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0-h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1], pos[2]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0+w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)(i+1), pos[1] + 0.026, pos[2]);
			glMultiTexCoord2f(GL_TEXTURE0, coord[0] + (8.0-w/2.0)/256.0, coord[1] + (8.0+h/2.0)/256.0);
			glVertex3f(pos[0] + 0.026*(w/h)*(float)i, pos[1] + 0.026, pos[2]);
		glEnd();
	}

	glUseProgram(current_program);
}
