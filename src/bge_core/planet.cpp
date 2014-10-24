#include "planet.h"
#include "canvas.h"

bufferprt::bufferprt(int off)
{
	part = NULL;
	offset = off;
	next = NULL;
	prev = NULL;
}

bufferprt::~bufferprt()
{
	part = NULL;
	offset = 0;
	next = NULL;
	prev = NULL;
}

planetprt::planetprt()
{
	a1 = 0.0;
	a2 = 0.0;
	a3 = 0.0;
	min = 0.0;
	max = 0.0;
	radius = 0.0;
	level = 0;
	polarity = 1;
	sibling_number = 0;
	interpolator = 0.0;
	cull = false;
	update_split = false;
	update_merge = false;
	orgin = NULL;
	parent = NULL;
	children = NULL;
	neighbors[0] = NULL;
	neighbors[1] = NULL;
	neighbors[2] = NULL;
	neighbor_type[0] = 0;
	neighbor_type[1] = 0;
	neighbor_type[2] = 0;
	buffer_location = NULL;
}

planetprt::~planetprt()
{
	release();
}

void planetprt::initialize(planethdl *org, location vert1, location vert2, location vert3, planetprt *n1, int nt1, planetprt *n2, int nt2, planetprt *n3, int nt3)
{
	orgin = org;
	orgin->allocate_part(this);
	if (buffer_location == NULL)
	{
		printf("Error: Not enough space allocated for base planet structure.\n");
		exit(1);
	}
	v1 = vert1;
	v2 = vert2;
	v3 = vert3;
	neighbors[0] = n1;
	neighbors[1] = n2;
	neighbors[2] = n3;
	neighbor_type[0] = nt1;
	neighbor_type[1] = nt2;
	neighbor_type[2] = nt3;

	center = (v1 + v2 + v3)/3.0;

	nv1 = v1.tovec()/orgin->radius,
	nv2 = v2.tovec()/orgin->radius,
	nv3 = v3.tovec()/orgin->radius;
	ncenter = norm(center.tovec());

	ntangent = norm(cross(ncenter, nv1));

	radius = acos(dot(nv1, ncenter));
	interpolator = 1.0;

	a1 = acos(dot(nv1, nv2));
	a2 = acos(dot(nv2, nv3));
	a3 = acos(dot(nv3, nv1));


	// vertex generation
	int x = 0, y = 0, count = 0;
	double x_vps_ratio = 0.0, y_x_ratio = 0.0, angle = 0.0;
	double sin_a1 = sin(a1), sin_a3 = sin(a3), sin_angle = 0.0;
	location v1_v2, v1_v3;
	location vector;
	vec final_vector;
	vec normal;
	double magnitude;
	vec lnormal;

	noisevec height;

	int offset = buffer_location->offset*verts_per_part;

	for (x = 0; x < verts_per_side; x++)
	{
		x_vps_ratio = double(x)/double(verts_per_side-1);
		v1_v2 = v1*(sin((1.0-x_vps_ratio)*a1)/sin_a1) + v2*(sin(x_vps_ratio*a1)/sin_a1);
		v1_v3 = v1*(sin((1.0-x_vps_ratio)*a3)/sin_a3) + v3*(sin(x_vps_ratio*a3)/sin_a3);
		angle = acos(dot(v1_v2.tovec()/orgin->radius, v1_v3.tovec()/orgin->radius));
		sin_angle = sin(angle);

		for (y = 0; y < x+1; y++)
		{
			y_x_ratio = x != 0 ? double(y)/double(x) : 0.0;
			count = (x*x + x)/2 + y;
			if (x_vps_ratio == 0.0)
				vector = v1;
			else
				vector = (v1_v2*(sin((1-y_x_ratio)*angle)/sin_angle) + v1_v3*(sin(y_x_ratio*angle)/sin_angle));

			magnitude = mag(vector.tovec());
			normal = (vector/magnitude).tovec();
			final_vector = (vector*(orgin->radius/magnitude) - center).tovec();

			for (int z = 0; z < 3; z++)
				orgin->terrain_geometry[offset + count].v[z] = final_vector[z];

			height = orgin->terrain(vector);
			orgin->terrain_geometry[offset + count].g[0] = height.h;
			orgin->terrain_geometry[offset + count].g[1] = orgin->terrain_geometry[offset + count].g[0];

			lnormal = height.normal(normal, ntangent);

			orgin->terrain_geometry[offset + count].dn[0] = lnormal.x;
			orgin->terrain_geometry[offset + count].dn[1] = lnormal.y;
			orgin->terrain_geometry[offset + count].dn[2] = lnormal.z;

			final_vector = (vector*(orgin->radius/magnitude)).tovec();

			for (int z = 0; z < 3; z++)
				orgin->terrain_geometry[offset + count].n[z] = normal[z];

			orgin->terrain_geometry[offset + count].t[0] = a1*orgin->radius*float(x)/float(verts_per_side-1);
			orgin->terrain_geometry[offset + count].t[1] = a1*orgin->radius*float(y)/float(verts_per_side-1);
			orgin->terrain_geometry[offset + count].t[2] = (y == 0 ? 1.0 : (x == (verts_per_side - 1) ? 2.0 : (x == y ? 3.0 : 0.0)));
		}
	}
}

int intraytri(location orig, location dir, location vert0, location vert1, location vert2, location1D *t, location1D *u, location1D *v)
{
	location edge1, edge2, tvec, pvec, qvec;
	location1D det, inv_det;
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;
	pvec = cross(dir, edge2);
	det = dot(edge1, pvec);

	if (det < m_epsilon)
		return 0;
	tvec = orig - vert0;
	*u = dot(tvec, pvec);
	if (*u < 0.0 || *u > det)
		return 0;
	qvec = cross(tvec, edge1);
	*v = dot(dir, qvec);
	if (*v < 0.0 || det < (location1D)(*u + *v))
		return 0;
	*t = dot(edge2, qvec);
	*t /= inv_det.todouble();
	*u /= inv_det.todouble();
	*v /= inv_det.todouble();
	return 1;
}

void planetprt::getheight(location vc, double *h)
{
	location1D tl, ul, vl;
	int a, a1;
	if (intraytri(location(), -vc, v1, v2, v3, &tl, &ul, &vl))
	{
		if (children != NULL)
			for (int x = 0; x < 4; x++)
				children[x].getheight(vc, h);
		else
		{
			location q, r;
			location1D ql, rl;
			q = cross(v2, (v3 - v2));
			ql = dot(q, (v2 - v1));
			r = cross(v2, (v2 - v1));
			rl = dot(r, (v3 - v2));
			double dx = (verts_per_side-1)*(1.0 - dot(v2 + vc, q).todouble()/ql.todouble());
			double dy = (verts_per_side-1)*dot(-vc - v2, r).todouble()/rl.todouble();

			int x = int(dx);
			int y = int(dy);

			if (x > 0 && y > 0)
			{
				a = (x*x + x)/2;
				a1 = ((x+1)*(x+1) + (x+1))/2;

				double h00 = orgin->terrain_geometry[buffer_location->offset + a + y].g[0];
				double h01 = orgin->terrain_geometry[buffer_location->offset + a1 + y].g[0];
				double h11 = orgin->terrain_geometry[buffer_location->offset + a1 + (y+1)].g[0];
				double h10;
				if (y+1 > x)
					h10 = h00 + h11 - h01;
				else
					h10 = orgin->terrain_geometry[buffer_location->offset + (x*x + x)/2 + (y+1)].g[0];

				double tx = dx - double(x);
				double ty = dy - double(y);

				double txty = tx * ty;

				*h =  h00 * (1.0 - ty - tx + txty)
					+ h01 * (tx - txty)
					+ h11 * txty
					+ h10 * (ty - txty) + 60.0;
			}
		}
	}
}

