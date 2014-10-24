/*
 * duality.cpp
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

#include "duality.h"

dualityhdl::dualityhdl()
{
	_elasticity_	= m_epsilon;
	_mass_			= m_epsilon;
	_inertia_		= m_epsilon;
	_charge_		= 0.0;

	_stresses_		= vec4F();
	_forces_		= vec4F();
	_torques_		= vec4f();

	pthread_mutex_init(&_phys_mtx_, NULL);
	pthread_rwlock_init(&_frame_lock_, NULL);
}

dualityhdl::~dualityhdl()
{
	_elasticity_	= m_epsilon;
	_mass_			= m_epsilon;
	_inertia_		= m_epsilon;
	_charge_		= 0.0;

	_stresses_		= vec4F();
	_forces_		= vec4F();
	_torques_		= vec4f();
}

float dualityhdl::elasticity()
{
	return _elasticity_;
}

float dualityhdl::mass()
{
	return _mass_;
}

float dualityhdl::inertia()
{
	return _inertia_;
}

float dualityhdl::charge()
{
	return _charge_;
}

vec4F dualityhdl::scale()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].scale();
}

vec4F dualityhdl::scalar_velocity()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].scalar_velocity();
}

vec4F dualityhdl::scalar_acceleration()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].scalar_acceleration();
}

vec4F dualityhdl::position()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].position();
}

vec4F dualityhdl::linear_velocity()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].linear_velocity();
}

vec4F dualityhdl::linear_acceleration()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].linear_acceleration();
}

vec4f dualityhdl::orientation()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].orientation();
}

vec4f dualityhdl::angular_velocity()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].angular_velocity();
}

vec4f dualityhdl::angular_acceleration()
{
	short id = 0;
	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	pthread_rwlock_unlock(&_frame_lock_);

	return _frames_[id].angular_acceleration();
}

vec4F dualityhdl::absolute_scale()
{
	if (parent != NULL)
		return ((dualityhdl*)parent)->absolute_scale()+scale();
	else
		return position();
}

vec4F dualityhdl::absolute_position()
{
	if (parent != NULL)
		return ((dualityhdl*)parent)->absolute_position()+position();
	else
		return position();
}

vec4f dualityhdl::absolute_orientation()
{
	if (parent != NULL)
		return ((dualityhdl*)parent)->absolute_orientation()+orientation();
	else
		return orientation();
}

void dualityhdl::init(float e, float m, float i, float c, vec4f s, vec4f p, vec4f o, float t)
{
	short id = 0;

	_elasticity_ = e <= 0.0 ? m_epsilon : e;
	_mass_ = m <= 0.0 ? m_epsilon : m;
	_inertia_ = i <= 0.0 ? m_epsilon : i;
	_charge_ = c;

	pthread_rwlock_rdlock(&_frame_lock_);
	id = _active_frame_;
	_frames_[id].init(s, p, o, t);
	pthread_rwlock_unlock(&_frame_lock_);
}

void dualityhdl::update(float t)
{
	printf("updating physics\n");
	pthread_mutex_lock(&_phys_mtx_);
	pthread_rwlock_rdlock(&_frame_lock_);
	printf("unlocked\n");
	short next_frame = (_active_frame_+1)%p_max_frames;
	printf("next frame %f %f %f\n", _elasticity_, _mass_, _inertia_);
	_frames_[next_frame].copy_update(&_frames_[_active_frame_], _stresses_/_elasticity_, _forces_/_mass_, _torques_/_inertia_, t);
	printf("updated\n");
	pthread_rwlock_unlock(&_frame_lock_);

	_stresses_  = vec4F();
	_forces_	= vec4F();
	_torques_	= vec4f();
	pthread_mutex_unlock(&_phys_mtx_);

	pthread_rwlock_wrlock(&_frame_lock_);
	printf("next frame\n");
	_active_frame_ = next_frame;
	pthread_rwlock_unlock(&_frame_lock_);
}

void dualityhdl::pressure(vec4f p)
{
	pthread_mutex_lock(&_phys_mtx_);
	_stresses_ += p;
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::force(vec4f f, vec4f p)
{
	pthread_mutex_lock(&_phys_mtx_);
	_forces_ += f;
	_torques_ += cross(p, f);
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::torque(vec4f t)
{
	pthread_mutex_lock(&_phys_mtx_);
	_torques_ += t;
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::force_field(vec4f f)
{
	pthread_mutex_lock(&_phys_mtx_);
	_forces_ += f;
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::grav_field(vec4f f)
{
	vec4F applied = mass()*f;

	pthread_mutex_lock(&_phys_mtx_);
	_forces_ += applied;
	//_stresses_ += applied;// /force_normal_area
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::elec_field(vec4f f)
{
	vec4F applied = charge()*f;

	pthread_mutex_lock(&_phys_mtx_);
	_forces_ += applied;
	//_stresses_ += applied;// /force_normal_area
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::mag_field(vec4f f)
{
	vec4F applied = charge()*cross(linear_velocity(), f);

	pthread_mutex_lock(&_phys_mtx_);
	_forces_ += applied;
	//_stresses_ += applied;// /force_normal_area
	pthread_mutex_unlock(&_phys_mtx_);
}

void dualityhdl::refract(float z1, float z2, vec4f n)
{

}

void dualityhdl::reflect(float z1, float z2, vec4f n)
{

}

float dualityhdl::diffract(float w, float n)
{
	return 0.0;
}

void dualityhdl::render(double t)
{

}

void dualityhdl::geometry(double t)
{

}

void dualityhdl::material(double t)
{

}

void dualityhdl::physics(double t)
{
	update(t);
}

void dualityhdl::chemistry(double t)
{

}


Float dist(dualityhdl *d1, dualityhdl *d2)
{
	return dist(d1->position(), d2->position());
}

vec4F dir(dualityhdl *d1, dualityhdl *d2)
{
	return dir(d1->position(), d2->position());
}

void grav_field(dualityhdl *d1, dualityhdl *d2)
{
	vec4F p1 = d1->position(), p2 = d2->position();
	Float d_2 = dist2(p1, p2);
	Float f = (p_g*d1->mass()*d2->mass())/(d_2);
	vec4F r = (p2 - p1)/sqrt(d_2);
	d1->force_field(r*f);
	d2->force_field(-r*f);
}

void elec_field(dualityhdl *d1, dualityhdl *d2)
{
	vec4F p1 = d1->position(), p2 = d2->position();
	Float d_2 = dist2(p1, p2);
	Float f = -(d1->charge()*d2->charge())/(4*m_pi*p_epsilon0*d_2);
	vec4F r = (p2 - p1)/sqrt(d_2);
	d1->force_field(r*f);
	d2->force_field(-r*f);
}

void mag_field(dualityhdl *d1, dualityhdl *d2)
{
	vec4F p1 = d1->position(), p2 = d2->position(), v1 = d1->linear_velocity(), v2 = d2->linear_velocity();
	Float d_2 = dist2(p1, p2);
	vec4F r = (p2 - p1)/sqrt(d_2);
	vec4F B1 = (p_mu0*d1->charge()*cross(v1, r))/(4*m_pi*d_2);
	vec4F B2 = (p_mu0*d2->charge()*cross(v2, r))/(4*m_pi*d_2);
	vec4F f1 = d1->charge()*cross(v1, B2);
	vec4F f2 = d2->charge()*cross(v2, B1);
	d1->force_field(f1);
	d2->force_field(f2);
}
