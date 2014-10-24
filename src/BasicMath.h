/*
 *  BasicMath.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef BasicMath_h
#define BasicMath_h

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

#include "Vector.h"

#ifndef pi
#define pi 3.14159265358979323
#endif

#ifndef elipson
#define elipson 10e-6
#endif

GLdouble GetMin(Vector *Points, int Num, Vector Axis);
GLdouble GetMax(Vector *Points, int Num, Vector Axis);
GLdouble absolute(GLdouble f);
GLdouble modulate(GLdouble f, GLdouble m);
GLdouble clamp(GLdouble f, GLdouble a, GLdouble b);

bool Intersect(Vector *p1, int n1, Vector *p2, int n2, Vector Axis);
bool IntersectBoxTri(Vector BoxPoints[16], Vector v1, Vector v2, Vector v3);

#endif
