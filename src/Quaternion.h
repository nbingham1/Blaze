/*
	Quaternion.h
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

#ifndef Quaternion_h
#define Quaternion_h

struct Quaternion
{
	Quaternion()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 0.0;
	}
	
	Quaternion(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}
	
	GLfloat x, y, z, w;
	
	Quaternion &operator =(Quaternion q);
};

inline Quaternion Conjugate(Quaternion q)
{
  return Quaternion(-q.x, -q.y, -q.z, q.w);
}

inline Quaternion operator *(Quaternion q1, Quaternion q2)
{
	return Quaternion(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
					  q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
					  q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
					  q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
}

inline Quaternion operator /(Quaternion q1, Quaternion q2)
{
	return q1*Conjugate(q2);
}

inline GLfloat Magnitude(Quaternion q)
{
  return sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

inline Quaternion Normalize(Quaternion q)
{
  GLfloat l = Magnitude(q);
  return Quaternion(q.x/l, q.y/l, q.z/l, q.w/l);
}

#endif
