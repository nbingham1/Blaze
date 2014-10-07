/*
 *  Quaternion.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Quaternion_h
#define Quaternion_h

#include <math.h>

struct Quaternion
{
	Quaternion()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 0.0;
	}
	
	Quaternion(double a, double b, double c, double d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}
	
	double x, y, z, w;
	
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

inline double Magnitude(Quaternion q)
{
  return sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

inline Quaternion Normalize(Quaternion q)
{
  double l = Magnitude(q);
  return Quaternion(q.x/l, q.y/l, q.z/l, q.w/l);
}

#endif