/*
 * planet.cpp
 *
 *  Created on: Nov 26, 2013
 *      Author: nbingham
 */

#include "planet.h"
#include "canvas.h"
#include "../graphics.h"

void render_vertex::copy(render_vertex v)
{
	this->v[0] = v.v[0];
	this->v[1] = v.v[1];
	this->v[2] = v.v[2];
	this->n[0] = v.n[0];
	this->n[1] = v.n[1];
	this->n[2] = v.n[2];
	this->t[0] = v.t[0];
	this->t[1] = v.t[1];
	this->t[2] = v.t[2];
	this->g[0] = v.g[0];
	this->g[1] = v.g[1];
	this->dn[0] = v.dn[0];
	this->dn[1] = v.dn[1];
	this->dn[2] = v.dn[2];
}

void render_vertex::copyshift(render_vertex v, float *voff)
{
	this->v[0] = v.v[0] + voff[0];
	this->v[1] = v.v[1] + voff[1];
	this->v[2] = v.v[2] + voff[2];
	this->n[0] = v.n[0];
	this->n[1] = v.n[1];
	this->n[2] = v.n[2];
	this->t[0] = v.t[0];
	this->t[1] = v.t[1];
	this->t[2] = v.t[2];
	this->g[0] = v.g[0];
	this->g[1] = v.g[1];
	this->dn[0] = v.dn[0];
	this->dn[1] = v.dn[1];
	this->dn[2] = v.dn[2];
}

planet_buffer::planet_buffer(int off)
{
	part = NULL;
	offset = off;
}

planet_buffer::~planet_buffer()
{
	part = NULL;
	offset = 0;
}

planet_part::planet_part()
{
	a1 = 0.0;
	a2 = 0.0;
	a3 = 0.0;
	min = 0.0;
	max = 0.0;
	radius = 0.0;
	level = 0;
	polarity = 1;
	sibling_number = 0;
	interpolator = 0.0;
	cull = false;
	origin = NULL;
	neighbors[0] = NULL;
	neighbors[1] = NULL;
	neighbors[2] = NULL;
	neighbor_type[0] = 0;
	neighbor_type[1] = 0;
	neighbor_type[2] = 0;
	buffer = NULL;
}

planet_part::~planet_part()
{
	release();
}

