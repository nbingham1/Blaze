/*
 * plant.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: nbingham
 */

#include "plant.h"
#include "canvas.h"

plantprt_type::plantprt_type()
{
	geometry = NULL;
	vertex_buffer = -1;
	index_buffer = -1;
	vx = -1;
	ft = -1;
	pm = -1;
	num_indices = 0;
	num_vertices = 0;
	symbol = '\0';
	delta_pos = vec();
	delta_ang = vec();
	push_delta = false;
	pop_delta = false;
}

plantprt_type::~plantprt_type()
{
	if (geometry != NULL)
		delete [] geometry;
}

void plantprt_type::render(list<pair<vec, space> > *offset, float age)
{
	if (num_indices > 0 && num_vertices > 0)
	{
		glUseProgram(pm);

		glUniform4f(glGetUniformLocation(pm, "position"), offset->rbegin()->first.x, offset->rbegin()->first.y, offset->rbegin()->first.z, 0.0);
		glUniform4f(glGetUniformLocation(pm, "x"), offset->rbegin()->second.x.x, offset->rbegin()->second.x.y, offset->rbegin()->second.x.z, 0.0);
		glUniform4f(glGetUniformLocation(pm, "y"), offset->rbegin()->second.y.x, offset->rbegin()->second.y.y, offset->rbegin()->second.y.z, 0.0);
		glUniform4f(glGetUniformLocation(pm, "z"), offset->rbegin()->second.z.x, offset->rbegin()->second.z.y, offset->rbegin()->second.z.z, 0.0);
		double C = 0.01;
		glUniform1f(glGetUniformLocation(pm, "C"), C);
		glUniform1f(glGetUniformLocation(pm, "FC"), 1.0/log(10000000000.0*C + 1.0));
		glUniform1f(glGetUniformLocation(pm, "age"), age);

		glActiveTexture(GL_TEXTURE0);
		tx.render();
		glUniform1i(glGetUniformLocation(pm, "tex_0"), 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(plantvtx), (float*)24);
		glNormalPointer(GL_FLOAT, sizeof(plantvtx), (float*)12);
		glVertexPointer(3, GL_FLOAT, sizeof(plantvtx), NULL);

		glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (push_delta)
		offset->push_back(*offset->rbegin());
	if (pop_delta)
		offset->drop_back();

	offset->rbegin()->first += offset->rbegin()->second.transform(age*delta_pos);
	offset->rbegin()->second.rotate_xyz(delta_ang);
}

plantprt::plantprt()
{
	age = 0.0;
	type = NULL;
}

plantprt::plantprt(float age, plantprt_type *type)
{
	this->age = age;
	this->type = type;
}

plantprt::~plantprt()
{

}

plantpattern::plantpattern()
{

}

plantpattern::plantpattern(const plantpattern &pattern) : list<plantprt>((list<plantprt>)pattern)
{

}

plantpattern::plantpattern(string p, list<plantprt_type> *types)
{
	for (int i = 0; i < (int)p.length(); i++)
	{
		for (node<plantprt_type> *type = types->begin(); type != types->end(); type = type->next)
			if (type->symbol == p[i])
			{
				float age = 0.0;
				if (i+1 < (int)p.length() && p[i+1] == '(')
				{
					int j = p.find_first_of(")", i+1);
					if (j != -1)
					{
						cout << p.substr(i+2, j-i-2) << endl;
						age = atof(p.substr(i+2, j-i-2).c_str());
						i = j;
					}
				}

				push_back(plantprt(age, type));
			}
	}
}

plantpattern::~plantpattern()
{

}

plantrule::plantrule()
{

}

plantrule::plantrule(string f, string t, list<plantprt_type> *types) : from(f, types), to(t, types)
{
}

plantrule::~plantrule()
{

}

planthdl::planthdl()
{

}

planthdl::~planthdl()
{

}

void planthdl::initialize_derived()
{
	cout << "Initializing Plant" << endl;
	type = OBJ_PLANT;

	int num_base = 8;

	/**
	 * Set up the data for the branch of a maple tree
	 */
	node<plantprt_type> *branch = types.push_back(plantprt_type());
	branch->delta_pos = vec(0.0, 1.0, 0.0, 0.0);

	branch->num_vertices = num_base*2 + 2;
	branch->num_indices  = num_base*2 + 2;

	branch->symbol = 'B';
	GLuint *indices = new GLuint[branch->num_indices];
	for (int i = 0; i < branch->num_vertices; i++)
		indices[i] = i;

	branch->geometry = new plantvtx[branch->num_vertices];
	for (int i = 0; i <= num_base; i++)
	{
		branch->geometry[i*2 + 0].v[0] = 0.05*cos(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 0].v[1] = 0.0;
		branch->geometry[i*2 + 0].v[2] = 0.05*sin(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 0].n[0] = cos(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 0].n[1] = 0.0;
		branch->geometry[i*2 + 0].n[2] = sin(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 0].t[0] = 4.0*(float)i/(float)num_base;
		branch->geometry[i*2 + 0].t[1] = 0.0;

		branch->geometry[i*2 + 1].v[0] = 0.05*cos(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 1].v[1] = 1.0;
		branch->geometry[i*2 + 1].v[2] = 0.05*sin(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 1].n[0] = cos(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 1].n[1] = 0.0;
		branch->geometry[i*2 + 1].n[2] = sin(2.0*m_pi*(float)i/(float)num_base);
		branch->geometry[i*2 + 1].t[0] = 4.0*(float)i/(float)num_base;
		branch->geometry[i*2 + 1].t[1] = 1.0;
	}

	glGenBuffers(1, &branch->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, branch->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plantvtx)*branch->num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plantvtx)*branch->num_vertices, branch->geometry);

	glGenBuffers(1, &branch->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, branch->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*branch->num_indices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*branch->num_indices, indices);

	delete [] indices;

	int width, height;
	unsigned char *bark_txdt = rgb_tga((char*)"res/bark.tga", &width, &height);
	branch->tx.initialize(bark_txdt, width, true, true, GL_RGB);
	delete [] bark_txdt;

	int TextLen;
	char ErrText[1024];

	TextLen = 1024;
	branch->vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/plant.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	branch->ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/plant.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	branch->pm = glCreateProgram();
	glAttachShader(branch->pm, branch->vx);
	glAttachShader(branch->pm, branch->ft);
	glLinkProgram(branch->pm);

	/**
	 * Set up the data for the leaf of a maple tree.
	 */
	node<plantprt_type> *leaf = types.push_back(plantprt_type());
	leaf->symbol = 'L';

	leaf->num_vertices = 4;
	leaf->num_indices = 4;
	indices = new GLuint[leaf->num_indices];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	leaf->geometry = new plantvtx[4];
	leaf->geometry[0].v[0] = -0.5;
	leaf->geometry[0].v[1] = 0.0;
	leaf->geometry[0].v[2] = 0.0;
	leaf->geometry[0].n[0] = 0.0;
	leaf->geometry[0].n[1] = 1.0;
	leaf->geometry[0].n[2] = 0.0;
	leaf->geometry[0].t[0] = 0.0;
	leaf->geometry[0].t[1] = 0.0;

	leaf->geometry[1].v[0] = -0.5;
	leaf->geometry[1].v[1] = 1.0;
	leaf->geometry[1].v[2] = 0.0;
	leaf->geometry[1].n[0] = 0.0;
	leaf->geometry[1].n[1] = 1.0;
	leaf->geometry[1].n[2] = 0.0;
	leaf->geometry[1].t[0] = 0.0;
	leaf->geometry[1].t[1] = 1.0;

	leaf->geometry[2].v[0] = 0.5;
	leaf->geometry[2].v[1] = 0.0;
	leaf->geometry[2].v[2] = 0.0;
	leaf->geometry[2].n[0] = 0.0;
	leaf->geometry[2].n[1] = 1.0;
	leaf->geometry[2].n[2] = 0.0;
	leaf->geometry[2].t[0] = 1.0;
	leaf->geometry[2].t[1] = 0.0;

	leaf->geometry[3].v[0] = 0.5;
	leaf->geometry[3].v[1] = 1.0;
	leaf->geometry[3].v[2] = 0.0;
	leaf->geometry[3].n[0] = 0.0;
	leaf->geometry[3].n[1] = 1.0;
	leaf->geometry[3].n[2] = 0.0;
	leaf->geometry[3].t[0] = 1.0;
	leaf->geometry[3].t[1] = 1.0;

	glGenBuffers(1, &leaf->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, leaf->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plantvtx)*leaf->num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plantvtx)*leaf->num_vertices, leaf->geometry);

	glGenBuffers(1, &leaf->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, leaf->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*leaf->num_indices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*leaf->num_indices, indices);

	delete [] indices;

	unsigned char *leaf_txdt = rgb_tga((char*)"leaf.tga", &width, &height);
	leaf->tx.initialize(leaf_txdt, width, true, false, GL_RGBA);
	delete [] leaf_txdt;

	TextLen = 1024;
	leaf->vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/plant.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	leaf->ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/plant.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	leaf->pm = glCreateProgram();
	glAttachShader(leaf->pm, leaf->vx);
	glAttachShader(leaf->pm, leaf->ft);
	glLinkProgram(leaf->pm);

	/**
	 * Set up the split and merge behavior
	 */
	node<plantprt_type> *split = types.push_back(plantprt_type());
	split->symbol = '[';
	split->push_delta = true;
	node<plantprt_type> *merge = types.push_back(plantprt_type());
	merge->symbol = ']';
	merge->pop_delta = true;

	node<plantprt_type> *plus = types.push_back(plantprt_type());
	plus->symbol = '+';
	plus->delta_ang = vec(m_pi*22.5/180, 0.0, 0.0, 0.0);
	node<plantprt_type> *minus = types.push_back(plantprt_type());
	minus->symbol = '-';
	minus->delta_ang = vec(-m_pi*22.5/180, 0.0, 0.0, 0.0);

	//parts.push_back(plantprt(0.0, branch));
	parts = plantpattern("L", &types);
	rules.push_back(plantrule("L(0.2)", "B(0.1)-[[L(0.1)]+L(0.1)]+B(0.1)[+B(0.1)L(0.1)]-L(0.1)", &types));
	rules.push_back(plantrule("B(0.2)", "B(0.1)B(0.1)", &types));

	position = location(100000000.0, 0.0, 0.0);
}

