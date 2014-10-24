/*
 *  Planet.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Planet.h"

/*************************************************
 *					QuadTreeNode				 *
 *				  Function: Load				 *
 *	   Use: Loads the first Node of the tree	 *
 *************************************************/
void QuadTreeNode::Load(int seed, GLdouble rad, terra_type *t)
{
	x_size = 2.0*pi;
	y_size = pi;
	longitude = 0.0;
	latitude = 0.0;
	radius = rad;
	parent = NULL;
	children = NULL;
	minimum = -0.000125*radius;
	maximum = 0.0003*radius;
	data_start = this;
	planet_minimum = &minimum;
	planet_maximum = &maximum;
	type = t;
	
	srand(seed);
	if (*type == height)
	{
		verts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn);
		
		for (int x = 0; x < vpn; x++)
			for (int y = 0; y < vpn; y++)
				verts[x*vpn + y] = GLdouble(rand()%int(1000000*(maximum - minimum)))/1000000.0 + minimum;
	
		for (int x = 0; x < vpn; x++)
		{
			verts[(vpn-1)*vpn + x] = verts[x];
			verts[x*vpn + vpn-1] = verts[vpn-1];
			verts[x*vpn] = verts[0];
		}
	}
	else if (*type == three_shift)
	{
		verts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn*3);
		for (int x = 0; x < vpn; x++)
			for (int y = 0; y < vpn; y++)
				for (int z = 0; z < 3; z++)
					verts[(x*vpn + y)*3 + z] = GLdouble(rand()%int(10000*(maximum - minimum)))/1000000.0 + minimum;
	
		for (int x = 0; x < vpn; x++)
			for (int z = 0; z < 3; z++)
			{
				verts[((vpn-1)*vpn + x)*3 + z] = verts[x*3 + z];
				verts[(x*vpn + vpn-1)*3 + z] = verts[(vpn-1)*3 + z];
				verts[x*vpn*3 + z] = verts[z];
			}
	}
}

/*************************************************
 *					QuadTreeNode				 *
 *				  Function: LODCheck			 *
 *		   Use: Keeps the level of detail		 *
 *************************************************/
void QuadTreeNode::LODCheck(camera_reference *info)
{
	Vector v = Normalize(half_vec);
	
	GLdouble h_dist = (radius + minimum)*(absolute(acos(Dot(info->pvec, v))) - x_size/2.0);
	
	rend = true;
	GLdouble vis = acos((radius + *planet_minimum)/info->distance);
	if ((!(vis >= 0.0) && !(vis < 0.0)) || (vis <= 0.01))
		vis = 0.01;
		
	if (h_dist/(radius + minimum) > vis)
		rend = false;
	
	GLdouble d = Dot(half_vec, info->vp_vis) + info->vp_dist + sqrt(y_size*y_size + y_size*y_size/4.0)*radius;
	if (d < 0.0)
		rend = false;
		
	if (rend)
	{
		GLdouble v_dist = info->distance - (radius + minimum);
		v_dist = v_dist < 0.0 ? 0.0 : v_dist;
		GLdouble dist = h_dist*h_dist + v_dist*v_dist;
		GLdouble long_scale = ((sin(latitude) + sin(latitude + y_size) + sin(latitude + y_size/2.0))/3.0);
		GLdouble percent = (long_scale*x_size*radius*y_size*radius)/dist;
		
		if (percent <= 0.8 && children != NULL)
			Merge();
		
		else if (percent >= 1.5 && children == NULL && long_scale*radius*x_size > 40.0*vpn/3.0)
			Split();
		
		if (children != NULL)
		{
			for (int x = 0; x < 4; x++)
				children[x].LODCheck(info);
		}
	}
}

/*************************************************
 *					QuadTreeNode				 *
 *				  Function: Split				 *
 *			Use: Splits a node into 4			 *
 *************************************************/