void planetprt::findneighbor(int relation)
{
	if (parent == NULL || parent->children == NULL)
		return;
	else if (relation == (sibling_number+2)%4)
	{
		neighbors[relation - 1] = &parent->children[2];
		neighbor_type[relation - 1] = relation;
	}
	else if (sibling_number == 2)
	{
		neighbors[relation - 1] = &parent->children[(relation+2)%4];
		neighbor_type[relation - 1] = relation;
	}
	else
	{
		if (parent->neighbors[relation - 1] == NULL || parent->neighbor_type[relation - 1] == 0)
			parent->findneighbor(relation);

		planetprt *parent_neighbor = parent->neighbors[relation - 1];
		int parent_neighbor_type = parent->neighbor_type[relation - 1];
		int new_sibling_number = sibling_number == 3 ? 2 : sibling_number;

		if (parent_neighbor == NULL)
			return;
		else if (parent_neighbor->children == NULL || parent_neighbor->update_merge)
		{
			neighbors[relation - 1] = parent_neighbor;
			neighbor_type[relation - 1] = 0;
		}
		else if (parent_neighbor_type == 1)
		{
			neighbors[relation - 1] = &parent_neighbor->children[(int)((relation - 1) == new_sibling_number)];
			neighbor_type[relation - 1] = 1;
		}
		else if (parent_neighbor_type == 2)
		{
			neighbors[relation - 1] = &parent_neighbor->children[(int)((relation - 1) == new_sibling_number)*2 + 1];
			neighbor_type[relation - 1] = 2;
		}
		else if (parent_neighbor_type == 3)
		{
			neighbors[relation - 1] = &parent_neighbor->children[(int)((relation - 1) != new_sibling_number)*3];
			neighbor_type[relation - 1] = 3;
		}
		else if (parent_neighbor_type == -1)
		{
			neighbors[relation - 1] = &parent_neighbor->children[(int)((relation - 1) != new_sibling_number)];
			neighbor_type[relation - 1] = -1;
		}
		else if (parent_neighbor_type == -2)
		{
			neighbors[relation - 1] = &parent_neighbor->children[(int)((relation - 1) != new_sibling_number)*2 + 1];
			neighbor_type[relation - 1] = -2;
		}
		else if (parent_neighbor_type == -3)
		{
			neighbors[relation - 1] = &parent_neighbor->children[(int)((relation - 1) == new_sibling_number)*3];
			neighbor_type[relation - 1] = -3;
		}
	}
}

void planetprt::update_neighbors()
{
	int x = 0;
	for (x = 0; x < 3; x++)
		findneighbor(x+1);

	if (children != NULL && (!update_merge || children[0].children != NULL || children[1].children != NULL || children[2].children != NULL || children[3].children != NULL))
		for (x = 0; x < 4; x++)
			children[x].update_neighbors();
}

void planetprt::prepare()
{
	if (((canvashdl*)orgin->super)->active_camera != NULL)
	{
		double angle = acos(dot(-orgin->camera_direction, ncenter));
		angle = m_max(m_abs(angle) - radius, 0.0);

		cull = false;
		if (angle > ((canvashdl*)orgin->super)->active_camera->frustum.farf)
			cull = true;

		if (!cull)
		{
			double size2 = a1*orgin->radius*a1*orgin->radius;
			vec to_cam = (((canvashdl*)orgin->super)->active_camera->position - orgin->position - center*(orgin->radius+max)/orgin->radius).tovec();
			double dist2 = dot(to_cam, to_cam);

			if (children != NULL && !update_merge)
			{
				interpolator = m_clamp((1.3 - dist2/size2)/.3, 0.0, 1.0);

				for (int x = 0; x < 4; x++)
					children[x].prepare();

				if (dist2/size2 > 1.3)
					orgin->merge(this);
			}
			else
			{
				if (dist2/size2 < 1.0 && size2/(verts_per_side*verts_per_side) > 10.0)
					orgin->split(this);

				interpolator = 1.0 - m_clamp((dist2/size2 - 1.3)/.3, 0.0, 1.0);

				if (parent != NULL)
					interpolator = interpolator*parent->interpolator;
			}
		}
	}
	else
		cull = false;
}

void planetprt::render(GLuint program)
{
	if (update_split && update_merge)
	{
		if (children != NULL)
			delete [] children;
		children = NULL;

		glBufferSubData(GL_ARRAY_BUFFER, buffer_location->offset*verts_per_part*sizeof(vertex_v3n3t3g3), verts_per_part*sizeof(vertex_v3n3t3g3), &orgin->terrain_geometry[buffer_location->offset*verts_per_part]);

		update_split = false;
		update_merge = false;
	}
	if (update_split)
	{
		if (children == NULL)
			glBufferSubData(GL_ARRAY_BUFFER, buffer_location->offset*verts_per_part*sizeof(vertex_v3n3t3g3), verts_per_part*sizeof(vertex_v3n3t3g3), &orgin->terrain_geometry[buffer_location->offset*verts_per_part]);
		else
			orgin->deallocate_part(this);

		update_split = false;
	}
	else if (update_merge)
	{
		if (children != NULL)
			delete [] children;
		children = NULL;

		glBufferSubData(GL_ARRAY_BUFFER, buffer_location->offset*verts_per_part*sizeof(vertex_v3n3t3g3), verts_per_part*sizeof(vertex_v3n3t3g3), &orgin->terrain_geometry[buffer_location->offset*verts_per_part]);

		update_merge = false;
	}

	if (cull)
		return;

	if (children != NULL)
		for (int x = 0; x < 4; x++)
			children[x].render(program);
	else
	{
		vec pos;
		if (((canvashdl*)orgin->super)->active_camera != NULL)
			pos = (orgin->position + center - ((canvashdl*)orgin->super)->active_camera->position).tovec();
		glUniform4f(glGetUniformLocation(program, "position"), pos.x, pos.y, pos.z, 0.0);
		glUniform3f(glGetUniformLocation(program, "ntangent"), ntangent.x, ntangent.y, ntangent.z);
		glUniform3f(glGetUniformLocation(program, "neighbors"), (float)(level - (neighbors[0] == NULL ? level : neighbors[0]->level)),
																(float)(level - (neighbors[1] == NULL ? level : neighbors[1]->level)),
																(float)(level - (neighbors[2] == NULL ? level : neighbors[2]->level)));
		glUniform1f(glGetUniformLocation(program, "interpolator"), interpolator);
		for (int x = 1, count = 0; x < verts_per_side && buffer_location; x++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, x*2 + 1, GL_UNSIGNED_INT, (GLuint*)((buffer_location->offset*indices_per_part+count)*sizeof(GLuint)));
			count += x*2 + 1;
		}
	}
}

void planetprt::release()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			children[x].release();
		delete [] children;
	}

	if (buffer_location != NULL)
		orgin->deallocate_part(this);

	a1 = 0.0;
	a2 = 0.0;
	a3 = 0.0;
	min = 0.0;
	max = 0.0;
	radius = 0.0;
	level = 0;
	cull = false;
	update_split = false;
	update_merge = false;
	orgin = NULL;
	parent = NULL;
	children = NULL;
	neighbors[0] = NULL;
	neighbors[1] = NULL;
	neighbors[2] = NULL;
	buffer_location = NULL;
}

planethdl::planethdl()
{
	first_leaf = NULL;
	last_leaf = NULL;
	first_unused = NULL;
	last_unused = NULL;

	memset(terrain_geometry, 0, sizeof(vertex_v3n3t3g3)*max_verts);

	terrain_geo = 0;
	terrain_ind = 0;

	terrain_vx = 0;
	terrain_ft = 0;
	terrain_pm = 0;

	num_parts = 0;

	height_multiplier = 1.0;
	noise_density = 1.0;

	atmosphere_radius = 0.0;
	atmosphere_size = 0;

	atmosphere_geo = 0;
	atmosphere_ind = 0;

	atmosphere_vx = 0;
	atmosphere_ft = 0;
	atmosphere_pm = 0;

	ocean_vx = 0;
	ocean_ft = 0;
	ocean_pm = 0;

	camera_distance = 0.0;

	seed = rand();
}

planethdl::planethdl(unsigned int s)
{
	first_leaf = NULL;
	last_leaf = NULL;
	first_unused = NULL;
	last_unused = NULL;

	memset(terrain_geometry, 0, sizeof(vertex_v3n3t3g3)*max_verts);

	terrain_geo = 0;
	terrain_ind = 0;

	terrain_vx = 0;
	terrain_ft = 0;
	terrain_pm = 0;

	num_parts = 0;

	height_multiplier = 1.0;
	noise_density = 1.0;

	atmosphere_radius = 0.0;
	atmosphere_size = 0;

	atmosphere_geo = 0;
	atmosphere_ind = 0;

	atmosphere_vx = 0;
	atmosphere_ft = 0;
	atmosphere_pm = 0;

	ocean_vx = 0;
	ocean_ft = 0;
	ocean_pm = 0;

	camera_distance = 0.0;

	seed = s;
}

planethdl::~planethdl()
{
	release();
}

