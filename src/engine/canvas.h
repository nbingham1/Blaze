/*
 * canvas.h
 *
 *  Created on: Nov 24, 2013
 *      Author: nbingham
 */

#include <math/geometry.h>
#include <math/big.h>
#include "input/controller.h"
#include "graphics/palette.h"
#include "graphics/opengl.h"
#include "graphics/frame.h"

#include "player.h"
#include "object.h"
#include "planet.h"

using namespace core;

#ifndef canvas_h
#define canvas_h

struct canvashdl
{
	canvashdl();
	~canvashdl();

	palettehdl palette;

	vec2i screen;

	map<string, controllerhdl> devices;
	list<objecthdl*> objects;
	array<playerhdl> players;

	double frame_number;
	double frame_start_time;
	double real_current_time;
	double game_current_time;
	double game_time_multiplier;

	framehdl frame;

	void initialize(int w, int h);
	void reshape(int w, int h);
	void release();

	void prepare();
	void render();

	void clock();
	void input();

	playerhdl *add_player();
};

#endif
