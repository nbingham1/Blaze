/*
	GeometricalTypes.h
	Blaze Game Engine 0.02

	Created by Ned Bingham on 4/13/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Main.h"

#ifndef GeometricalTypes_h
#define GeometricalTypes_h

/********************************************
 *											*
 *											*
 *				***Matrices***				*
 *											*
 *											*
 ********************************************/
 
typedef GLfloat Matrix[16];

void SetMatrix(Matrix Input, Matrix Output);

void InterchangeRows(Matrix M, int row1, int row2);
void ScaleRow(Matrix M, int row, GLfloat scale);
void AddRow(Matrix M, int row1, int row2, GLfloat scale);

void Inverse(Matrix M);
void Transpose(Matrix M);

/********************************************
 *											*
 *											*
 *	  	   ***Vector & Vertex***			*
 *											*
 *											*
 ********************************************/

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

typedef Vector Vertex;

/********************************************
 *											*
 *											*
 *			   ***EulerAngle***		    	*
 *											*
 *											*
 ********************************************/

struct EulerAngle
{
	EulerAngle(GLfloat X, GLfloat Y, GLfloat Z);
	EulerAngle();
	~EulerAngle();
	
	GLfloat x, y, z;
	
	EulerAngle &operator =(EulerAngle a);
	EulerAngle &operator =(Vector v);
	
	EulerAngle &operator +=(EulerAngle a);
	EulerAngle &operator -=(EulerAngle a);
	EulerAngle &operator *=(EulerAngle a);
	EulerAngle &operator /=(EulerAngle a);
	
	EulerAngle &operator +=(GLfloat f);
	EulerAngle &operator -=(GLfloat f);
	EulerAngle &operator *=(GLfloat f);
	EulerAngle &operator /=(GLfloat f);
};

inline EulerAngle operator +(EulerAngle a1, EulerAngle a2)
{
	return EulerAngle(a1.x + a2.x, a1.y + a2.y, a1.z + a2.z);
}

inline EulerAngle operator -(EulerAngle a1, EulerAngle a2)
{
	return EulerAngle(a1.x - a2.x, a1.y - a2.y, a1.z - a2.z);
}

inline EulerAngle operator *(EulerAngle a1, EulerAngle a2)
{
	return EulerAngle(a1.x * a2.x, a1.y * a2.y, a1.z * a2.z);
}

inline EulerAngle operator /(EulerAngle a1, EulerAngle a2)
{
	return EulerAngle(a1.x / a2.x, a1.y / a2.y, a1.z / a2.z);
}

inline EulerAngle operator +(EulerAngle a, GLfloat f)
{
	return EulerAngle(a.x + f, a.y + f, a.z + f);
}

inline EulerAngle operator -(EulerAngle a, GLfloat f)
{
	return EulerAngle(a.x - f, a.y - f, a.z - f);
}

inline EulerAngle operator *(EulerAngle a, GLfloat f)
{
	return EulerAngle(a.x * f, a.y * f, a.z * f);
}

inline EulerAngle operator /(EulerAngle a, GLfloat f)
{
	return EulerAngle(a.x / f, a.y / f, a.z / f);
}

inline EulerAngle operator +(GLfloat f, EulerAngle a)
{
	return EulerAngle(f + a.x, f + a.y, f + a.z);
}

inline EulerAngle operator -(GLfloat f, EulerAngle a)
{
	return EulerAngle(f - a.x, f - a.y, f - a.z);
}

inline EulerAngle operator *(GLfloat f, EulerAngle a)
{
	return EulerAngle(f * a.x, f * a.y, f * a.z);
}

inline EulerAngle operator /(GLfloat f, EulerAngle a)
{
	return EulerAngle(f / a.x, f / a.y, f / a.z);
}

void EulertoMatrix(EulerAngle e, GLfloat m[16]);
Vector EulertoVector(EulerAngle e);

/********************************************
 *											*
 *											*
 *			   ***AxisAngle***				*
 *											*
 *											*
 ********************************************/

struct AxisAngle
{
	AxisAngle(Vector a, GLfloat r);
	AxisAngle();
	~AxisAngle();
	
	Vector  axis;
	GLfloat angle;
	
	AxisAngle &operator =(AxisAngle a);
	AxisAngle &operator =(EulerAngle a);
	
	AxisAngle &operator +=(AxisAngle a);
	AxisAngle &operator -=(AxisAngle a);
	AxisAngle &operator *=(AxisAngle a);
	AxisAngle &operator /=(AxisAngle a);
	
	AxisAngle &operator +=(EulerAngle a);
	AxisAngle &operator -=(EulerAngle a);
	AxisAngle &operator *=(EulerAngle a);
	AxisAngle &operator /=(EulerAngle a);
	
