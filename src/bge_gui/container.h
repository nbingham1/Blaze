#include "../bge_math.h"
#include "../common.h"
#include "value.h"
#include "checkbox.h"
#include "slider.h"
#include "text.h"
#include "font.h"

#ifndef container_h
#define container_h

enum objecthdl_t
{
	value_t,
	checkbox_t,
	slider_t,
	text_t
};

struct containerobj
{
	containerobj();
	~containerobj();

	void *obj;
	objecthdl_t type;

	containerobj *prev, *next;

	void render(fonthdl *font);
	void release();

	void select(vec coord);
	void unselect();
	void move(vec pos, vec delta);
};

struct containerhdl
{
	containerhdl();
	~containerhdl();

	char name[32];
	double x, y;
	double width, height;
	bool visible;
	bool bar_selected;
	bool space_selected;
	bool mouse_over;

	containerobj *list;
	containerobj *end;

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