void QuadTreeNode::Split()
{
	if (children != NULL)
		for (int x = 0; x < 4; x++)
			children[x].Split();
	
	else
	{
		children = (QuadTreeNode*)malloc(sizeof(QuadTreeNode)*4);
		if (children == NULL)
		{
			cout << "Error: " << __LINE__ << " in " << __FILE__ << endl;
			exit(1);
		}
		
		int u_pos, v_pos, u1_pos, v1_pos;
		Vector n1, n2;
		GLdouble count;
		GLdouble half_vpn = GLdouble(int(vpn/2))/GLdouble(vpn);
		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
			{
				children[x*2+y].x_size = x_size/2.0;
				children[x*2+y].y_size = y_size/2.0;
				children[x*2+y].longitude = longitude + x*(x_size/2.0);
				children[x*2+y].latitude = latitude + y*(y_size/2.0);
				children[x*2+y].radius = radius;
				children[x*2+y].children = NULL;
				children[x*2+y].parent = this;
				children[x*2+y].average = 0.0;
				children[x*2+y].minimum = maximum;
				children[x*2+y].maximum = minimum;
				children[x*2+y].data_start = data_start;
				children[x*2+y].planet_minimum = planet_minimum;
				children[x*2+y].planet_maximum = planet_maximum;
				children[x*2+y].type = type;
				
				children[x*2+y].mat.Init();
				children[x*2+y].mat.VertShad = mat.VertShad;
				children[x*2+y].mat.FragShad = mat.FragShad;
				children[x*2+y].mat.ShadProg = mat.ShadProg;
				
				GLdouble long_scale = ((sin(children[x*2+y].latitude) + sin(children[x*2+y].latitude + children[x*2+y].y_size) + sin(children[x*2+y].latitude + children[x*2+y].y_size/2.0))/3.0);
				
				count = 0.0;
				GLdouble distbet = .5*radius*(long_scale*children[x*2+y].x_size + children[x*2+y].y_size)/GLdouble(vpn);
				GLdouble rough = clamp((pi - x_size)/pi, 0.0, 1.0);
				
				if (*type == height)
				{
					children[x*2+y].verts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn);
					if (children[x*2+y].verts == NULL)
						cout << "Error: " << __LINE__ << " in " << __FILE__ << endl;
					
					GLdouble heightincrease;
					GLdouble mult;
					srand(verts[(x*vpn/2)*vpn + (y*vpn/2)]);
					
					for (int u = 0; u < vpn; u++)
					{
						u_pos = (u + x*vpn - x)/2;
						u1_pos = (u + 1 + x*vpn - x)/2;
						for (int v = 0; v < vpn; v++)
						{
							v_pos = (v + y*vpn - y)/2;
							v1_pos = (v + 1 + y*vpn - y)/2;
							children[x*2+y].verts[u*vpn + v] = (verts[u_pos*vpn + v_pos] + 
																verts[u1_pos*vpn + v_pos] + 
																verts[u1_pos*vpn + v1_pos] + 
																verts[u_pos*vpn + v1_pos])/4.0;
							
							heightincrease = rough*.1*absolute(pi - x_size)*((GLdouble(rand()%int(1000.0*distbet))/1000.0) - .5*distbet);
							
							/*mult = 1.0;
							while (true)
							{
								if (children[x*2+y].verts[u*vpn + v] + heightincrease*mult <= maximum && children[x*2+y].verts[u*vpn + v] + heightincrease*mult >= minimum)
									break;
									
								mult -= .1;
								if (mult <= -1.0)
									break;
							}
							heightincrease *= mult;*/
							
							mult = 1.0;
							while (true)
							{
								if (children[x*2+y].verts[u*vpn + v] > 0.0 && children[x*2+y].verts[u*vpn + v] + heightincrease*mult > 0.0)
									break;
								
								if (children[x*2+y].verts[u*vpn + v] < 0.0 && children[x*2+y].verts[u*vpn + v] + heightincrease*mult < 0.0)
									break;
								
								if (children[x*2+y].verts[u*vpn + v] == 0.0)
									break;
								
								mult -= .1;
								if (mult <= -1.0)
									break;
							}
							heightincrease *= mult;
							
							if (u_pos != u1_pos || v_pos != v1_pos)
							{
								if (u > 1 && v > 1 && u < vpn-1 && v < vpn-1)/* && (u != 1 && v != 1 && u != vpn-1 && v != vpn-1)*/
									children[x*2+y].verts[u*vpn + v] += heightincrease;
								if (u == 0 && x == 1 && v != vpn-1 && v != 0)
								{
									children[x*2+y].verts[u*vpn + v] += heightincrease;
									children[y].verts[(vpn-1)*vpn + v] = children[x*2+y].verts[u*vpn + v];
								
									if (children[x*2+y].verts[u*vpn + v] < children[y].minimum)
										children[y].minimum = children[x*2+y].verts[u*vpn + v];
									if (children[x*2+y].verts[u*vpn + v] > children[y].maximum)
										children[y].maximum = children[x*2+y].verts[u*vpn + v];
								}
								if (v == 0 && y == 1 && u != vpn-1 && u != 0)
								{
									children[x*2+y].verts[u*vpn + v] += heightincrease;
									children[x*2].verts[u*vpn + (vpn-1)] = children[x*2+y].verts[u*vpn + v];
									
									if (children[x*2+y].verts[u*vpn + v] < children[x*2].minimum)
										children[x*2].minimum = children[x*2+y].verts[u*vpn + v];
									if (children[x*2+y].verts[u*vpn + v] > children[x*2].maximum)
										children[x*2].maximum = children[x*2+y].verts[u*vpn + v];
								}
							}
							
							if (children[x*2+y].verts[u*vpn + v] < children[x*2+y].minimum)
								children[x*2+y].minimum = children[x*2+y].verts[u*vpn + v];
							if (children[x*2+y].verts[u*vpn + v] > children[x*2+y].maximum)
								children[x*2+y].maximum = children[x*2+y].verts[u*vpn + v];
							
							children[x*2+y].average += children[x*2+y].verts[u*vpn + v];
							count+=1.0;
						}
					}
					
					children[x*2+y].half_vec = Vector(cos(children[x*2+y].longitude + children[x*2+y].x_size*half_vpn) * cos(children[x*2+y].latitude + children[x*2+y].y_size*half_vpn - pi/2.0),
													  sin(children[x*2+y].latitude + children[x*2+y].y_size*half_vpn - pi/2.0),
													  sin(children[x*2+y].longitude + children[x*2+y].x_size*half_vpn) * cos(children[x*2+y].latitude + children[x*2+y].y_size*half_vpn - pi/2.0))*(radius + children[x*2+y].verts[int(half_vpn*GLdouble(vpn))*vpn + int(half_vpn*GLdouble(vpn))]);
				}
				else if (*type == three_shift)
				{
					children[x*2+y].verts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn*3);
					if (children[x*2+y].verts == NULL)
						cout << "Error: " << __LINE__ << " in " << __FILE__ << endl;
					
					GLdouble change[3];
					Vector project_vec;
					Vector temp_vert;
					GLdouble l;
					srand(verts[((x*vpn/2)*vpn + (y*vpn/2))*3]);
					
					for (int u = 0; u < vpn; u++)
					{
						u_pos = (u + x*vpn - x)/2;
						u1_pos = (u + 1 + x*vpn - x)/2;
						for (int v = 0; v < vpn; v++)
						{
							v_pos = (v + y*vpn - y)/2;
							v1_pos = (v + 1 + y*vpn - y)/2;
							project_vec = Vector(cos(children[x*2+y].longitude + children[x*2+y].x_size*GLdouble(u)/GLdouble(vpn)) * cos(children[x*2+y].latitude + children[x*2+y].y_size*GLdouble(v)/GLdouble(vpn) - pi/2.0),
												 sin(children[x*2+y].latitude + children[x*2+y].y_size*GLdouble(v)/GLdouble(vpn) - pi/2.0),
												 sin(children[x*2+y].longitude + children[x*2+y].x_size*GLdouble(u)/GLdouble(vpn)) * cos(children[x*2+y].latitude + children[x*2+y].y_size*GLdouble(v)/GLdouble(vpn) - pi/2.0))*radius;
							
							for (int z = 0; z < 3; z++)
							{
								children[x*2+y].verts[(u*vpn + v)*3 + z] = (verts[(u_pos*vpn + v_pos)*3 + z] + 
																			verts[(u1_pos*vpn + v_pos)*3 + z] + 
																			verts[(u1_pos*vpn + v1_pos)*3 + z] + 
																			verts[(u_pos*vpn + v1_pos)*3 + z])/4.0;
						
								change[z] = rough*.1*absolute(pi - x_size)*((GLdouble(rand()%int(1000.0*distbet))/1000.0) - .5*distbet);
							}
							
							while (true)
							{
								temp_vert = Vector(&children[x*2+y].verts[(u*vpn + v)*3 + 0]) + Vector(change);
								l = Magnitude(project_vec + temp_vert) - radius;
								if ((l <= maximum && l >= minimum) || (change[0] == 0 && change[1] == 0 && change[2] == 0))
									break;
								else
									for (int z = 0; z < 3; z++)
										change[z] *= -.5;
							}
							
							if ((u_pos != u1_pos || v_pos != v1_pos) && (u > 1 && v > 1 && u < vpn-1 && v < vpn-1))
								for (int z = 0; z < 3; z++)
									children[x*2+y].verts[(u*vpn + v)*3 + z] += change[z];
							if ((u_pos != u1_pos || v_pos != v1_pos) && (u == 0 && x == 1 && v != vpn-1 && v != 0))
							{
								for (int z = 0; z < 3; z++)
								{
									children[x*2+y].verts[(u*vpn + v)*3 + z] += change[z];
									children[y].verts[((vpn-1)*vpn + v)*3 + z] = children[x*2+y].verts[(u*vpn + v)*3 + z];
								}
								
								if (l < children[y].minimum)
									children[y].minimum = l;
								if (l > children[y].maximum)
									children[y].maximum = l;
							}
							if ((u_pos != u1_pos || v_pos != v1_pos) && (v == 0 && y == 1 && u != vpn-1 && u != 0))
							{
								for (int z = 0; z < 3; z++)
								{
									children[x*2+y].verts[(u*vpn + v)*3 + z] += change[z];
									children[x*2].verts[(u*vpn + (vpn-1))*3 + z] = children[x*2+y].verts[(u*vpn + v)*3 + z];
								}
								
								if (l < children[x*2].minimum)
									children[x*2].minimum = l;
								if (l > children[x*2].maximum)
									children[x*2].maximum = l;
							}
	
							project_vec += Vector(&children[x*2+y].verts[(u*vpn + v)*3 + 0]);
								
							if (u == int(vpn/2) && v == int(vpn/2))
								children[x*2+y].half_vec = project_vec;
							
							if (l < children[x*2+y].minimum)
								children[x*2+y].minimum = l;
							if (l > children[x*2+y].maximum)
								children[x*2+y].maximum = l;
							children[x*2+y].average += l;
							count+=1.0;
						}
					}

				}
				children[x*2+y].average /= count;
			}
		for (int x = 0; x < 4; x++)
		{
			if (children[x].minimum < *children[x].planet_minimum)
				*children[x].planet_minimum = children[x].minimum;
			if (children[x].maximum > *children[x].planet_maximum)
				*children[x].planet_maximum = children[x].maximum;
			children[x].MakeTexture();
		}
		
		free(verts);
		verts = NULL;
	}
}

