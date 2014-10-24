#include "object.h"
#include "canvas.h"
#include "../bge_mtrl.h"

objecthdl::objecthdl()
{
	busy = true;
	mass = 0.0;
	inertia = 0.0;

	radius = 0.0;
	volume = 0.0;
	density = 0.0;

	camera_distance2 = 0.0;

	super = NULL;
	parent = NULL;

	children_s = NULL;
	children_e = NULL;

	next = NULL;
	prev = NULL;

	size = vec(1.0, 1.0, 1.0, 1.0);

	selected = false;
	hovered = false;
	remove = false;
	busy = false;
	rendering = false;
	preparing = false;
}

objecthdl::~objecthdl()
{
	release();
}

void objecthdl::initialize()
{
	if (remove)
		return;

	bool temp = busy;
	busy = true;

	initialize_derived();

	busy = temp || rendering || preparing;
}

void objecthdl::release()
{
	bool temp = busy;
	busy = true;

	objecthdl *curr = children_s, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		if (prev != ((canvashdl*)super)->active_camera)
		{
			prev->release();
			delete prev;
			prev = NULL;
		}
		else
		{
			prev->parent = NULL;
			prev->next = NULL;
			prev->prev = NULL;
			if (parent != NULL && !parent->remove)
				parent->add_child(prev);
			else
				((canvashdl*)super)->addobject(prev);
		}
	}

	mass = 0.0;
	inertia = 0.0;

	radius = 0.0;
	volume = 0.0;
	density = 0.0;

	super = NULL;
	parent = NULL;

	children_s = NULL;
	children_e = NULL;

	camera_distance2 = 0.0;

	next = NULL;
	prev = NULL;

	size = vec(1.0, 1.0, 1.0, 1.0);

	selected = false;
	hovered = false;

	release_derived();

	busy = temp || rendering || preparing;
}

void objecthdl::prepare()
{
	if (remove)
		return;

	preparing = true;

	busy = true;

	prepare_derived();

	busy = rendering;
	preparing = false;
}

void objecthdl::render()
{
	if (remove)
		return;

	rendering = true;

	busy = true;

	double distance;

	objecthdl *curr = children_s, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		if (prev->remove && !prev->busy)
		{
			((canvashdl*)super)->object_info.release();
			((canvashdl*)super)->object_menu.release();
			remove_child(prev);
			prev->release();
			delete prev;
		}
	}

	if (parent != NULL && parent->parent != NULL)
	{
		distance = mag2((position - parent->position).tovec());
		if (distance > m_sqr(parent->radius))
		{
			((canvashdl*)super)->object_info.release();
			((canvashdl*)super)->object_menu.release();
			parent->parent->add_child(this);
		}
	}

	curr = parent != NULL ? parent->children_s : NULL;
	while (curr != NULL)
	{
		if (this != curr)
		{
			distance = mag2((position - curr->position).tovec());
			if (distance <= m_sqr(curr->radius) && curr->radius > radius)
			{
				((canvashdl*)super)->object_info.release();
				((canvashdl*)super)->object_menu.release();
				curr->add_child(this);
				curr = NULL;
			}
		}
		if (curr != NULL)
			curr = curr->next;
	}

	if (mass != 0.0)
		acceleration += forces/mass;
	forces = vec();

	velocity += acceleration;///(*framerate);
	position += velocity;///(*framerate);
	momentum = velocity*mass;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		camera_distance2 = mag2((position - ((canvashdl*)super)->active_camera->position).tovec());

	acceleration = vec();

	if (inertia != 0.0)
		angular_acceleration += torques/inertia;
	torques = vec();

	angular_velocity += angular_acceleration;///(*framerate);
	orientation += angular_velocity;///(*framerate);
	angular_momentum = inertia*angular_velocity;

	angular_acceleration = vec();

	render_derived();

	busy = preparing;

	rendering = false;
}

void objecthdl::render_hierarchy()
{
	if (parent != NULL)
		parent->render_hierarchy();

	sort_children();

	objecthdl *curr = children_s;
	while (curr != NULL)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		curr->render();
		curr = curr->next;
	}
}