void planethdl::initialize_derived()
{
	type = OBJ_PLANET;

	noise.initialize(seed);
	int r = 0;
	for (int x = 0; x < 6250; x++)
		r += rand();

	radius = double(r%(max_radius - min_radius)) + min_radius;
	mass = 5454.5*4*3.14159*radius*radius*radius/3;
	printf("Mass: %f\n", mass);

	height_multiplier = 3.0;
	noise_density = 100.0;

	// set up the unused parts and indices
	int c1 = 0, c2 = 0, index = 0;
	GLuint *indices = new GLuint[max_indices];

	first_unused = new bufferprt(0);
	last_unused = first_unused;
	c1 = last_unused->offset*verts_per_part + 1;
	c2 = last_unused->offset*verts_per_part;
	index = 0;
	for (int y = 0; y < verts_per_side - 1; y++)
	{
		for (int z = 0; z < y+1; z++)
		{
			indices[last_unused->offset*indices_per_part + index++] = c1++;
			indices[last_unused->offset*indices_per_part + index++] = c2++;
		}

		indices[last_unused->offset*indices_per_part + index++] = c1++;
	}

	for (int x = 0; x < max_parts-1; x++)
	{
		last_unused->next = new bufferprt(x + 1);
		last_unused->next->prev = last_unused;
		last_unused = last_unused->next;

		c1 = last_unused->offset*verts_per_part + 1;
		c2 = last_unused->offset*verts_per_part;
		index = 0;
		for (int y = 0; y < verts_per_side - 1; y++)
		{
			for (int z = 0; z < y+1; z++)
			{
				indices[last_unused->offset*indices_per_part + index++] = c1++;
				indices[last_unused->offset*indices_per_part + index++] = c2++;
			}

			indices[last_unused->offset*indices_per_part + index++] = c1++;
		}
	}

	// Load Terrain Geometry
	location v0(1.0*radius, 0.0, 0.0),
			 v1(0.447213595500*radius,  0.894427191000*radius, 0.0),
			 v2(0.447213595500*radius,  0.276393202252*radius, 0.850650808354*radius),
			 v3(0.447213595500*radius, -0.723606797748*radius, 0.525731112119*radius),
			 v4(0.447213595500*radius, -0.723606797748*radius, -0.525731112119*radius),
			 v5(0.447213595500*radius,  0.276393202252*radius, -0.850650808354*radius),
			 v6(-0.447213595500*radius, -0.894427191000*radius, 0.0),
			 v7(-0.447213595500*radius, -0.276393202252*radius, 0.850650808354*radius),
			 v8(-0.447213595500*radius,  0.723606797748*radius, 0.525731112119*radius),
			 v9(-0.447213595500*radius,  0.723606797748*radius, -0.525731112119*radius),
			 v10(-0.447213595500*radius, -0.276393202252*radius, -0.850650808354*radius),
			 v11(-1.0*radius, 0.0, 0.0);

	part_list[0].initialize(this,  v2,  v1,  v0,  &part_list[5],   3, &part_list[4],   3, &part_list[1],   2);
	part_list[1].initialize(this,  v3,  v2,  v0,  &part_list[6],   3, &part_list[0],   3, &part_list[2],   2);
	part_list[2].initialize(this,  v4,  v3,  v0,  &part_list[7],   3, &part_list[1],   3, &part_list[3],   2);
	part_list[3].initialize(this,  v5,  v4,  v0,  &part_list[8],   3, &part_list[2],   3, &part_list[4],   2);
	part_list[4].initialize(this,  v1,  v5,  v0,  &part_list[9],   3, &part_list[3],   3, &part_list[0],   2);
	part_list[5].initialize(this,  v2,  v8,  v1,  &part_list[11],  2, &part_list[10],  3, &part_list[0],   1);
	part_list[6].initialize(this,  v3,  v7,  v2,  &part_list[12],  2, &part_list[11],  3, &part_list[1],   1);
	part_list[7].initialize(this,  v4,  v6,  v3,  &part_list[13],  2, &part_list[12],  3, &part_list[2],   1);
	part_list[8].initialize(this,  v5,  v10, v4,  &part_list[14],  2, &part_list[13],  3, &part_list[3],   1);
	part_list[9].initialize(this,  v1,  v9,  v5,  &part_list[10],  2, &part_list[14],  3, &part_list[4],   1);
	part_list[10].initialize(this, v8,  v9,  v1,  &part_list[16],  1, &part_list[9],   1, &part_list[5],   2);
	part_list[11].initialize(this, v7,  v8,  v2,  &part_list[17],  1, &part_list[5],   1, &part_list[6],   2);
	part_list[12].initialize(this, v6,  v7,  v3,  &part_list[18],  1, &part_list[6],   1, &part_list[7],   2);
	part_list[13].initialize(this, v10, v6,  v4,  &part_list[19],  1, &part_list[7],   1, &part_list[8],   2);
	part_list[14].initialize(this, v9,  v10, v5,  &part_list[15],  1, &part_list[8],   1, &part_list[9],   2);
	part_list[15].initialize(this, v10, v9,  v11, &part_list[14],  1, &part_list[16],  3, &part_list[19],  2);
	part_list[16].initialize(this, v9,  v8,  v11, &part_list[10],  1, &part_list[17],  3, &part_list[15],  2);
	part_list[17].initialize(this, v8,  v7,  v11, &part_list[11],  1, &part_list[18],  3, &part_list[16],  2);
	part_list[18].initialize(this, v7,  v6,  v11, &part_list[12],  1, &part_list[19],  3, &part_list[17],  2);
	part_list[19].initialize(this, v6,  v10, v11, &part_list[13],  1, &part_list[15],  3, &part_list[18],  2);

	glGenBuffers(1, &terrain_geo);
	glBindBuffer(GL_ARRAY_BUFFER, terrain_geo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_v3n3t3g3)*max_verts, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_v3n3t3g3)*max_verts, terrain_geometry);

	glGenBuffers(1, &terrain_ind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_ind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*max_indices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*max_indices, indices);

	delete [] indices;

	// Load Terrain Shaders
	material = new materialhdl();
	int TextLen;
	char ErrText[1024];

	TextLen = 1024;
	terrain_vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/terrain.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	terrain_ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/terrain.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	terrain_pm = glCreateProgram();
	glAttachShader(terrain_pm, terrain_vx);
	glAttachShader(terrain_pm, terrain_ft);
	glLinkProgram(terrain_pm);

	// Load Terrain Textures
	int width, height;
	unsigned char *sand = rgb_tga((char*)"res/sand.tga", &width, &height);
	unsigned char *dirt = rgb_tga((char*)"res/dirt.tga", &width, &height);
	unsigned char *grass = rgb_tga((char*)"res/grass.tga", &width, &height);
	unsigned char *rock = rgb_tga((char*)"res/rock.tga", &width, &height);
	unsigned char *snow = rgb_tga((char*)"res/snow.tga", &width, &height);
	GLubyte *terrain_txdt = new GLubyte[tex_size*tex_size*num_textures*4];
	for (int x = 0; x < tex_size; x++)
	{
		for (int y = 0; y < tex_size; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				terrain_txdt[(tex_size*tex_size*0 + x*tex_size + y)*3 + z] = sand[(x*tex_size + y)*3 + z];
				terrain_txdt[(tex_size*tex_size*1 + x*tex_size + y)*3 + z] = dirt[(x*tex_size + y)*3 + z];
				terrain_txdt[(tex_size*tex_size*2 + x*tex_size + y)*3 + z] = grass[(x*tex_size + y)*3 + z];
				terrain_txdt[(tex_size*tex_size*3 + x*tex_size + y)*3 + z] = rock[(x*tex_size + y)*3 + z];
				terrain_txdt[(tex_size*tex_size*4 + x*tex_size + y)*3 + z] = snow[(x*tex_size + y)*3 + z];
			}
		}
	}

	delete [] snow;
	delete [] rock;
	delete [] dirt;
	delete [] grass;
	delete [] sand;

	terrain_tx.initialize(terrain_txdt, tex_size, num_textures, true, GL_REPEAT, GL_RGB);

	delete [] terrain_txdt;

	// Load Atmosphere Geometry
	atmosphere_radius = radius*1.025;
	atmosphere_size = 64;
	vertex_v3t1 *atmosphere_geometry = new vertex_v3t1[atmosphere_size*atmosphere_size];

	GLuint *atmosphere_indices = new GLuint[atmosphere_size*atmosphere_size*6];

	float x_temp, y_temp;
	float a = acos(radius/atmosphere_radius);
	vec v;

	for (int x = 0; x < atmosphere_size; x++)
	{
		x_temp = 2.0*m_pi*float(atmosphere_size-1 - x)/float(atmosphere_size-1);
		for (int y = 0; y < atmosphere_size; y++)
		{
			y_temp = (a + 0.5*m_pi)*float(y)/float(atmosphere_size-1) - a;

			v.x = cos(x_temp) * cos(y_temp);
			v.y = sin(y_temp);
			v.z = sin(x_temp) * cos(y_temp);

			atmosphere_geometry[x*atmosphere_size + y].v[0] = v.x*radius;
			atmosphere_geometry[x*atmosphere_size + y].v[1] = v.y*radius;
			atmosphere_geometry[x*atmosphere_size + y].v[2] = v.z*radius;

			atmosphere_geometry[x*atmosphere_size + y].t = float(y)/float(atmosphere_size-1);
		}
	}

	index = 0;
	for (int x = 1; x < atmosphere_size; x++)
	{
		for (int y = 0; y < atmosphere_size; y++)
		{
			atmosphere_indices[index++] = x*atmosphere_size+y;
			atmosphere_indices[index++] = (x-1)*atmosphere_size+y;
		}
	}

	glGenBuffers(1, &atmosphere_geo);
	glBindBuffer(GL_ARRAY_BUFFER, atmosphere_geo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_v3t1)*atmosphere_size*atmosphere_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_v3t1)*atmosphere_size*atmosphere_size, atmosphere_geometry);

	glGenBuffers(1, &atmosphere_ind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, atmosphere_ind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*index, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*index, atmosphere_indices);

	delete [] atmosphere_geometry;
	delete [] atmosphere_indices;

	// Load Atmosphere Shaders
	TextLen = 1024;
	atmosphere_vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/atmosphere.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	atmosphere_ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/atmosphere.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	atmosphere_pm = glCreateProgram();
	glAttachShader(atmosphere_pm, atmosphere_vx);
	glAttachShader(atmosphere_pm, atmosphere_ft);
	glLinkProgram(atmosphere_pm);

	// Load Ocean Shaders
	TextLen = 1024;
	ocean_vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/ocean.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	ocean_ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/ocean.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	ocean_pm = glCreateProgram();
	glAttachShader(ocean_pm, ocean_vx);
	glAttachShader(ocean_pm, ocean_ft);
	glLinkProgram(ocean_pm);

	// Load Atmosphere Texture
	int atmosphere_texture_size = 64;
	rgba_ldr *atmosphere_texture = new rgba_ldr[atmosphere_texture_size*atmosphere_texture_size*atmosphere_texture_size];
	double phi, theta, depth;
	vec Rvp, Pv;
	for (int x = 0; x < atmosphere_texture_size; x++)
	{
		theta = ((double)x/(double)(atmosphere_texture_size-1))*m_pi;
		Rvp = vec(sin(theta), cos(theta), 0.0);
		for (int y = 0; y < atmosphere_texture_size; y++)
		{
			phi = ((double)y/(double)(atmosphere_texture_size-1))*m_pi;
			for (int z = 0; z < atmosphere_texture_size; z++)
			{
				depth = ((double)z/(double)(atmosphere_texture_size-1))*(atmosphere_radius - radius) + radius;
				Pv = vec(0.0, height, 0.0);

				atmosphere_texture[x*atmosphere_texture_size*atmosphere_texture_size + y*atmosphere_texture_size + z] = scatter(theta, depth, phi, rgb_hdr(1.0, 1.0, 1.0));
			}
		}
	}

	atmosphere_tx.initialize((unsigned char*)atmosphere_texture, atmosphere_texture_size, atmosphere_texture_size, false, GL_CLAMP_TO_EDGE, GL_RGBA);

	delete [] atmosphere_texture;
}

