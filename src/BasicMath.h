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

double GetMin(Vector *Points, int Num, Vector Axis);
double GetMax(Vector *Points, int Num, Vector Axis);
GLfloat absolute(GLfloat f);
GLfloat modulate(GLfloat f, GLfloat m);

bool Intersect(Vector *p1, int n1, Vector *p2, int n2, Vector Axis);
bool IntersectBoxTri(Vector BoxPoints[16], Vector v1, Vector v2, Vector v3);

#endif
