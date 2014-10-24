/*
 * font.cpp
 *
 *  Created on: Aug 3, 2011
 *      Author: Ned Bingham
 */

#include "font.h"

fonthdl::fonthdl()
{
	font_vx = 0;
	font_ft = 0;
	font_pm = 0;
}

fonthdl::~fonthdl()
{
	release();
}

void fonthdl::initialize(const char *texture, const char *vertex, const char *fragment)
{
	int w = 256, h = 256;
	font_space.initialize(rgb_tga(texture, &w, &h), 256, false, GL_CLAMP_TO_EDGE, GL_RGB);

	int TextLen;
	char ErrText[1024];

	TextLen = 1024;
	font_vx = LoadGLSLShader(GL_VERTEX_SHADER, vertex, ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	font_ft = LoadGLSLShader(GL_FRAGMENT_SHADER, fragment, ErrText, &TextLen);
	printf("%s", ErrText);

	font_pm = glCreateProgram();
	glAttachShader(font_pm, font_vx);
	glAttachShader(font_pm, font_ft);
	glLinkProgram(font_pm);
}

void fonthdl::release()
{
	font_space.release();
	if (font_vx != 0)
		glDeleteShader(font_vx);
	if (font_ft != 0)
		glDeleteShader(font_ft);
	if (font_pm != 0)
		glDeleteShader(font_pm);
}

void fonthdl::render()
{
	glEnable(GL_TEXTURE_2D);

	glUseProgram(font_pm);

	glActiveTexture(GL_TEXTURE0);
	font_space.render();
	glUniform1i(glGetUniformLocation(font_pm, "tex_0"), 0);
}

void renderstring2d(fonthdl *f, char string[], float r, float g, float b, float x, float y)
{
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

	if (f != NULL)
		f->render();
	float s, t;
	float w = 9.0;
	float h = 15.0;

	for (unsigned int i = 0; i < strlen(string); i++)
	{
		s = (float)(string[i]%16)/16.0;
		t = 1.0 - (float)(string[i]/16 + 1)/16.0;
		glBegin(GL_QUADS);
			glColor4f(r, g, b, 1.0);
			glMultiTexCoord2f(GL_TEXTURE0, s + (8.0-w/2.0)/256.0, t + (8.0-h/2.0)/256.0);
			glVertex2f(x + 0.026*(w/h)*(float)i, y);
			glMultiTexCoord2f(GL_TEXTURE0, s + (8.0+w/2.0)/256.0, t + (8.0-h/2.0)/256.0);
			glVertex2f(x + 0.026*(w/h)*(float)(i+1), y);
			glMultiTexCoord2f(GL_TEXTURE0, s + (8.0+w/2.0)/256.0, t + (8.0+h/2.0)/256.0);
			glVertex2f(x + 0.026*(w/h)*(float)(i+1), y + 0.026);
			glMultiTexCoord2f(GL_TEXTURE0, s + (8.0-w/2.0)/256.0, t + (8.0+h/2.0)/256.0);
			glVertex2f(x + 0.026*(w/h)*(float)i, y + 0.026);
		glEnd();
	}

	glUseProgram(current_program);
}
