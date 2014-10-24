/*
 *  Vector.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Vector_h
#define Vector_h

#include "Matrix.h"
#include "OpenGLIncludes.h"

#include <math.h>

struct Vector
{
	Vector(GLdouble X, GLdouble Y, GLdouble Z);
	Vector(GLdouble *arr);
	Vector(GLfloat X, GLfloat Y, GLfloat Z);
	Vector(GLfloat *arr);
	Vector();
	~Vector();
	
	GLdouble x, y, z;
	
	Vector &operator =(Vector v);
	
	Vector &operator +=(Vector v);
	Vector &operator -=(Vector v);
	Vector &operator *=(Vector v);
	Vector &operator /=(Vector v);
	
	Vector &operator +=(GLdouble f);
	Vector &operator -=(GLdouble f);
	Vector &operator *=(GLdouble f);
	Vector &operator /=(GLdouble f);
	
	Vector &operator +=(GLfloat f1);
	Vector &operator -=(GLfloat f1);
	Vector &operator *=(GLfloat f1);
	Vector &operator /=(GLfloat f1);
};

Vector Normalize(Vector v);

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

inline Vector operator +(Vector v, GLdouble f)
{
	return Vector(v.x + f, v.y + f, v.z + f);
}

inline Vector operator -(Vector v, GLdouble f)
{
	return Vector(v.x - f, v.y - f, v.z - f);
}

inline Vector operator *(Vector v, GLdouble f)
{
	return Vector(v.x * f, v.y * f, v.z * f);
}

inline Vector operator /(Vector v, GLdouble f)
{
	return Vector(v.x / f, v.y / f, v.z / f);
}

inline Vector operator +(GLdouble f, Vector v)
{
	return Vector(f + v.x, f + v.y, f + v.z);
}

inline Vector operator -(GLdouble f, Vector v)
{
	return Vector(f - v.x, f - v.y, f - v.z);
}

inline Vector operator *(GLdouble f, Vector v)
{
	return Vector(f * v.x, f * v.y, f * v.z);
}

inline Vector operator /(GLdouble f, Vector v)
{
	return Vector(f / v.x, f / v.y, f / v.z);
}

inline Vector operator +(Vector v, GLfloat f1)
{
	GLdouble f = GLdouble(f1);
	return Vector(v.x + f, v.y + f, v.z + f);
}

inline Vector operator -(Vector v, GLfloat f1)
{
	GLdouble f = GLdouble(f1);
	return Vector(v.x - f, v.y - f, v.z - f);
}

inline Vector operator *(Vector v, GLfloat f1)
{
	GLdouble f = GLdouble(f1);
	return Vector(v.x * f, v.y * f, v.z * f);
}

inline Vector operator /(Vector v, GLfloat f1)
{
	GLdouble f = GLdouble(f1);
	return Vector(v.x / f, v.y / f, v.z / f);
}

inline Vector operator +(GLfloat f1, Vector v)
{
	GLdouble f = GLdouble(f1);
	return Vector(f + v.x, f + v.y, f + v.z);
}

inline Vector operator -(GLfloat f1, Vector v)
{
	GLdouble f = GLdouble(f1);
	return Vector(f - v.x, f - v.y, f - v.z);
}

inline Vector operator *(GLfloat f1, Vector v)
{
	GLdouble f = GLdouble(f1);
	return Vector(f * v.x, f * v.y, f * v.z);
}

inline Vector operator /(GLfloat f1, Vector v)
{
	GLdouble f = GLdouble(f1);
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

inline GLdouble Magnitude(Vector v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline GLdouble Distance(Vector v1, Vector v2)
{
	return Magnitude(v2 - v1);
}

inline GLdouble Dot(Vector v1, Vector v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

inline Vector  Cross(Vector v1, Vector v2)
{
	return Vector(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

inline Vector  MidPoint(Vector v1, Vector v2)
{
	return (v1+v2)/2.0;
}

inline Vector  CalculateNormal(Vector v1, Vector v2, Vector v3)
{
	return Normalize(Cross(v2 - v1, v3 - v1));
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
	GLdouble c1 = cos(e.x), c2 = cos(e.y), c3 = cos(e.z);
	GLdouble s1 = sin(e.x), s2 = sin(e.y), s3 = sin(e.z);
	
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

inline Vector RotateX(Vector v, GLdouble a)
{
	return Vector(v.x, v.y*cos(a) - v.z*sin(a), v.y*sin(a) + v.z*cos(a));
}

inline Vector RotateY(Vector v, GLdouble a)
{
	return Vector(v.x*cos(a) + v.z*sin(a), v.y, -v.x*sin(a) + v.z*cos(a));
}

inline Vector RotateZ(Vector v, GLdouble a)
{
	return Vector(v.x*cos(a) - v.y*sin(a), v.x*sin(a) + v.y*cos(a), v.z);
}

Vector AngleBetween(Vector v1, Vector v2);

#endif
