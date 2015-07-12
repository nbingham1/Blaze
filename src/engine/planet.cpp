/*
 * planet.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: nbingham
 */

#include "planet.h"
#include "core/algorithm.h"
#include "core/curve.h"

cavehdl::cavehdl()
{
	strength = 1.0f;
}

cavehdl::~cavehdl()
{
}

float cavehdl::operator()(vec3f location)
{
	float d = 1e100;
	for (int i = 0; i < control.size()-1; i++)
	{
		vec3f v = control[i+1].first - control[i].first;
		vec3f w = location - control[i].first;

		float c1 = dot(w, v);
		float c2 = mag2(v);
		float b = c1/c2;
		if (c1 <= 0)
			d = min(d, dist2(location, control[i].first)/control[i].second);
		else if (c2 <= c1)
			d = min(d, dist2(location, control[i+1].first)/control[i+1].second);
		else
			d = min(d, dist2(location, control[i].first + b*v)/(control[i].second*(1-b) + control[i+1].second*b));
	}

	if (control.size() == 1)
		d = min(d, dist2(control.back().first, location)/control.back().second);

	float x = strength*d;
	x *= x;
	return 1.0/(1.0+x);
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
	program = palette.program("res/core/block.vx", "res/core/block.ft");
	geometry.density = density;
	geometry.size = 100.0;

	int n = rand()%50;
	caves.resize(n);
	for (int i = 0; i < n; i++)
	{
		int m = rand()%50 + 1;
		vec3f point;
		point[0] = (float)rand()/(float)(RAND_MAX-1);
		point[1] = (float)rand()/(float)(RAND_MAX-1);
		point[2] = (float)rand()/(float)(RAND_MAX-1);
		point *= geometry.size;

		caves[i].control.reserve(m);
		caves[i].control.push_back(pair<vec3f, float>());
		caves[i].control[0].first = point;
		caves[i].control[0].second = 5.0*(float)rand()/(float)(RAND_MAX-1) + 1.0;
		for (int j = 1; j < m; j++)
		{
			caves[i].control.push_back(pair<vec3f, float>());
			caves[i].control[j].second = 5.0*(float)rand()/(float)(RAND_MAX-1) + 1.0;
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

	geometry.render(vertex_location, origin_location);
}

void planethdl::prepare(palettehdl &palette)
{
}

float planethdl::density(vec3f location, void *data)
{
	planethdl *planet = (planethdl*)data;

	float result = 0.0;
	for (int i = 0; i < planet->caves.size(); i++)
		result -= planet->caves[i](location);

	float x = mag(location - vec3f(50, 50, 50)) - 40.0;
	float y = 1.0f/(1.0f+pow(2.0, x));

	float n = ridge_noise(location/10.0, 1.0, 5.0, 3.0, 1.0, 2.0, planet->noise) - 0.2;

	return (result + n)*y + y - 1.0f;
}