/*************************************************
 *					QuadTreeNode				 *
 *				  Function: Merge				 *
 *		   Use: Merges 4 nodes into 1			 *
 *************************************************/
void QuadTreeNode::Merge()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			if (children[x].children != NULL)
				children[x].Merge();
		
		if (*type == height)
		{
			verts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn);
			
			if (verts == NULL)
			{
				cout << "Error: " << __LINE__ << " in " << __FILE__ << endl;
				exit(1);
			}
		
			for (int u = 0; u < vpn; u++)
				for (int v = 0; v < vpn; v++)
					verts[u*vpn + v] = children[(u/(vpn/2))*2+(v/(vpn/2))].verts[(2*(u - (vpn/2)*(u/(vpn/2))) + u/(vpn/2))*vpn + (2*(v - (vpn/2)*(v/(vpn/2))) + v/(vpn/2))];
		}
		else if (*type == three_shift)
		{
			verts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn*3);
			
			if (verts == NULL)
			{
				cout << "Error: " << __LINE__ << " in " << __FILE__ << endl;
				exit(1);
			}
		
			for (int u = 0; u < vpn; u++)
				for (int v = 0; v < vpn; v++)
					for (int z = 0; z < 3; z++)
						verts[(u*vpn + v)*3 + z] = children[(u/(vpn/2))*2+(v/(vpn/2))].verts[((2*(u - (vpn/2)*(u/(vpn/2))) + u/(vpn/2))*vpn + (2*(v - (vpn/2)*(v/(vpn/2))) + v/(vpn/2)))*3 + z];
		}
		
		for (int x = 0; x < 4; x++)
			children[x].Release();
		
		free(children);
		children = NULL;
	}
}

