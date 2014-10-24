/*
 *  Planet.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/24/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Planet.h"

void ReferenceInformation::Update(void *planet)
{
	// current pos data

	this->planet = planet;

	GLdouble PlayerHeight = (((Planet*)planet)->data.get_height(this));

	Vector player_pos;
	Vector player_vel;

	if (ReferencePoint->Host)
	{
		player_pos = ReferencePoint->Host->Physics.Position;
		player_vel = player_vel;
	}
	else
	{
		player_pos = ReferencePoint->Position;
		player_vel = Vector();
	}

	distance = Distance(player_pos, ((Planet*)planet)->Physics.Position);
	vec1 = Normalize(player_pos - ((Planet*)planet)->Physics.Position);

	GLdouble scale;
	if ((distance-(((Planet*)planet)->data.radius + PlayerHeight)) <= 7.0)
		scale = 0.2;
	else
		scale = ((1.0/(distance - (((Planet*)planet)->data.radius + PlayerHeight))) + absolute((1.0/(distance - (((Planet*)planet)->data.radius)))))/2.0;

	if (ReferencePoint->viewscale < scale)
		ReferencePoint->viewscale = scale;

	y_angle = asin(vec1.y) + pi/2.0;
	if (vec1.x/cos(y_angle - pi/2.0) > 1.0 || vec1.x/cos(y_angle - pi/2.0) < -1.0)
		x_angle = atan(vec1.z/vec1.x);
	else
		x_angle = acos(vec1.x/cos(y_angle - pi/2.0));

	if (vec1.z < 0)
		x_angle = -x_angle + 2.0*pi;

	// next pos data

	vec2 = Normalize((player_pos + player_vel) - ((Planet*)planet)->Physics.Position);

	ny_angle = asin(vec2.y) + pi/2.0;
	if (vec2.x/cos(ny_angle - pi/2.0) > 1.0 || vec2.x/cos(ny_angle - pi/2.0) < -1.0)
		nx_angle = atan(vec2.z/vec2.x);
	else
		nx_angle = acos(vec2.x/cos(ny_angle - pi/2.0));

	if (vec2.z < 0)
		nx_angle = -nx_angle + 2.0*pi;

	ndistance = Distance(player_pos + player_vel, ((Planet*)planet)->Physics.Position);

	if (distance < ((Planet*)planet)->data.radius + PlayerHeight || ndistance < ((Planet*)planet)->data.radius + PlayerHeight)
	{
		GLdouble angle_x = x_angle;
		if (vec1.z < 0)
			angle_x = -(angle_x - 2.0*pi);
		if (vec1.z > 0)
			angle_x = -angle_x + 2.0*pi;
		ReferencePoint->Host->Physics.Orientation = (180.0/pi)*(Vector(0.0, angle_x, 2.0*pi - acos(Dot(Vector(0.0, 1.0, 0.0), vec1))));

		Collide(&ReferencePoint->Host->Physics, &((Planet*)planet)->Physics);

		player_vel -= vec1*Dot(player_vel, vec1);
		player_pos += vec1*(((Planet*)planet)->data.radius + PlayerHeight - distance);

		distance = Distance(player_pos, ((Planet*)planet)->Physics.Position);
		vec1 = Normalize(player_pos - ((Planet*)planet)->Physics.Position);
	}

	visualangle = acos(((((Planet*)planet)->data.radius + PlayerHeight)/max(distance, (((Planet*)planet)->data.radius + PlayerHeight)+1)));
	if (!(visualangle >= 0.0) && !(visualangle < 0.0))
		visualangle = 0.000931667;

	visualangle1 = acos(((((Planet*)planet)->data.radius)/distance));
	if (!(visualangle1 >= 0.0) && !(visualangle1 < 0.0))
		visualangle1 = 0.000931667;
}

void planet_node::load(int sd, ReferenceInformation *reference)
{
	seed = sd;
	srand(seed);

	heights = (GLdouble*)malloc(sizeof(GLdouble)*node_size*node_size);
	if (!heights)
		cout << __FILE__ << " " << __LINE__ << " heights are unable to be allocated\n";

	x_size = 2.0*pi;
	y_size = pi;
	x_angle = 0.0;
	y_angle = 0.0;
	parent = NULL;
	children = NULL;

	radius = GLdouble(rand()%16000 + 1000)*1000;

	average = 0.0;
	minimum = -radius*.000125;
	maximum = radius*.0003;
	GLdouble random;

	for (int x = 0; x < node_size; x++)
		for (int y = 0; y < node_size; y++)
		{
			random = GLdouble(rand()%16000)*(maximum - minimum)/16000.0 + minimum;
			heights[x*node_size + y] = random;
			average += random;
		}
	average /= node_size*node_size;

	for (int x = 0; x < node_size; x++)
	{
		heights[(node_size-1)*node_size + x] = heights[x];
		heights[x*node_size + node_size-1] = heights[node_size-1];
		heights[x*node_size] = heights[0];
	}

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

	material.ReleaseTextures();
}

void planet_node::split(ReferenceInformation *reference)
{
	GLdouble count;
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			children[x].split(reference);
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
				children[x*2+y].foglimit = foglimit;
				children[x*2+y].heights = (GLdouble*)malloc(sizeof(GLdouble)*node_size*node_size);
				if (!children[x*2+y].heights)
					cout << "ARRG" << endl;
				children[x*2+y].average = 0.0;
				children[x*2+y].minimum = minimum;
				children[x*2+y].maximum = maximum;
				count = 0.0;

				GLdouble distbet = .5*children[x*2+y].radius*(children[x*2+y].x_size + children[x*2+y].y_size)/GLdouble(node_size);
				GLdouble rough = clamp((pi - x_size)/pi, 0.0, 1.0);
				GLdouble heightincrease;
				int tendancy;
				GLdouble mult;
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

						heightincrease = rough*0.1*absolute(pi - x_size)*((GLdouble(rand()%16000)*distbet/16000.0) - 0.5*distbet);

						mult = 1.0;
						while (true)
						{
							if (children[x*2+y].heights[u*node_size + v] > 0.0 && children[x*2+y].heights[u*node_size + v] + heightincrease*mult > 0.0)
								break;

							if (children[x*2+y].heights[u*node_size + v] < 0.0 && children[x*2+y].heights[u*node_size + v] + heightincrease*mult < 0.0)
								break;

							if (children[x*2+y].heights[u*node_size + v] == 0.0)
								break;

							mult -= .1;
							if (mult <= -1.0)
								 break;
						}
						heightincrease *= mult;

						/*mult = 1.0;
						while (true)
						{
							if (!(children[x*2+y].heights[u*node_size + v] + heightincrease*mult >= children[x*2+y].maximum || children[x*2+y].heights[u*node_size + v] + heightincrease*mult <= children[x*2+y].minimum))
								break;

							mult -= .1;
							if (mult <= -1.0)
								 break;
						}
						heightincrease *= mult;*/

						if ((u_pos != u1_pos || v_pos != v1_pos) && (u > 1 && v > 1 && u < node_size-1 && v < node_size-1)/* && (u != 1 && v != 1 && u != node_size-1 && v != node_size-1)*/)
								children[x*2+y].heights[u*node_size + v] += heightincrease;
						if ((u_pos != u1_pos || v_pos != v1_pos) && (u == 0 && x == 1 && v != node_size-1 && v != 0))
						{
							children[x*2+y].heights[u*node_size + v] += heightincrease;
							children[y].heights[(node_size-1)*node_size + v] = children[x*2+y].heights[u*node_size + v];
						}
						if ((u_pos != u1_pos || v_pos != v1_pos) && (v == 0 && y == 1 && u != node_size-1 && u != 0))
						{
							children[x*2+y].heights[u*node_size + v] += heightincrease;
							children[x*2].heights[u*node_size + (node_size-1)] = children[x*2+y].heights[u*node_size + v];
						}

						children[x*2+y].average += children[x*2+y].heights[u*node_size + v];
						count+=1.0;
					}
				}
				children[x*2+y].average /= count;
				children[x*2+y].maximum = minimum;
				children[x*2+y].minimum = maximum;
				for (int u = 0; u < node_size; u++)
					for (int v = 0; v < node_size; v++)
					{
						if (children[x*2+y].heights[u*node_size + v] > children[x*2+y].maximum && children[x*2+y].heights[u*node_size + v] <= maximum)
							children[x*2+y].maximum = children[x*2+y].heights[u*node_size + v];
						if (children[x*2+y].heights[u*node_size + v] < children[x*2+y].minimum && children[x*2+y].heights[u*node_size + v] >= minimum)
							children[x*2+y].minimum = children[x*2+y].heights[u*node_size + v];
					}

				children[x*2+y].material.Init();
				children[x*2+y].material.VertShad = material.VertShad;
				children[x*2+y].material.FragShad = material.FragShad;
				children[x*2+y].material.ShadProg = material.ShadProg;
			}
		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
				children[x*2+y].CreateTextureMap(x, y, reference);
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