void planet_part::initialize(planethdl *org, vec3F vert1, vec3F vert2, vec3F vert3, planet_part *n1, int nt1, planet_part *n2, int nt2, planet_part *n3, int nt3)
{
	origin = org;
	origin->allocate_part(this);
	if (buffer == NULL)
	{
		printf("Error: Not enough space allocated for base planet structure.\n");
		exit(1);
	}
	v1 = vert1;
	v2 = vert2;
	v3 = vert3;
	neighbors[0] = n1;
	neighbors[1] = n2;
	neighbors[2] = n3;
	neighbor_type[0] = nt1;
	neighbor_type[1] = nt2;
	neighbor_type[2] = nt3;

	center = (v1 + v2 + v3)/3.0;

	nv1 = v1/origin->boundary,
	nv2 = v2/origin->boundary,
	nv3 = v3/origin->boundary;
	ncenter = norm(center);

	ntangent = norm(cross(ncenter, nv1));

	radius = acos(dot(nv1, ncenter));
	interpolator = 1.0;

	a1 = acos(dot(nv1, nv2));
	a2 = acos(dot(nv2, nv3));
	a3 = acos(dot(nv3, nv1));

	// vertex generation
	int x = 0, y = 0, count = 0;
	double x_vps_ratio = 0.0, y_x_ratio = 0.0, angle = 0.0;
	double sin_a1 = sin(a1), sin_a3 = sin(a3), sin_angle = 0.0;
	vec3F v1_v2, v1_v3;
	vec3F vector;
	vec3f final_vector;
	vec3f normal;
	double magnitude;
	vec3f lnormal;

	grad3f height;

	int offset = buffer->offset*verts_per_part;

	for (x = 0; x < verts_per_side; x++)
	{
		x_vps_ratio = double(x)/double(verts_per_side-1);
		v1_v2 = v1*(sin((1.0-x_vps_ratio)*a1)/sin_a1) + v2*(sin(x_vps_ratio*a1)/sin_a1);
		v1_v3 = v1*(sin((1.0-x_vps_ratio)*a3)/sin_a3) + v3*(sin(x_vps_ratio*a3)/sin_a3);
		angle = acos(dot(v1_v2/origin->boundary, v1_v3/origin->boundary));
		sin_angle = sin(angle);

		for (y = 0; y < x+1; y++)
		{
			y_x_ratio = x != 0 ? double(y)/double(x) : 0.0;
			count = (x*x + x)/2 + y;
			if (x_vps_ratio == 0.0)
				vector = v1;
			else
				vector = (v1_v2*(sin((1-y_x_ratio)*angle)/sin_angle) + v1_v3*(sin(y_x_ratio*angle)/sin_angle));

			magnitude = mag(vector);
			normal = (vector/magnitude);
			final_vector = (vector*(origin->boundary/magnitude) - center);

			for (int z = 0; z < 3; z++)
				origin->geometry[offset + count].v[z] = final_vector[z];

			height = origin->terrain(vector);
			origin->geometry[offset + count].g[0] = height[3];
			origin->geometry[offset + count].g[1] = origin->geometry[offset + count].g[0];

			lnormal = height.normal(normal, ntangent);

			origin->geometry[offset + count].dn[0] = lnormal[0];
			origin->geometry[offset + count].dn[1] = lnormal[1];
			origin->geometry[offset + count].dn[2] = lnormal[2];

			final_vector = (vector*(origin->boundary/magnitude));

			for (int z = 0; z < 3; z++)
				origin->geometry[offset + count].n[z] = normal[z];

			origin->geometry[offset + count].t[0] = a1*origin->boundary*float(x)/float(verts_per_side-1);
			origin->geometry[offset + count].t[1] = a1*origin->boundary*float(y)/float(verts_per_side-1);
			origin->geometry[offset + count].t[2] = (y == 0 ? 1.0 : (x == (verts_per_side - 1) ? 2.0 : (x == y ? 3.0 : 0.0)));
		}
	}
}

void planet_part::findneighbor(int relation)
{
	if (parent == NULL || parent->size() < 4)
		return;
	else if (relation == (sibling_number+2)%4)
	{
		neighbors[relation - 1] = &parent->at(2);
		neighbor_type[relation - 1] = relation;
	}
	else if (sibling_number == 2)
	{
		neighbors[relation - 1] = &parent->at((relation+2)%4);
		neighbor_type[relation - 1] = relation;
	}
	else
	{
		if (parent->neighbors[relation - 1] == NULL || parent->neighbor_type[relation - 1] == 0)
			parent->findneighbor(relation);

		planet_part *parent_neighbor = parent->neighbors[relation - 1];
		int parent_neighbor_type = parent->neighbor_type[relation - 1];
		int new_sibling_number = sibling_number == 3 ? 2 : sibling_number;

		if (parent_neighbor == NULL)
			return;
		else if (parent_neighbor->size() < 4 || parent_neighbor->update_merge)
		{
			neighbors[relation - 1] = parent_neighbor;
			neighbor_type[relation - 1] = 0;
		}
		else if (parent_neighbor_type == 1)
		{
			neighbors[relation - 1] = &parent_neighbor->at((int)((relation - 1) == new_sibling_number));
			neighbor_type[relation - 1] = 1;
		}
		else if (parent_neighbor_type == 2)
		{
			neighbors[relation - 1] = &parent_neighbor->at((int)((relation - 1) == new_sibling_number)*2 + 1);
			neighbor_type[relation - 1] = 2;
		}
		else if (parent_neighbor_type == 3)
		{
			neighbors[relation - 1] = &parent_neighbor->at((int)((relation - 1) != new_sibling_number)*3);
			neighbor_type[relation - 1] = 3;
		}
		else if (parent_neighbor_type == -1)
		{
			neighbors[relation - 1] = &parent_neighbor->at((int)((relation - 1) != new_sibling_number));
			neighbor_type[relation - 1] = -1;
		}
		else if (parent_neighbor_type == -2)
		{
			neighbors[relation - 1] = &parent_neighbor->at((int)((relation - 1) != new_sibling_number)*2 + 1);
			neighbor_type[relation - 1] = -2;
		}
		else if (parent_neighbor_type == -3)
		{
			neighbors[relation - 1] = &parent_neighbor->at((int)((relation - 1) == new_sibling_number)*3);
			neighbor_type[relation - 1] = -3;
		}
	}
}

