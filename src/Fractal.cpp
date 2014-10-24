/*
 *  Fractal.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Fractal.h"

void FilterHeightBand(GLdouble *Band, int Stride, int Count, GLdouble Filter)
{
	GLdouble v = Band[0];
	int j = Stride;
	int i;
	
	for (i = 0; i < Count-1; i++)
	{
		Band[j] = Filter*v + (1 - Filter)*Band[j];
		v = Band[j];
		j += Stride;
	}
}

void FilterHeightField(GLdouble *Heights, int Size, GLdouble Filter)
{
	int i;
	
	for (i = 0; i < Size; i++)
		FilterHeightBand(&Heights[Size*i], 1, Size, Filter);
	
	for (i = 0; i < Size; i++)
		FilterHeightBand(&Heights[Size*i + Size-1], -1, Size, Filter);
		
	for (i = 0; i < Size; i++)
		FilterHeightBand(&Heights[i], Size, Size, Filter);
		
	for (i = 0; i < Size; i++)
		FilterHeightBand(&Heights[Size*(Size-1) + i], -Size, Size, Filter);
}

void NormalizeHeightField(GLdouble *Heights, int Size)
{
	GLdouble Max = 0, Min = 0;
	GLdouble Height;
	
	for (int x = 0; x < Size*Size; x++)
	{
		if (Heights[x] > Max)
			Max = Heights[x];
		if (Heights[x] < Min)
			Min = Heights[x];
	}
	
	Height = Max-Min;
	
	for (int x = 0; x < Size*Size; x++)
		Heights[x] = 256*Heights[x]/Height;
}

GLdouble *GenerateFaultHeights(int RandomSeed, int Size, int Iterations, int MinHeight, int MaxHeight, GLdouble Filter)
{
	GLdouble *Heights = NULL;
	int CurrentIter;
	GLdouble Height;
	int RandX1, RandZ1;
	int RandX2, RandZ2;
	int DirX1, DirZ1;
	int DirX2, DirZ2;
	int x, z;
	int i;
	
	srand(RandomSeed);
		
	Heights = new GLdouble[Size*Size];
	
	for (i = 0; i < Size*Size; i++)
		Heights[i] = 0;
	
	for (CurrentIter = 0; CurrentIter < Iterations; CurrentIter++)
	{
		Height = (GLdouble)MaxHeight - (GLdouble(MaxHeight - MinHeight)*GLdouble(CurrentIter))/GLdouble(Iterations);
		RandX1 = rand() % Size;
		RandZ1 = rand() % Size;
		
		do
		{
			RandX2 = rand() % Size;
			RandZ2 = rand() % Size;
		} while (RandX2 == RandX1 && RandZ2 == RandZ1);
		
		DirX1 = RandX2 - RandX1;
		DirZ1 = RandZ2 - RandZ1;
		
		for (z = 0; z < Size; z++)
		{
			for (x = 0; x < Size; x++)
			{
				DirX2 = x - RandX1;
				DirZ2 = z - RandZ1;
				
				if ((DirX2*DirZ1 - DirX1*DirZ2) > 0)
					Heights[(z*Size) + x] += Height;
			}
		}
	}
	
	FilterHeightField(Heights, Size, Filter);
	
	//NormalizeHeightField(Heights, Size);
		
	return Heights;
}

GLdouble *GenerateRandomHeights(int RandomSeed, int Size, int MaxHeightChange, GLdouble Filter, GLdouble StartHeight)
{
	GLdouble *Heights = new GLdouble[Size*Size];
	
	for (int i = 0; i < Size*Size; i++)
		Heights[i] = StartHeight;
	
	srand(RandomSeed);
	GLdouble newchange;
	for (int x = 1; x < Size-1; x++)
	{
		for (int y = 1; y < Size-1; y++)
		{
			newchange = double((rand() % (MaxHeightChange + MaxHeightChange)) - MaxHeightChange);
			Heights[x*Size + y] = (Heights[x*Size + (y-1)] + Heights[(x-1)*Size + y])/2 + newchange;
			if (Heights[x*Size + y] < StartHeight-1000)
				Heights[x*Size + y] = StartHeight-1000;
		}
	}
	
	FilterHeightField(Heights, Size, Filter);
	
	return Heights;
}