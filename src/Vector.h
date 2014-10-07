/*
	Vector.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/7/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "Matrix.h"

#ifndef Vector_h
#define Vector_h

struct Vector
{
	Vector(GLfloat X, GLfloat Y, GLfloat Z);
	Vector();
	~Vector();
	
	GLfloat x, y, z;
	
	Vector &operator =(Vector v);
	
	Vector &operator +=(Vector v);
	Vector &operator -=(Vector v);
	Vector &operator *=(Vector v);
	Vector &operator /=(Vector v);
	
	Vector &operator +=(GLfloat f);
	Vector &operator -=(GLfloat f);
	Vector &operator *=(GLfloat f);
	Vector &operator /=(GLfloat f);
};

inline Vector operator +(Vector v1, Vector v2)
{
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector operator -(Vector v1, Vector v2)
{
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector operator *(Vector v1, Vector v2)
{
	return Vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vector operator /(Vector v1, Vector v2)
{
	return Vector(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline Vector operator +(Vector v, GLfloat f)
{
	return Vector(v.x + f, v.y + f, v.z + f);
}

inline Vector operator -(Vector v, GLfloat f)
{
	return Vector(v.x - f, v.y - f, v.z - f);
}

inline Vector operator *(Vector v, GLfloat f)
{
	return Vector(v.x * f, v.y * f, v.z * f);
}

inline Vector operator /(Vector v, GLfloat f)
{
	return Vector(v.x / f, v.y / f, v.z / f);
}

inline Vector operator +(GLfloat f, Vector v)
{
	return Vector(f + v.x, f + v.y, f + v.z);
}

inline Vector operator -(GLfloat f, Vector v)
{
	return Vector(f - v.x, f - v.y, f - v.z);
}

inline Vector operator *(GLfloat f, Vector v)
{
	return Vector(f * v.x, f * v.y, f * v.z);
}

inline Vector operator /(GLfloat f, Vector v)
{
	return Vector(f / v.x, f / v.y, f / v.z);
}

inline bool operator ==(Vector v1, Vector v2)
{
	if ((v1.x <= v2.x+0.01 && v1.x >= v2.x-0.01) && (v1.y <= v2.y+0.01 && v1.y >= v2.y-0.01) && (v1.z <= v2.z+0.01 && v1.z >= v2.z-0.01))
		return true;
	else
		return false;
}

inline Vector abs(Vector v)
{
	Vector r;
	if (v.x < 0.0)
		r.x = v.x*-1;
	if (v.y < 0.0)
		r.y = v.y*-1;
	if (v.z < 0.0)
		r.z = v.z*-1;
	
	return r;
}

inline GLfloat Magnitude(Vector v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline GLfloat Distance(Vector v1, Vector v2)
{
	return Magnitude(v2 - v1);
}

inline GLfloat Dot(Vector v1, Vector v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

inline Vector  Cross(Vector v1, Vector v2)
{
	return Vector(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

inline Vector  MidPoint(Vector v1, Vector v2)
{
	return (v1+v2)/2;
}

inline Vector  Normalize(Vector v)
{
	GLfloat length = Magnitude(v);

	if (length == 0.0f)
		length = 1.0f;

	return (v / length);
}

inline Vector  CalculateNormal(Vector v1, Vector v2, Vector v3)
{
	Vector a = v2 - v1;
	Vector b = v3 - v1;
	
	return Normalize(Cross(a, b));
}

inline Vector operator *(Matrix m, Vector v)
{
	return Vector(m[0]*v.x + m[4]*v.y + m[8]*v.z,
				  m[1]*v.x + m[5]*v.y + m[9]*v.z,
				  m[2]*v.x + m[6]*v.y + m[10]*v.z);
}

inline Vector operator /(Vector v, Matrix m)
{
	Inverse(m);
	return Vector(m[0]*v.x + m[4]*v.y + m[8]*v.z,
				  m[1]*v.x + m[5]*v.y + m[9]*v.z,
				  m[2]*v.x + m[6]*v.y + m[10]*v.z);
}

inline void EulertoMatrix(Vector e, Matrix m)
{
	GLfloat c1 = cos(e.x), c2 = cos(e.y), c3 = cos(e.z);
	GLfloat s1 = sin(e.x), s2 = sin(e.y), s3 = sin(e.z);
	
	m[0] = c2*c1;
	m[1] = s1;
	m[2] = -s2*c1;
	m[3] = 0;
	m[4] = -c2*s1*c3 + s2*s3;
	m[5] = c1*c3;
	m[6] = s2*s1*c3 + c2*s3;
	m[7] = 0;
	m[8] = c2*s1*s3 + s2*c3;
	m[9] = -c1*s3;
	m[10] = -s2*s1*s3 + c2*c3;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;
}

#endif
