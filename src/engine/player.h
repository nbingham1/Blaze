/*
 * player.h
 *
 *  Created on: Nov 11, 2014
 *      Author: nbingham
 */

#include "camera.h"
#include "../input/controller.h"

#ifndef player_h
#define player_h

struct playerhdl
{
	playerhdl();
	~playerhdl();

	camerahdl *camera;
	vec4f viewport;
	double speed;
	double sensitivity;

	void view(framehdl &frame, vec2i screen);

	float view_area();
	float view_width();
	float view_height();

	// Keyboard
	static void forward(preference *pref, vec3f value);
	static void backward(preference *pref, vec3f value);
	static void left(preference *pref, vec3f value);
	static void right(preference *pref, vec3f value);
	static void up(preference *pref, vec3f value);
	static void down(preference *pref, vec3f value);
	static void accelerate(preference *pref, vec3f value);
	static void deccelerate(preference *pref, vec3f value);
	static void stop(preference *pref, vec3f value);

	static void horizontal(preference *pref, vec3f value);
	static void vertical(preference *pref, vec3f value);
};

#endif
