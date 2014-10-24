#include "../base.h"
#include "../math.h"

#ifndef object_h
#define object_h

struct canvashdl;

struct objecthdl : iltree<objecthdl>
{
	objecthdl();
	virtual ~objecthdl();

	double mass;
	double inertia;

	double radius;
	double volume;
	double density;

	double depth2;

	vec3F position;
	vec3F velocity;
	vec3F acceleration;
	vec3F momentum;
	vec3f orientation;
	vec3f angular_velocity;
	vec3f angular_acceleration;
	vec3f angular_momentum;

	vec3f forces;
	vec3f torques;

	canvashdl *origin;

	void initialize();
	void release();

	// These two functions have to be thread safe
	void prepare();
	void render();
	void render_hierarchy();

	void applyforce(vec3f direction, vec3f position);
	void applytorque(vec3f direction);

	void add_child(objecthdl *child);
	void remove_child(objecthdl *child);
	void sort_children();
	void release_children();

	// this group is relative to the parent objecthdl
	void apply_orient_zyx();
	void apply_orient_xyz();
	void apply_orient_nzyx();
	void apply_orient_nxyz();
	void apply_pos();
	void apply_pos_n();
};

#endif
