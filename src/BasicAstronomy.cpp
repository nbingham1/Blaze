/*
 *  BasicAstronomy.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "BasicAstronomy.h"

void camera_reference::Initialize(Camera *c, PhsHandle *phs)
{
	cam = c;
	body_physics = phs;
	ground_height = 0.0;
}

void camera_reference::Update()
{
	// current pos data
	
	distance = Distance(cam->Host->Physics.Position, body_physics->Position);
	pvec = Normalize(cam->Host->Physics.Position - body_physics->Position);
	
	latitude = asin(pvec.y) + pi/2.0;
	if (pvec.x/cos(latitude - pi/2.0) > 1.0 || pvec.x/cos(latitude - pi/2.0) < -1.0)
		longitude = atan(pvec.z/pvec.x);
	else
		longitude = acos(pvec.x/cos(latitude - pi/2.0));
		
	if (pvec.z < 0)
		longitude = -longitude + 2.0*pi;
	
	// next pos data
	
	npvec = Normalize((cam->Host->Physics.Position + cam->Host->Physics.LinearVelocity) - body_physics->Position);
	
	nlatitude = asin(npvec.y) + pi/2.0;
	if (npvec.x/cos(nlatitude - pi/2.0) > 1.0 || npvec.x/cos(nlatitude - pi/2.0) < -1.0)
		nlongitude = atan(npvec.z/npvec.x);
	else
		nlongitude = acos(npvec.x/cos(nlatitude - pi/2.0));
		
	if (npvec.z < 0)
		nlongitude = -nlongitude + 2.0*pi;
	
	ndistance = Distance(cam->Host->Physics.Position + cam->Host->Physics.LinearVelocity, body_physics->Position);
	visualangle = acos(body_physics->Radius/distance);
	if (!(visualangle >= 0.0) && !(visualangle < 0.0))
		visualangle = 0.000931667;
	
	Vector p = pi/180 * cam->Host->Physics.Orientation;
	Vector v1 = -pi/180 * cam->Orientation;
	
	vp_vis = RotateX(RotateY(RotateZ(RotateZ(RotateY(RotateX(Vector(0.0, 0.0, -1.0), v1.x), v1.y), v1.z), p.z), p.y), p.x);
	vp_dist = -1*Dot(vp_vis, distance*pvec);
}

void camera_reference::UpdateScale()
{
	if (distance-(body_physics->Radius + ground_height) <= 7.0)
		scale = 0.5;
	else
		scale = ((1.0/(distance - (body_physics->Radius + ground_height))) + absolute((1.0/(distance - (body_physics->Radius + ground_height)))))/2.0;
	
	if (cam->viewscale < scale)
		cam->viewscale = scale;
}

void camera_reference::CheckCollision(PhsHandle *phs)
{
	if (distance < phs->Radius + ground_height || ndistance < phs->Radius + ground_height)
	{
		GLdouble angle_x = -longitude + 2.0*pi;
		
		GLdouble VelMag = Magnitude(cam->Host->Physics.LinearVelocity);
		GLdouble ForceMag = cam->Host->Physics.Mass*(-VelMag*cam->Host->Physics.Elasticity*phs->Elasticity - VelMag);
		
		cam->Host->Physics.ApplyForce(Normalize(cam->Host->Physics.LinearVelocity)*ForceMag, Normalize(phs->Position - cam->Host->Physics.Position)*cam->Host->Physics.Radius);
		
		cam->Host->Physics.Orientation = (180.0/pi)*(Vector(0.0, angle_x, 2.0*pi - acos(Dot(Vector(0.0, 1.0, 0.0), pvec))));
		cam->Host->Physics.AngularVelocity = Vector();
		
		//cam->Host->Physics.LinearVelocity -= pvec*Dot(cam->Host->Physics.LinearVelocity, pvec);
		cam->Host->Physics.Position += pvec*(phs->Radius + ground_height - distance);
		
		distance = Distance(cam->Host->Physics.Position, phs->Position);
		pvec = Normalize(cam->Host->Physics.Position - phs->Position);
	}
}