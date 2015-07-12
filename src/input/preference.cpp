/*
 * preference.cpp
 *
 *  Created on: May 16, 2015
 *      Author: nbingham
 */

#include "preference.h"
#include "core/list.h"

preference::preference()
{
	data = NULL;
	function = NULL;
	game_current_time = 0.0;
	real_current_time = 0.0;
	game_last_time = 0.0;
	real_last_time = 0.0;
}

preference::preference(void *data, void (*function)(preference *, vec3f))
{
	game_current_time = 0.0;
	real_current_time = 0.0;
	game_last_time = 0.0;
	real_last_time = 0.0;
	this->data = data;
	this->function = function;
}

preference::~preference()
{

}

void preference::operator()(vec3f value, double real_current_time, double game_current_time)
{
	this->real_last_time = this->real_current_time;
	this->game_last_time = this->game_current_time;
	this->real_current_time = real_current_time;
	this->game_current_time = game_current_time;
	if (function != NULL)
		function(this, value);
}

bool preference::is_valid()
{
	return function != NULL;
}