void objecthdl::applyforce(vec direction, vec position)
{
	forces += direction;
	torques += cross(position, direction);
}

void objecthdl::applytorque(vec direction)
{
	torques += direction;
}

void objecthdl::add_child(objecthdl *child)
{
	if (remove)
	{
		child->release();
		delete child;
	}

	bool temp = busy;
	busy = true;

	if (child->parent != NULL)
		child->parent->remove_child(child);

	child->parent = this;
	child->super = super;

	if (children_s == NULL)
	{
		children_s = child;
		children_e = child;
	}
	else
	{
		children_e->next = child;
		child->prev = children_e;
		children_e = children_e->next;
	}

	busy = temp || rendering || preparing;
}

void objecthdl::remove_child(objecthdl *child)
{
	bool temp = busy;
	busy = true;

	if (child->parent == this)
	{
		child->parent = NULL;

		if (child == children_s)
			children_s = children_s->next;

		if (child == children_e)
			children_e = children_e->prev;

		if (child->next != NULL)
			child->next->prev = child->prev;

		if (child->prev != NULL)
			child->prev->next = child->next;

		child->next = NULL;
		child->prev = NULL;
	}

	busy = temp || rendering || preparing;
}

void objecthdl::sort_children()
{
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
	{
		location *camera_position = &((canvashdl*)super)->active_camera->position;

		objecthdl *next = children_s, *prev = NULL, *curr = NULL;
		while (next != NULL)
		{
			next->camera_distance2 = mag2((next->position - *camera_position).tovec());

			curr = next;
			next = curr->next;
			prev = curr->prev;
			while (prev != NULL && prev->camera_distance2 < curr->camera_distance2)
				prev = prev->prev;

			if (prev != NULL && children_s != curr)
			{
				if (curr->next)
					curr->next->prev = curr->prev;
				if (curr->prev)
					curr->prev->next = curr->next;

				if (children_e == curr)
					children_e = curr->prev;

				curr->next = prev->next;
				curr->prev = prev;

				if (prev->next)
					prev->next->prev = curr;
				prev->next = curr;

				if (children_e->next != NULL)
					children_e = children_e->next;
			}
			else if (children_s != curr)
			{
				if (curr->next)
					curr->next->prev = curr->prev;
				if (curr->prev)
					curr->prev->next = curr->next;

				if (children_e == curr)
					children_e = curr->prev;

				curr->next = children_s;
				curr->prev = NULL;
				children_s->prev = curr;
				children_s = curr;
			}
		}
	}
}

void objecthdl::release_children()
{
	bool temp = busy;
	busy = true;

	objecthdl *curr = children_s, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		prev->release();
		delete prev;
		prev = NULL;
	}

	children_s = NULL;
	children_e = NULL;

	busy = temp || rendering || preparing;
}

void objecthdl::generate_info(containerhdl *contain)
{
	if (remove)
		return;

	bool temp = busy;
	busy = true;

	valuehdl *mass_value = new valuehdl();
	mass_value->init((char*)"Mass:", &mass, double_ty);
	valuehdl *inertia_value = new valuehdl();
	inertia_value->init((char*)"Inertia:", &inertia, double_ty);
	valuehdl *radius_value = new valuehdl();
	radius_value->init((char*)"Radius:", &radius, double_ty);
	valuehdl *volume_value = new valuehdl();
	volume_value->init((char*)"Volume:", &volume, double_ty);
	valuehdl *density_value = new valuehdl();
	density_value->init((char*)"Density:", &density, double_ty);

	valuehdl *position_value = new valuehdl();
	position_value->init((char*)"Position:", &position, location_ty);
	valuehdl *velocity_value = new valuehdl();
	velocity_value->init((char*)"Velocity:", &velocity, location_ty);
	valuehdl *acceleration_value = new valuehdl();
	acceleration_value->init((char*)"Acceleration:", &acceleration, location_ty);

	valuehdl *orientation_value = new valuehdl();
	orientation_value->init((char*)"Orientation:", &orientation, vec_ty);
	valuehdl *avelocity_value = new valuehdl();
	avelocity_value->init((char*)"Angular Velocity:", &angular_velocity, vec_ty);
	valuehdl *aaccelleration_value = new valuehdl();
	aaccelleration_value->init((char*)"Angular Acceleration:", &angular_acceleration, vec_ty);

	valuehdl *size_value = new valuehdl();
	size_value->init((char*)"Size:", &size, vec_ty);

	contain->addobj(mass_value, value_t);
	contain->addobj(inertia_value, value_t);
	contain->addobj(radius_value, value_t);
	contain->addobj(volume_value, value_t);
	contain->addobj(density_value, value_t);
	contain->addobj(position_value, value_t);
	contain->addobj(velocity_value, value_t);
	contain->addobj(acceleration_value, value_t);
	contain->addobj(orientation_value, value_t);
	contain->addobj(avelocity_value, value_t);
	contain->addobj(aaccelleration_value, value_t);
	contain->addobj(size_value, value_t);

	busy = temp || rendering || preparing;
}

