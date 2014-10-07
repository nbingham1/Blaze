/*
 *  Collision.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 8/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Collision.h"
#include "BasicPhysics.h"

bool model_triangle_collision(Model *m, Vector *t)
{
	Matrix A1, A2;
	Vector v[16];
	Vector up = -1*Normalize(m->Physics.Gravity);
	Vector next = m->Physics.Position + m->Physics.LinearVelocity;
	Vector current = m->Physics.Position + MidPoint(m->Min, m->Max);
	Box b;
	b.SetBox(m->Min, m->Max);
	EulertoMatrix(m->Physics.Orientation, A1);
	EulertoMatrix(m->Physics.Orientation + m->Physics.AngularVelocity, A2);
	for (int x = 0; x < 8; x++)
	{
		v[x] = A1*b.Vertices[x] + m->Physics.Position;
		v[x+8] = A2*b.Vertices[x] + next;
	}
	
	bool coll = IntersectBoxTri(v, t[0], t[1], t[2]);
		
	if (coll)
	{
		Vector n = CalculateNormal(t[0], t[1], t[2]);
		
		if (Dot(n, up) < 0.0001)
			n*=-1.0;
		
		GLdouble dot = Dot(m->Physics.LinearVelocity, n);
		
		m->Physics.SumForces.Force -= (m->Physics.LinearMomentum - m->Physics.Mass*(m->Physics.LinearVelocity - 2*dot*n));
	}
	
	return coll;
}