/*************************************************
 *					QuadTreeNode				 *
 *			   Function: MakeTexture			 *
 *		   Use: Creates the node's Texture		 *
 *************************************************/
void QuadTreeNode::MakeTexture()
{
	unsigned char colors[3*ppn*ppn];
	
	if (*type == height)
	{
		GLdouble height, height2;
		GLdouble red, green, blue;
		int z, w;
		GLdouble lightpercent;
		GLdouble slope;
		GLdouble mult = 3000.0/ppn_into_vpn;
	
		GLdouble sand = 0.0, snow = 10000.0, divisor = 1000.0;
		GLdouble sand_snow_slope = (snow - sand)/divisor;
	
		for (int x = 0; x < ppn; x++)
			for (int y = 0; y < ppn; y++)
			{
				GLdouble dx = ppn_into_vpn*GLdouble(x);
				GLdouble dy = ppn_into_vpn*GLdouble(y);
				
				z = int(dx);
				w = int(dy);
				
				
				GLdouble h00 = verts[z*vpn + w];
				GLdouble h01 = verts[(z+1)*vpn + w];
				GLdouble h10 = verts[z*vpn + (w+1)];
				GLdouble h11 = verts[(z+1)*vpn + (w+1)];
				
				GLdouble tx = dx - GLdouble(z);
				GLdouble ty = dy - GLdouble(w);
				
				GLdouble txty = tx * ty;
				
				height = h00 * (1.0 - ty - tx + txty)
					   + h01 * (tx - txty)
					   + h11 * txty
					   + h10 * (ty - txty);
				
				if (dx-mult < 0.0 || dy-mult < 0.0)
					height2 = data_start->GetGroundHeight(modulate(longitude + x_size*(dx-mult)/GLdouble(vpn-1), 2.0*pi), modulate(latitude + y_size*(dy-mult)/GLdouble(vpn-1), pi));
				else
				{
					GLdouble dx1 = dx-mult;
					GLdouble dy1 = dy-mult;
					
					z = int(dx1);
					w = int(dy1);
					
					
					h00 = verts[z*vpn + w];
					h01 = verts[(z+1)*vpn + w];
					h10 = verts[z*vpn + (w+1)];
					h11 = verts[(z+1)*vpn + (w+1)];
					
					tx = dx1 - GLdouble(z);
					ty = dy1 - GLdouble(w);
					
					txty = tx * ty;
					
					height2 = h00 * (1.0 - ty - tx + txty)
						   + h01 * (tx - txty)
						   + h11 * txty
						   + h10 * (ty - txty);
				}
							
				slope = (height - height2)/(2.24*mult*y_size/GLdouble(vpn-1));//sqrt(ppn_into_vpn*ppn_into_vpn*x_size*x_size/GLdouble((vpn-1)*(vpn-1)) + ppn_into_vpn*ppn_into_vpn*y_size*y_size/GLdouble((vpn-1)*(vpn-1)));
				if (!(slope > 0.0) && !(slope <= 0.0))
					slope = 0.0;
				
				if (height <= 0.0)
				{
					red = 208.0;
					green = 175.0;
					blue = 111.0;
				}
				else if (height <= sand_snow_slope)
				{
					red = (0.0 - 208.0)/(sand_snow_slope - 0.0)*height + 208.0;
					green = (70.0 - 175.0)/(sand_snow_slope - 0.0)*height + 175.0;
					blue = (0.0 - 111.0)/(sand_snow_slope - 0.0)*height + 111.0;
				}
				else if (height <= snow)
				{
					red = (255.0 - 0.0)/(snow - sand_snow_slope)*(height - sand_snow_slope) + 0.0;
					green = (255.0 - 70.0)/(snow - sand_snow_slope)*(height - sand_snow_slope) + 70.0;
					blue = (255.0 - 0.0)/(snow - sand_snow_slope)*(height - sand_snow_slope) + 0.0;
				}
				else
				{
					red = 255.0;
					green = 255.0;
					blue = 255.0;
				}
				
				if (x > 0 && x < ppn-1 && y > 0 && y < ppn-1)
					lightpercent = (GLdouble(rand()%24 + 38)/50.0)*(1.0 - (slope*.4/2000000.0));
				else
					lightpercent = (1.0 - (slope*.4/2000000.0));
				colors[3*(x*ppn + y) + 0] = (unsigned char)int(clamp(red*lightpercent, 0.0, 255.0));
				colors[3*(x*ppn + y) + 1] = (unsigned char)int(clamp(green*lightpercent, 0.0, 255.0));
				colors[3*(x*ppn + y) + 2] = (unsigned char)int(clamp(blue*lightpercent, 0.0, 255.0));
			}
	}
	else if (*type == three_shift)
	{
		for (int x = 0; x < ppn; x++)
			for (int y = 0; y < ppn; y++)
			{
				GLdouble random = GLdouble(rand()%10000)/10000.0;
				colors[3*(x*ppn + y) + 0] = (unsigned char)int((0.0 - 208.0)*random + 208.0);
				colors[3*(x*ppn + y) + 1] = (unsigned char)int((70.0 - 175.0)*random + 175.0);
				colors[3*(x*ppn + y) + 2] = (unsigned char)int((0.0 - 111.0)*random + 111.0);
			}
	}
	
	GLuint map;
	glGenTextures(1, &map);
	glBindTexture(GL_TEXTURE_2D, map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ppn, ppn, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)colors);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	mat.AddTexture(map);
}

