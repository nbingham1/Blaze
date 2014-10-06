/*
	GameMisc.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 12/18/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameMisc.h"

char* string_to_char(string value)
{
	char *result = new char[value.length() + 1];
	
	value.copy(result, value.length());
	result[value.length()] = '\0';
	
	return result;
}

char* int_to_char(int Num)
{
	char* n = new char[20];
	sprintf(n, "%i", Num);
	return n;
}

char* better_fgets(char *line, int len, FILE *in_file)
{
   char *temp = line;
   int   val;

   if (--len < 0)
      return NULL;

   if (len)
   {
      do
      {
         val = getc(in_file);

         if (val == EOF)
         {
            if (feof(in_file) && temp != line)
               break;
            else
            {
               line = NULL;
               return NULL;
            }
         }
         *temp++ = val;
      } while (val != '\r' && val != '\n' && --len);
   }
   *temp = '\0';

   return line;
}

void CalculateNormal(Vector p1, Vector p2, Vector p3, Vector *Normal)
{
	Vector v1(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
	Vector v2(p2.x - p3.x, p2.y - p3.y, p2.z - p3.z);
	
	Normal->x = v1.y * v2.z - v1.z * v2.y;
	Normal->y = v1.z * v2.x - v1.x * v2.z;
	Normal->z = v1.x * v2.y - v1.y * v2.x;

	Normal->Normalize();
}

GLfloat Det(GLfloat a[4*4])
{
	return a[0+0*4] * (
          a[1+1*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
        - a[1+2*4] * ( a[2+1*4] * a[3+3*4] - a[2+3*4] * a[3+1*4] )
        + a[1+3*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] ) )
    - a[0+1*4] * (
          a[1+0*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
        - a[1+2*4] * ( a[2+0*4] * a[3+3*4] - a[2+3*4] * a[3+0*4] )
        + a[1+3*4] * ( a[2+0*4] * a[3+2*4] - a[2+2*4] * a[3+0*4] ) )
    + a[0+2*4] * (
          a[1+0*4] * ( a[2+1*4] * a[3+3*4] - a[2+3*4] * a[3+1*4] )
        - a[1+1*4] * ( a[2+0*4] * a[3+3*4] - a[2+3*4] * a[3+0*4] )
        + a[1+3*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] ) )
    - a[0+3*4] * (
          a[1+0*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] )
        - a[1+1*4] * ( a[2+0*4] * a[3+2*4] - a[2+2*4] * a[3+0*4] )
        + a[1+2*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] ) );
}

GLfloat VolumeOfTetrahedron(GLfloat tetra[3*4])
{
	GLfloat a[4*4];
    int i;
    int j;
    GLfloat volume;

    for (i = 0; i < 3; i++)
    {
		for (j = 0; j < 4; j++)
		{ 
			a[i+j*4] = tetra[i+j*3];
		}
	}

	i = 3;
	for (j = 0; j < 4; j++)
	{
		a[i+j*4] = 1.0;
	}

	volume = fabs(Det(a)) / 6.0;

	return volume;
}

GLfloat AreaOfTriangle(Vector v0, Vector v1, Vector v2)
{
	GLfloat a, b, c, s;
	a = sqrt((v0.x-v1.x)*(v0.x-v1.x) + (v0.y-v1.y)*(v0.y-v1.y) + (v0.z-v1.z)*(v0.z-v1.z));
	b = sqrt((v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y) + (v1.z-v2.z)*(v1.z-v2.z));
	c = sqrt((v2.x-v0.x)*(v2.x-v0.x) + (v2.y-v0.y)*(v2.y-v0.y) + (v2.z-v0.z)*(v2.z-v0.z));
	s = (a + b + c)/2;
	
	return sqrt(s*(s-a)*(s-b)*(s-c));
}

int
intersect_RayTriangle(Vector R1, Vector R2, Vector T1, Vector T2, Vector T3, Vector *I, Vector *N)
{
    Vector    u, v, n;             // triangle vectors
    Vector    dir, w0, w;          // ray vectors
    GLfloat     r, a, b;             // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = T2 - T1;
    v = T3 - T1;
    n = u.Cross(v);             // cross product
	*N = n;
    if (n == Vector(0, 0, 0))            // triangle is degenerate
        return -1;                 // do not deal with this case

    dir = R2 - R1;             // ray direction vector
    w0 = R1 - T1;
    a = -n.Dot(w0);
    b = n.Dot(dir);
    if (fabs(b) < 0.00000001)
	{							// ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0) // ray goes away from triangle
		return 0;                  // => no intersect
	if (r > 1.0) // ray goes away from triangle
		return 0;  
    // for a segment, also test if (r > 1.0) => no intersect

    *I = R1 + dir * r;           // intersect point of ray and plane

    // is I inside T?
    GLfloat    uu, uv, vv, wu, wv, D;
    uu = u.Dot(u);
    uv = u.Dot(v);
    vv = v.Dot(v);
    w = *I - T1;
    wu = w.Dot(u);
    wv = w.Dot(v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    GLfloat s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0)        // I is outside T
        return 3;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 3;

    return 1;                      // I is in T
}

GLfloat DistanceFormula(Vector V1, Vector V2)
{
	return sqrt((V2.x-V1.x)*(V2.x-V1.x) + (V2.y-V1.y)*(V2.y-V1.y) + (V2.z-V1.z)*(V2.z-V1.z));
}

int IntersectRayVolume(Vector R1, Vector R2, Vector *Vol, Vector VolNormal, int NumVol)
{
	// Functionality unfinished
	return 0;
}