	AxisAngle &operator +=(GLfloat f);
	AxisAngle &operator -=(GLfloat f);
	AxisAngle &operator *=(GLfloat f);
	AxisAngle &operator /=(GLfloat f);
};

EulerAngle	AxistoEuler(AxisAngle a);

inline AxisAngle operator +(AxisAngle a1, AxisAngle a2)
{
	AxisAngle a;
	a = AxistoEuler(a1) + AxistoEuler(a2);
	return a;
}

inline AxisAngle operator -(AxisAngle a1, AxisAngle a2)
{
	AxisAngle a;
	a = AxistoEuler(a1) - AxistoEuler(a2);
	return a;
}

inline AxisAngle operator *(AxisAngle a1, AxisAngle a2)
{
	AxisAngle a;
	a = AxistoEuler(a1) * AxistoEuler(a2);
	return a;
}

inline AxisAngle operator /(AxisAngle a1, AxisAngle a2)
{
	AxisAngle a;
	a = AxistoEuler(a1) / AxistoEuler(a2);
	return a;
}

inline EulerAngle operator +(AxisAngle a, EulerAngle e)
{
	return AxistoEuler(a) + e;
}

inline EulerAngle operator -(AxisAngle a, EulerAngle e)
{
	return AxistoEuler(a) - e;
}

inline EulerAngle operator *(AxisAngle a, EulerAngle e)
{
	return AxistoEuler(a) * e;
}

inline EulerAngle operator /(AxisAngle a, EulerAngle e)
{
	return AxistoEuler(a) / e;
}

inline EulerAngle operator +(EulerAngle e, AxisAngle a)
{
	return e + AxistoEuler(a);
}

inline EulerAngle operator -(EulerAngle e, AxisAngle a)
{
	return e - AxistoEuler(a);
}

inline EulerAngle operator *(EulerAngle e, AxisAngle a)
{
	return e * AxistoEuler(a);
}

inline EulerAngle operator /(EulerAngle e, AxisAngle a)
{
	return e / AxistoEuler(a);
}

inline AxisAngle operator +(AxisAngle a, GLfloat f)
{
	return AxisAngle(a.axis, a.angle + f);
}

inline AxisAngle operator -(AxisAngle a, GLfloat f)
{
	return AxisAngle(a.axis, a.angle - f);
}

inline AxisAngle operator *(AxisAngle a, GLfloat f)
{
	return AxisAngle(a.axis, a.angle * f);
}

inline AxisAngle operator /(AxisAngle a, GLfloat f)
{
	return AxisAngle(a.axis, a.angle / f);
}

inline AxisAngle operator +(GLfloat f, AxisAngle a)
{
	return AxisAngle(a.axis, f + a.angle);
}

inline AxisAngle operator -(GLfloat f, AxisAngle a)
{
	return AxisAngle(a.axis, f - a.angle);
}

inline AxisAngle operator *(GLfloat f, AxisAngle a)
{
	return AxisAngle(a.axis, f * a.angle);
}

inline AxisAngle operator /(GLfloat f, AxisAngle a)
{
	return AxisAngle(a.axis, f / a.angle);
}

inline AxisAngle VectorstoAxisAngle(Vector v1, Vector v2)
{
	return AxisAngle(Normalize(Cross(v1, v2)), acos(Dot(v1, v2)));
}
void		AxistoMatrix(AxisAngle a, GLfloat m[16]);

/********************************************
 *											*
 *											*
 *		   	   ***Quaternion***		    	*
 *											*
 *											*
 ********************************************/

struct Quaternion
{
	Quaternion(GLfloat R, Vector I);
	Quaternion(GLfloat R, GLfloat I, GLfloat J, GLfloat K);
	Quaternion();
	~Quaternion();
	
	GLfloat r, i, j, k;
	
	Quaternion &operator =(Quaternion q);
	Quaternion &operator =(AxisAngle a);
	Quaternion &operator =(EulerAngle a);
	
	Quaternion &operator +=(Quaternion q);
	Quaternion &operator -=(Quaternion q);
	Quaternion &operator *=(Quaternion q);
	Quaternion &operator /=(Quaternion q);
	
	Quaternion &operator +=(AxisAngle a);
	Quaternion &operator -=(AxisAngle a);
	Quaternion &operator *=(AxisAngle a);
	Quaternion &operator /=(AxisAngle a);
	
	Quaternion &operator +=(EulerAngle a);
	Quaternion &operator -=(EulerAngle a);
	Quaternion &operator *=(EulerAngle a);
	Quaternion &operator /=(EulerAngle a);
};

inline Quaternion Conjugate(Quaternion q)
{
	return Quaternion(q.r, -q.i, -q.j, -q.k);
}

inline Quaternion operator +(Quaternion q1, Quaternion q2)
{
	return Quaternion(q1.r + q2.r, q1.i + q2.i, q1.j + q2.j, q1.k + q2.k);
}

