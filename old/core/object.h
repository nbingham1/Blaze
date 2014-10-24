/*
 * object.h
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

#include "../common.h"
#include "../phys.h"
#include "../math.h"

#ifndef object_h
#define object_h

struct objecthdl : dualityhdl
{
	objecthdl();
	objecthdl(char *filename);
	virtual ~objecthdl();

	GLint		   data_size;
	GLenum		   data_mode;

	vec9f		  *geometry_data;	// vertex texture normal
	vec6f		  *physics_data;	// force  momentum
	GLuint		   geometry_list;
	GLuint		   indices_list;

	// Basic Geometry
	// Basic 0D Shapes
	void point();
	void point(vec3f p);													// One Point

	// Basic 1D Shapes
	void line(float l);														// Length
	void line(vec3f p1, vec3f p2);											// Two Points

	// Basic 2D Shapes
	void triangle(vec3f p1, vec3f p2, vec3f p3);							// Three Points
	void sss_triangle(float A, float B, float C);							// Side, Side, Side
	void sas_triangle(float A, float c, float B);							// Side, Angle, Side
	void asa_triangle(float c, float B, float a);							// Angle, Side, Angle
	void triangle(float b, float h);										// Base, Height
	void triangle(float r, float a1, float a2, float a3);					// Radius, 3 Angles
	void triangle(float r);													// Radius

	void quadrilateral(vec3f p1, vec3f p2, vec3f p3, vec3f p4);				// Four Points
	void quadrilateral(float r, float a1, float a2, float a3, float a4);	// Radius, 4 Angles
	void parallelogram(float w, float h, float a);							// Side, Angle, Side
	void rectangle(float w, float h);										// Width, Height
	void square(float s);													// Size

	void circle(int n, float r);											// Number of Points, Radius
	void ellipse(int n, float w, float h);									// Number of Points, Width, Height

	void polygon(int n, ...);												// Number of Points, N Points
	void polygon(int n, float r, ...);										// Number of Points, Radius, N Angles

	// Basic 3D Shapes
	void tetrahedron(vec3f p1, vec3f p2, vec3f p3, vec3f p4);				// Four Points
	void tetrahedron(float s);												// Side Length
	void octahedron(vec3f p1, vec3f p2, vec3f p3, vec3f p4, vec3f p5);		// Five Points
	void octahedron(float s);												// Side Length
	void icosahedron(vec3f p1, vec3f p2, vec3f p3, vec3f p4, 				// Twelve Points
			vec3f p5, vec3f p6, vec3f p7, vec3f p8,
			vec3f p9, vec3f p10, vec3f p11, vec3f p12);
	void icosahedron(float r);												// Radius

	void sphere(int n, float r);											// Number of Points, Radius
	void ellipsoid(int n, float w, float h, float d);						// Number of Points, Width, Height, Depth

	void polyhedron(int n, ...);											// Number of Points, N Points
	void polyhedron(int n, float r, ...);									// Number of Points, N 2D Angles

	void release();

	// Main Functionality
	void render(double t);
	void geometry(double t);
	void material(double t);
	void physics(double t);
	void chemistry(double t);
};

#endif
