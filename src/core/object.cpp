#include "object.h"
#include "canvas.h"
#include "../graphics.h"

objecthdl::objecthdl()
{
	busy = true;
	mass = 0.0;
	inertia = 0.0;

	radius = 0.0;
	volume = 0.0;
	density = 0.0;

	depth2 = 0.0;

	origin = NULL;

	remove = false;
	busy = false;
	rendering = false;
	preparing = false;
}

objecthdl::~objecthdl()
{
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

	mass = 0.0;
	inertia = 0.0;

	radius = 0.0;
	volume = 0.0;
	density = 0.0;

	origin = NULL;

	depth2 = 0.0;

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

	for (objecthdl *object = begin(); object != end(); )
	{
		if (object->remove && !object->busy)
		{
			origin->object_info.release();
			origin->object_menu.release();
			object = erase(object);
		}
		else
			object = object->next;
	}

	if (parent != NULL && parent->parent != NULL)
	{
		distance = mag2((position - parent->position));
		if (distance > m_sqr(parent->radius))
		{
			origin->object_info.release();
			origin->object_menu.release();
			parent->parent->add_child(this);
		}
	}

	objecthdl *curr = parent != NULL ? parent->begin() : NULL;
	while (curr != NULL)
	{
		if (this != curr)
		{
			distance = mag2((position - curr->position));
			if (distance <= m_sqr(curr->radius) && curr->radius > radius)
			{
				origin->object_info.release();
				origin->object_menu.release();
				curr->add_child(this);
				curr = NULL;
			}
		}
		if (curr != NULL)
			curr = curr->next;
	}

	if (mass != 0.0)
		acceleration += forces/mass;
	forces = vec3f();

	velocity += acceleration;///(*framerate);
	position += velocity;///(*framerate);
	momentum = velocity*mass;
	if (origin != NULL && origin->player.camera != NULL)
		depth2 = mag2(position - origin->player.camera->position);

	acceleration = vec3f();

	if (inertia != 0.0)
		angular_acceleration += torques/inertia;
	torques = vec3f();

	angular_velocity += angular_acceleration;///(*framerate);
	orientation += angular_velocity;///(*framerate);
	angular_momentum = inertia*angular_velocity;

	angular_acceleration = vec3f();

	render_derived();

	busy = preparing;

	rendering = false;
}

void objecthdl::render_hierarchy()
{
	if (parent != NULL)
		parent->render_hierarchy();

	sort_children();

	for (objecthdl *object = begin(); object != end(); object = object->next)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		object->render();
	}
}

void objecthdl::applyforce(vec3f direction, vec3f position)
{
	forces += direction;
	torques += cross(position, direction);
}

void objecthdl::applytorque(vec3f direction)
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
	child->origin = origin;

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
	if (origin != NULL && origin->player.camera != NULL)
	{
		vec3F *camera_position = &origin->player.camera->position;

		objecthdl *next = children_s, *prev = NULL, *curr = NULL;
		while (next != NULL)
		{
			next->depth2 = mag2((next->position - *camera_position));

			curr = next;
			next = curr->next;
			prev = curr->prev;
			while (prev != NULL && prev->depth2 < curr->depth2)
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

void objecthdl::apply_orient_zyx()
{
	glRotated(orientation[2], 0.0, 0.0, 1.0);
	glRotated(orientation[1], 0.0, 1.0, 0.0);
	glRotated(orientation[0], 1.0, 0.0, 0.0);
}

void objecthdl::apply_orient_xyz()
{
	glRotated(orientation[0], 1.0, 0.0, 0.0);
	glRotated(orientation[1], 0.0, 1.0, 0.0);
	glRotated(orientation[2], 0.0, 0.0, 1.0);
}

void objecthdl::apply_orient_nzyx()
{
	glRotated(-orientation[2], 0.0, 0.0, 1.0);
	glRotated(-orientation[1], 0.0, 1.0, 0.0);
	glRotated(-orientation[0], 1.0, 0.0, 0.0);
}

void objecthdl::apply_orient_nxyz()
{
	glRotated(-orientation[0], 1.0, 0.0, 0.0);
	glRotated(-orientation[1], 0.0, 1.0, 0.0);
	glRotated(-orientation[2], 0.0, 0.0, 1.0);
}

void objecthdl::apply_pos()
{
	vec3f pos;
	if (origin != NULL && origin->player.camera != NULL)
		pos = (position - origin->player.camera->position);
	else
		pos = position;
	glTranslated(pos[0], pos[1], pos[2]);
}

void objecthdl::apply_pos_n()
{
	vec3f pos;
	if (origin != NULL && origin->player.camera != NULL)
		pos = -(position - origin->player.camera->position);
	else
		pos = -position;
	glTranslated(pos[0], pos[1], pos[2]);
}