void planethdl::release_derived()
{
	// Release Terrain Geometry
	for (int x = 0; x < 20; x++)
		part_list[x].release();

	if (terrain_geo != 0)
		glDeleteBuffers(1, &terrain_geo);
	if (terrain_ind != 0)
		glDeleteBuffers(1, &terrain_ind);

	// Release Terrain Shaders
	if (terrain_vx != 0)
		glDeleteShader(terrain_vx);
	if (terrain_ft != 0)
		glDeleteShader(terrain_ft);
	if (terrain_pm != 0)
		glDeleteShader(terrain_pm);

	// Release Terrain Textures
	terrain_tx.release();

	// Release Atmosphere Geometry
	if (atmosphere_geo != 0)
		glDeleteBuffers(1, &terrain_geo);
	if (atmosphere_ind != 0)
		glDeleteBuffers(1, &terrain_ind);

	// Release Atmosphere Shaders
	if (atmosphere_vx != 0)
		glDeleteShader(atmosphere_vx);
	if (atmosphere_ft != 0)
		glDeleteShader(atmosphere_ft);
	if (atmosphere_pm != 0)
		glDeleteShader(atmosphere_pm);

	// Release Atmosphere Textures
	atmosphere_tx.release();

	// Release the buffer management system
	bufferprt *currbufferprt = first_unused, *prevbufferprt = NULL;
	while (currbufferprt != NULL)
	{
		prevbufferprt = currbufferprt;
		currbufferprt = currbufferprt->next;
		delete prevbufferprt;
	}
	first_unused = NULL;
	last_unused = NULL;

	currbufferprt = first_leaf;
	prevbufferprt = NULL;
	while (currbufferprt != NULL)
	{
		prevbufferprt = currbufferprt;
		currbufferprt = currbufferprt->next;
		delete prevbufferprt;
	}
	first_leaf = NULL;
	last_leaf = NULL;

	// Reset everything to zero
	memset(terrain_geometry, 0, sizeof(vertex_v3n3t3g3)*max_verts);

	terrain_geo = 0;
	terrain_ind = 0;

	terrain_vx = 0;
	terrain_ft = 0;
	terrain_pm = 0;

	num_parts = 0;

	height_multiplier = 1.0;
	noise_density = 1.0;

	atmosphere_radius = 0.0;
	atmosphere_size = 0;

	atmosphere_geo = 0;
	atmosphere_ind = 0;

	atmosphere_vx = 0;
	atmosphere_ft = 0;
	atmosphere_pm = 0;
}

void planethdl::prepare_derived()
{
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
	{
		location dir = position - ((canvashdl*)super)->active_camera->position;
		camera_direction = dir.tovec();
		camera_distance = mag(camera_direction);
		camera_direction = camera_direction/camera_distance;
		dir = dir / camera_distance;

		double ground_height;
		for (int i = 0; i < 20; i++)
			part_list[i].getheight(dir, &ground_height);

	/*	if (camera_distance - radius < ground_height)
		{
			cout << camera_distance << " " << radius << " " << ground_height << endl;
			((canvashdl*)super)->active_camera->position -= dir*(ground_height + radius - camera_distance);
		}*/


		((canvashdl*)super)->active_camera->frustum.farf = acos(radius/camera_distance);
		if (m_nan(((canvashdl*)super)->active_camera->frustum.farf) || ((canvashdl*)super)->active_camera->frustum.farf <= 0.1)
			((canvashdl*)super)->active_camera->frustum.farf = 0.1;
	}
	for (int x = 0; x < 20; x++)
		part_list[x].prepare();
}

