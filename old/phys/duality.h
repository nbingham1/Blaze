/*
 * duality.h
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
#include "frame.h"

#ifndef duality_h
#define duality_h

struct dualityhdl : generichdl
{
private:
	float _elasticity_;
	float _mass_;
	float _inertia_;
	float _charge_;
	vec4F _stresses_;
	vec4F _forces_;
	vec4f _torques_;

	framehdl	_frames_[p_max_frames];
	short		_active_frame_;

	pthread_rwlock_t _frame_lock_;
	pthread_mutex_t  _phys_mtx_;

public:
	dualityhdl();
	virtual ~dualityhdl();

	float elasticity();
	float mass();
	float inertia();
	float charge();

	vec4F scale();
	vec4F scalar_velocity();
	vec4F scalar_acceleration();
	vec4F position();
	vec4F linear_velocity();
	vec4F linear_acceleration();
	vec4f orientation();
	vec4f angular_velocity();
	vec4f angular_acceleration();

	vec4F absolute_scale();
	vec4F absolute_position();
	vec4f absolute_orientation();

	vec4F observed_scale();
	vec4F observed_position();
	vec4f observed_orientation();

	void init(float e, float m, float i, float c, vec4f s, vec4f p, vec4f o, float t);
	void update(float t);

	void pressure(vec4f p);
	void force(vec4f f, vec4f p);
	void torque(vec4f t);

	void force_field(vec4f f);
	void grav_field(vec4f f);
	void elec_field(vec4f f);
	void mag_field(vec4f f);

	void refract(float z1, float z2, vec4f n);
	void reflect(float z1, float z2, vec4f n);
	float diffract(float w, float n);

	void render(double t);
	void geometry(double t);
	void material(double t);
	void physics(double t);
	void chemistry(double t);
};

Float dist(dualityhdl *d1, dualityhdl *d2);
vec4F dir(dualityhdl *d1, dualityhdl *d2);
void grav_field(dualityhdl *d1, dualityhdl *d2);
void elec_field(dualityhdl *d1, dualityhdl *d2);
void mag_field(dualityhdl *d1, dualityhdl *d2);

#endif
