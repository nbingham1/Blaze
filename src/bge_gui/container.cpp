#include "container.h"
#include "../bge_mtrl.h"

containerobj::containerobj()
{
	obj = NULL;
	next = NULL;
	prev = NULL;
}

containerobj::~containerobj()
{
	release();
}

void containerobj::render(fonthdl *font)
{
	if (obj == NULL)
		return;

	if (type == value_t)
		((valuehdl*)obj)->render(font);
	if (type == checkbox_t)
		((checkboxhdl*)obj)->render(font);
	if (type == slider_t)
		((sliderhdl*)obj)->render(font);
	if (type == text_t)
		((texthdl*)obj)->render(font);
}

void containerobj::release()
{
	if (obj != NULL)
	{
		if (type == value_t)
		{
			((valuehdl*)obj)->release();
			delete ((valuehdl*)obj);
			obj = NULL;
		}
		if (type == checkbox_t)
		{
			((checkboxhdl*)obj)->release();
			delete ((checkboxhdl*)obj);
			obj = NULL;
		}
		if (type == slider_t)
		{
			((sliderhdl*)obj)->release();
			delete ((sliderhdl*)obj);
			obj = NULL;
		}
		if (type == text_t)
		{
			((texthdl*)obj)->release();
			delete ((texthdl*)obj);
			obj = NULL;
		}
	}
}

void containerobj::select(vec coord)
{
	if (obj != NULL)
	{
		if (type == checkbox_t)
			((checkboxhdl*)obj)->select(coord);
		if (type == slider_t)
			((sliderhdl*)obj)->select(coord);
	}
}

void containerobj::unselect()
{
	if (obj != NULL)
	{
		if (type == slider_t)
			((sliderhdl*)obj)->unselect();
	}
}

void containerobj::move(vec pos, vec delta)
{
	if (obj != NULL)
	{
		if (type == slider_t)
			((sliderhdl*)obj)->move(pos, delta);
	}
}

containerhdl::containerhdl()
{
	list = NULL;
	end = NULL;
	name[0] = '\0';
}

containerhdl::~containerhdl()
{
	release();
}

void containerhdl::init(const char *n, double a, double b, double w)
{
	strcpy(name, n);
	x = a;
	y = b;
	width = w;
	list = NULL;
	end = NULL;
	visible = false;
	bar_selected = false;
	space_selected = false;

	height = 0.07;
}
void containerhdl::render(fonthdl *font)
{
	if (visible)
	{
		glLoadIdentity();

		glBegin(GL_QUADS);
			glColor4f(0.4, 0.4, 0.4, 0.8);
			glVertex2f(x, y-height);
			glVertex2f(x+width, y-height);
			glColor4f(0.6, 0.6, 0.6, 0.8);
			glVertex2f(x+width, y);
			glVertex2f(x, y);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glColor4f(0.0, 0.0, 0.0, 1.0);
			glVertex2f(x, y-height);
			glVertex2f(x+width, y-height);
			glVertex2f(x+width, y);
			glVertex2f(x, y);
		glEnd();

		glBegin(GL_QUADS);
			glColor4f(0.3, 0.0, 0.0, 0.6);
			glVertex2f(x+width-0.03*double(width != 0.0), y-0.05*double(height != 0.0));
			glVertex2f(x+width, y-0.05*double(height != 0.0));
			glColor4f(0.7, 0.0, 0.0, 0.6);
			glVertex2f(x+width, y);
			glVertex2f(x+width-0.03*double(width != 0.0), y);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glColor4f(0.0, 0.0, 0.0, 1.0);
			glVertex2f(x+width-0.03*double(width != 0.0), y-0.05*double(height != 0.0));
			glVertex2f(x+width, y-0.05*double(height != 0.0));
			glVertex2f(x+width, y);
			glVertex2f(x+width-0.03*double(width != 0.0), y);
		glEnd();

		glBegin(GL_LINES);
			glColor4f(0.0, 0.0, 0.0, 1.0);
			glVertex2f(x, y-0.05*double(height != 0.0));
			glVertex2f(x+width, y-0.05*double(height != 0.0));
		glEnd();

		if (height != 0.0 && width != 0.0)
			renderstring2d(font, name, 1.0, 1.0, 1.0, x+0.01, y-0.035);

		glTranslatef(x+0.01, y-0.06, 0.0);
		containerobj *curr = list;
		while (curr != NULL)
		{
			curr->render(font);
			if (curr->type == value_t)
				glTranslatef(0.0, -0.07, 0.0);
			if (curr->type == checkbox_t)
				glTranslatef(0.0, -0.035, 0.0);
			if (curr->type == slider_t)
				glTranslatef(0.0, -0.105, 0.0);
			if (curr->type == text_t)
				glTranslatef(0.0, -0.07, 0.0);

			curr = curr->next;
		}
	}
}