inline Quaternion operator -(Quaternion q1, Quaternion q2)
{
	return Quaternion(q1.r - q2.r, q1.i - q2.i, q1.j - q2.j, q1.k - q2.k);
}

inline Quaternion operator *(Quaternion q1, Quaternion q2)
{
	Vector v1(q1.i, q1.j, q1.k);
	Vector v2(q2.i, q2.j, q2.k);
	return Quaternion(q1.r*q2.r - Dot(v1, v2), q1.r*v2 + q2.r*v1 + Cross(v1, v2));
}

inline Quaternion operator /(Quaternion q1, Quaternion q2)
{
	return q1*Conjugate(q2);
}

inline Quaternion operator +(Quaternion q, AxisAngle a)
{
	Quaternion q2;
	q2 = a;
	return q + q2;
}

inline Quaternion operator -(Quaternion q, AxisAngle a)
{
	Quaternion q2;
	q2 = a;
	return q - q2;
}

inline Quaternion operator *(Quaternion q, AxisAngle a)
{
	Quaternion q2;
	q2 = a;
	return q * q2;
}

inline Quaternion operator /(Quaternion q, AxisAngle a)
{
	Quaternion q2;
	q2 = a;
	return q / q2;
}

inline Quaternion operator +(AxisAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 + q;
}

inline Quaternion operator -(AxisAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 - q;
}

inline Quaternion operator *(AxisAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 * q;
}

inline Quaternion operator /(AxisAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 / q;
}

inline Quaternion operator +(Quaternion q, EulerAngle a)
{
	Quaternion q2;
	q2 = a;
	return q + q2;
}

inline Quaternion operator -(Quaternion q, EulerAngle a)
{
	Quaternion q2;
	q2 = a;
	return q - q2;
}

inline Quaternion operator *(Quaternion q, EulerAngle a)
{
	Quaternion q2;
	q2 = a;
	return q * q2;
}

inline Quaternion operator /(Quaternion q, EulerAngle a)
{
	Quaternion q2;
	q2 = a;
	return q / q2;
}

inline Quaternion operator +(EulerAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 + q;
}

inline Quaternion operator -(EulerAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 - q;
}

inline Quaternion operator *(EulerAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 * q;
}

inline Quaternion operator /(EulerAngle a, Quaternion q)
{
	Quaternion q2;
	q2 = a;
	return q2 / q;
}

inline Quaternion operator +(Quaternion q, GLfloat f)
{
	return Quaternion(q.r + f, q.i + f, q.j + f, q.k + f);
}

inline Quaternion operator -(Quaternion q, GLfloat f)
{
	return Quaternion(q.r - f, q.i - f, q.j - f, q.k - f);
}

inline Quaternion operator *(Quaternion q, GLfloat f)
{
	return Quaternion(q.r * f, q.i * f, q.j * f, q.k * f);
}

inline Quaternion operator /(Quaternion q, GLfloat f)
{
	return Quaternion(q.r / f, q.i / f, q.j / f, q.k / f);
}

inline Quaternion operator +(GLfloat f, Quaternion q)
{
	return Quaternion(f + q.r, f + q.i, f + q.j, f + q.k);
}

inline Quaternion operator -(GLfloat f, Quaternion q)
{
	return Quaternion(f - q.r, f - q.i, f - q.j, f - q.k);
}

inline Quaternion operator *(GLfloat f, Quaternion q)
{
	return Quaternion(f * q.r, f * q.i, f * q.j, f * q.k);
}

inline Quaternion operator /(GLfloat f, Quaternion q)
{
	return Quaternion(f / q.r, f / q.i, f / q.j, f / q.k);
}

inline GLfloat Magnitude(Quaternion q)
{
	return sqrt(q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k);
}

inline Quaternion Normalize(Quaternion q)
{
	GLfloat length = Magnitude(q);
	return Quaternion(q.r/length, q.i/length, q.j/length, q.k/length);
}

inline GLfloat Dot(Quaternion q1, Quaternion q2)
{
	return q1.r*q2.r + q1.i*q2.i + q1.j*q2.j + q1.k*q2.k;
}

inline GLfloat Distance(Quaternion q1, Quaternion q2)
{
	return sqrt(sqr(q1.r - q2.r) + sqr(q1.i - q2.i) + sqr(q1.j - q2.j) + sqr(q1.k - q2.k));
}

inline Quaternion LERP(Quaternion q1, Quaternion q2, GLfloat t)
{
	return (1 - t)*q1 + t*q2;
}

