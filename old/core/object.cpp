/*
 * object.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on December 13, 2011.
 * Modified by Ned Bingham on December 13, 2011.
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

#include "object.h"
#include "load/load.h"

objecthdl::objecthdl()
{
	geometry_data	= NULL;
	geometry_list	= 0;
	indices_list	= 0;
}

objecthdl::objecthdl(char *filename)
{
	geometry_data	= NULL;
	geometry_list	= 0;
	indices_list	= 0;

	load(filename, this);
}

objecthdl::~objecthdl()
{
	if (geometry_data != NULL)
		delete [] geometry_data;

	geometry_data	= NULL;
	geometry_list	= 0;
	indices_list	= 0;

	data_size = 0;
}

/* point()
 *
 * Allocates geometry data for one point and makes it
 * a renderable and interactive object.
 */
void objecthdl::point()
{
	release();

	// Load the geometry
	geometry_data = new vec9f[1];
	geometry_data[0] = vec9f();

	// Generate the render list
	data_mode = GL_POINTS;
	data_size = 1;

	GLuint *index_data = new GLuint[1];
	index_data[0] = 0;

	glGenBuffers(1, &geometry_list);
	glBindBuffer(GL_ARRAY_BUFFER, geometry_list);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec9f), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec9f), geometry_data);

	glGenBuffers(1, &indices_list);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_list);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint), index_data);

	delete [] index_data;
}

/* point(p)
 *
 * This function takes one point for input representing
 * an offset center of mass.
 *
 * Allocates geometry data for one point and makes it
 * a renderable and interactive object.
 */
void objecthdl::point(vec3f p)
{
	release();

	// Load the geometry
	geometry_data = new vec9f[1];
	geometry_data[0] = p;

	// Generate the render list
	data_mode = GL_POINTS;
	data_size = 1;

	GLuint *index_data = new GLuint[1];
	index_data[0] = 0;

	glGenBuffers(1, &geometry_list);
	glBindBuffer(GL_ARRAY_BUFFER, geometry_list);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec9f), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec9f), geometry_data);

	glGenBuffers(1, &indices_list);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_list);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint), index_data);

	delete [] index_data;
}

/* line(p1, p2)
 *
 * This function takes in two points for input, each
 * one representing an end point to the line.
 *
 * Allocates the geometry for a line and makes it
 * a renderable and interactive object
 */
void objecthdl::line(vec3f p1, vec3f p2)
{
	release();

	// Load the geometry
	geometry_data = new vec9f[2];
	geometry_data[0] = p1;

	geometry_data[1] = p2;
	geometry_data[1].data[4] = 1.0f;

	// Generate the render list
}

void objecthdl::line(float l)
{

}

void objecthdl::triangle(vec3f p1, vec3f p2, vec3f p3)
{

}

void objecthdl::sss_triangle(float A, float B, float C)
{

}

void objecthdl::sas_triangle(float A, float c, float B)
{

}

void objecthdl::asa_triangle(float c, float B, float a)
{

}

void objecthdl::triangle(float b, float h)
{

}

void objecthdl::triangle(float r, float a1, float a2, float a3)
{

}

void objecthdl::triangle(float r)
{

}

void objecthdl::quadrilateral(vec3f p1, vec3f p2, vec3f p3, vec3f p4)
{

}

void objecthdl::quadrilateral(float r, float a1, float a2, float a3, float a4)
{

}

void objecthdl::parallelogram(float w, float h, float a)
{

}

void objecthdl::rectangle(float w, float h)
{

}

void objecthdl::square(float s)
{

}

void objecthdl::circle(int n, float r)
{

}

void objecthdl::ellipse(int n, float w, float h)
{

}

void objecthdl::polygon(int n, ...)
{

}

void objecthdl::polygon(int n, float r, ...)
{

}

void objecthdl::tetrahedron(vec3f p1, vec3f p2, vec3f p3, vec3f p4)
{

}

void objecthdl::tetrahedron(float s)
{

}

void objecthdl::octahedron(vec3f p1, vec3f p2, vec3f p3, vec3f p4, vec3f p5)
{

}

void objecthdl::octahedron(float s)
{

}

void objecthdl::icosahedron(vec3f p1, vec3f p2, vec3f p3, vec3f p4,
		vec3f p5, vec3f p6, vec3f p7, vec3f p8,
		vec3f p9, vec3f p10, vec3f p11, vec3f p12)
{

}

void objecthdl::icosahedron(float r)
{

}

void objecthdl::sphere(int n, float r)
{

}

void objecthdl::ellipsoid(int n, float w, float h, float d)
{

}

void objecthdl::polyhedron(int n, ...)
{

}

void objecthdl::polyhedron(int n, float r, ...)
{

}

void objecthdl::release()
{
	if (geometry_data != NULL)
		delete [] geometry_data;

	geometry_data = NULL;

	if (geometry_list != 0)
		glDeleteBuffers(1, &geometry_list);
	if (indices_list != 0)
		glDeleteBuffers(1, &indices_list);
}

void objecthdl::render(double t)
{
	glBindBuffer(GL_ARRAY_BUFFER, geometry_list);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_list);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(vec9f), NULL);
	glTexCoordPointer(3, GL_FLOAT, sizeof(vec9f), (float*)16);
	glNormalPointer(GL_FLOAT, sizeof(vec9f), (float*)32);

	glDrawElements(data_mode, data_size, GL_UNSIGNED_INT, (GLuint*)NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void objecthdl::geometry(double t)
{

}

void objecthdl::material(double t)
{

}

void objecthdl::physics(double t)
{

}

void objecthdl::chemistry(double t)
{

}
