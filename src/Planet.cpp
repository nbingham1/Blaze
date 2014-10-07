/*
 *  Planet.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 7/31/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Planet.h"

void planet_node::load(int s)
{
	heights = (GLdouble*)malloc(sizeof(GLdouble)*node_size*node_size);
	if (!heights)
		cout << "ARRG" << endl;
	
	seed = s;
	x_size = 2.0*pi;
	y_size = pi;
	x_angle = 0;
	y_angle = 0;
	parent = NULL;
	children = NULL;
	srand(seed);
	
	radius = 6380;//GLdouble(rand()%(9000000) + 1000000);
	
	GLdouble r = 0;
	int t = node_size;
	GLdouble count = 0.0;
	average = 0.0;
	minimum = -radius/10;
	maximum = radius/10;

	for (int x = 0; x < t; x++)
		for (int y = 0; y < t; y++)
		{
			r = GLdouble(rand()%int(10000000*(maximum - minimum)))/10000000.0 + minimum;
			for (int x1 = 0; x1 < (node_size/t); x1++)
				for (int y1 = 0; y1 < (node_size/t); y1++)
				{
					heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)] = r;
					//if ((x*(node_size/t) + x1 - 1) >= 0 && (y*(node_size/t) + y1 - 1) >= 0)
					//	heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)] += heights[(x*(node_size/t) + x1 - 1)*node_size + (y*(node_size/t) + y1 - 1)];
					average += heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)];
					count += 1.0;
					//if (heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)] > maximum)
					//	maximum = heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)];
					//if (heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)] < minimum)
					//	minimum = heights[(x*(node_size/t) + x1)*node_size + (y*(node_size/t) + y1)];
				}
		}
	
	average /= count;
	
	for (int x = 0; x < node_size; x++)
	{
		heights[(node_size-1)*node_size + x] = heights[x];
		heights[x*node_size + node_size-1] = heights[node_size-1];
		heights[x*node_size] = heights[0];
	}
	
	Vector n1, n2;
	percent_loaded = 1.00;
}

void planet_node::destroy()
{
	if (heights)
		free(heights);
	
	if (children)
	{
		for (int x = 0; x < 4; x++)
			children[x].destroy();
		
		free(children);
	}
}

void planet_node::split()
{
	GLdouble count;
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			children[x].split();
	}
	else
	{
		children = (planet_node*)malloc(sizeof(planet_node)*4);
		if (!children)
			cout << "ARRG" << endl;
		int u_pos, v_pos, u1_pos, v1_pos;
		Vector n1, n2;
		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
			{
				children[x*2+y].seed = heights[(x*node_size/2)*node_size + (y*node_size/2)];
				srand(children[x*2+y].seed);
				children[x*2+y].x_size = x_size/2.0;
				children[x*2+y].y_size = y_size/2.0;
				children[x*2+y].x_angle = x_angle + x*(x_size/2.0);
				children[x*2+y].y_angle = y_angle + y*(y_size/2.0);
				children[x*2+y].radius = radius;
				children[x*2+y].children = NULL;
				children[x*2+y].parent = this;
				children[x*2+y].heights = (GLdouble*)malloc(sizeof(GLdouble)*node_size*node_size);
				if (!children[x*2+y].heights)
					cout << "ARRG" << endl;
				children[x*2+y].average = 0.0;
				children[x*2+y].minimum = minimum;
				children[x*2+y].maximum = maximum;
				count = 0.0;
				for (int u = 0; u < node_size; u++)
				{
					u_pos = (u + x*node_size - x)/2;
					u1_pos = (u + 1 + x*node_size - x)/2;
					for (int v = 0; v < node_size; v++)
					{
						v_pos = (v + y*node_size - y)/2;
						v1_pos = (v + 1 + y*node_size - y)/2;
						children[x*2+y].heights[u*node_size + v] = (heights[u_pos*node_size + v_pos] + 
																	heights[u1_pos*node_size + v_pos] + 
																	heights[u1_pos*node_size + v1_pos] + 
																	heights[u_pos*node_size + v1_pos])/4.0;
						if ((u_pos != u1_pos || v_pos != v1_pos) && (u > 1 && v > 1 && u < node_size-1 && v < node_size-1))
						{
							GLdouble min = 1000000000, max = -1000000000;
							if (heights[u_pos*node_size + v_pos] < min)
								min = heights[u_pos*node_size + v_pos];
							if (heights[u_pos*node_size + v_pos] > max)
								max = heights[u_pos*node_size + v_pos];
							
							if (heights[u1_pos*node_size + v_pos] < min)
								min = heights[u1_pos*node_size + v_pos];
							if (heights[u1_pos*node_size + v_pos] > max)
								max = heights[u1_pos*node_size + v_pos];
								
							if (heights[u1_pos*node_size + v1_pos] < min)
								min = heights[u1_pos*node_size + v1_pos];
							if (heights[u1_pos*node_size + v1_pos] > max)
								max = heights[u1_pos*node_size + v1_pos];
								
							if (heights[u_pos*node_size + v1_pos] < min)
								min = heights[u_pos*node_size + v1_pos];
							if (heights[u_pos*node_size + v1_pos] > max)
								max = heights[u_pos*node_size + v1_pos];
							
							
							if (1000000.0*(max - min) > 1.0 && u != 1 && v != 1 && u != node_size-1 && v != node_size-1)
								children[x*2+y].heights[u*node_size + v] += GLdouble(rand()%int(1000000.0*(max - min)))/1000000.0 - (max - min)/2;
						}			
						children[x*2+y].average += children[x*2+y].heights[u*node_size + v];
						count+=1.0;
						if (children[x*2+y].heights[u*node_size + v] > children[x*2+y].maximum)
							children[x*2+y].maximum = children[x*2+y].heights[u*node_size + v];
						if (children[x*2+y].heights[u*node_size + v] < children[x*2+y].minimum)
							children[x*2+y].minimum = children[x*2+y].heights[u*node_size + v];
					}
				}
				children[x*2+y].average /= count;
			}
		
		free(heights);
		heights = NULL;
	}
}

void planet_node::merge()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			if (children[x].children != NULL)
				children[x].merge();
		
		heights = (GLdouble*)malloc(sizeof(GLdouble)*node_size*node_size);
		if (!heights)
					cout << "ARRG" << endl;
		
		for (int u = 0; u < node_size; u++)
			for (int v = 0; v < node_size; v++)
				heights[u*node_size + v] = children[(u/(node_size/2))*2+(v/(node_size/2))].heights[(2*(u - (node_size/2)*(u/(node_size/2))) + u/(node_size/2))*node_size + (2*(v - (node_size/2)*(v/(node_size/2))) + v/(node_size/2))];
		
		for (int x = 0; x < 4; x++)
			children[x].destroy();
		
		free(children);
		children = NULL;
	}
}

void planet_node::render(bool r, Player *p_player, Vector *player_vec, GLdouble ax, GLdouble ay, GLdouble nax, GLdouble nay, GLdouble dist)
{
	GLdouble avis = acos((radius + maximum)/dist);
	if (!(avis >= 0.0) && !(avis < 0.0))
		avis = pi*.001;
		
	//avis /= 16.0/GLdouble(node_size);
				
	GLdouble cx = x_angle, cy = y_angle;
	
	if (cx + x_size < ax && ax - (cx + x_size) > pi)
		cx += pi/2;
	else if (cx > ax && cx - ax > pi)
		cx -=  pi/2;
	
	GLdouble x_len = min(ax + avis, cx + x_size) - max(ax - avis, cx);
	x_len = max(x_len, GLdouble(0.0));
	GLdouble y_len = min(ay + avis, cy + y_size) - max(ay - avis, cy);
	y_len = max(y_len, GLdouble(0.0));
	
	GLdouble area = x_len*y_len;
	GLdouble vis_area = 4.0*avis*avis;
	
	GLdouble percent = area/vis_area;
	if (percent >= .25 && children == NULL)
		split();
	if (percent <= 0.05 && children)
		merge();
	
	GLdouble fax, sax, fay, say;
	if (ax < nax)
	{
		fax = ax;
		sax = nax;
	}
	else
	{
		fax = nax;
		sax = ax;
	}
	if (ay < nay)
	{
		fay = ay;
		say = nay;
	}
	else
	{
		fay = nay;
		say = ay;
	}
	
	if (children)
	{
		if (r)
			for (int x = 0; x < 4; x++)
			{
				if (dist < radius + maximum + 5 && sax + pi/100.0 >= children[x].x_angle && fax <= children[x].x_angle + children[x].x_size + pi/100.0 && say + pi/100.0 >= children[x].y_angle && fay <= children[x].y_angle + children[x].y_size + pi/100.0)
					children[x].render(r, p_player, player_vec, ax, ay, nax, nay, dist);
				else
					children[x].render(false, p_player, player_vec, ax, ay, nax, nay, dist);
			}
				else
					for (int x = 0; x < 4; x++)
						children[x].render(false, p_player, player_vec, ax, ay, nax, nay, dist);
	}
	else
	{
		GLdouble x_temp, y_temp;
		Vector v;
		
		GLdouble vert[3*node_size*node_size];
		GLdouble text[2*node_size*node_size];
		unsigned int indices[node_size*node_size*6];
		for (int x = 0; x < node_size; x++)
		{
			x_temp = x_size*(GLdouble(x)/GLdouble(node_size-1)) + x_angle;
			for (int y = 0; y < node_size; y++)
			{
				y_temp = y_size*(GLdouble(y)/GLdouble(node_size-1)) + y_angle  - pi/2.0;
				
				v.x = cos(x_temp) * cos(y_temp);
				v.y = sin(y_temp);
				v.z = sin(x_temp) * cos(y_temp);
				
				vert[3*(x*node_size + y) + 0] = v.x*(radius + (heights[x*node_size + y]*.5 + 255.0)) - dist*player_vec->x;
				vert[3*(x*node_size + y) + 1] = v.y*(radius + (heights[x*node_size + y]*.5 + 255.0)) - dist*player_vec->y;
				vert[3*(x*node_size + y) + 2] = v.z*(radius + (heights[x*node_size + y]*.5 + 255.0)) - dist*player_vec->z;

				text[2*(x*node_size + y) + 0] = x_size*radius*100*GLdouble(x)/GLdouble(node_size-1);
				text[2*(x*node_size + y) + 1] = y_size*radius*100*GLdouble(y)/GLdouble(node_size-1);
			}
		}
		
		int index = 0;
		for (int x = 1; x < node_size; x++)
		{
			for (int y = 0; y < node_size; y++)
			{
				indices[index++] = x*node_size+y;
				indices[index++] = (x-1)*node_size+y;
			}
		}
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vert);
		glTexCoordPointer(2, GL_DOUBLE, 0, text);
		for (int x = 0; x < node_size-1; x++)
			glDrawElements(GL_TRIANGLE_STRIP, node_size*2, GL_UNSIGNED_INT, &indices[x*node_size*2]);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		if (r)
		{
			Vector vec[3];
			for (int x = 0; x < node_size-1; x++)
				for (int y = 0; y < node_size*2 - 2; y++)
				{
					vec[0] = Vector(&vert[indices[x*node_size*2 + y]*3]);
					vec[1] = Vector(&vert[indices[x*node_size*2 + y+1]*3]);
					vec[2] = Vector(&vert[indices[x*node_size*2 + y+2]*3]);
					//if (model_triangle_collision(&p_player->PlayerModel, vec))
					//{
						GLdouble angle_x = ax;
						if (player_vec->z < 0)
							angle_x = -(angle_x - 2.0*pi);
						if (player_vec->z > 0)
							angle_x = -angle_x + 2.0*pi;
						p_player->PlayerModel.Physics.Orientation = (180.0/pi)*(Vector(0.0, angle_x, 2.0*pi - acos(Dot(Vector(0.0, 1.0, 0.0), *player_vec))));
					//}
				}
		}
	}
}

void planet_node::renderwater(bool r, Player *p_player, Vector *player_vec, GLdouble ax, GLdouble ay, GLdouble nax, GLdouble nay, GLdouble dist)
{
	GLdouble fax, sax, fay, say;
	if (ax < nax)
	{
		fax = ax;
		sax = nax;
	}
	else
	{
		fax = nax;
		sax = ax;
	}
	if (ay < nay)
	{
		fay = ay;
		say = nay;
	}
	else
	{
		fay = nay;
		say = ay;
	}
	
	if (children)
	{
		if (r)
			for (int x = 0; x < 4; x++)
			{
				if (dist < radius + maximum + 5 && sax + pi/100.0 >= children[x].x_angle && fax <= children[x].x_angle + children[x].x_size + pi/100.0 && say + pi/100.0 >= children[x].y_angle && fay <= children[x].y_angle + children[x].y_size + pi/100.0)
					children[x].renderwater(r, p_player, player_vec, ax, ay, nax, nay, dist);
				else
					children[x].renderwater(false, p_player, player_vec, ax, ay, nax, nay, dist);
			}
				else
					for (int x = 0; x < 4; x++)
						children[x].renderwater(false, p_player, player_vec, ax, ay, nax, nay, dist);
	}
	else
	{
		GLdouble x_temp, y_temp;
		Vector v;
		
		GLdouble vert[3*node_size*node_size];
		GLdouble text[2*node_size*node_size];
		unsigned int indices[node_size*node_size*6];
		for (int x = 0; x < node_size; x++)
		{
			x_temp = x_size*(GLdouble(x)/GLdouble(node_size-1)) + x_angle;
			for (int y = 0; y < node_size; y++)
			{
				y_temp = y_size*(GLdouble(y)/GLdouble(node_size-1)) + y_angle  - pi/2.0;
				
				v.x = cos(x_temp) * cos(y_temp);
				v.y = sin(y_temp);
				v.z = sin(x_temp) * cos(y_temp);
				
				vert[3*(x*node_size + y) + 0] = v.x*(radius) - dist*player_vec->x;
				vert[3*(x*node_size + y) + 1] = v.y*(radius) - dist*player_vec->y;
				vert[3*(x*node_size + y) + 2] = v.z*(radius) - dist*player_vec->z;
				
				text[2*(x*node_size + y) + 0] = y_size*radius*10*GLdouble(y)/GLdouble(node_size-1);
				text[2*(x*node_size + y) + 1] = x_size*radius*10*GLdouble(x)/GLdouble(node_size-1);
			}
		}
		
		int index = 0;
		for (int x = 1; x < node_size; x++)
		{
			for (int y = 0; y < node_size; y++)
			{
				indices[index++] = x*node_size+y;
				indices[index++] = (x-1)*node_size+y;
			}
		}
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vert);
		glTexCoordPointer(2, GL_DOUBLE, 0, text);
		for (int x = 0; x < node_size-1; x++)
			glDrawElements(GL_TRIANGLE_STRIP, node_size*2, GL_UNSIGNED_INT, &indices[x*node_size*2]);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void Planet::Load(string Name, Player *p_player)
{
	name = Name;
	int seed = 0;
	for (int x = 0; x < Name.length(); x++)
		seed += int(Name[x]);
	data.load(seed);
	player_pointer = p_player;
	
	Physics.Mass = 0.0;//5515*5*(4/3)*pi*pow(data.radius*1000.0, 3);
	
	int TextLen;
	char ErrText[1280];
	
	tex = Load2DTexture("res/GrassSample.tga", true);
	
	TextLen = 1280;
	VertShad = LoadGLSLShader(GL_VERTEX_SHADER_ARB, "res/Planet.vx", ErrText, &TextLen);
	//if(!VertShad)
	//{
		cout << ErrText << endl;
	//	exit(0);
	//}

	TextLen = 128;
	FragShad = LoadGLSLShader(GL_FRAGMENT_SHADER_ARB, "res/Planet.ft", ErrText, &TextLen);
	//if(!FragShad)
	//{
		cout << ErrText << endl;
	//	exit(0);
	//}

	ShadProg = glCreateProgramObjectARB();
	glAttachObjectARB(ShadProg, VertShad);
	glAttachObjectARB(ShadProg, FragShad);
	glLinkProgramARB(ShadProg);
	
	glUseProgramObjectARB(ShadProg);
	glUniform1fARB(glGetUniformLocationARB(ShadProg, "radius"), GLfloat(data.radius));
	glUniform1fARB(glGetUniformLocationARB(ShadProg, "max"), GLfloat(data.maximum));
	glUniform1fARB(glGetUniformLocationARB(ShadProg, "min"), GLfloat(data.minimum));
	
	watertex = Load2DTexture("res/waterish.tga", true);
	
	TextLen = 1280;
	WaterVertShad = LoadGLSLShader(GL_VERTEX_SHADER_ARB, "res/WaterPlanet.vx", ErrText, &TextLen);
	//if(!WaterVertShad || TextLen > 0)
	//{
		cout << ErrText << endl;
	//	exit(0);
	//}

	TextLen = 128;
	WaterFragShad = LoadGLSLShader(GL_FRAGMENT_SHADER_ARB, "res/WaterPlanet.ft", ErrText, &TextLen);
	//if(!WaterFragShad || TextLen > 0)
	//{
		cout << ErrText << endl;
	//	exit(0);
	//}

	WaterShadProg = glCreateProgramObjectARB();
	glAttachObjectARB(WaterShadProg, WaterVertShad);
	glAttachObjectARB(WaterShadProg, WaterFragShad);
	glLinkProgramARB(WaterShadProg);
	
	glUseProgramObjectARB(WaterShadProg);
}

void Planet::Render()
{
	GLdouble Dist = Distance(player_pointer->PlayerModel.Physics.Position, Physics.Position)/1000.0000;
	if (Dist < player_pointer->distance)
		player_pointer->distance = Dist;
	Vector player_vec = Normalize(player_pointer->PlayerModel.Physics.Position - Physics.Position);
	GLdouble ay = asin(player_vec.y) + pi/2.0;
	GLdouble ax;
	if (player_vec.x/cos(ay - pi/2.0) > 1.0 || player_vec.x/cos(ay - pi/2.0) < -1.0)
		ax = atan(player_vec.z/player_vec.x);
	else
		ax = acos(player_vec.x/cos(ay - pi/2.0));
		
	if (player_vec.z < 0)
		ax = -ax + 2.0*pi;
		
	Vector nplayer_vec = Normalize((player_pointer->PlayerModel.Physics.Position + player_pointer->PlayerModel.Physics.LinearVelocity) - Physics.Position);
		
	GLdouble nay = asin(nplayer_vec.y) + pi/2.0;
	GLdouble nax;
	if (nplayer_vec.x/cos(nay - pi/2.0) > 1.0 || nplayer_vec.x/cos(nay - pi/2.0) < -1.0)
		nax = atan(nplayer_vec.z/nplayer_vec.x);
	else
		nax = acos(nplayer_vec.x/cos(nay - pi/2.0));
		
	if (nplayer_vec.z < 0)
		nax = -nax + 2.0*pi;
	
	GLdouble D = Distance(player_pointer->PlayerModel.Physics.Position + player_pointer->PlayerModel.Physics.LinearVelocity, Physics.Position)/1000.0000;
	
	glScaled(1000, 1000, 1000);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1iARB(glGetUniformLocationARB(ShadProg, "texture"), 0);
	glUniform1fARB(glGetUniformLocationARB(ShadProg, "dist"), GLfloat(D));
	glUniform3fARB(glGetUniformLocationARB(ShadProg, "pvec"), GLfloat(nplayer_vec.x), GLfloat(nplayer_vec.y), GLfloat(nplayer_vec.z));
	glUseProgramObjectARB(ShadProg);
	data.render(true, player_pointer, &player_vec, ax, ay, nax, nay, Dist);
	
	glDisable(GL_CULL_FACE);
	if (Dist-6380 < 0.0)
		glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, watertex);
	glUniform1iARB(glGetUniformLocationARB(ShadProg, "texture"), 0);
	glUniform1fARB(glGetUniformLocationARB(ShadProg, "dist"), GLfloat(D));
	glUniform3fARB(glGetUniformLocationARB(ShadProg, "pvec"), GLfloat(nplayer_vec.x), GLfloat(nplayer_vec.y), GLfloat(nplayer_vec.z));
	glUniform3fARB(glGetUniformLocationARB(ShadProg, "pos"), GLfloat(player_pointer->PlayerModel.Physics.Position.x), GLfloat(player_pointer->PlayerModel.Physics.Position.y), GLfloat(player_pointer->PlayerModel.Physics.Position.z));
	glUseProgramObjectARB(WaterShadProg);
	data.renderwater(true, player_pointer, &player_vec, ax, ay, nax, nay, Dist);
	glScaled(.001, .001, .001);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	Vector v = Normalize(Vector(cos(ax) * cos(ay - pi/2.0), sin(ay - pi/2.0), sin(ax) * cos(ay - pi/2.0)));
	Vector Color = max(double(Dot(Vector(.97, 0.24, 0.0), v)), 1.0)*Vector(0.0, (pow(10, 1 - (Dist - 1000.0 - data.radius)/(data.radius*.0098)) - 8)/3.333, (pow(10, 1 - (Dist - 1000.0 - data.radius)/(data.radius*.0098))-8));
	
	if (Dist <= (data.radius*1.0098) + 1)
	{
		glClearColor(Color.x/100000000.0, Color.y/70000000000000000.0, Color.z/30000000000000000.0, 0.0);
	}
	if (Dist <= data.radius*1.025)
		player_pointer->Planet = 1;
	if (Dist <= data.radius*1.04)
	{
		player_pointer->GridX = 1;
		player_pointer->GridY = 1;
	}
}

void Planet::Release()
{
	data.destroy();
	glDeleteObjectARB(VertShad);
	glDeleteObjectARB(FragShad);
	glDeleteObjectARB(ShadProg);
	glDeleteObjectARB(WaterVertShad);
	glDeleteObjectARB(WaterFragShad);
	glDeleteObjectARB(WaterShadProg);
	glDeleteTextures(1, &tex);
	glDeleteTextures(1, &watertex);	
}