/*************************************************
 *					QuadTreeNode				 *
 *				  Function: Render				 *
 *			 Use: Renders the Quad Tree	 		 *
 *************************************************/
void QuadTreeNode::Render(GLdouble *texts, unsigned int *indices, camera_reference *info)
{
	if (rend)
	{
		if (children != NULL)
			for (int x = 0; x < 4; x++)
				children[x].Render(texts, indices, info);
		
		else if (children == NULL)
		{
			GLdouble vert[3*vpn*vpn];
			
			if (verts && *type == height)
			{
				Vector v;
				GLdouble x_temp, y_temp;
				for (int x = 0; x < vpn; x++)
				{
					x_temp = x_size*GLdouble(x)/GLdouble(vpn-1) + longitude;
					for (int y = 0; y < vpn; y++)
					{
						y_temp = y_size*(GLdouble(y)/GLdouble(vpn-1)) + latitude - pi/2.0;
						
						v = Vector(cos(x_temp) * cos(y_temp),
								   sin(y_temp),
								   sin(x_temp) * cos(y_temp));
						
						vert[3*(x*vpn + y) + 0] = v.x*(radius + verts[x*vpn + y]) - info->distance*info->pvec.x;
						vert[3*(x*vpn + y) + 1] = v.y*(radius + verts[x*vpn + y]) - info->distance*info->pvec.y;
						vert[3*(x*vpn + y) + 2] = v.z*(radius + verts[x*vpn + y]) - info->distance*info->pvec.z;
					}
				}
			}
			else if (verts && *type == three_shift)
			{
				Vector v;
				GLdouble x_temp, y_temp;
				for (int x = 0; x < vpn; x++)
				{
					x_temp = x_size*GLdouble(x)/GLdouble(vpn-1) + longitude;
					for (int y = 0; y < vpn; y++)
					{
						y_temp = y_size*(GLdouble(y)/GLdouble(vpn-1)) + latitude - pi/2.0;
						
						v = Vector(cos(x_temp) * cos(y_temp),
								   sin(y_temp),
								   sin(x_temp) * cos(y_temp));
						
						vert[3*(x*vpn + y) + 0] = v.x*radius + verts[(x*vpn + y)*3 + 0] - info->distance*info->pvec.x;
						vert[3*(x*vpn + y) + 1] = v.y*radius + verts[(x*vpn + y)*3 + 1] - info->distance*info->pvec.y;
						vert[3*(x*vpn + y) + 2] = v.z*radius + verts[(x*vpn + y)*3 + 2] - info->distance*info->pvec.z;
					}
				}
			}
					
			else
				return;
			
			mat.Use();
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, vert);
			glTexCoordPointer(2, GL_DOUBLE, 0, texts);
			for (int x = 0; x < vpn-1; x++)
				glDrawElements(GL_TRIANGLE_STRIP, vpn*2, GL_UNSIGNED_INT, &indices[x*vpn*2]);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
	else
		return;
}

/*************************************************
 *					QuadTreeNode				 *
 *				  Function: Release				 *
 *	     Use: Frees a Quad Tree from memory		 *
 *************************************************/
void QuadTreeNode::Release()
{
	if (verts)
		free(verts);
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			children[x].Release();
		free(children);
		children = NULL;
	}
	
	mat.ReleaseTextures();
}

