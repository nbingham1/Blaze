
/*
 * block.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: nbingham
 */

#include "block.h"
#include <std/fill.h>

rw_lock::rw_lock()
{
	lock = PTHREAD_MUTEX_INITIALIZER;
	reader_gate = PTHREAD_COND_INITIALIZER;
	writer_gate = PTHREAD_COND_INITIALIZER;
	count = 0;
	wait = 0;
}

rw_lock::~rw_lock()
{

}

bool rw_lock::w_lock()
{
	bool acquired = false;
	if (pthread_mutex_lock(&lock) == 0)
	{
		wait++;
		while (count != 0)
			pthread_cond_wait(&writer_gate, &lock);

		wait--;
		count--;
		acquired = true;
		pthread_mutex_unlock(&lock);
	}
	return acquired;
}

bool rw_lock::w_trylock()
{
	bool acquired = false;
	if (pthread_mutex_lock(&lock) == 0)
	{
		if (count == 0)
		{
			count--;
			acquired = true;
		}
		pthread_mutex_unlock(&lock);
	}
	return acquired;
}

bool rw_lock::w_unlock()
{
	bool released = false;
	if (pthread_mutex_lock(&lock) == 0)
	{
		count++;
		if (wait > 0)
			pthread_cond_signal(&writer_gate);
		else
			pthread_cond_broadcast(&reader_gate);
		released = true;
		pthread_mutex_unlock(&lock);
	}

	return released;
}

bool rw_lock::r_lock()
{
	bool acquired = false;
	if (pthread_mutex_lock(&lock) == 0)
	{
		while (count < 0)
			pthread_cond_wait(&reader_gate, &lock);

		count++;
		acquired = true;
		pthread_mutex_unlock(&lock);
	}
	return acquired;
}

bool rw_lock::r_trylock()
{
	bool acquired = false;
	if (pthread_mutex_lock(&lock) == 0)
	{
		if (count >= 0)
		{
			count++;
			acquired = true;
		}
		pthread_mutex_unlock(&lock);
	}
	return acquired;
}

bool rw_lock::r_unlock()
{
	bool released = false;
	if (pthread_mutex_lock(&lock) == 0)
	{
		count--;
		if (wait > 0)
			pthread_cond_signal(&writer_gate);
		else
			pthread_cond_broadcast(&reader_gate);
		released = true;
		pthread_mutex_unlock(&lock);
	}

	return released;
}

blockhdl::blockhdl()
{
	size = 0.0;
	density = NULL;
	parent = NULL;
}

blockhdl::blockhdl(vec3F origin, double size, grad3f (*density)(gvec3f location, void *data))
{
	this->parent = NULL;
	this->origin = origin;
	this->size = size;
	this->density = density;
}

blockhdl::~blockhdl()
{

}

inline int blockhdl::index(int i, int j, int k, int width)
{
	return (i*width + j)*width + k;
}

// Not thread safe
void blockhdl::generate(void *data)
{
	if (density != NULL)
	{
		values.append_back(fill_t((cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1), grad3f()));
		float step = size/(float)cubes_per_side;
		for (int i = 0; i < cubes_per_side+1; i++)
			for (int j = 0; j < cubes_per_side+1; j++)
				for (int k = 0; k < cubes_per_side+1; k++)
					values[index(i, j, k)] = density(input_gvec<float, 3>(vec3f(origin) + step*vec3f(i, j, k)), data);
	}
}

// Not thread safe
void blockhdl::generate(array<grad3f> svalues, vec3i offset, void *data)
{
	if (density != NULL && svalues.size() == (cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1))
	{
		values.append_back(fill_t((cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1), grad3f()));
		float step = size/(float)cubes_per_side;
		for (int i = 0; i < cubes_per_side+1; i++)
			for (int j = 0; j < cubes_per_side+1; j++)
				for (int k = 0; k < cubes_per_side+1; k++)
				{
					if (!(i&1) && !(j&1) && !(k&1))
						values[index(i, j, k)] = svalues[index(i/2 + offset[0], j/2 + offset[1], k/2 + offset[2])];
					else
						values[index(i, j, k)] = density(input_gvec<float, 3>(vec3f(origin) + step*vec3f(i, j, k)), data);
				}
	}
	else if (density != NULL)
		generate(data);
}