void planet_node::determine_lod(ReferenceInformation *reference)
{
	if (!children || !children->children)
	{
		GLdouble cx = x_angle, cy = y_angle;

		if (x_angle + x_size < reference->x_angle && reference->x_angle - (x_angle + x_size) > pi)
			cx += pi/2;
		else if (x_angle > reference->x_angle && x_angle - reference->x_angle > pi)
			cx -= pi/2;

		GLdouble x_dist = min(absolute(cx - reference->x_angle), absolute(cx + x_size - reference->x_angle));
		GLdouble y_dist = min(absolute(cy - reference->y_angle), absolute(cy + y_size - reference->y_angle));
		GLdouble h_dist = radius*sqrt(x_dist*x_dist + y_dist*y_dist);
		GLdouble v_dist = reference->distance - (radius + average);
		v_dist = v_dist < 0.0 ? 0.0 : v_dist;
		GLdouble dist = h_dist*h_dist + v_dist*v_dist;

		GLdouble percent = (x_size*radius*x_size*radius)/dist;
		if (percent >= 1.5 && children == NULL && radius*x_size > 1000.0 && radius*y_size > 1000.0)
			split(reference);
		if (percent <= 0.8 && x_size < pi/4.0 && children)
			merge();
	}
	if (children)
	{
		for (int x = 0; x < 4; x++)
			children[x].determine_lod(reference);
	}
}

