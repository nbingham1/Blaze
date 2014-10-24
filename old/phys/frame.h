/*
 * frame.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on January 15, 2012.
 * Modified by Ned Bingham on January 15, 2012.
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

#include "../math.h"
#include "../common.h"

#include "constants.h"

#ifndef frame_h
#define frame_h

/* framehdl
 *
 * This structure defines a reference frame that
 * specifies absolute place and time.
 */
struct framehdl
{
private:
	double _gamma_; // relativistic
	double _time_;

	vec4F  _scale_;
	vec4F  _scalar_velocity_;
	vec4F  _scalar_acceleration_;
	vec4F  _position_;
	vec4F  _linear_velocity_;
	vec4F  _linear_acceleration_;
	vec4f  _orientation_;
	vec4f  _angular_velocity_;
	vec4f  _angular_acceleration_;

	pthread_rwlock_t _lock_;

public:
	framehdl();
	~framehdl();

	vec4F scale();
	vec4F scalar_velocity();
	vec4F scalar_acceleration();
	vec4F position();
	vec4F linear_velocity();
	vec4F linear_acceleration();
	vec4f orientation();
	vec4f angular_velocity();
	vec4f angular_acceleration();
	double time();
	double gamma();

	mat<Float,9,4> frame();
	mat<Float,3,4> sframe();
	mat<Float,3,4> pframe();
	mat<float,3,4> rframe();

	void init(vec4F scale, vec4F position, vec4f orientation, double time);
	void copy(framehdl *frame);
	void update(vec4F scalar_acceleration, vec4F linear_acceleration, vec4f angular_acceleration, double time);
	void copy_update(framehdl *frame, vec4F scalar_acceleration, vec4F linear_acceleration, vec4f angular_acceleration, double time);
};

#endif
