/*
 * planet.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: nbingham
 */

#include "planet.h"
#include <std/algorithm.h>
#include <math/curve.h>
#include "canvas.h"
#include <std/fill.h>

cavehdl::cavehdl()
{
	strength = 1.0f;
}

cavehdl::~cavehdl()
{
}

grad3f cavehdl::operator()(gvec3f location)
{
	grad3f d = 1e30f;
	for (int i = 0; i < control.size()-1; i++)
	{
		vec3f v = control[i+1].first - control[i].first;
		gvec3f w = location - control[i].first;

		grad3f c1 = dot(w, v);
		float c2 = mag2(v);
		grad3f b = c1/c2;
		if (c1 <= 0.0f)
			d = min(d, dist2(location, control[i].first)/(control[i].second*control[i].second));
		else if (c2 <= c1)
			d = min(d, dist2(location, control[i+1].first)/(control[i+1].second*control[i+1].second));
		else
		{
			grad3f rad = (control[i].second*(1.0f-b) + control[i+1].second*b);
			d = min(d, dist2(location, gvec3f(control[i].first) + b*gvec3f(v))/(rad*rad));
		}
	}

	if (control.size() == 1)
		d = min(d, dist2(location, control[0].first)/(control[0].second*control[0].second));

	//cout << d << " " << dot(d.normal(), norm(control[0].first - vec3f(location[0][3], location[1][3], location[2][3]))) << endl;

	return d;
}

planethdl::planethdl()
{
	type = "planet";
	noise.initialize(0);
	program = 0;
}

planethdl::planethdl(palettehdl &palette, int seed)
{
	type = "planet";
	noise.initialize(seed);
	program = palette.program("res/engine/block.vx", "res/engine/block.ft");
	geometry.density = density;
	geometry.size = 6000000.0;

	int n = rand()%50;
	caves.append_back(fill_t(n, cavehdl()));
	for (int i = 0; i < n; i++)
	{
		int m = rand()%50 + 1;
		vec3f point;
		point[0] = (float)rand()/(float)(RAND_MAX-1);
		point[1] = (float)rand()/(float)(RAND_MAX-1);
		point[2] = (float)rand()/(float)(RAND_MAX-1);
		point *= geometry.size;

		caves[i].strength = 1.0f;
		caves[i].control.reserve(m);
		caves[i].control.push_back(pair<vec3f, float>());
		caves[i].control[0].first = point;
		caves[i].control[0].second = 3.0*(float)rand()/(float)(RAND_MAX-1) + 1.0;
		for (int j = 1; j < m; j++)
		{
			caves[i].control.push_back(pair<vec3f, float>());
			caves[i].control[j].second = 3.0*(float)rand()/(float)(RAND_MAX-1) + 1.0;
			float ry = 2.0*m_pi*(float)rand()/(float)(RAND_MAX-1);
			float rx = m_pi*(float)rand()/(float)(RAND_MAX-1);
			float srx = sin(rx);
			float step = (caves[i].control[j].second + caves[i].control[j-1].second)*(float)(rand()%4 + 1);
			point += step*vec3f(srx*cos(ry), srx*sin(ry), cos(rx));
			caves[i].control[j].first = point;

			if (point[0] < 0.0 || point[0] > geometry.size || point[1] < 0.0 || point[1] > geometry.size || point[2] < 0 || point[2] > geometry.size)
				break;
		}

	}

	geometry.generate(this);
	geometry.load();
}

planethdl::~planethdl()
{

}

void planethdl::render(framehdl &frame)
{
	glUseProgram(program);

	//mat3f normal_matrix = normal(frame.modelview.value());
	mat4f modelview_projection_matrix = frame.projection.value()*frame.modelview.value();
	glUniform4f(glGetUniformLocation(program, "color"), 1.0, 1.0, 1.0, 1.0);

	// Find the locations of the vertex, normal, and texcoord variables in the shader
	int vertex_location = glGetAttribLocation(program, "vertex");
	//int normal_location = glGetAttribLocation(program, "normal");
	//int texcoord_location = glGetAttribLocation(program, "texcoord0");
	int mvp_matrix_location = glGetUniformLocation(program, "modelview_projection_matrix");
	//int normal_matrix_location = glGetUniformLocation(program, "normal_matrix");
	int origin_location = glGetUniformLocation(program, "origin");

	glUniformMatrix4fv(mvp_matrix_location, 1, true, (GLfloat*)&modelview_projection_matrix);
	//glUniformMatrix3fv(normal_matrix_location, 1, true, (GLfloat*)&normal_matrix);

	geometry.render(frame, vertex_location, origin_location);
}

void planethdl::prepare(canvashdl &canvas)
{
	for (int c = 0; c < canvas.players.size(); c++)
	{
		objecthdl *camera = canvas.players[c].camera;

		if (camera != NULL)
		{
			array<blockhdl*> loc;

			if (geometry.contains(camera->position, 1.0f))
				loc.push_back(&geometry);
			else
				geometry.merge();

			bool change = true;
			while (loc.size() > 0 && change)
			{
				change = false;
				for (int i = loc.size()-1; i >= 0; i--)
				{
					bool found = false;
					for (int j = 0; j < loc[i]->children.size(); j++)
					{
						if (loc[i]->children[j].contains(camera->position, 1.0f))
						{
							loc.push_back(&loc[i]->children[j]);
							found = true;
						}
						else
							loc[i]->children[j].merge();
					}

					if (found)
					{
						(loc.begin() + i).pop();
						change = true;
					}
				}
			}

			for (int i = 0; i < loc.size(); i++)
				if (loc[i]->size > (float)cubes_per_side)
					loc[i]->split(this);
		}
	}
}

grad3f planethdl::density(gvec3f location, void *data)
{
	planethdl *planet = (planethdl*)data;

	grad3f d = 1e30f;
	for (int i = 0; i < planet->caves.size(); i++)
		d = core::min(d, planet->caves[i](location));

	d = 1.0f/(1.0f + d);

	grad3f x = mag(location - vec3f(planet->geometry.size, planet->geometry.size, planet->geometry.size)/2.0f) - (float)planet->geometry.size/2.5f;
	grad3f y = 1.0f/(1.0f+pow(2.0f, x));

	grad3f n = ridge_noise(location/grad3f(10.0f), 1.0, 5.0, 3.0, 1.0, 2.0, planet->noise) - 0.2f;

	return (n - d)*y + y - 1.0f;
}

