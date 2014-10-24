/*
 *  Fractal.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Fractal_h
#define Fractal_h

#include "graphics.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

void FilterHeightBand(GLdouble *Band, int Stride, int Count, GLdouble Filter);
void FilterHeightField(GLdouble *Heights, int Size, GLdouble Filter);
void NormalizeHeightField(GLdouble *Heights, int Size);

GLdouble *GenerateFaultHeights(int RandomSeed, int Size, int Iterations, int MinHeight, int MaxHeight, GLdouble Filter);
GLdouble *GenerateRandomHeights(int RandomSeed, int Size, int MaxHeightChange, GLdouble Filter, GLdouble StartHeight);

#endif