/*************************************************
 *					QuadTreeNode				 *
 *			 Function: GetGroundHeight			 *
 *	   Use: Retrieves a height at a position	 *
 *************************************************/
GLdouble QuadTreeNode::GetGroundHeight(GLdouble longitude, GLdouble latitude)
{
	if (children)
	{
		for (int x = 0; x < 4; x++)
			if (longitude >= children[x].longitude && longitude < children[x].longitude + children[x].x_size && latitude >= children[x].latitude && latitude < children[x].latitude + children[x].y_size)
				return children[x].GetGroundHeight(longitude, latitude);
	}
	else
	{
		if (*type == height)
		{
			GLdouble dx = GLdouble(vpn-1)*(longitude-this->longitude)/x_size;
			GLdouble dy = GLdouble(vpn-1)*(latitude-this->latitude)/y_size;
			
			int x = int(dx);
			int y = int(dy);
			
			GLdouble h00 = verts[x*vpn + y];
			GLdouble h01 = verts[(x+1)*vpn + y];
			GLdouble h10 = verts[x*vpn + (y+1)];
			GLdouble h11 = verts[(x+1)*vpn + (y+1)];
			
			GLdouble tx = dx - GLdouble(x);
			GLdouble ty = dy - GLdouble(y);

			GLdouble txty = tx * ty;

			return h00 * (1.0 - ty - tx + txty)
							+ h01 * (tx - txty)
							+ h11 * txty
							+ h10 * (ty - txty);
		}
		else if (*type == three_shift)
		{
			return 0.0;
		}
	}
}

