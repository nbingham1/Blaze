#include "../common.h"
#include "../bge_phys.h"
#include "../bge_gui.h"
#include "../bge_math.h"

#ifndef object_h
#define object_h

#define OBJ_BASE 0

struct objecthdl
{
private:
	virtual void initialize_derived() {type = OBJ_BASE;}
	virtual void release_derived() {}

	virtual void prepare_derived() {}
	virtual void render_derived() {}

public:
	objecthdl();
	virtual ~objecthdl();

	int type;

	// these should not be changed after initialization
	double mass;
	double inertia;

	double radius;
	double volume;
	double density;

	location position;
	location velocity;
	location acceleration;
	location momentum;
	vec orientation;
	vec angular_velocity;
	vec angular_acceleration;
	vec angular_momentum;

	vec size;

	vec forces;

	vec torques;

	void   *super;
	objecthdl *parent;

	objecthdl *children_s;	// start of list
	objecthdl *children_e;	// end of list

	objecthdl *next;
	objecthdl *prev;

	double camera_distance2;

	// gui
	bool remove;
	bool busy;
	bool rendering;
	bool preparing;

	bool selected;
	bool hovered;

	void initialize();
	void release();

	// These two functions have to be thread safe
	void prepare();
	void render();
	void render_hierarchy();

	void applyforce(vec direction, vec position);
	void applytorque(vec direction);

	void add_child(objecthdl *child);
	void remove_child(objecthdl *child);
	void sort_children();
	void release_children();

	// debug gui
	virtual void generate_info(containerhdl *contain);
	virtual void generate_menu(containerhdl *contain);
	virtual bool select(vec coord);
	virtual bool hover(vec coord);

	// this group is relative to the parent objecthdl
	void apply_orient_zyx();
	void apply_orient_xyz();
	void apply_orient_nzyx();
	void apply_orient_nxyz();
	void apply_scale();
	void apply_scale_n();
	void apply_pos();
	void apply_pos_n();
};

#endif
