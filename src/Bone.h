/*
 *  Bone.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/6/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Bone_h
#define Bone_h

#include "CoreMathematics.h"
#include "OpenGLIncludes.h"

struct Bone
{
	int Name;
	int Parent;
	
	GLfloat Mass;
	
	Vector		Position;
	Vector		LinearVelocity;
	Vector		LinearAccelaration;
	
	Quaternion	Orientation;
	Quaternion	AngularVelocity;
	Quaternion	AngularAccelaration;
	
	Vector RotationalInertia;
	Vector Torque;
	
	unsigned short NumRotationFrames;
	unsigned short NumTranslationFrames;
	
	Vector		TranslationFrames[300];
	Quaternion	RotationFrames[300];
};

#endif
