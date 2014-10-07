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

#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

void FilterHeightBand(float *Band, int Stride, int Count, float Filter);
void FilterHeightField(float *Heights, int Size, float Filter);
void NormalizeHeightField(float *Heights, int Size);

float *GenerateFaultHeights(int RandomSeed, int Size, int Iterations, int MinHeight, int MaxHeight, float Filter);
float *GenerateRandomHeights(int RandomSeed, int Size, int MaxHeightChange, float Filter, float StartHeight);

#endif

