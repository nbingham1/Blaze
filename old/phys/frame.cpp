/*
 * frame.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on January 21, 2012.
 * Modified by Ned Bingham on January 21, 2012.
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

#include "frame.h"

framehdl::framehdl()
{
	_gamma_ = 0.0;
	_time_  = 0.0;

	_scale_					= vec4F();
	_scalar_velocity_		= vec4F();
	_scalar_acceleration_	= vec4F();
	_position_				= vec4F();
	_linear_velocity_		= vec4F();
	_linear_acceleration_	= vec4F();
	_orientation_			= vec4f();
	_angular_velocity_		= vec4f();
	_angular_acceleration_	= vec4f();

	pthread_rwlock_init(&_lock_, NULL);
}

framehdl::~framehdl()
{
	_gamma_ = 0.0;
	_time_  = 0.0;

	_scale_					= vec4F();
	_scalar_velocity_		= vec4F();
	_scalar_acceleration_	= vec4F();
	_position_				= vec4F();
	_linear_velocity_		= vec4F();
	_linear_acceleration_	= vec4F();
	_orientation_			= vec4f();
	_angular_velocity_		= vec4f();
	_angular_acceleration_	= vec4f();

	pthread_rwlock_destroy(&_lock_);
}

vec4F framehdl::scale()
{
	vec4F ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _scale_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4F framehdl::scalar_velocity()
{
	vec4F ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _scalar_velocity_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4F framehdl::scalar_acceleration()
{
	vec4F ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _scalar_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4F framehdl::position()
{
	vec4F ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _position_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4F framehdl::linear_velocity()
{
	vec4F ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _linear_velocity_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4F framehdl::linear_acceleration()
{
	vec4F ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _linear_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4f framehdl::orientation()
{
	vec4f ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _orientation_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4f framehdl::angular_velocity()
{
	vec4f ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _angular_velocity_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

vec4f framehdl::angular_acceleration()
{
	vec4f ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _angular_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

double framehdl::time()
{
	double ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _time_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

double framehdl::gamma()
{
	double ret;
	pthread_rwlock_rdlock(&_lock_);
	ret = _gamma_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

mat<Float,9,4> framehdl::frame()
{
	//printf("defining matrix\n");
	mat<Float,9,4> ret;
	printf("matrix!\n");
	pthread_rwlock_rdlock(&_lock_);
	printf("scale\n");
	ret.data[0] = _scale_;
	printf("scalar_vel\n");
	ret.data[1] = _scalar_velocity_;
	printf("scalar_acc\n");
	ret.data[2] = _scalar_acceleration_;
	printf("position\n");
	ret.data[3] = _position_;
	printf("linear_vel\n");
	ret.data[4] = _linear_velocity_;
	printf("linear_acc\n");
	ret.data[5] = _linear_acceleration_;
	printf("orientation\n");
	ret.data[6] = _orientation_;
	printf("angular_vel\n");
	ret.data[7] = _angular_velocity_;
	printf("angular_acc\n");
	ret.data[8] = _angular_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

mat<Float,3,4> framehdl::sframe()
{
	mat<Float,3,4> ret;
	pthread_rwlock_rdlock(&_lock_);
	ret.data[0] = _scale_;
	ret.data[1] = _scalar_velocity_;
	ret.data[2] = _scalar_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

mat<Float,3,4> framehdl::pframe()
{
	mat<Float,3,4> ret;
	pthread_rwlock_rdlock(&_lock_);
	ret.data[0] = _position_;
	ret.data[1] = _linear_velocity_;
	ret.data[2] = _linear_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

mat<float,3,4> framehdl::rframe()
{
	mat<float,3,4> ret;
	pthread_rwlock_rdlock(&_lock_);
	ret.data[0] = _orientation_;
	ret.data[1] = _angular_velocity_;
	ret.data[2] = _angular_acceleration_;
	pthread_rwlock_unlock(&_lock_);
	return ret;
}

void framehdl::init(vec4F scale, vec4F position, vec4f orientation, double time)
{
	pthread_rwlock_wrlock(&_lock_);
	_time_ = time;
	_scale_ = scale;
	_position_ = position;
	_orientation_ = orientation;
	pthread_rwlock_unlock(&_lock_);
}

void framehdl::copy(framehdl *frame)
{
	mat<Float,9,4>	f = frame->frame();
	double			t = frame->time();
	double			g = frame->gamma();

	pthread_rwlock_wrlock(&_lock_);
	_scale_					= f[0];
	_scalar_velocity_ 		= f[1];
	_scalar_acceleration_	= f[2];
	_position_				= f[3];
	_linear_velocity_ 		= f[4];
	_linear_acceleration_	= f[5];
	_orientation_			= f[6];
	_angular_velocity_		= f[7];
	_angular_acceleration_	= f[8];
	_time_					= t;
	_gamma_					= g;
	pthread_rwlock_unlock(&_lock_);
}

void framehdl::update(vec4F scalar_acceleration, vec4F linear_acceleration, vec4f angular_acceleration, double time)
{
	pthread_rwlock_wrlock(&_lock_);
	float elapsed = time - _time_;
	_time_ = time;

	_scalar_acceleration_	+= scalar_acceleration;
	_scalar_velocity_		+= _scalar_acceleration_*elapsed;
	_scale_					+= _scalar_velocity_*elapsed;

	_linear_acceleration_	+= linear_acceleration;
	_linear_velocity_		+= _linear_acceleration_*elapsed;
	_position_				+= _linear_velocity_*elapsed;

	_linear_acceleration_ = vec4F();
	_gamma_ = ((Float)(1.0/sqrt(1.0 - mag2(_linear_velocity_)/(p_c0*p_c0))));

	_angular_acceleration_	+= angular_acceleration;
	_angular_velocity_		+= _angular_acceleration_*elapsed;
	_orientation_			+= _angular_velocity_*elapsed;

	_angular_acceleration_ = vec4f();
	pthread_rwlock_unlock(&_lock_);
}

void framehdl::copy_update(framehdl *frame, vec4F scalar_acceleration, vec4F linear_acceleration, vec4f angular_acceleration, double time)
{
	printf("frame\n");
	mat<Float,9,4>	f = frame->frame();
	printf("time\n");
	double			t = frame->time();
	printf("gamma\n");
	double			g = frame->gamma();

	printf("frame retrieved\n");

	pthread_rwlock_wrlock(&_lock_);
	_scale_					= f[0];
	_scalar_velocity_ 		= f[1];
	_scalar_acceleration_	= f[2];
	_position_				= f[3];
	_linear_velocity_ 		= f[4];
	_linear_acceleration_	= f[5];
	_orientation_			= f[6];
	_angular_velocity_		= f[7];
	_angular_acceleration_	= f[8];
	_time_					= t;
	_gamma_					= g;

	float elapsed = time - _time_;
	_time_ = time;

	printf("updated frame\n");

	_scalar_acceleration_	+= scalar_acceleration;
	_scalar_velocity_		+= _scalar_acceleration_*elapsed;
	_scale_					+= _scalar_velocity_*elapsed;

	_linear_acceleration_	+= linear_acceleration;
	_linear_velocity_		+= _linear_acceleration_*elapsed;
	_position_				+= _linear_velocity_*elapsed;

	_linear_acceleration_ = vec4F();
	_gamma_ = Float(1.0/sqrt(1.0 - mag2(_linear_velocity_)/(p_c0*p_c0)));

	_angular_acceleration_	+= angular_acceleration;
	_angular_velocity_		+= _angular_acceleration_*elapsed;
	_orientation_			+= _angular_velocity_*elapsed;

	_angular_acceleration_ = vec4f();
	pthread_rwlock_unlock(&_lock_);
}
