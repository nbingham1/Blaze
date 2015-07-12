/*
 * controller.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on December 7, 2011.
 * Modified by Ned Bingham on December 7, 2011.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller.h"

controllerhdl::controllerhdl()
{
	flags = 0;
}

controllerhdl::controllerhdl(int num_axes)
{
	flags = 0;
	axes.resize(num_axes);
}

controllerhdl::~controllerhdl()
{
}

void controllerhdl::update(double real_current_time, double game_current_time)
{
	for (int i = 0; i < buttons.pressed.size(); i++)
	{
		map<int, preference>::iterator j = buttons.control.find(buttons.pressed[i]);
		if (j != buttons.control.end())
			j->value(vec3f(1.0f, 0.0f, 0.0f), real_current_time, game_current_time);
	}

	for (int i = 0; i < axes.size(); i++)
		axes[i].move(0, real_current_time, game_current_time);
}