void planethdl::render_atmosphere()
{
	// Render Atmosphere
	vec atm_pos, atm_ori;
	vec planet_direction;
	vec star_direction;
	if (parent != NULL && parent->type == OBJ_SYSTEM && ((systemhdl*)parent)->sun != NULL)
		star_direction = norm((((systemhdl*)parent)->sun->position - position).tovec());
	float planet_distance = 0.0;

	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
	{
		planet_direction = (position - ((canvashdl*)super)->active_camera->position).tovec();
		atm_pos = planet_direction;
		planet_distance = mag(planet_direction);
		planet_direction /= planet_distance;

		double y_angle, x_angle;
		y_angle = asin(planet_direction.y) + m_pi/2.0;
		if ((planet_direction.x/cos(y_angle - m_pi/2.0) > 1.0 || planet_direction.x/cos(y_angle - m_pi/2.0) < -1.0) && planet_direction.z != 0.0 && planet_direction.x != 0.0)
			x_angle = atan(planet_direction.z/planet_direction.x);
		else if (cos(y_angle - m_pi/2.0) != 0.0)
			x_angle = acos(planet_direction.x/cos(y_angle - m_pi/2.0));
		else
			x_angle = 0.0;

		if (planet_direction.z < 0)
			x_angle = 2.0*m_pi - x_angle;

		atm_ori = (180.0/m_pi)*vec(0.0, 2.0*m_pi - x_angle, m_pi - acos(dot(vec(0.0, 1.0, 0.0), planet_direction)));
	}

	glUseProgram(atmosphere_pm);

	glUniform3f(glGetUniformLocation(atmosphere_pm, "camera"), -atm_pos.x, -atm_pos.y, -atm_pos.z);
	glUniform3f(glGetUniformLocation(atmosphere_pm, "light"), star_direction.x, star_direction.y, star_direction.z);
	glUniform3f(glGetUniformLocation(atmosphere_pm, "invwavelength"), 1.0/pow(0.650f, 4), 1.0/pow(0.570f, 4), 1.0/pow(0.475f, 4));
	glUniform1f(glGetUniformLocation(atmosphere_pm, "height"), planet_distance);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "height2"), planet_distance*planet_distance);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "inner_radius"), radius);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "inner_radius2"), radius*radius);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "outer_radius"), atmosphere_radius);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "outer_radius2"), atmosphere_radius*atmosphere_radius);

	glUniform1f(glGetUniformLocation(atmosphere_pm, "KrESun"), 0.0025*10.0);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "KmESun"), 0.0015*10.0);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "Kr4Pi"), 0.0025*m_pi*4.0);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "Km4Pi"), 0.0015*m_pi*4.0);

	glUniform1f(glGetUniformLocation(atmosphere_pm, "scale"), 1.0/(atmosphere_radius - radius));
	glUniform1f(glGetUniformLocation(atmosphere_pm, "scaledepth"), 0.25);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "scale_scaledepth"), 4.0/(atmosphere_radius - radius));

	glUniform1f(glGetUniformLocation(atmosphere_pm, "g"), -0.99);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "g2"), 0.99*0.99);

	glUniform1i(glGetUniformLocation(atmosphere_pm, "samples"), 5);

	double C = 0.01;
	glUniform1f(glGetUniformLocation(atmosphere_pm, "C"), C);
	glUniform1f(glGetUniformLocation(atmosphere_pm, "FC"), 1.0/log(10000000000.0*C + 1.0));

	GLfloat x = cos((m_pi/180.0)*atm_ori.x);
	GLfloat y = cos((m_pi/180.0)*atm_ori.y);
	GLfloat z = cos((m_pi/180.0)*atm_ori.z);
	GLfloat a = sin((m_pi/180.0)*atm_ori.x);
	GLfloat b = sin((m_pi/180.0)*atm_ori.y);
	GLfloat c = sin((m_pi/180.0)*atm_ori.z);

	GLfloat matrix[16] = {y*z, -c*y, b, 0, c*x + a*b*z, -a*b*c + x*z, -a*y, 0, a*c - b*x*z, b*c*x + a*z, x*y, 0, 0, 0, 0, 1};

	glUniformMatrix4fv(glGetUniformLocation(atmosphere_pm, "matrix"), 1, GL_TRUE, matrix);

	glPushMatrix();
	glTranslatef(atm_pos.x, atm_pos.y, atm_pos.z);
	glRotated(atm_ori.x, 1.0, 0.0, 0.0);
    glRotated(atm_ori.y, 0.0, 1.0, 0.0);
    glRotated(atm_ori.z, 0.0, 0.0, 1.0);

	glBindBuffer(GL_ARRAY_BUFFER, atmosphere_geo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, atmosphere_ind);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 16, (GLvoid*)0);
	glTexCoordPointer(1, GL_FLOAT, 16, (GLvoid*)12);
	for (int x = 0; x < atmosphere_size-1; x++)
		glDrawElements(GL_TRIANGLE_STRIP, atmosphere_size*2, GL_UNSIGNED_INT, (GLuint*)(x*atmosphere_size*2*sizeof(GLuint)));
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void planethdl::render_ocean()
{
	vec star_direction;
	if (parent != NULL && parent->type == OBJ_SYSTEM && ((systemhdl*)parent)->sun != NULL)
		star_direction = norm((((systemhdl*)parent)->sun->position - position).tovec());
	vec planet_direction;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		planet_direction = (position - ((canvashdl*)super)->active_camera->position).tovec();
	float planet_distance = mag(planet_direction);

	// Render Terrain
	glEnable(GL_TEXTURE_2D_ARRAY);

	glUseProgram(ocean_pm);

	glUniform3f(glGetUniformLocation(ocean_pm, "camera"), -planet_direction.x, -planet_direction.y, -planet_direction.z);
	glUniform3f(glGetUniformLocation(ocean_pm, "light"), star_direction.x, star_direction.y, star_direction.z);
	glUniform3f(glGetUniformLocation(ocean_pm, "invwavelength"), 1.0/pow(0.650f, 4), 1.0/pow(0.570f, 4), 1.0/pow(0.475f, 4));
	glUniform1f(glGetUniformLocation(ocean_pm, "height"), planet_distance);
	glUniform1f(glGetUniformLocation(ocean_pm, "height2"), planet_distance*planet_distance);
	glUniform1f(glGetUniformLocation(ocean_pm, "inner_radius"), radius);
	glUniform1f(glGetUniformLocation(ocean_pm, "inner_radius2"), radius*radius);
	glUniform1f(glGetUniformLocation(ocean_pm, "outer_radius"), atmosphere_radius);
	glUniform1f(glGetUniformLocation(ocean_pm, "outer_radius2"), atmosphere_radius*atmosphere_radius);

	glUniform1f(glGetUniformLocation(ocean_pm, "KrESun"), 0.0025*20.0);
	glUniform1f(glGetUniformLocation(ocean_pm, "KmESun"), 0.001*20.0);
	glUniform1f(glGetUniformLocation(ocean_pm, "Kr4Pi"), 0.0025*m_pi*4.0);
	glUniform1f(glGetUniformLocation(ocean_pm, "Km4Pi"), 0.001*m_pi*4.0);

	glUniform1f(glGetUniformLocation(ocean_pm, "scale"), 1.0/(atmosphere_radius - radius));
	glUniform1f(glGetUniformLocation(ocean_pm, "scaledepth"), 0.25);
	glUniform1f(glGetUniformLocation(ocean_pm, "scale_scaledepth"), 4.0/(atmosphere_radius - radius));

	glUniform1f(glGetUniformLocation(ocean_pm, "g"), -0.99);
	glUniform1f(glGetUniformLocation(ocean_pm, "g2"), 0.99*0.99);

	glUniform1i(glGetUniformLocation(ocean_pm, "samples"), 5);

	double C = 0.01;
	glUniform1f(glGetUniformLocation(ocean_pm, "C"), C);
	glUniform1f(glGetUniformLocation(ocean_pm, "FC"), 1.0/log(10000000000.0*C + 1.0));

	glBindBuffer(GL_ARRAY_BUFFER, terrain_geo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_ind);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(vertex_v3n3t3g3), (float*)24);
	glNormalPointer(GL_FLOAT, sizeof(vertex_v3n3t3g3), (float*)12);
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_v3n3t3g3), NULL);

	for (int x = 0; x < 20; x++)
		part_list[x].render(ocean_pm);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void planethdl::render_terrain()
{
	vec star_direction;
	if (parent != NULL && parent->type == OBJ_SYSTEM && ((systemhdl*)parent)->sun != NULL)
		star_direction = norm((((systemhdl*)parent)->sun->position - position).tovec());
	vec planet_direction;
	if (super != NULL && ((canvashdl*)super)->active_camera != NULL)
		planet_direction = (position - ((canvashdl*)super)->active_camera->position).tovec();
	float planet_distance = mag(planet_direction);


	// Render Terrain
	glEnable(GL_TEXTURE_2D_ARRAY);

	glUseProgram(terrain_pm);

	glUniform3f(glGetUniformLocation(terrain_pm, "camera"), -planet_direction.x, -planet_direction.y, -planet_direction.z);
	glUniform3f(glGetUniformLocation(terrain_pm, "light"), star_direction.x, star_direction.y, star_direction.z);
	glUniform3f(glGetUniformLocation(terrain_pm, "invwavelength"), 1.0/pow(0.650f, 4), 1.0/pow(0.570f, 4), 1.0/pow(0.475f, 4));
	glUniform1f(glGetUniformLocation(terrain_pm, "height"), planet_distance);
	glUniform1f(glGetUniformLocation(terrain_pm, "height2"), planet_distance*planet_distance);
	glUniform1f(glGetUniformLocation(terrain_pm, "inner_radius"), radius);
	glUniform1f(glGetUniformLocation(terrain_pm, "inner_radius2"), radius*radius);
	glUniform1f(glGetUniformLocation(terrain_pm, "outer_radius"), atmosphere_radius);
	glUniform1f(glGetUniformLocation(terrain_pm, "outer_radius2"), atmosphere_radius*atmosphere_radius);

	glUniform1f(glGetUniformLocation(terrain_pm, "KrESun"), 0.0025*20.0);
	glUniform1f(glGetUniformLocation(terrain_pm, "KmESun"), 0.001*20.0);
	glUniform1f(glGetUniformLocation(terrain_pm, "Kr4Pi"), 0.0025*m_pi*4.0);
	glUniform1f(glGetUniformLocation(terrain_pm, "Km4Pi"), 0.001*m_pi*4.0);

	glUniform1f(glGetUniformLocation(terrain_pm, "scale"), 1.0/(atmosphere_radius - radius));
	glUniform1f(glGetUniformLocation(terrain_pm, "scaledepth"), 0.25);
	glUniform1f(glGetUniformLocation(terrain_pm, "scale_scaledepth"), 4.0/(atmosphere_radius - radius));

	glUniform1f(glGetUniformLocation(terrain_pm, "g"), -0.99);
	glUniform1f(glGetUniformLocation(terrain_pm, "g2"), 0.99*0.99);

	glUniform1i(glGetUniformLocation(terrain_pm, "samples"), 5);

	double C = 0.01;
	glUniform1f(glGetUniformLocation(terrain_pm, "C"), C);
	glUniform1f(glGetUniformLocation(terrain_pm, "FC"), 1.0/log(10000000000.0*C + 1.0));

	glUniform4fv(glGetUniformLocation(terrain_pm, "pg3"), noise_size, noise.data);

	GLint gl_Height = glGetAttribLocation(terrain_pm, "Height");
	GLint gl_HNormal = glGetAttribLocation(terrain_pm, "HNormal");

	glActiveTexture(GL_TEXTURE0);
	terrain_tx.render();
	glUniform1i(glGetUniformLocation(terrain_pm, "tex_0"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, terrain_geo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_ind);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(gl_Height);
	glEnableVertexAttribArray(gl_HNormal);
	glTexCoordPointer(3, GL_FLOAT, sizeof(vertex_v3n3t3g3), (float*)24);
	glNormalPointer(GL_FLOAT, sizeof(vertex_v3n3t3g3), (float*)12);
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_v3n3t3g3), NULL);
	glVertexAttribPointer(gl_Height, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_v3n3t3g3), (float*)36);
	glVertexAttribPointer(gl_HNormal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_v3n3t3g3), (float*)44);

	for (int x = 0; x < 20; x++)
		part_list[x].render(terrain_pm);

	glDisableVertexAttribArray(gl_HNormal);
	glDisableVertexAttribArray(gl_Height);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void planethdl::render_derived()
{
	glDisable(GL_DEPTH_TEST);
	render_atmosphere();
	glEnable(GL_DEPTH_TEST);

	render_terrain();

	glDisable(GL_CULL_FACE);
	render_ocean();
	glEnable(GL_CULL_FACE);
}