inline Quaternion SLERP(Quaternion q1, Quaternion q2, GLfloat t)
{
	GLfloat dot = Dot(q1, q2);
	Quaternion q = q1;
	if (dot < 0)
	{
		q = Conjugate(q1);
		dot = Dot(q, q2);
	}
	GLfloat theta = acos(dot);
	return (sin((1-t)*theta)/sin(theta))*q + (sin(t*theta)/sin(theta))*q2;
}

inline Quaternion Interpolate(Quaternion q1, Quaternion q2, GLfloat t)
{
	if (Distance(q1, q2) <= 0.01)
		return LERP(q1, q2, t);
	else
		return SLERP(q1, q2, t);
}

inline void QuaterniontoMatrix(Quaternion q, GLfloat m[16])
{
	m[0] = 1 - 2*q.j*q.j - 2*q.k*q.k;
	m[1] = 2*q.i*q.j + 2*q.r*q.k;
	m[2] = 2*q.i*q.k - 2*q.r*q.j;
	m[3] = 0;
	m[4] = 2*q.i*q.j - 2*q.r*q.k;
	m[5] = 1 - 2*q.i*q.i - 2*q.k*q.k;
	m[6] = 2*q.j*q.k + 2*q.r*q.i;
	m[7] = 0;
	m[8] = 2*q.i*q.k + 2*q.r*q.j;
	m[9] = 2*q.j*q.k - 2*q.r*q.i;
	m[10] = 1 - 2*q.i*q.i - 2*q.j*q.j;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;
}

EulerAngle	QuaterniontoEuler(Quaternion q);
AxisAngle	QuaterniontoAxis(Quaternion q);

/********************************************
 *											*
 *											*
 *			***Simple Geometries***		   	*
 *											*
 *											*
 ********************************************/

struct Triangle
{
	Triangle()
	{
		Next = NULL;
	}
	Triangle(Vector v1, Vector v2, Vector v3)
	{
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
		Next = NULL;
	}
	Vertex v[3];
	
	int CollisionType;
	
	Triangle *Next;
	
	Triangle &operator =(Triangle t);
};

void AddTriangle(Triangle *Data, Triangle *t);
void ReleaseTriangles(Triangle *t);

GLfloat AreaOfTriangle(Triangle t);

struct Box
{
	Box()
	{
		edges[0][0] = 0;
		edges[0][1] = 1;
		edges[1][0] = 1;
		edges[1][1] = 2;
		edges[2][0] = 2;
		edges[2][1] = 3;
		edges[3][0] = 3;
		edges[3][1] = 0;
		
		edges[4][0] = 4;
		edges[4][1] = 5;
		edges[5][0] = 5;
		edges[5][1] = 6;
		edges[6][0] = 6;
		edges[6][1] = 7;
		edges[7][0] = 7;
		edges[7][1] = 4;
		
		edges[8][0] = 0;
		edges[8][1] = 4;
		edges[9][0] = 1;
		edges[9][1] = 5;
		edges[10][0] = 2;
		edges[10][1] = 6;
		edges[11][0] = 3;
		edges[11][1] = 7;
		
		faces[0][0] = 0;
		faces[0][1] = 1;
		faces[0][2] = 2;
		faces[0][3] = 3;
		
		faces[1][0] = 4;
		faces[1][1] = 5;
		faces[1][2] = 6;
		faces[1][3] = 7;
		
		faces[2][0] = 0;
		faces[2][1] = 4;
		faces[2][2] = 5;
		faces[2][3] = 1;
		
		faces[3][0] = 1;
		faces[3][1] = 5;
		faces[3][2] = 6;
		faces[3][3] = 2;
		
		faces[4][0] = 2;
		faces[4][1] = 6;
		faces[4][2] = 7;
		faces[4][3] = 3;
		
		faces[5][0] = 3;
		faces[5][1] = 7;
		faces[5][2] = 4;
		faces[5][3] = 0;
	}
	Vertex v[8];
	short edges[12][2];
	short faces[6][4];
};

void CreateBox(Vertex Max, Vertex Min, Box *b);
void AddGeometries(Box b, Triangle t, Vertex Output[24]);

struct Plane
{
	Plane()
	{
	}
	Plane(Vector n, GLfloat d)
	{
		Normal = n;
		Distance = d;
	}
	Plane(GLfloat f[4])
	{
		Normal = Vector(f[0], f[1], f[2]);
		Distance = f[3];
	}
	Vector Normal;
	GLfloat Distance;
	
	Plane &operator =(Plane p);
};

Plane TriangleToPlane(Vertex v1, Vertex v2, Vertex v3);
int ClassifyVertextoPlane(Plane p, Vertex v);
int ClassifyTriangletoPlane(Plane p, Triangle *t);

int intersect_RayTriangle(Vector R1, Vector R2, GLfloat radius, Vector T1, Vector T2, Vector T3, Vector *I, Vector *Normal);
int intersect_RayPlane(Vector R1, Vector R2, Plane p, Vector *I);

#endif