void blockhdl::split(void *data)
{
	bool success = false;
	if (child_lock.w_lock())
	{
		if (children.size() == 0)
		{
			children.append_back(fill_t(8, blockhdl()));
			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++)
					{
						int c = index(i, j, k, 2);
						children[c].parent = this;
						children[c].points = points;
						children[c].density = density;
						children[c].size = size/2.0;
						children[c].origin = origin + vec3F(vec3f(i, j, k)*size/2.0f);
						children[c].generate(values, vec3i(i, j, k)*cubes_per_side/2, data);
					}

			for (int i = 0; i < children.size(); i++)
				children[i].load();

			success = true;
		}

		child_lock.w_unlock();
	}

	if (success)
	{
		if (geo_lock.w_lock())
		{
			unload();
			values.clear();

			geo_lock.w_unlock();
		}
	}
	else
	{
		if (child_lock.r_lock())
		{
			for (int i = 0; i < children.size(); i++)
				children[i].split(data);

			child_lock.r_unlock();
		}
	}
}

void blockhdl::merge()
{
	if (child_lock.r_lock())
	{
		bool sub = false;
		for (int i = 0; i < children.size(); i++)
			if (children[i].children.size() > 0)
			{
				children[i].merge();
				sub = true;
			}

		if (!sub)
		{
			if (geo_lock.w_lock())
			{
				if (children.size() > 0)
				{
					values.append_back(fill_t((cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1), grad3f()));
					for (int i = 0; i < cubes_per_side+1; i++)
						for (int j = 0; j < cubes_per_side+1; j++)
							for (int k = 0; k < cubes_per_side+1; k++)
							{
								int a = min((2*i)/cubes_per_side, 1);
								int b = min((2*j)/cubes_per_side, 1);
								int c = min((2*k)/cubes_per_side, 1);
								int x = (i >= cubes_per_side/2 ? i - cubes_per_side/2 : i)*2;
								int y = (j >= cubes_per_side/2 ? j - cubes_per_side/2 : j)*2;
								int z = (k >= cubes_per_side/2 ? k - cubes_per_side/2 : k)*2;

								values[index(i, j, k)] = children[index(a, b, c, 2)].values[index(x, y, z)];
							}

					load();
				}

				geo_lock.w_unlock();
			}
		}

		child_lock.r_unlock();

		if (!sub)
		{
			if (child_lock.w_lock())
			{
				for (int i = 0; i < children.size(); i++)
					children[i].unload();

				children.clear();

				child_lock.w_unlock();
			}
		}
	}
}

