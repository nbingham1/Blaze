/*
 * canvas.h
 *
 *  Created on: Nov 24, 2013
 *      Author: nbingham
 */

#include "../input.h"
#include "../math.h"
#include "../graphics.h"
#include "camera.h"
#include "player.h"

#ifndef canvas_h
#define canvas_h

struct canvashdl
{
	playerhdl player;
	palettehdl palette;

	vec2i screen;

	bushdl devices;

	double frame_number;
	double frame_start_time;
	double real_current_time;
	double game_elapsed_time;
	double game_current_time;
	double game_time_multiplier;

	void initialize();
	void reshape(int w, int h);
	void release();

	void render();

	double handle_time();
};

#endif