/*************************************************
 *					  Planet					 *
 *				  Function: Load				 *
 *			  Use: Generates a planet			 *
 *************************************************/
void Planet::Load(string name, Camera *view)
{
	Name = name;
	int seed = 0;
	for (int x = 0; x < Name.length(); x++)
		seed += int(Name[x]);
		
	player_info.Initialize(view, &Physics);
	srand(seed);
	Radius = GLdouble(rand()%9000000 + 1500000);
	
	node_texts = (GLdouble*)malloc(sizeof(GLdouble)*vpn*vpn*2);
	node_indices = (unsigned int*)malloc(sizeof(unsigned int)*vpn*(vpn-1)*2);
	
	for (int x = 0; x < vpn; x++)
		for (int y = 0; y < vpn; y++)
		{
			node_texts[2*(x*vpn + y) + 0] = GLdouble(y)/GLdouble(vpn-1);
			node_texts[2*(x*vpn + y) + 1] = GLdouble(x)/GLdouble(vpn-1);
		}
		
	int index = 0;
	for (int x = 1; x < vpn; x++)
		for (int y = 0; y < vpn; y++)
		{
			node_indices[index++] = x*vpn+y;
			node_indices[index++] = (x-1)*vpn+y;
		}
	
	type = height;
	vert_data = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
	vert_data->Load(seed, Radius, &type);
	
	vert_data->mat.Init();
	vert_data->mat.Load("res/Planet.vx", "res/Planet.ft");
	vert_data->MakeTexture();
	
	Physics.Initialize(5.9742E24, Radius, 0.1);
}