void planet_node::renderheights(ReferenceInformation *reference)
{
	if (children)
		for (int x = 0; x < 4; x++)
			children[x].renderheights(reference);
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
				y_temp = y_size*(GLdouble(y)/GLdouble(node_size-1)) + y_angle - pi/2.0;

				v.x = cos(x_temp) * cos(y_temp);
				v.y = sin(y_temp);
				v.z = sin(x_temp) * cos(y_temp);

				vert[3*(x*node_size + y) + 0] = v.x*(radius + heights[x*node_size + y]) - reference->distance*reference->vec1.x;
				vert[3*(x*node_size + y) + 1] = v.y*(radius + heights[x*node_size + y]) - reference->distance*reference->vec1.y;
				vert[3*(x*node_size + y) + 2] = v.z*(radius + heights[x*node_size + y]) - reference->distance*reference->vec1.z;

				text[2*(x*node_size + y) + 1] = GLdouble(x)/GLdouble(node_size-1);
				text[2*(x*node_size + y) + 0] = GLdouble(y)/GLdouble(node_size-1);
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

		material.Use();
		material.ImportNumber(foglimit, "limit");
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

void planet_node::renderwater(ReferenceInformation *reference)
{
	if (children)
		for (int x = 0; x < 4; x++)
			children[x].renderwater(reference);
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
				y_temp = y_size*(GLdouble(y)/GLdouble(node_size-1)) + y_angle - pi/2.0;

				v.x = cos(x_temp) * cos(y_temp);
				v.y = sin(y_temp);
				v.z = sin(x_temp) * cos(y_temp);

				vert[3*(x*node_size + y) + 0] = v.x*(radius) - reference->distance*reference->vec1.x;
				vert[3*(x*node_size + y) + 1] = v.y*(radius) - reference->distance*reference->vec1.y;
				vert[3*(x*node_size + y) + 2] = v.z*(radius) - reference->distance*reference->vec1.z;

				text[2*(x*node_size + y) + 0] = y_size*radius*.25*GLdouble(y)/GLdouble(node_size-1);
				text[2*(x*node_size + y) + 1] = x_size*radius*.25*GLdouble(x)/GLdouble(node_size-1);
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

void planet_node::rendersky(ReferenceInformation *reference, GLdouble scale, GLdouble angle)
{
	GLdouble x_temp, y_temp;
	Vector v;

	int size = 20;

	GLdouble vert[3*size*size];
	GLdouble text[2*size*size];
	unsigned int indices[size*size*6];
	for (int x = 0; x < size; x++)
	{
		x_temp = 2*pi*(GLdouble(x)/GLdouble(size-1));
		for (int y = 0; y < size; y++)
		{
			y_temp = pi*(GLdouble(y)/GLdouble(size-1)) - pi/2.0;

			v.x = cos(x_temp) * cos(y_temp);
			v.y = sin(y_temp);
			v.z = sin(x_temp) * cos(y_temp);

			vert[3*(x*size + y) + 0] = scale*v.x*(radius*1.03) - reference->distance*reference->vec1.x;
			vert[3*(x*size + y) + 1] = scale*v.y*(radius*1.03) - reference->distance*reference->vec1.y;
			vert[3*(x*size + y) + 2] = scale*v.z*(radius*1.03) - reference->distance*reference->vec1.z;

			text[2*(x*size + y) + 1] = GLdouble(y)/GLdouble(size-1);
			text[2*(x*size + y) + 0] = (GLdouble(x)/GLdouble(size-1) + angle) - GLdouble(int(angle));
		}
	}

	int index = 0;
	for (int x = 1; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			indices[index++] = x*size+y;
			indices[index++] = (x-1)*size+y;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, vert);
	glTexCoordPointer(2, GL_DOUBLE, 0, text);
	for (int x = 0; x < size-1; x++)
		glDrawElements(GL_TRIANGLE_STRIP, size*2, GL_UNSIGNED_INT, &indices[x*size*2]);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

GLdouble planet_node::get_height(ReferenceInformation *reference)
{
	if (children)
	{
		for (int x = 0; x < 4; x++)
			if (reference->x_angle >= children[x].x_angle && reference->x_angle < children[x].x_angle + children[x].x_size && reference->y_angle >= children[x].y_angle && reference->y_angle < children[x].y_angle + children[x].y_size)
			{
				return children[x].get_height(reference);
				break;
			}
	}
	else
	{
		GLdouble dx = GLdouble(node_size-1)*(reference->x_angle-x_angle)/x_size;
		GLdouble dy = GLdouble(node_size-1)*(reference->y_angle-y_angle)/y_size;

		int x = int(dx);
		int y = int(dy);

		GLdouble h00 = heights[x*node_size + y];
		GLdouble h01 = heights[(x+1)*node_size + y];
		GLdouble h10 = heights[x*node_size + (y+1)];
		GLdouble h11 = heights[(x+1)*node_size + (y+1)];

		GLdouble tx = dx - GLdouble(x);
		GLdouble ty = dy - GLdouble(y);

		GLdouble txty = tx * ty;

		return h00 * (1.0 - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);
	}
}

GLdouble planet_node::get_minimum(GLdouble curr)
{
	GLdouble c = curr;
	GLdouble y = 0;
	if (children)
	{
		for (int x = 0; x < 4; x++)
		{
			y = children[x].get_minimum(c);
			c = y < c ? y : c;
		}
	}

	return c;
}

GLdouble planet_node::get_maximum(GLdouble curr)
{
	GLdouble c = curr;
	GLdouble y = 0;
	if (children)
	{
		for (int x = 0; x < 4; x++)
		{
			y = children[x].get_maximum(c);
			c = y > c ? y : c;
		}
	}

	return c;
}

void planet_node::CreateTextureMap(int a, int b, ReferenceInformation *reference)
{
	unsigned char colors[3*(tex_size*tex_size)];
	GLdouble height, height2;
	GLdouble red, green, blue;
	int z, w;
	GLdouble lightpercent;
	GLdouble slope = 0.0;
	GLdouble mult = 1000.0/node_tex_radio;

	GLdouble sand = 0.0, snow = 10000.0, divisor = 1000.0;
	GLdouble sand_snow_slope = (snow - sand)/divisor;

	for (int x = 0; x < tex_size; x++)
		for (int y = 0; y < tex_size; y++)
		{
			GLdouble dx = node_tex_radio*GLdouble(x);
			GLdouble dy = node_tex_radio*GLdouble(y);

			z = int(dx);
			w = int(dy);


			GLdouble h00 = heights[z*node_size + w];
			GLdouble h01 = heights[(z+1)*node_size + w];
			GLdouble h10 = heights[z*node_size + (w+1)];
			GLdouble h11 = heights[(z+1)*node_size + (w+1)];

			GLdouble tx = dx - GLdouble(z);
			GLdouble ty = dy - GLdouble(w);

			GLdouble txty = tx * ty;

			height = h00 * (1.0 - ty - tx + txty)
				   + h01 * (tx - txty)
				   + h11 * txty
				   + h10 * (ty - txty);

			if (dx-mult < 0.0 || dy-mult < 0.0)
			{
				ReferenceInformation r;

				r.x_angle = modulate(x_angle + x_size*(dx-mult)/GLdouble(node_size-1), 2.0*pi);
				r.y_angle = modulate(y_angle + y_size*(dy-mult)/GLdouble(node_size-1), pi);

				height2 = (((Planet*)reference->planet)->data.get_height(&r));
			}
			else
			{
				GLdouble dx1 = dx-mult;
				GLdouble dy1 = dy-mult;

				z = int(dx1);
				w = int(dy1);


				h00 = heights[z*node_size + w];
				h01 = heights[(z+1)*node_size + w];
				h10 = heights[z*node_size + (w+1)];
				h11 = heights[(z+1)*node_size + (w+1)];

				tx = dx1 - GLdouble(z);
				ty = dy1 - GLdouble(w);

				txty = tx * ty;

				height2 = h00 * (1.0 - ty - tx + txty)
					   + h01 * (tx - txty)
					   + h11 * txty
					   + h10 * (ty - txty);
			}

			slope = (height - height2)/(2.24*mult*y_size/GLdouble(node_size-1));//sqrt(node_tex_radio*node_tex_radio*x_size*x_size/GLdouble((node_size-1)*(node_size-1)) + node_tex_radio*node_tex_radio*y_size*y_size/GLdouble((node_size-1)*(node_size-1)));
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

			lightpercent = 1.0;
			if (x > 0 && x < tex_size-1 && y > 0 && y < tex_size-1)
				lightpercent = (GLdouble(rand()%24 + 38)/50.0)*(1.0 - (slope*.4/2000000.0));
			else
				lightpercent = (1.0 - (slope*.4/2000000.0));
			colors[3*(x*tex_size + y) + 0] = (unsigned char)int(clamp(red*lightpercent, 0.0, 255.0));
			colors[3*(x*tex_size + y) + 1] = (unsigned char)int(clamp(green*lightpercent, 0.0, 255.0));
			colors[3*(x*tex_size + y) + 2] = (unsigned char)int(clamp(blue*lightpercent, 0.0, 255.0));
		}

	GLuint map;
	glGenTextures(1, &map);
	glBindTexture(GL_TEXTURE_2D, map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_size, tex_size, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)colors);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	material.AddTexture(map);
}

void Planet::Load(string Name, Camera *referencepoint)
{
	this->Name = Name;
	int seed = 0;
	for (int x = 0; x < Name.length(); x++)
		seed += int(Name[x]);
	this->referencepoint.ReferencePoint = referencepoint;
	this->referencepoint.planet = this;
	data.load(seed, &this->referencepoint);
	radius = data.radius;

	Water.Init();
	Water.Load("res/WaterPlanet.vx", "res/WaterPlanet.ft");
	Water.AddTexture("res/waterish", "tga", 1);

	Sky.Init();
	Sky.Load("res/SkyPlanet.vx", "res/SkyPlanet.ft");
	Sky.AddTexture("res/clouds", "tga", 1);

	Shell.Init();
	Shell.Load("res/ShellPlanet.vx", "res/ShellPlanet.ft");

	data.material.Init();
	data.material.Load("res/Planet.vx", "res/Planet.ft");
	data.CreateTextureMap(0, 0, &this->referencepoint);

	Physics.Mass = 5515*(4/3)*pi*pow(radius, 3);
	Physics.Inertia = (2.0/5.0)*Physics.Mass*radius*radius;

	srand(seed);
	AtmosphereColor = Vector(GLdouble(rand()%100)/100.0, GLdouble(rand()%100)/100.0, GLdouble(rand()%100)/100.0);
	OceanColor = Vector(GLdouble(rand()%100)/100.0, GLdouble(rand()%100)/100.0, GLdouble(rand()%100)/100.0);
	AtmosphereDensity = GLfloat(rand()%1000)/10000000.0 + .000004;
	AtmosphereScale = GLdouble(rand()%1000)/10000.0 + 1.008;

	data.foglimit = AtmosphereScale;

	glEnable(GL_FOG);
	GLfloat fogColor[] = {GLfloat(AtmosphereColor.x), GLfloat(AtmosphereColor.y), GLfloat(AtmosphereColor.z), 1.0};
	glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_DENSITY, GLfloat(AtmosphereDensity));

	sky_angle = 0.0;

	data.split(&this->referencepoint);
	data.split(&this->referencepoint);
	data.split(&this->referencepoint);

	Physics.Position.z = 146000000000.0;
}

void Planet::Prepare()
{
	referencepoint.Update(this);
	data.determine_lod(&referencepoint);
}

void Planet::Render()
{
	GLdouble SkyScale = 1.0006;
	sky_angle += 0.0000025;

	Physics.Orientation += Vector(0.0, 0.1, 0.0);

	GLdouble r2 = data.radius*data.radius;
	GLdouble r12 = data.radius*AtmosphereScale*data.radius*AtmosphereScale;
	GLdouble d2 = referencepoint.distance*referencepoint.distance;

	GLdouble end = (referencepoint.distance < AtmosphereScale*data.radius ? -20.0*(AtmosphereScale*data.radius - referencepoint.distance) : sqrt(d2 - r12));
	GLdouble divisor = referencepoint.distance < data.radius ? sqrt(r12 - r2) - end : sqrt(d2 - r2) + sqrt(r12 - r2) - end;

	data.renderheights(&referencepoint);

	if (referencepoint.distance < data.radius)
	{
		glCullFace(GL_FRONT);
		Shell.Use();
		Shell.ImportNumber(referencepoint.ReferencePoint->viewscale, "scale");
		Shell.ImportNumber(end, "end");
		Shell.ImportNumber(divisor, "divisor");
		data.rendersky(&referencepoint, AtmosphereScale, 0.0);
		glCullFace(GL_BACK);

		glDisable(GL_CULL_FACE);
		Sky.Use();
		Sky.ImportNumber(AtmosphereScale, "limit");
		Sky.ImportNumber(referencepoint.ReferencePoint->viewscale, "scale");
		data.rendersky(&referencepoint, SkyScale, sky_angle);
		glEnable(GL_CULL_FACE);
	}

	glDisable(GL_CULL_FACE);
	Water.Use();
	Water.ImportNumber(AtmosphereScale, "limit");
	Water.ImportVector(OceanColor, "color");
	data.renderwater(&referencepoint);
	glEnable(GL_CULL_FACE);

	if (referencepoint.distance >= data.radius)
	{
		glCullFace(GL_FRONT);
		Shell.Use();
		Shell.ImportNumber(referencepoint.ReferencePoint->viewscale, "scale");
		Shell.ImportNumber(divisor, "divisor");
		Shell.ImportNumber(end, "end");
		data.rendersky(&referencepoint, AtmosphereScale, 0.0);
		glCullFace(GL_BACK);

		glDisable(GL_CULL_FACE);
		Sky.Use();
		Sky.ImportNumber(AtmosphereScale, "limit");
		Sky.ImportNumber(referencepoint.ReferencePoint->viewscale, "scale");
		data.rendersky(&referencepoint, SkyScale, sky_angle);
		glEnable(GL_CULL_FACE);
	}
}

void Planet::Release()
{
	data.destroy();
	data.material.Release();
	Water.Release();
	Sky.Release();
	Shell.Release();
}