void planet_part::update_neighbors()
{
	int x = 0;
	for (x = 0; x < 3; x++)
		findneighbor(x+1);

	if (size() == 4 && (!update_merge || at(0).size() == 4 || at(1).size() == 4 || at(2).size() == 4 || at(3).size() == 4))
		for (int i = 0; i < 4; i++)
			at(i).update_neighbors();
}

void planet_part::prepare()
{
	if (origin->origin->player.camera != NULL)
	{
		double angle = acos(dot(-origin->camera_direction, ncenter));
		angle = maximum(absolute(angle) - radius, 0.0);

		cull = false;
		if (angle > origin->origin->player.camera->frustum[2][1])
			cull = true;

		if (!cull)
		{
			double size2 = a1*origin->boundary*a1*origin->boundary;
			vec4f to_cam = (origin->origin->player.camera->position - origin->position - center*(origin->boundary+max)/origin->boundary);
			double dist2 = dot(to_cam, to_cam);

			if (size() == 4 && !update_merge)
			{
				interpolator = clamp((1.3 - dist2/size2)/.3, 0.0, 1.0);

				for (int i = 0; i < 4; i++)
					at(i).prepare();

				if (dist2/size2 > 1.3)
					origin->merge(this);
			}
			else
			{
				if (dist2/size2 < 1.0 && size2/(verts_per_side*verts_per_side) > 10.0)
					origin->split(this);

				interpolator = 1.0 - clamp((dist2/size2 - 1.3)/.3, 0.0, 1.0);

				if (parent != NULL)
					interpolator = interpolator*parent->interpolator;
			}
		}
	}
	else
		cull = false;
}

void planet_part::render(GLuint program)
{
	if (update_split && update_merge)
	{
		clear();

		glBufferSubData(GL_ARRAY_BUFFER, buffer->offset*verts_per_part*sizeof(render_vertex), verts_per_part*sizeof(render_vertex), &origin->geometry[buffer->offset*verts_per_part]);

		update_split = false;
		update_merge = false;
	}
	if (update_split)
	{
		if (empty())
			glBufferSubData(GL_ARRAY_BUFFER, buffer->offset*verts_per_part*sizeof(render_vertex), verts_per_part*sizeof(render_vertex), &origin->geometry[buffer->offset*verts_per_part]);
		else
			origin->deallocate_part(this);

		update_split = false;
	}
	else if (update_merge)
	{
		clear();

		glBufferSubData(GL_ARRAY_BUFFER, buffer->offset*verts_per_part*sizeof(render_vertex), verts_per_part*sizeof(render_vertex), &origin->geometry[buffer->offset*verts_per_part]);

		update_merge = false;
	}

	if (cull)
		return;

	if (!empty())
		for (int i = 0; i < 4; i++)
			at(i).render(program);
	else
	{
		vec4f pos;
		if (origin->origin->player.camera != NULL)
			pos = (origin->position + center - origin->origin->player.camera->position);
		glUniform4f(glGetUniformLocation(program, "position"), pos[0], pos[1], pos[2], 0.0);
		glUniform3f(glGetUniformLocation(program, "ntangent"), ntangent[0], ntangent[1], ntangent[2]);
		glUniform3f(glGetUniformLocation(program, "neighbors"), (float)(level - (neighbors[0] == NULL ? level : neighbors[0]->level)),
																(float)(level - (neighbors[1] == NULL ? level : neighbors[1]->level)),
																(float)(level - (neighbors[2] == NULL ? level : neighbors[2]->level)));
		glUniform1f(glGetUniformLocation(program, "interpolator"), interpolator);
		for (int x = 1, count = 0; x < verts_per_side && buffer; x++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, x*2 + 1, GL_UNSIGNED_INT, (GLuint*)((buffer->offset*indices_per_part+count)*sizeof(GLuint)));
			count += x*2 + 1;
		}
	}
}

