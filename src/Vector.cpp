/*
 *  Vector.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Vector.h"
#include "BasicMath.h"

Vector::Vector(GLdouble X, GLdouble Y, GLdouble Z)
{
	x = X;
	y = Y;
	z = Z;
}

Vector::Vector(GLdouble *arr)
{
	x = arr[0];
	y = arr[1];
	z = arr[2];
}

Vector::Vector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::~Vector()
{
}

Vector &Vector::operator =(Vector v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

Vector &Vector::operator +=(Vector v)
{
	*this = *this + v;
	return *this;
}

Vector &Vector::operator -=(Vector v)
{
	*this = *this - v;
	return *this;
}

Vector &Vector::operator *=(Vector v)
{
	*this = *this * v;
	return *this;
}

Vector &Vector::operator /=(Vector v)
{
	*this = *this / v;
	return *this;
}

Vector &Vector::operator +=(GLdouble f)
{
	*this = *this + f;
	return *this;
}

Vector &Vector::operator -=(GLdouble f)
{
	*this = *this - f;
	return *this;
}

Vector &Vector::operator *=(GLdouble f)
{
	*this = *this * f;
	return *this;
}

Vector &Vector::operator /=(GLdouble f)
{
	*this = *this / f;
	return *this;
}

Vector Normalize(Vector v)
{
	return (v / GLdouble(max(double(Magnitude(v)), 0.001)));
}

Vector AngleBetween(Vector v1, Vector v2)
{
	GLdouble a = acos(Dot(v1, v2));
	Vector n = Normalize(Cross(v1, v2));

	Vector result;

	result.y = atan2(n.y*sin(a)-n.x*n.z*(1.0-cos(a)), 1.0-(n.y*n.y+n.z*n.z)*(1.0-cos(a)));
	result.z = asin(n.x*n.y*(1.0-cos(a))+n.z*sin(a));
	result.x = atan2(n.x*sin(a)-n.y*n.z*(1.0-cos(a)), 1.0-(n.x*n.x+n.z*n.z)*(1.0-cos(a)));

	if (n.y == 1.0)
	{
		result.y = 2.0*atan2(n.x*sin(a/2.0), cos(a/2.0));
		result.z = 3.1415926535898;
		result.x = 0.0;
	}
	else if (n.y == -1.0)
	{
		result.y = -2.0*atan2(n.x*sin(a/2.0), cos(a/2.0));
		result.z = -3.1415926535898;
		result.x = 0;
	}

	return result;
}