/*************************************************
 *					  Planet					 *
 *				  Function: Prepare				 *
 *		  Use: Setup for a render function		 *
 *************************************************/
void Planet::Prepare()
{
	player_info.Update();
	player_info.ground_height = vert_data->GetGroundHeight(player_info.longitude, player_info.latitude);
	player_info.UpdateScale();
	
	if (type == height)
	{
		if (player_info.distance < vert_data->radius + player_info.ground_height || player_info.ndistance < vert_data->radius + player_info.ground_height)
		{
			GLdouble angle_x = -player_info.longitude + 2.0*pi;
			
			GLdouble VelMag = Magnitude(player_info.cam->Host->Physics.LinearVelocity);
			GLdouble ForceMag = player_info.cam->Host->Physics.Mass*(-VelMag*player_info.cam->Host->Physics.Elasticity*Physics.Elasticity - VelMag);
			
			player_info.cam->Host->Physics.ApplyForce(Normalize(player_info.cam->Host->Physics.LinearVelocity)*ForceMag, Normalize(Physics.Position - player_info.cam->Host->Physics.Position)*player_info.cam->Host->Physics.Radius);
			
			player_info.cam->Host->Physics.Orientation = (180.0/pi)*(Vector(0.0, angle_x, 2.0*pi - acos(Dot(Vector(0.0, 1.0, 0.0), player_info.pvec))));
			player_info.cam->Host->Physics.AngularVelocity = Vector();
			
			//player_info.cam->Host->Physics.LinearVelocity -= player_info.pvec*Dot(player_info.cam->Host->Physics.LinearVelocity, player_info.pvec);
			player_info.cam->Host->Physics.Position += player_info.pvec*(vert_data->radius + player_info.ground_height - player_info.distance);
			
			player_info.distance = Distance(player_info.cam->Host->Physics.Position, Physics.Position);
			player_info.pvec = Normalize(player_info.cam->Host->Physics.Position - Physics.Position);
		}
	}
		
	vert_data->LODCheck(&player_info);
}

/*************************************************
 *					  Planet					 *
 *				  Function: Render				 *
 *			   Use: Renders a planet			 *
 *************************************************/
void Planet::Render()
{
	vert_data->Render(node_texts, node_indices, &player_info);
}

/*************************************************
 *					  Planet					 *
 *				  Function: Release				 *
 *		  Use: Frees a planet from memory		 *
 *************************************************/
void Planet::Release()
{
	free(node_texts);
	free(node_indices);
	vert_data->Release();
	vert_data->mat.Release();
	free(vert_data);
}