void planet_part::release()
{
	if (buffer != NULL)
		origin->deallocate_part(this);

	a1 = 0.0;
	a2 = 0.0;
	a3 = 0.0;
	min = 0.0;
	max = 0.0;
	radius = 0.0;
	level = 0;
	cull = false;
	update_split = false;
	update_merge = false;
	origin = NULL;
	neighbors[0] = NULL;
	neighbors[1] = NULL;
	neighbors[2] = NULL;
	buffer = NULL;
}

void planethdl::allocate_part(planet_part *part)
{
	if (part == NULL)
		return;

	part->buffer = unused.pop_back();
	part->buffer->part = part;
	used.push_back(part->buffer);
}

void planethdl::deallocate_part(planet_part *part)
{
	if (part == NULL || part->buffer == NULL)
		return;

	part->buffer->part = NULL;
	used.remove(part->buffer);
	unused.push_back(part->buffer);
}

void planethdl::initialize(poly base, double boundary)
{
	this->base = base;
	this->boundary = boundary;
	planet_buffer *buffer = unused.push_back(planet_buffer(0));
	GLuint indices[max_indices];
	int c1 = buffer->offset*verts_per_part + 1;
	int c2 = buffer->offset*verts_per_part;
	int index = 0;
	for (int y = 0; y < verts_per_side - 1; y++)
	{
		for (int z = 0; z < y+1; z++)
		{
			indices[buffer->offset*indices_per_part + index++] = c1++;
			indices[buffer->offset*indices_per_part + index++] = c2++;
		}

		indices[buffer->offset*indices_per_part + index++] = c1++;
	}

	for (int x = 0; x < max_parts-1; x++)
	{
		buffer = unused.push_back(planet_buffer(x+1));

		c1 = buffer->offset*verts_per_part + 1;
		c2 = buffer->offset*verts_per_part;
		index = 0;
		for (int y = 0; y < verts_per_side - 1; y++)
		{
			for (int z = 0; z < y+1; z++)
			{
				indices[buffer->offset*indices_per_part + index++] = c1++;
				indices[buffer->offset*indices_per_part + index++] = c2++;
			}

			indices[buffer->offset*indices_per_part + index++] = c1++;
		}
	}

	vec3F v0(1.0*boundary, 0.0, 0.0),
		  v1(0.447213595500*boundary,  0.894427191000*boundary, 0.0),
		  v2(0.447213595500*boundary,  0.276393202252*boundary, 0.850650808354*boundary),
		  v3(0.447213595500*boundary, -0.723606797748*boundary, 0.525731112119*boundary),
		  v4(0.447213595500*boundary, -0.723606797748*boundary, -0.525731112119*boundary),
		  v5(0.447213595500*boundary,  0.276393202252*boundary, -0.850650808354*boundary),
		  v6(-0.447213595500*boundary, -0.894427191000*boundary, 0.0),
		  v7(-0.447213595500*boundary, -0.276393202252*boundary, 0.850650808354*boundary),
		  v8(-0.447213595500*boundary,  0.723606797748*boundary, 0.525731112119*boundary),
		  v9(-0.447213595500*boundary,  0.723606797748*boundary, -0.525731112119*boundary),
		  v10(-0.447213595500*boundary, -0.276393202252*boundary, -0.850650808354*boundary),
		  v11(-1.0*boundary, 0.0, 0.0);

	parts[0].initialize(this,  v2,  v1,  v0,  &parts[5],   3, &parts[4],   3, &parts[1],   2);
	parts[1].initialize(this,  v3,  v2,  v0,  &parts[6],   3, &parts[0],   3, &parts[2],   2);
	parts[2].initialize(this,  v4,  v3,  v0,  &parts[7],   3, &parts[1],   3, &parts[3],   2);
	parts[3].initialize(this,  v5,  v4,  v0,  &parts[8],   3, &parts[2],   3, &parts[4],   2);
	parts[4].initialize(this,  v1,  v5,  v0,  &parts[9],   3, &parts[3],   3, &parts[0],   2);
	parts[5].initialize(this,  v2,  v8,  v1,  &parts[11],  2, &parts[10],  3, &parts[0],   1);
	parts[6].initialize(this,  v3,  v7,  v2,  &parts[12],  2, &parts[11],  3, &parts[1],   1);
	parts[7].initialize(this,  v4,  v6,  v3,  &parts[13],  2, &parts[12],  3, &parts[2],   1);
	parts[8].initialize(this,  v5,  v10, v4,  &parts[14],  2, &parts[13],  3, &parts[3],   1);
	parts[9].initialize(this,  v1,  v9,  v5,  &parts[10],  2, &parts[14],  3, &parts[4],   1);
	parts[10].initialize(this, v8,  v9,  v1,  &parts[16],  1, &parts[9],   1, &parts[5],   2);
	parts[11].initialize(this, v7,  v8,  v2,  &parts[17],  1, &parts[5],   1, &parts[6],   2);
	parts[12].initialize(this, v6,  v7,  v3,  &parts[18],  1, &parts[6],   1, &parts[7],   2);
	parts[13].initialize(this, v10, v6,  v4,  &parts[19],  1, &parts[7],   1, &parts[8],   2);
	parts[14].initialize(this, v9,  v10, v5,  &parts[15],  1, &parts[8],   1, &parts[9],   2);
	parts[15].initialize(this, v10, v9,  v11, &parts[14],  1, &parts[16],  3, &parts[19],  2);
	parts[16].initialize(this, v9,  v8,  v11, &parts[10],  1, &parts[17],  3, &parts[15],  2);
	parts[17].initialize(this, v8,  v7,  v11, &parts[11],  1, &parts[18],  3, &parts[16],  2);
	parts[18].initialize(this, v7,  v6,  v11, &parts[12],  1, &parts[19],  3, &parts[17],  2);
	parts[19].initialize(this, v6,  v10, v11, &parts[13],  1, &parts[15],  3, &parts[18],  2);

	glGenBuffers(1, &geometry_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(render_vertex)*max_verts, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(render_vertex)*max_verts, geometry);

	glGenBuffers(1, &geometry_ind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry_ind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*max_indices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*max_indices, indices);

	land = origin->palette.insert("res/core/planet");
}