// Not thread safe
void blockhdl::load()
{
	float step = size/(float)cubes_per_side;

	points.clear();
	points.reserve(cubes_per_side*cubes_per_side*cubes_per_side);
	int grid[cubes_per_side*cubes_per_side*cubes_per_side];
	for (int i = 0; i < cubes_per_side; i++)
		for (int j = 0; j < cubes_per_side; j++)
			for (int k = 0; k < cubes_per_side; k++)
			{
				array<vec3f> ipoints;
				array<vec3f> inorms;

				bool v0 = (values[index(i,   j,   k  )] > 0.0f);
				bool v1 = (values[index(i,   j+1, k  )] > 0.0f);
				bool v2 = (values[index(i+1, j+1, k  )] > 0.0f);
				bool v3 = (values[index(i+1, j,   k  )] > 0.0f);
				bool v4 = (values[index(i,   j,   k+1)] > 0.0f);
				bool v5 = (values[index(i,   j+1, k+1)] > 0.0f);
				bool v6 = (values[index(i+1, j+1, k+1)] > 0.0f);
				bool v7 = (values[index(i+1, j,   k+1)] > 0.0f);

				if (v0 != v1)
				{
					float p = values[index(i, j, k)][3]/(values[index(i, j, k)][3] - values[index(i, j+1, k)][3]);
					ipoints.push_back(step*vec3f((float)i, (float)j+p, (float)k));
					inorms.push_back(-slerp(values[index(i, j, k)].normal(), values[index(i, j+1, k)].normal(), p));
				}

				if (v1 != v2)
				{
					float p = values[index(i, j+1, k)][3]/(values[index(i, j+1, k)][3] - values[index(i+1, j+1, k)][3]);
					ipoints.push_back(step*vec3f((float)i+p, (float)j+1.0f, (float)k));
					inorms.push_back(-slerp(values[index(i, j+1, k)].normal(), values[index(i, j+1, k+1)].normal(), p));
				}

				if (v2 != v3)
				{
					float p = values[index(i+1, j, k)][3]/(values[index(i+1, j, k)][3] - values[index(i+1, j+1, k)][3]);
					ipoints.push_back(step*vec3f((float)i+1.0f, (float)j+p, (float)k));
					inorms.push_back(-slerp(values[index(i+1, j, k)].normal(), values[index(i+1, j+1, k)].normal(), p));
				}

				if (v3 != v0)
				{
					float p = values[index(i, j, k)][3]/(values[index(i, j, k)][3] - values[index(i+1, j, k)][3]);
					ipoints.push_back(step*vec3f((float)i+p, (float)j, (float)k));
					inorms.push_back(-slerp(values[index(i, j, k)].normal(), values[index(i+1, j, k)].normal(), p));
				}

				if (v4 != v5)
				{
					float p = values[index(i, j, k+1)][3]/(values[index(i, j, k+1)][3] - values[index(i, j+1, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i, (float)j+p, (float)k+1.0f));
					inorms.push_back(-slerp(values[index(i, j, k+1)].normal(), values[index(i, j+1, k+1)].normal(), p));
				}

				if (v5 != v6)
				{
					float p = values[index(i, j+1, k+1)][3]/(values[index(i, j+1, k+1)][3] - values[index(i+1, j+1, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i+p, (float)j+1.0f, (float)k+1.0f));
					inorms.push_back(-slerp(values[index(i, j+1, k+1)].normal(), values[index(i+1, j+1, k+1)].normal(), p));
				}

				if (v6 != v7)
				{
					float p = values[index(i+1, j, k+1)][3]/(values[index(i+1, j, k+1)][3] - values[index(i+1, j+1, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i+1.0f, (float)j+p, (float)k+1.0f));
					inorms.push_back(-slerp(values[index(i+1, j, k+1)].normal(), values[index(i+1, j+1, k+1)].normal(), p));
				}

				if (v7 != v4)
				{
					float p = values[index(i, j, k+1)][3]/(values[index(i, j, k+1)][3] - values[index(i+1, j, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i+p, (float)j, (float)k+1.0f));
					inorms.push_back(-slerp(values[index(i, j, k+1)].normal(), values[index(i+1, j, k+1)].normal(), p));
				}

				if (v0 != v4)
				{
					float p = values[index(i, j, k)][3]/(values[index(i, j, k)][3] - values[index(i, j, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i, (float)j, (float)k+p));
					inorms.push_back(-slerp(values[index(i, j, k)].normal(), values[index(i, j, k+1)].normal(), p));
				}

				if (v1 != v5)
				{
					float p = values[index(i, j+1, k)][3]/(values[index(i, j+1, k)][3] - values[index(i, j+1, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i, (float)j+1.0f, (float)k+p));
					inorms.push_back(-slerp(values[index(i, j+1, k)].normal(), values[index(i, j+1, k+1)].normal(), p));
				}

				if (v2 != v6)
				{
					float p = values[index(i+1, j+1, k)][3]/(values[index(i+1, j+1, k)][3] - values[index(i+1, j+1, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i+1.0f, (float)j+1.0f, (float)k+p));
					inorms.push_back(-slerp(values[index(i+1, j+1, k)].normal(), values[index(i+1, j+1, k+1)].normal(), p));
				}

				if (v3 != v7)
				{
					float p = values[index(i+1, j, k)][3]/(values[index(i+1, j, k)][3] - values[index(i+1, j, k+1)][3]);
					ipoints.push_back(step*vec3f((float)i+1.0f, (float)j, (float)k+p));
					inorms.push_back(-slerp(values[index(i+1, j, k)].normal(), values[index(i+1, j, k+1)].normal(), p));
				}

				if (ipoints.size() > 0)
				{
					vec3f point = vec3f(0.0f, 0.0f, 0.0f);
					for (int m = 0; m < ipoints.size(); m++)
						point += ipoints[m];
					point /= (float)ipoints.size();
					grid[index(i, j, k, cubes_per_side)] = points.size();
					points.push_back(point);
				}
				else
					grid[index(i, j, k, cubes_per_side)] = -1;
			}

	for (int i = 0; i < cubes_per_side-1; i++)
		for (int j = 0; j < cubes_per_side-1; j++)
			for (int k = 0; k < cubes_per_side-1; k++)
				if (values[index(i+1, j+1, k+1)] > 0.0f)
				{
					// front
					if (grid[index(i, j  , k, cubes_per_side)] >= 0 &&
						grid[index(i, j+1, k, cubes_per_side)] >= 0 && grid[index(i+1, j+1, k, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j  , k, cubes_per_side)]);
						indices.push_back(grid[index(i  , j+1, k, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k, cubes_per_side)]);
					}

					if (grid[index(i, j  , k, cubes_per_side)] >= 0 && grid[index(i+1, j  , k, cubes_per_side)] >= 0 &&
						grid[index(i+1, j+1, k, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j  , k, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j  , k, cubes_per_side)]);
					}

					// back
					if (grid[index(i, j  , k+1, cubes_per_side)] >= 0 && grid[index(i+1, j  , k+1, cubes_per_side)] >= 0 &&
						 grid[index(i+1, j+1, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j  , k+1, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j  , k+1, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k+1, cubes_per_side)]);
					}

					if (grid[index(i, j  , k+1, cubes_per_side)] >= 0 &&
						grid[index(i, j+1, k+1, cubes_per_side)] >= 0 && grid[index(i+1, j+1, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j  , k+1, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k+1, cubes_per_side)]);
						indices.push_back(grid[index(i  , j+1, k+1, cubes_per_side)]);
					}

					// bottom
					if (grid[index(i, j, k, cubes_per_side)] >= 0 && grid[index(i+1, j, k, cubes_per_side)] >= 0 &&
						grid[index(i+1, j, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j, k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j, k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j, k+1, cubes_per_side)]);
					}

					if (grid[index(i, j, k, cubes_per_side)] >= 0 &&
						grid[index(i+1, j, k+1, cubes_per_side)] >= 0 && grid[index(i, j, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j, k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j, k+1, cubes_per_side)]);
						indices.push_back(grid[index(i  , j, k+1, cubes_per_side)]);
					}

					// top
					if (grid[index(i, j+1, k, cubes_per_side)] >= 0 &&
						grid[index(i+1, j+1, k+1, cubes_per_side)] >= 0 && grid[index(i, j+1, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j+1, k  , cubes_per_side)]);
						indices.push_back(grid[index(i  , j+1, k+1, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k+1, cubes_per_side)]);
					}

					if (grid[index(i, j+1, k, cubes_per_side)] >= 0 && grid[index(i+1, j+1, k, cubes_per_side)] >= 0 &&
						grid[index(i+1, j+1, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i  , j+1, k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k+1, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k  , cubes_per_side)]);
					}

					// left
					if (grid[index(i, j  , k  , cubes_per_side)] >= 0 &&
						grid[index(i, j+1, k+1, cubes_per_side)] >= 0 && grid[index(i, j  , k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i, j  , k  , cubes_per_side)]);
						indices.push_back(grid[index(i, j  , k+1, cubes_per_side)]);
						indices.push_back(grid[index(i, j+1, k+1, cubes_per_side)]);
					}

					if (grid[index(i, j  , k  , cubes_per_side)] >= 0 && grid[index(i, j+1, k  , cubes_per_side)] >= 0 &&
						grid[index(i, j+1, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i, j  , k  , cubes_per_side)]);
						indices.push_back(grid[index(i, j+1, k+1, cubes_per_side)]);
						indices.push_back(grid[index(i, j+1, k  , cubes_per_side)]);
					}

					// right
					if (grid[index(i+1, j  , k  , cubes_per_side)] >= 0 && grid[index(i+1, j+1, k  , cubes_per_side)] >= 0 &&
						grid[index(i+1, j+1, k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i+1, j  , k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k+1, cubes_per_side)]);
					}

					if (grid[index(i+1, j  , k  , cubes_per_side)] >= 0 &&
						grid[index(i+1, j+1, k+1, cubes_per_side)] >= 0 && grid[index(i+1, j  , k+1, cubes_per_side)] >= 0)
					{
						indices.push_back(grid[index(i+1, j  , k  , cubes_per_side)]);
						indices.push_back(grid[index(i+1, j+1, k+1, cubes_per_side)]);
						indices.push_back(grid[index(i+1, j  , k+1, cubes_per_side)]);
					}
				}
}