void containerhdl::release()
{
	visible = false;
	containerobj *curr = list, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		prev->release();
		delete prev;
		prev = NULL;
	}
	height = 0.07;

	list = NULL;
	end = NULL;
}

void containerhdl::addobj(void *obj, objecthdl_t type)
{
	containerobj *newobj = new containerobj;
	newobj->obj = obj;
	newobj->type = type;
	newobj->next = NULL;
	newobj->prev = NULL;

	if (type == value_t)
		height += 0.07;
	if (type == checkbox_t)
		height += 0.035;
	if (type == slider_t)
		height += 0.105;
	if (type == text_t)
		height += 0.07;

	if (list == NULL)
	{
		list = newobj;
		end = list;
	}
	else
	{
		end->next = newobj;
		newobj->prev = end;
		end = end->next;
	}
}

bool containerhdl::select(vec coord)
{
	if (visible && coord.x > x && coord.x < x+width && coord.y > y-0.05 && coord.y < y)
	{
		if (coord.x > x+width-0.03 && coord.y > y - 0.05)
			visible = false;
		else
		{
			bar_selected = true;
		}

		return true;
	}
	else if (visible && coord.x > x && coord.x < x+width && coord.y > y-height && coord.y < y)
	{
		space_selected = true;

		vec pos = vec(coord.x - (x+0.01), (y - 0.06) - coord.y);
		int objnum = (int)trunc(pos.y/0.035);
		int count = 0;
		containerobj *curr = list;
		if (curr != NULL)
		{
			if (curr->type == value_t)
				count += 2;
			else if (curr->type == checkbox_t)
				count++;
			else if (curr->type == slider_t)
				count += 3;
			else if (curr->type == text_t)
				count += 2;
		}
		while (count <= objnum && curr != NULL)
		{
			curr = curr->next;
			if (curr != NULL)
			{
				if (curr->type == value_t)
					count += 2;
				else if (curr->type == checkbox_t)
					count++;
				else if (curr->type == slider_t)
					count += 3;
				else if (curr->type == text_t)
					count += 2;
			}
		}

		if (curr != NULL)
		{
			if (curr->type == value_t)
				count -= 2;
			else if (curr->type == checkbox_t)
				count--;
			else if (curr->type == slider_t)
				count -= 3;
			else if (curr->type == text_t)
				count -= 2;

			pos = pos - vec(0.0, double(count)*0.035);

			curr->select(pos);
			return true;
		}
	}
	return false;
}

void containerhdl::unselect()
{
	bar_selected = false;

	containerobj *curr = list;
	while (curr != NULL)
	{
		curr->unselect();
		curr = curr->next;
	}
}

void containerhdl::move(vec pos, vec delta)
{
	if (bar_selected)
	{
		x += delta.x;
		y += delta.y;
	}
	else if (space_selected)
	{
		containerobj *curr = list;
		double y_off = 0.07;
		while (curr != NULL)
		{
			curr->move(pos - vec(x + 0.01, y - y_off), delta);
			if (curr->type == value_t)
				y_off += 0.07;
			else if (curr->type == checkbox_t)
				y_off += 0.035;
			else if (curr->type == slider_t)
				y_off += 0.105;
			else if (curr->type == text_t)
				y_off += 0.07;
			curr = curr->next;
		}
	}
}

bool containerhdl::mousehover(vec coord)
{
	if (visible && coord.x > x && coord.x < x+width && coord.y > y-height && coord.y < y)
	{
		mouse_over = true;
		return true;
	}
	else
		mouse_over = false;

	return false;
}