void planethdl::allocate_part(planetprt *part)
{
	if (part == NULL || first_unused == NULL)
		return;

	bufferprt *result = first_unused;

	// remove the node from the unused list
	first_unused = first_unused->next;

	if (first_unused != NULL)
		first_unused->prev = NULL;

	// add the node to the leaf list
	result->next = NULL;
	result->part = part;

	if (first_leaf == NULL)
	{
		result->prev = NULL;
		first_leaf = result;
		last_leaf = result;
	}
	else
	{
		result->prev = last_leaf;
		last_leaf->next = result;
		last_leaf = last_leaf->next;
	}

	num_parts++;

	part->buffer_location = result;
}

void planethdl::deallocate_part(planetprt *part)
{
	if (part == NULL || part->buffer_location == NULL || first_leaf == NULL)
		return;

	bufferprt *result = part->buffer_location;
	part->buffer_location = NULL;
	result->part = NULL;

	if (result->next)
		result->next->prev = result->prev;
	else
		last_leaf = last_leaf->prev;

	if (result->prev)
		result->prev->next = result->next;
	else
		first_leaf = first_leaf->next;

	result->next = NULL;

	if (first_unused == NULL)
	{
		first_unused = result;
		result->prev = NULL;
		last_unused = result;
	}
	else
	{
		last_unused->next = result;
		result->prev = last_unused;
		last_unused = result;
	}

	num_parts--;
}