// Not thread safe
void blockhdl::unload()
{
	points.clear();
	indices.clear();
}

void blockhdl::render(framehdl &frame, int vertex_location, int origin_location)
{
	vec3F loc = origin - frame.offset;
	// Render the geometry
	if (geo_lock.r_trylock())
	{
		if (points.size() > 0)
		{
			glUniform3f(origin_location, (float)loc[0], (float)loc[1], (float)loc[2]);
			GLuint vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec3f)*points.size(), (GLfloat*)points.data, GL_STATIC_DRAW);
			GLuint ibo;
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), (GLuint*)indices.data, GL_STATIC_DRAW);


			glEnableVertexAttribArray(vertex_location);
			glVertexAttribPointer(vertex_location, 3, GL_FLOAT, false, 0, 0);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(vertex_location);

			glDeleteBuffers(1, &ibo);
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
		}

		geo_lock.r_unlock();
	}

	// Descend to the child nodes
	if (child_lock.r_trylock())
	{
		for (int i = 0; i < children.size(); i++)
			children[i].render(frame, vertex_location, origin_location);

		child_lock.r_unlock();
	}
}

bool blockhdl::contains(vec3F location, float radius)
{
	vec3f dir(location - origin);

	if (dir[0] + radius < 0.0f || dir[0] - radius > size ||
		dir[1] + radius < 0.0f || dir[1] - radius > size ||
		dir[2] + radius < 0.0f || dir[2] - radius > size)
		return false;
	else
		return true;
}