void planethdl::split(planet_part *part)
{
}

void planethdl::merge(planet_part *part)
{
	if (part == NULL || part->empty() || part->update_merge || part->update_split)
		return;

	bool done = false;
	for (int i = 0; i < 4; i++)
		if (!part->at(i).empty())
		{
			merge(&part->at(i));
			done = true;
		}

	if (!done)
	{
		allocate_part(part);

		if (part->buffer == NULL)
			return;

		int offset = part->buffer->offset*verts_per_part;

		vec3f off0 = (part->at(0).center - part->center);
		vec3f off1 = (part->at(1).center - part->center);
		vec3f off2 = (part->at(2).center - part->center);
		vec3f off3 = (part->at(3).center - part->center);

		int u = 0, v = 0, c = 0;
		for (int x = 0; x < verts_per_side; x+=2)
			for (int y = 0; y < x+1; y+=2)
			{
				c = (x*x + x)/2;
				u = x/2;
				v = y/2;
				geometry[offset + (u*u + u)/2 + v].copyshift(geometry[part->at(0).buffer->offset*verts_per_part + c + y], off0.data);
				geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(u)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(v)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v-1)%2 == 1 && (u-1)%2 == 1 && y > 0)
						geometry[offset + ((u-1)*(u-1) + (u-1))/2 + (v-1)].g[1] = (geometry[offset + ((u-2)*(u-2) + (u-2))/2 + (v-2)].g[0] +
								geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u-1)%2 == 1 && y < x)
						geometry[offset + ((u-1)*(u-1) + (u-1))/2 + v].g[1] = (geometry[offset + ((u-2)*(u-2) + (u-2))/2 + v].g[0] +
								geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v-1)%2 == 1 && y > 0)
						geometry[offset + (u*u + u)/2 + (v-1)].g[1] = (geometry[offset + (u*u + u)/2 + (v-2)].g[0] +
								geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}

				u = x/2 + verts_per_side/2;
				v = y/2;
				geometry[offset + (u*u + u)/2 + v].copyshift(geometry[part->at(0).buffer->offset*verts_per_part + c + y], off1.data);
				geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(u)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(v)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v-1)%2 == 1 && (u-1)%2 == 1 && y > 0)
						geometry[offset + ((u-1)*(u-1) + (u-1))/2 + (v-1)].g[1] = (geometry[offset + ((u-2)*(u-2) + (u-2))/2 + (v-2)].g[0] +
																						   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u-1)%2 == 1 && y < x)
						geometry[offset + ((u-1)*(u-1) + (u-1))/2 + v].g[1] = (geometry[offset + ((u-2)*(u-2) + (u-2))/2 + v].g[0] +
																					   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v-1)%2 == 1 && y > 0)
						geometry[offset + (u*u + u)/2 + (v-1)].g[1] = (geometry[offset + (u*u + u)/2 + (v-2)].g[0] +
																			   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}

				u = (verts_per_side-1) - x/2;
				v = verts_per_side/2 - y/2;
				geometry[offset + (u*u + u)/2 + v].copyshift(geometry[part->at(0).buffer->offset*verts_per_part + c + y], off2.data);
				geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(u)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(v)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v+1)%2 == 1 && (u+1)%2 == 1 && y > 0)
						geometry[offset + ((u+1)*(u+1) + (u+1))/2 + (v+1)].g[1] = (geometry[offset + ((u+2)*(u+2) + (u+2))/2 + (v+2)].g[0] +
																						   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u+1)%2 == 1 && y < x)
						geometry[offset + ((u+1)*(u+1) + (u+1))/2 + v].g[1] = (geometry[offset + ((u+2)*(u+2) + (u+2))/2 + v].g[0] +
																					   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v+1)%2 == 1 && y > 0)
						geometry[offset + (u*u + u)/2 + (v+1)].g[1] = (geometry[offset + (u*u + u)/2 + (v+2)].g[0] +
																			   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}

				u = x/2 + verts_per_side/2;
				v = y/2 + verts_per_side/2;
				geometry[offset + (u*u + u)/2 + v].copyshift(geometry[part->at(0).buffer->offset*verts_per_part + c + y], off3.data);
				geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(u)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*boundary*float(v)/float(verts_per_side-1);
				geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v-1)%2 == 1 && (u-1)%2 == 1 && y > 0)
						geometry[offset + ((u-1)*(u-1) + (u-1))/2 + (v-1)].g[1] = (geometry[offset + ((u-2)*(u-2) + (u-2))/2 + (v-2)].g[0] +
																						   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u-1)%2 == 1 && y < x)
						geometry[offset + ((u-1)*(u-1) + (u-1))/2 + v].g[1] = (geometry[offset + ((u-2)*(u-2) + (u-2))/2 + v].g[0] +
																					   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v-1)%2 == 1 && y > 0)
						geometry[offset + (u*u + u)/2 + (v-1)].g[1] = (geometry[offset + (u*u + u)/2 + (v-2)].g[0] +
																			   geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}
			}

		part->update_merge = true;
		for (int x = 0; x < 3; x++)
			part->neighbors[x]->update_neighbors();
	}
}