void planethdl::split(planetprt *part)
{
	if (part == NULL || num_parts+4 > max_parts || part->update_split || part->update_merge)
		return;

	if (part->children != NULL)
		for (int x = 0; x < 4; x++)
			split(&part->children[x]);
	else
	{
		planetprt *result = new planetprt[4];

		location v12, v13, v23;
		vec t1, t2, t3;

		// vertex generation
		int x = 0, y = 0, z = 0, count = 0;
		double x_vps_ratio = 0.0, y_x_ratio = 0.0, angle = 0.0;
		double sin_a1 = 0.0, sin_a3 = 0.0, sin_angle = 0.0;
		float magnitude = 0.0;
		location v1_v2, v1_v3;
		location sphere;
		vec normal;
		vec lnormal;
		vec vertex;
		int offset;

		// height and normal generation (noise)
		noisevec height;
		vec sn1, sn2, sn3;

		if (part->a1 < 0.001)
		{
			v12 = (part->v1 + part->v2)/2.0;
			v13 = (part->v1 + part->v3)/2.0;
			v23 = (part->v2 + part->v3)/2.0;
		}
		else
		{
			double s1 = sin(part->a1/2.0)/sin(part->a1);
			double s2 = sin(part->a2/2.0)/sin(part->a2);
			double s3 = sin(part->a3/2.0)/sin(part->a3);
			v12 = part->v1*s1 + part->v2*s1;
			v13 = part->v1*s3 + part->v3*s3;
			v23 = part->v2*s2 + part->v3*s2;
		}

		result[0].v1 = part->v1;
		result[0].v2 = v12;
		result[0].v3 = v13;

		result[1].v1 = v12;
		result[1].v2 = part->v2;
		result[1].v3 = v23;

		result[2].v1 = v23;
		result[2].v2 = v13;
		result[2].v3 = v12;

		result[3].v1 = v13;
		result[3].v2 = v23;
		result[3].v3 = part->v3;

		for (int w = 0; w < 4; w++)
		{
			result[w].level = part->level+1;

			allocate_part(&result[w]);

			if (result[w].buffer_location == NULL)
			{
				delete [] result;
				return;
			}

			offset = result[w].buffer_location->offset*verts_per_part;

			result[w].orgin = this;
			result[w].update_split = true;
			result[w].parent = part;
			result[w].sibling_number = w;

			result[w].center = (result[w].v1 + result[w].v2 + result[w].v3)/3.0;

			result[w].nv1 = (result[w].v1/radius).tovec(),
			result[w].nv2 = (result[w].v2/radius).tovec(),
			result[w].nv3 = (result[w].v3/radius).tovec();
			result[w].ncenter = norm((result[w].center/radius).tovec());

			result[w].ntangent = norm(result[w].nv2 - result[w].nv1);

			result[w].radius = acos(dot(result[w].nv1, result[w].ncenter));

			result[w].a1 = acos(dot(result[w].nv1, result[w].nv2));
			result[w].a2 = acos(dot(result[w].nv2, result[w].nv3));
			result[w].a3 = acos(dot(result[w].nv3, result[w].nv1));

			result[w].min = 99999999999999.0;
			result[w].max = -99999999999999.0;

			result[w].polarity = part->polarity;
			if (w == 2)
				result[w].polarity *= -1;

			// vertex generation
			x = 0;
			y = 0;
			count = 0;
			x_vps_ratio = 0.0;
			y_x_ratio = 0.0;
			angle = 0.0;
			sin_a1 = sin(result[w].a1);
			sin_a3 = sin(result[w].a3);
			sin_angle = 0.0;

			// at some point, just use linear interpolation instead of spherical for optimization and accuracy purposes
			if (result[w].a1 < 0.001)
			{
				vec edge1 = (result[w].v2 - result[w].v1).tovec();
				vec edge2 = (result[w].v3 - result[w].v2).tovec();
				vec edge3 = (result[w].v1 - result[w].v3).tovec();

				result[w].a1 = sqrt(dot(edge1, edge1))/radius;
				result[w].a2 = sqrt(dot(edge2, edge2))/radius;
				result[w].a3 = sqrt(dot(edge3, edge3))/radius;

				for (x = 0; x < verts_per_side; x++)
				{
					x_vps_ratio = double(x)/double(verts_per_side-1);
					for (y = 0; y < x+1; y++)
					{
						y_x_ratio = x != 0 ? double(y)/double(x) : 0.0;
						count = (x*x + x)/2 + y;
						sphere = (result[w].v1*(1.0 - x_vps_ratio) + result[w].v2*(x_vps_ratio*(1.0 - y_x_ratio)) + result[w].v3*(x_vps_ratio*y_x_ratio));

						normal = sphere.tovec();
						magnitude = magnorm(&normal);

						vertex = (sphere*(radius/magnitude) - result[w].center).tovec();

						for (z = 0; z < 3; z++)
						{
							terrain_geometry[offset + count].v[z] = vertex[z];
							terrain_geometry[offset + count].n[z] = normal[z];
						}

						// TODO fix height generation
						height = terrain(sphere);
						terrain_geometry[offset + count].g[0] = height.h;
						terrain_geometry[offset + count].g[1] = terrain_geometry[offset + count].g[0];

						lnormal = height.normal(normal, result[w].ntangent);

						terrain_geometry[offset + count].dn[0] = lnormal.x;
						terrain_geometry[offset + count].dn[1] = lnormal.y;
						terrain_geometry[offset + count].dn[2] = lnormal.z;

						result[w].max = result[w].max < terrain_geometry[offset + count].g[0] ? terrain_geometry[offset + count].g[0] : result[w].max;
						result[w].min = result[w].min > terrain_geometry[offset + count].g[0] ? terrain_geometry[offset + count].g[0] : result[w].min;

						// TODO fix geomorphing
						if (x%2 == 0 && y%2 == 0)
						{
							if ((y-1)%2 == 1 && (x-1)%2 == 1)
								terrain_geometry[offset + ((x-1)*(x-1) + (x-1))/2 + (y-1)].g[1] = (terrain_geometry[offset + ((x-2)*(x-2) + (x-2))/2 + (y-2)].g[0] +
																								   terrain_geometry[offset + count].g[0])/2.0;
							if ((x-1)%2 == 1 && y < x)
								terrain_geometry[offset + ((x-1)*(x-1) + (x-1))/2 + y].g[1] = (terrain_geometry[offset + ((x-2)*(x-2) + (x-2))/2 + y].g[0] +
																							   terrain_geometry[offset + count].g[0])/2.0;
							if ((y-1)%2 == 1)
								terrain_geometry[offset + count-1].g[1] = (terrain_geometry[offset + count-2].g[0] +
																		   terrain_geometry[offset + count].g[0])/2.0;
						}

						terrain_geometry[offset + count].t[0] = (result[w].polarity == -1.0 ? 1.0 : 0.0) + result[w].polarity*result[w].a1*radius*float(x)/float(verts_per_side-1);
						terrain_geometry[offset + count].t[1] = (result[w].polarity == -1.0 ? 1.0 : 0.0) + result[w].polarity*result[w].a1*radius*float(y)/float(verts_per_side-1);
						terrain_geometry[offset + count].t[2] = (y == 0 ? 1.0 : (x == (verts_per_side - 1) ? 2.0 : (x == y ? 3.0 : 0.0)));
					}
				}
			}
			// spherical interpolation
			else
			{
				for (x = 0; x < verts_per_side; x++)
				{
					x_vps_ratio = double(x)/double(verts_per_side-1);
					v1_v2 = result[w].v1*(sin((1.0-x_vps_ratio)*result[w].a1)/sin_a1) + result[w].v2*(sin(x_vps_ratio*result[w].a1)/sin_a1);
					v1_v3 = result[w].v1*(sin((1.0-x_vps_ratio)*result[w].a3)/sin_a3) + result[w].v3*(sin(x_vps_ratio*result[w].a3)/sin_a3);
					angle = acos(dot(v1_v2.tovec()/radius, v1_v3.tovec()/radius));
					sin_angle = sin(angle);

					for (y = 0; y < x+1; y++)
					{
						y_x_ratio = x != 0 ? double(y)/double(x) : 0.0;
						count = (x*x + x)/2 + y;
						sphere = x_vps_ratio == 0.0 ? result[w].v1 : (v1_v2*(sin((1-y_x_ratio)*angle)/sin_angle) + v1_v3*(sin(y_x_ratio*angle)/sin_angle));

						normal = sphere.tovec();
						magnitude = magnorm(&normal);

						vertex = (sphere*(radius/magnitude) - result[w].center).tovec();

						for (z = 0; z < 3; z++)
						{
							terrain_geometry[offset + count].v[z] = vertex[z];
							terrain_geometry[offset + count].n[z] = normal[z];
						}

						// TODO fix height generation
						height = terrain(sphere);
						terrain_geometry[offset + count].g[0] = height.h;
						terrain_geometry[offset + count].g[1] = terrain_geometry[offset + count].g[0];

						lnormal = height.normal(normal, result[w].ntangent);

						terrain_geometry[offset + count].dn[0] = lnormal.x;
						terrain_geometry[offset + count].dn[1] = lnormal.y;
						terrain_geometry[offset + count].dn[2] = lnormal.z;

						result[w].max = result[w].max < terrain_geometry[offset + count].g[0] ? terrain_geometry[offset + count].g[0] : result[w].max;
						result[w].min = result[w].min > terrain_geometry[offset + count].g[0] ? terrain_geometry[offset + count].g[0] : result[w].min;

						// TODO spherical linear interpolation here instead of linear interpolation
						// TODO fix geomorphing
						if (x%2 == 0 && y%2 == 0)
						{
							if ((y-1)%2 == 1 && (x-1)%2 == 1)
								terrain_geometry[offset + ((x-1)*(x-1) + (x-1))/2 + (y-1)].g[1] = (terrain_geometry[offset + ((x-2)*(x-2) + (x-2))/2 + (y-2)].g[0] +
																								   terrain_geometry[offset + count].g[0])/2.0;
							if ((x-1)%2 == 1 && y < x)
								terrain_geometry[offset + ((x-1)*(x-1) + (x-1))/2 + y].g[1] = (terrain_geometry[offset + ((x-2)*(x-2) + (x-2))/2 + y].g[0] +
																							   terrain_geometry[offset + count].g[0])/2.0;
							if ((y-1)%2 == 1)
								terrain_geometry[offset + count-1].g[1] = (terrain_geometry[offset + count-2].g[0] +
																		   terrain_geometry[offset + count].g[0])/2.0;
						}

						terrain_geometry[offset + count].t[0] = (result[w].polarity == -1.0 ? 1.0 : 0.0) + result[w].polarity*result[w].a1*radius*float(x)/float(verts_per_side-1);
						terrain_geometry[offset + count].t[1] = (result[w].polarity == -1.0 ? 1.0 : 0.0) + result[w].polarity*result[w].a1*radius*float(y)/float(verts_per_side-1);
						terrain_geometry[offset + count].t[2] = (y == 0 ? 1.0 : (x == (verts_per_side - 1) ? 2.0 : (x == y ? 3.0 : 0.0)));
					}
				}
			}
		}

		part->children = result;
		for (x = 0; x < 4; x++)
			for (y = 0; y < 3; y++)
				part->children[x].findneighbor(y+1);
		for (x = 0; x < 3; x++)
			part->neighbors[x]->update_neighbors();

		part->update_split = true;
	}
}

