#include "slider.h"
#include "../bge_mtrl.h"

void sliderhdl::init(char *n, void *v, value_t t, double w, double low, double hi)
{
	strcpy(name, n);
	min = low;
	max = hi;
	type = t;
	value = v;
	width = w;
	if (type == float_ty)
		curr = ((*(float*)v) - min)/(max - min);
	else if (type == double_ty)
		curr = ((*(double*)v) - min)/(max - min);
	else if (type == int_ty)
		curr = (double(*(int*)v) - min)/(max - min);
	selected = false;
}

void sliderhdl::render(fonthdl *font)
{
	renderstring2d(font, name, 1.0, 1.0, 0.0, 0.0, -0.0257);
	glPushMatrix();
	glTranslatef(0.0, -0.035, 0.0);
	glBegin(GL_LINES);
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glVertex2f(0.0 + 0.0125, -0.0175);
		glVertex2f(width + 0.0125, -0.0175);
	glEnd();

	glBegin(GL_QUADS);
		glColor4f(0.0, 0.7, 0.0, 1.0);
		glVertex2f(curr*width, 0.0);
		glVertex2f(curr*width + 0.025, 0.0);
		glColor4f(0.0, 0.3, 0.0, 1.0);
		glVertex2f(curr*width + 0.025, -0.035);
		glVertex2f(curr*width, -0.035);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor4f(0.0, 0.0, 0.0, 1.0);
		glVertex2f(curr*width, 0.0);
		glVertex2f(curr*width + 0.025, 0.0);
		glVertex2f(curr*width + 0.025, -0.035);
		glVertex2f(curr*width, -0.035);
	glEnd();
	glPopMatrix();

	char out[32];
	if (type == bool_ty)
		sprintf(out, "%d", *(bool*)value);
	if (type == int_ty)
		sprintf(out, "%d", *(int*)value);
	if (type == float_ty)
		sprintf(out, "%f", *(float*)value);
	if (type == double_ty)
		sprintf(out, "%f", *(double*)value);
	if (type == vec_ty)
		sprintf(out, "%f %f %f", ((vec*)value)->x, ((vec*)value)->y, ((vec*)value)->z);
	if (type != texture_ty)
		renderstring2d(font, out, 0.0, 1.0, 0.0, 0.0, -0.0975);
}

void sliderhdl::release()
{

}

void sliderhdl::select(vec coord)
{
	if (coord.x > curr*width && coord.x < curr*width + 0.025 && coord.y > 0.035 && coord.y < 0.07)
	{
		selected = true;
	}
}

void sliderhdl::unselect()
{
	selected = false;
}

void sliderhdl::move(vec pos, vec delta)
{
	if (selected)
		curr = (pos.x - 0.0125)/width;

	if (curr < 0.0)
		curr = 0.0;
	if (curr > 1.0)
		curr = 1.0;

	if (type == float_ty)
		*(float*)value = min + curr*(max - min);
	else if (type == double_ty)
		*(double*)value = min + curr*(max - min);
	else if (type == int_ty)
		*(int*)value = int(min + curr*(max - min));
}
