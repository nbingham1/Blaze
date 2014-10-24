#include "../math.h"
#include "value.h"
#include "font.h"

#ifndef slider_h
#define slider_h

template <class type>
struct sliderhdl
{
	sliderhdl()
	{
		selected = false;
	}

	sliderhdl(const sliderhdl &s)
	{
		low = s.low;
		high = s.high;
		left = s.left;
		right = s.right;
		min = s.min;
		max = s.max;
		selected = s.selected;
	}

	sliderhdl(type *value, type min, type max, vec2f left, vec2f right)
	{
		this->low = valuehdl<type>(value);
		this->high = valuehdl<type>(value);
		this->min = min;
		this->max = max;
		this->left = left;
		this->right = right;
		this->selected = false;
	}

	sliderhdl(type *low, type *high, type min, type max, vec2f left, vec2f right)
	{
		this->low = valuehdl<type>(low);
		this->high = valuehdl<type>(high);
		this->min = min;
		this->max = max;
		this->left = left;
		this->right = right;
		this->selected = false;
	}

	~sliderhdl()
	{

	}

	valuehdl<type> low, high;
	vec2f left, right;
	type min, max;

	bool selected;

	void render(fonthdl *font)
	{
		glPushMatrix();
		glTranslatef(0.0, -0.035, 0.0);
		glBegin(GL_LINES);
			glColor4f(1.0, 1.0, 0.0, 1.0);
			glVertex2f(left[0] + 0.0125, left[1] - 0.0175);
			glVertex2f(right[0] + 0.0125, right[1] - 0.0175);
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
	}

	void select(vec2f coord)
	{
		if (coord[0] > curr*width && coord.x < curr*width + 0.025 && coord.y > 0.035 && coord.y < 0.07)
		{
			selected = true;
		}
	}

	void unselect()
	{
		selected = false;
	}

	void move(vec pos, vec delta)
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
};

#endif