void planethdl::merge(planetprt *part)
{
	if (part == NULL || part->children == NULL || part->update_merge || part->update_split)
		return;

	bool done = false;
	for (int x = 0; x < 4; x++)
		if (part->children[x].children != NULL)
		{
			merge(&part->children[x]);
			done = true;
		}

	if (!done)
	{
		allocate_part(part);

		if (part->buffer_location == NULL)
			return;

		int offset = part->buffer_location->offset*verts_per_part;

		vec o = (part->children[0].center - part->center).tovec();
		float off0[] = {(float)o.x, (float)o.y, (float)o.z};
		o = (part->children[1].center - part->center).tovec();
		float off1[] = {(float)o.x, (float)o.y, (float)o.z};
		o = (part->children[2].center - part->center).tovec();
		float off2[] = {(float)o.x, (float)o.y, (float)o.z};
		o = (part->children[3].center - part->center).tovec();
		float off3[] = {(float)o.x, (float)o.y, (float)o.z};

		int u = 0, v = 0, c = 0;
		for (int x = 0; x < verts_per_side; x+=2)
			for (int y = 0; y < x+1; y+=2)
			{
				c = (x*x + x)/2;
				u = x/2;
				v = y/2;
				terrain_geometry[offset + (u*u + u)/2 + v].copywithshift(terrain_geometry[part->children[0].buffer_location->offset*verts_per_part + c + y], off0);
				terrain_geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(u)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(v)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v-1)%2 == 1 && (u-1)%2 == 1 && y > 0)
						terrain_geometry[offset + ((u-1)*(u-1) + (u-1))/2 + (v-1)].g[1] = (terrain_geometry[offset + ((u-2)*(u-2) + (u-2))/2 + (v-2)].g[0] +
																						   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u-1)%2 == 1 && y < x)
						terrain_geometry[offset + ((u-1)*(u-1) + (u-1))/2 + v].g[1] = (terrain_geometry[offset + ((u-2)*(u-2) + (u-2))/2 + v].g[0] +
																					   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v-1)%2 == 1 && y > 0)
						terrain_geometry[offset + (u*u + u)/2 + (v-1)].g[1] = (terrain_geometry[offset + (u*u + u)/2 + (v-2)].g[0] +
																			   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}

				u = x/2 + verts_per_side/2;
				v = y/2;
				terrain_geometry[offset + (u*u + u)/2 + v].copywithshift(terrain_geometry[part->children[1].buffer_location->offset*verts_per_part + c + y], off1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(u)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(v)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v-1)%2 == 1 && (u-1)%2 == 1 && y > 0)
						terrain_geometry[offset + ((u-1)*(u-1) + (u-1))/2 + (v-1)].g[1] = (terrain_geometry[offset + ((u-2)*(u-2) + (u-2))/2 + (v-2)].g[0] +
																						   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u-1)%2 == 1 && y < x)
						terrain_geometry[offset + ((u-1)*(u-1) + (u-1))/2 + v].g[1] = (terrain_geometry[offset + ((u-2)*(u-2) + (u-2))/2 + v].g[0] +
																					   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v-1)%2 == 1 && y > 0)
						terrain_geometry[offset + (u*u + u)/2 + (v-1)].g[1] = (terrain_geometry[offset + (u*u + u)/2 + (v-2)].g[0] +
																			   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}

				u = (verts_per_side-1) - x/2;
				v = verts_per_side/2 - y/2;
				terrain_geometry[offset + (u*u + u)/2 + v].copywithshift(terrain_geometry[part->children[2].buffer_location->offset*verts_per_part + c + y], off2);
				terrain_geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(u)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(v)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v+1)%2 == 1 && (u+1)%2 == 1 && y > 0)
						terrain_geometry[offset + ((u+1)*(u+1) + (u+1))/2 + (v+1)].g[1] = (terrain_geometry[offset + ((u+2)*(u+2) + (u+2))/2 + (v+2)].g[0] +
																						   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u+1)%2 == 1 && y < x)
						terrain_geometry[offset + ((u+1)*(u+1) + (u+1))/2 + v].g[1] = (terrain_geometry[offset + ((u+2)*(u+2) + (u+2))/2 + v].g[0] +
																					   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v+1)%2 == 1 && y > 0)
						terrain_geometry[offset + (u*u + u)/2 + (v+1)].g[1] = (terrain_geometry[offset + (u*u + u)/2 + (v+2)].g[0] +
																			   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}

				u = x/2 + verts_per_side/2;
				v = y/2 + verts_per_side/2;
				terrain_geometry[offset + (u*u + u)/2 + v].copywithshift(terrain_geometry[part->children[3].buffer_location->offset*verts_per_part + c + y], off3);
				terrain_geometry[offset + (u*u + u)/2 + v].t[0] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(u)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[1] = (part->polarity == -1.0 ? 1.0 : 0.0) + part->polarity*part->a1*radius*float(v)/float(verts_per_side-1);
				terrain_geometry[offset + (u*u + u)/2 + v].t[2] = (v == 0 ? 1.0 : (u == (verts_per_side - 1) ? 2.0 : (u == v ? 3.0 : 0.0)));

				// TODO fix geomorphing
				if (u%2 == 0 && v%2 == 0 && x > 0 && part->parent != NULL)
				{
					if ((v-1)%2 == 1 && (u-1)%2 == 1 && y > 0)
						terrain_geometry[offset + ((u-1)*(u-1) + (u-1))/2 + (v-1)].g[1] = (terrain_geometry[offset + ((u-2)*(u-2) + (u-2))/2 + (v-2)].g[0] +
																						   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((u-1)%2 == 1 && y < x)
						terrain_geometry[offset + ((u-1)*(u-1) + (u-1))/2 + v].g[1] = (terrain_geometry[offset + ((u-2)*(u-2) + (u-2))/2 + v].g[0] +
																					   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
					if ((v-1)%2 == 1 && y > 0)
						terrain_geometry[offset + (u*u + u)/2 + (v-1)].g[1] = (terrain_geometry[offset + (u*u + u)/2 + (v-2)].g[0] +
																			   terrain_geometry[offset + (u*u + u)/2 + v].g[0])/2.0;
				}
			}

		part->update_merge = true;
		for (int x = 0; x < 3; x++)
			part->neighbors[x]->update_neighbors();
	}
}

noisevec planethdl::terrain(location n)
{
	vec vn = n.tovec();
	vec nn = vn;
	float mag = magnorm(&nn);

	noisevec s = 10.0*(noise(nn, 2.0, 1.0/mag) + noise(nn*2.0, 4.0, 2.0/mag) + noise(nn*4.0, 8.0, 4.0/mag) + 0.25);

	if (s.h < 0.0)
		s = noisevec(0.0);
	else if (s.h > 1.0)
		s = noisevec(1.0);

	noisevec m = noise(vn/4000.0, 2.0*mag/4000.0, 1.0/4000.0)*2000.0 +
				 noise(vn/2000.0, 2.0*mag/2000.0, 1.0/2000.0)*1000.0 +
				 noise(vn/1000.0, 2.0*mag/1000.0, 1.0/1000.0)*500.0  +
				 noise(vn/500.0,  2.0*mag/500.0,  1.0/500.0 )*250.0  +
				 noise(vn/250.0,  2.0*mag/250.0,  1.0/250.0 )*125.0  +
				 noise(vn/125.0,  2.0*mag/125.0,  1.0/125.0 )*62.5   +
				 noise(vn/62.5,   2.0*mag/62.5,   1.0/62.5  )*31.25  +
				 noise(vn/31.25,  2.0*mag/31.25,  1.0/31.25 )*15.625 +
				 noise(vn/15.625, 2.0*mag/15.625, 1.0/15.625)*7.5    +
				 noise(vn/7.5,    2.0*mag/7.5,    1.0/7.5   )*3.75   +
				 noise(vn/3.75,   2.0*mag/3.75,   1.0/3.75  )*1.85   +
				 noise(vn/1.85,   2.0*mag/1.85,   1.0/1.85  );

	noisevec r = noise(nn*10.0, 20.0, 10.0/mag)*3.0;

	if (r.h > 0.0)
		r = -r;

	r = 1.0 + r;

	if (r.h < 0.0)
		r = noisevec(0.0);

	noisevec o = -4000.0 + m*r;
	noisevec t = 4000.0 + m*r*1.5;

	return s*t + (1.0 - s)*o;
}

void planethdl::generate_info(containerhdl *contain)
{
	valuehdl *mass_value = new valuehdl;
	mass_value->init((char*)"Mass:", &mass, double_ty);
	valuehdl *inertia_value = new valuehdl;
	inertia_value->init((char*)"Inertia:", &inertia, double_ty);
	valuehdl *radius_value = new valuehdl;
	radius_value->init((char*)"Radius:", &radius, double_ty);
	valuehdl *volume_value = new valuehdl;
	volume_value->init((char*)"Volume:", &volume, double_ty);
	valuehdl *density_value = new valuehdl;
	density_value->init((char*)"Density:", &density, double_ty);

	valuehdl *position_value = new valuehdl;
	position_value->init((char*)"Position:", &position, location_ty);
	valuehdl *velocity_value = new valuehdl;
	velocity_value->init((char*)"Velocity:", &velocity, location_ty);
	valuehdl *acceleration_value = new valuehdl;
	acceleration_value->init((char*)"Acceleration:", &acceleration, location_ty);

	valuehdl *orientation_value = new valuehdl;
	orientation_value->init((char*)"Orientation:", &orientation, vec_ty);
	valuehdl *avelocity_value = new valuehdl;
	avelocity_value->init((char*)"Angular Velocity:", &angular_velocity, vec_ty);
	valuehdl *aacelleration_value = new valuehdl;
	aacelleration_value->init((char*)"Angular Acceleration:", &angular_acceleration, vec_ty);

	valuehdl *size_value = new valuehdl;
	size_value->init((char*)"Size:", &size, vec_ty);

	contain->addobj(mass_value, value_t);
	contain->addobj(inertia_value, value_t);
	contain->addobj(radius_value, value_t);
	contain->addobj(volume_value, value_t);
	contain->addobj(density_value, value_t);
	contain->addobj(position_value, value_t);
	contain->addobj(velocity_value, value_t);
	contain->addobj(acceleration_value, value_t);
	contain->addobj(orientation_value, value_t);
	contain->addobj(avelocity_value, value_t);
	contain->addobj(aacelleration_value, value_t);
	contain->addobj(size_value, value_t);
}

void planethdl::generate_menu(containerhdl *contain)
{
	checkboxhdl *remove_box = new checkboxhdl();
	remove_box->init((char*)"Delete This Object", &remove);

	contain->addobj(remove_box, checkbox_t);
}
