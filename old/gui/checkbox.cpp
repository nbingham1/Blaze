#include "checkbox.h"
#include "../graphics.h"

void checkboxhdl::init(const char *n, bool *v)
{
	strcpy(name, n);
	value = v;
}

void checkboxhdl::render(fonthdl *font)
{
	glBegin(GL_QUADS);
		if (*value)
			glColor3f(0.0, 0.3, 0.0);
		else
			glColor3f(0.3, 0.0, 0.0);
		glVertex2f(0.0, -0.035);
		glVertex2f(0.025, -0.035);
		if (*value)
			glColor3f(0.0, 0.7, 0.0);
		else
			glColor3f(0.7, 0.0, 0.0);
		glVertex2f(0.025, 0.0);
		glVertex2f(0.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.0, 0.00);
		glVertex2f(0.025, 0.00);
		glVertex2f(0.025, -0.035);
		glVertex2f(0.0, -0.035);
	glEnd();

	font->print(name, rgba_hdr(1.0, 1.0, 1.0, 1.0), vec2f(0.035, -0.0275));
}

void checkboxhdl::release()
{
}

void checkboxhdl::select(vec2i coord)
{
	if (coord[0] > 0.0 && coord[0] < 0.025 && coord[1] > 0.0 && coord[1] < 0.035)
		(*value) = !(*value);
}