void objecthdl::generate_menu(containerhdl *contain)
{
	if (remove)
		return;

	bool temp = busy;
	busy = true;

	checkboxhdl *remove_box = new checkboxhdl();
	remove_box->init((char*)"Delete This Object", &remove);

	contain->addobj(remove_box, checkbox_t);

	busy = temp || rendering || preparing;
}

bool objecthdl::select(vec coord)
{
	if (remove)
		return false;

	bool temp = busy;
	busy = true;

	location pos = position;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		pos -= ((canvashdl*)super)->active_camera->position;

	if (sphere(pos.tovec(), radius).intersect_ray(coord, NULL, NULL))
	{
		selected = !selected;
		return true;
	}
	selected = false;

	busy = temp || rendering || preparing;

	return false;
}

bool objecthdl::hover(vec coord)
{
	if (remove)
		return false;

	bool temp = busy;
	busy = true;

	location pos = position;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		pos -= ((canvashdl*)super)->active_camera->position;

	if (sphere(pos.tovec(), radius).intersect_ray(coord, NULL, NULL))
	{
		hovered = true;
		return true;
	}
	hovered = false;

	busy = temp || rendering || preparing;

	return false;
}

void objecthdl::apply_orient_zyx()
{
	glRotated(orientation.z, 0.0, 0.0, 1.0);
	glRotated(orientation.y, 0.0, 1.0, 0.0);
	glRotated(orientation.x, 1.0, 0.0, 0.0);
}

void objecthdl::apply_orient_xyz()
{
	glRotated(orientation.x, 1.0, 0.0, 0.0);
	glRotated(orientation.y, 0.0, 1.0, 0.0);
	glRotated(orientation.z, 0.0, 0.0, 1.0);
}

void objecthdl::apply_orient_nzyx()
{
	glRotated(-orientation.z, 0.0, 0.0, 1.0);
	glRotated(-orientation.y, 0.0, 1.0, 0.0);
	glRotated(-orientation.x, 1.0, 0.0, 0.0);
}

void objecthdl::apply_orient_nxyz()
{
	glRotated(-orientation.x, 1.0, 0.0, 0.0);
	glRotated(-orientation.y, 0.0, 1.0, 0.0);
	glRotated(-orientation.z, 0.0, 0.0, 1.0);
}

void objecthdl::apply_scale()
{
	glScalef(size.x, size.y, size.z);
}

void objecthdl::apply_scale_n()
{
	glScalef(1.0/size.x, 1.0/size.y, 1.0/size.z);
}

void objecthdl::apply_pos()
{
	vec pos;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		pos = (position - ((canvashdl*)super)->active_camera->position).tovec();
	else
		pos = position.tovec();
	glTranslated(pos.x, pos.y, pos.z);
}

void objecthdl::apply_pos_n()
{
	vec pos;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		pos = -(position - ((canvashdl*)super)->active_camera->position).tovec();
	else
		pos = -position.tovec();
	glTranslated(pos.x, pos.y, pos.z);
}
