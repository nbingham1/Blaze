/*
 *  Planet.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 7/31/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Planet_h
#define Planet_h

#include "OpenGLIncludes.h"
#include "CoreMathematics.h"
#include "CorePhysics.h"
#include "Shader.h"
#include "Player.h"
#include "Collision.h"

const int node_size = 16;

struct planet_node
{
	int seed;
	
	GLdouble *heights;
		
	GLdouble percent_loaded;
	
	GLdouble x_size, x_angle;
	GLdouble y_size, y_angle;
	GLdouble radius;
	
	GLdouble average;
	GLdouble minimum;
	GLdouble maximum;
	
	planet_node *parent;
	planet_node *children;
		
	void load(int s);
	void destroy();
	
	void split();
	void merge();
	
	void render(bool r, Player *p_player, Vector *player_vec, GLdouble ax, GLdouble ay, GLdouble nax, GLdouble nay, GLdouble dist);
	void renderwater(bool r, Player *p_player, Vector *player_vec, GLdouble ax, GLdouble ay, GLdouble nax, GLdouble nay, GLdouble dist);
};

struct Planet
{
	string		name;
	planet_node data;
	ModelPhysics Physics;
	
	Player *player_pointer;
	
	GLhandleARB VertShad;
	GLhandleARB FragShad;
	GLhandleARB ShadProg;
	GLuint tex;
	
	GLhandleARB WaterVertShad;
	GLhandleARB WaterFragShad;
	GLhandleARB WaterShadProg;
	GLuint watertex;
	
	void Load(string Name, Player *p_player);
	void Render();
	void Release();
};

#endif