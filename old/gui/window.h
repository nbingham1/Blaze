#include "../math.h"
#include "value.h"
#include "checkbox.h"
#include "slider.h"
#include "text.h"
#include "font.h"

#ifndef window_h
#define window_h

enum objecthdl_t
{
	value_t,
	checkbox_t,
	slider_t,
	text_t
};

struct windowobj
{
	windowobj();
	~windowobj();

	void *obj;
	objecthdl_t type;

	void render(fonthdl *font);
	void release();

	void select(vec coord);
	void unselect();
	void move(vec pos, vec delta);
};

struct windowhdl
{
	windowhdl();
	~windowhdl();

	vec2f coord;
	vec2f boundary
	bool visible;
	bool bar_selected;
	bool space_selected;
	bool mouse_over;

	void init(const char *n, double a, double b, double w);
	void render(fonthdl *font);
	void release();

	void addobj(void *obj, objecthdl_t type);

	bool select(vec coord);
	void unselect();
	void move(vec pos, vec delta);

	bool mousehover(vec coord);
};

#endif