void planthdl::release_derived()
{
	parts.clear();
}

void planthdl::prepare_derived()
{
	for (node<plantprt> *part = parts.begin(); part != parts.end(); part = part->next)
		part->age += 0.00001;

	list<pair<node<plantrule>*, node<plantprt>*> > matches;

	for (node<plantprt> *base = parts.begin(); base != parts.end();)
	{
		matches.clear();
		for (node<plantrule> *rule = rules.begin(); rule != rules.end(); rule = rule->next)
			matches.push_back(pair<node<plantrule>*, node<plantprt>*>(rule, rule->from.begin()));

		for (node<plantprt> *part = base; part != parts.end();)
		{
			for (node<pair<node<plantrule>*, node<plantprt>*> > *match = matches.begin(); match != matches.end();)
			{
				if (match->second->type == part->type && part->age >= match->second->age)
				{
					match->second = match->second->next;
					if (match->second == NULL)
					{
						node<plantprt> *iprt = parts.erase(base, part);
						part = parts.end();
						base = parts.end();
						for (node<plantprt> *add = match->first->to.rbegin(); add != match->first->to.rend(); add = add->prev)
							iprt = parts.insert_before(iprt, *add);
						match = matches.end();
					}
					else
						match = match->next;
				}
				else
					match = matches.erase(match);
			}

			if (part != parts.end())
				part = part->next;
		}

		if (base != parts.end())
			base = base->next;
	}
}

void planthdl::render_derived()
{
	glDisable(GL_CULL_FACE);
	list<pair<vec, space> > offset_stack;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		offset_stack.push_back(pair<vec, space>((position - ((canvashdl*)super)->active_camera->position).tovec(), space()));
	else
		offset_stack.push_back(pair<vec, space>(vec(), space()));

	for (node<plantprt> *part = parts.begin(); part != parts.end(); part = part->next)
		part->type->render(&offset_stack, part->age);
	glEnable(GL_CULL_FACE);
}
