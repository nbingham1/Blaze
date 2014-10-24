#include "planet.h"

void planet_node::init(planet *o)
{
	children = NULL;
	parent = NULL;

	cull = false;
	cullliq = false;
	size = 0.0;
	splitdepth = 0;

	orgin = o;
	verts = NULL;
	//norms = NULL;
	data = NULL;
	ldata = NULL;

	min = 99999999.0;
	max = -99999999.0;
}

void planet_node::generate(vec vec1, vec vec2, vec vec3)
{
	v1 = vec1;
	v2 = vec2;
	v3 = vec3;
	mid = norm(v1 + v2 + v3);

	if (parent != NULL)
		size = parent->size/2.0;
	else
		size = m_abs(acos(dot(v1, v3)));

	double length = 0.0;
	double x_d_vps = 0.0;
	double y_d_x = 0.0;
	int count = 0;

	verts = new double[vpn*3];
	//norms = new double[vpn*3];
	data = new double[vpn];
	ldata = new double[vpn];

	for (int x = 0; x < vpn; x++)
	{
		data[x] = 0.0;
		ldata[x] = 0.0;

		//for (int y = 0; y < 3; y++)
		//	norms[x*3 + y] = 0.0;
	}

	for (int x = 0; x < vps; x++)
		for (int y = 0; y < x+1; y++)
		{
			x_d_vps = double(x)/double(vps-1);
			y_d_x = x != 0 ? double(y)/double(x) : 0.0;
			count = ((x*x + x)/2 + y)*3;
			verts[count + 0] = v1.x*(1.0 - x_d_vps) + v2.x*x_d_vps*(1.0 - y_d_x) + v3.x*x_d_vps*y_d_x;
			verts[count + 1] = v1.y*(1.0 - x_d_vps) + v2.y*x_d_vps*(1.0 - y_d_x) + v3.y*x_d_vps*y_d_x;
			verts[count + 2] = v1.z*(1.0 - x_d_vps) + v2.z*x_d_vps*(1.0 - y_d_x) + v3.z*x_d_vps*y_d_x;

			length = mag(vec(&verts[count], 3));
			for (int z = 0; z < 3; z++)
				verts[count + z] /= length;
		}
}

void planet_node::detail()
{
	float h_dist = m_max(m_abs(acos(dot(-orgin->camera.vector, mid))) - size, 0.0);
	float distfromrad = orgin->camera.distance - orgin->physics.radius;
	float vis = acos(orgin->physics.radius/orgin->camera.distance);
	if (m_nan(vis) || vis <= 0.1)
		vis = 0.1;

	cull = false;
	cullliq = false;
	if (h_dist > vis + size/* || dot(orgin->camera.distance*orgin->camera.vector - mid*orgin->physics.radius, orgin->camera.ref->lookat) > 0.0*/)
	{
		cull = true;
		cullliq = true;
	}
	else if ((distfromrad > 800.0 && max < 0.0) || (distfromrad < -1000.0 && min > 0.0))
		cull = true;

	if (min > 0.0)
		cullliq = true;
	else
	{
		planet_node *curr = this;
		while (curr != NULL)
		{
			if (!curr->cull)
				curr->cullliq = false;
			curr = curr->parent;
		}
	}

	if (!cull || !cullliq)
	{
		if (children != NULL)
			for (int x = 0; x < 4; x++)
				children[x].detail();

		float v_dist = m_max(distfromrad - orgin->camera.ground_height, 0.0);

		float dist = m_sqr(h_dist*orgin->physics.radius) + m_sqr(v_dist);
		float percent = m_pi*m_sqr(size*orgin->physics.radius)/dist;

		if (percent < 0.64 && children != NULL)
				merge();

		else if (percent >= 1.0 && children == NULL && orgin->physics.radius*size/vps > 60.0)
			split();
	}
}

void planet_node::split()
{
	if (children != NULL)
		for (int x = 0; x < 4; x++)
			children[x].split();
	else
	{
		children = new planet_node[4];
		if (children == NULL)
		{
			printf("Error: Failed allocation of children.\n file: %s line: %i", __FILE__, __LINE__);
			return;
		}

		vec vec1 = v1,
			vec2 = norm(v1 + v2),
			vec3 = norm(v1 + v3),
			vec4 = v2,
			vec5 = norm(v2 + v3),
			vec6 = v3;

		srand(seed);
		for (int i = 0; i < 4; i++)
		{
			children[i].init(orgin);
			children[i].parent = this;
			children[i].seed = rand();
			children[i].splitdepth = splitdepth+1;
			children[i].min = 999999999.0;
			children[i].max = -999999999.0;
		}

		children[0].generate(vec1, vec2, vec3);
		children[1].generate(vec2, vec4, vec5);
		children[2].generate(vec5, vec3, vec2);
		children[3].generate(vec3, vec5, vec6);

		srand(seed);

		double expon[8] = {3.0, 2.0, 1.0, 0.5, 0.25, 0.125, 0.25, 0.5};
		double mult = 4000.0;

		for (int x = 0; x < vpn; x++)
		{
			children[0].data[x] = mult*Brownian(children[0].verts[x*3 + 0], children[0].verts[x*3 + 1], children[0].verts[x*3 + 2], 2.5, expon, 7.0);
			children[1].data[x] = mult*Brownian(children[1].verts[x*3 + 0], children[1].verts[x*3 + 1], children[1].verts[x*3 + 2], 2.5, expon, 7.0);
			children[2].data[x] = mult*Brownian(children[2].verts[x*3 + 0], children[2].verts[x*3 + 1], children[2].verts[x*3 + 2], 2.5, expon, 7.0);
			children[3].data[x] = mult*Brownian(children[3].verts[x*3 + 0], children[3].verts[x*3 + 1], children[3].verts[x*3 + 2], 2.5, expon, 7.0);

			for (int y = 0; y < 4; y++)
			{
				children[y].max = children[y].data[x] > children[y].max ? children[y].data[x] : children[y].max;
				children[y].min = children[y].data[x] < children[y].min ? children[y].data[x] : children[y].min;
			}
		}
		/*if (norms != NULL)
		{
			delete [] norms;
			norms = NULL;
		}*/
		if (verts != NULL && parent != NULL)
		{
			delete [] verts;
			verts = NULL;
 		}
 		if (data != NULL)
 		{
 			delete [] data;
 			data = NULL;
 		}
 		if (ldata != NULL)
 		{
 			delete [] ldata;
 			ldata = NULL;
 		}
	}
}

void planet_node::merge()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			if (children[x].children != NULL)
				children[x].merge();

		verts = new double[vpn*3];
		//norms = new double[vpn*3];
		data = new double[vpn];
		ldata = new double[vpn];

		for (int x = 0; x < vpn; x++)
		{
			data[x] = 0.0;
			ldata[x] = 0.0;

			//for (int y = 0; y < 3; y++)
			//	norms[x*3 + y] = 0.0;
		}

		int u, v, c;
		for (int x = 0; x < vps; x+=2)
			for (int y = 0; y < x+1; y+=2)
			{
				c = (x*x + x)/2;
				u = x/2;
				v = y/2;
				data[((u*u + u)/2 + v)] = children[0].data[c + y];
				for (int z = 0; z < 3; z++)
					verts[((u*u + u)/2 + v)*3 + z] = children[0].verts[(c + y)*3 + z];
				//for (int z = 0; z < 3; z++)
				//	norms[((u*u + u)/2 + v)*3 + z] = children[0].norms[(c + y)*3 + z];

				u = x/2 + vps/2;
				v = y/2;
				data[((u*u + u)/2 + v)] = children[1].data[c + y];
				for (int z = 0; z < 3; z++)
					verts[((u*u + u)/2 + v)*3 + z] = children[1].verts[(c + y)*3 + z];
				//for (int z = 0; z < 3; z++)
				//	norms[((u*u + u)/2 + v)*3 + z] = children[1].norms[(c + y)*3 + z];

				u = (vps-1) - x/2;
				v = vps/2 - y/2;
				data[((u*u + u)/2 + v)] = children[2].data[c + y];
				for (int z = 0; z < 3; z++)
					verts[((u*u + u)/2 + v)*3 + z] = children[2].verts[(c + y)*3 + z];
				//for (int z = 0; z < 3; z++)
				//	norms[((u*u + u)/2 + v)*3 + z] = children[2].norms[(c + y)*3 + z];

				u = x/2 + vps/2;
				v = y/2 + vps/2;
				data[((u*u + u)/2 + v)] = children[3].data[c + y];
				for (int z = 0; z < 3; z++)
					verts[((u*u + u)/2 + v)*3 + z] = children[3].verts[(c + y)*3 + z];
				//for (int z = 0; z < 3; z++)
				//	norms[((u*u + u)/2 + v)*3 + z] = children[3].norms[(c + y)*3 + z];
			}

		for (int x = 0; x < 4; x++)
			children[x].release();

		delete children;
		children = NULL;
	}
}

void planet_node::release()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			children[x].release();
		delete [] children;
	}

	children = NULL;
	parent = NULL;
	orgin = NULL;
	if (verts != NULL)
		delete [] verts;
	//if (norms != NULL)
	//	delete [] norms;
	if (data != NULL)
		delete [] data;
	if (ldata != NULL)
		delete [] ldata;

	verts = NULL;
	//norms = NULL;
	data = NULL;
	ldata = NULL;
}

void planet_node::render()
{
	if (!cull)
	{
		if (children != NULL)
			for (int x = 0; x < 4; x++)
				children[x].render();
		else
		{
			double v[vpn*3];
			int count = 0;
			for (int x = 0; x < vpn; x++)
			{
				v[x*3 + 0] = verts[x*3 + 0]*(orgin->physics.radius + data[x]) + orgin->camera.vector.x*orgin->camera.distance;
				v[x*3 + 1] = verts[x*3 + 1]*(orgin->physics.radius + data[x]) + orgin->camera.vector.y*orgin->camera.distance;
				v[x*3 + 2] = verts[x*3 + 2]*(orgin->physics.radius + data[x]) + orgin->camera.vector.z*orgin->camera.distance;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, v);
			glTexCoordPointer(2, GL_DOUBLE, 0, orgin->texts);

			count = 0;
			for (int x = 1; x < vps; x++)
			{
				glDrawElements(GL_TRIANGLE_STRIP, x*2+1, GL_UNSIGNED_INT, &orgin->indices[count]);
				count += x*2 + 1;
			}

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
}

void planet_node::render_liquid()
{
	if (!cullliq)
	{
		if (children != NULL)
			for (int x = 0; x < 4; x++)
				children[x].render_liquid();
		else
		{
			double v[vpn*3];
			int count = 0;

			for (int x = 0; x < vpn; x++)
			{
				v[x*3 + 0] = verts[x*3 + 0]*(orgin->physics.radius + ldata[x]) + orgin->camera.vector.x*orgin->camera.distance;
				v[x*3 + 1] = verts[x*3 + 1]*(orgin->physics.radius + ldata[x]) + orgin->camera.vector.y*orgin->camera.distance;
				v[x*3 + 2] = verts[x*3 + 2]*(orgin->physics.radius + ldata[x]) + orgin->camera.vector.z*orgin->camera.distance;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, v);

			count = 0;
			for (int x = 1; x < vps; x++)
			{
				glDrawElements(GL_TRIANGLE_STRIP, x*2+1, GL_UNSIGNED_INT, &orgin->indices[count]);
				count += x*2 + 1;
			}

			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
}

void planet_node::getheight(vec vc, double *h)
{
	double t, u, v;
	int a, a1;
	if (intraytri(vec(), -vc, v1*orgin->physics.radius, v2*orgin->physics.radius, v3*orgin->physics.radius, &t, &u, &v))
	{
		if (children != NULL)
			for (int x = 0; x < 4; x++)
				children[x].getheight(vc, h);
		else
		{
			vec q, r;
			double ql, rl;
			q = cross(v2, v3 - v2);
			ql = dot(q, v2 - v1);
			r = cross(v2, v2 - v1);
			rl = dot(r, v3 - v2);
			double dx = (vps-1)*(1.0 - dot(v2 + vc, q)/ql);
			double dy = (vps-1)*dot(-vc - v2, r)/rl;

			int x = int(dx);
			int y = int(dy);

			a = (x*x + x)/2;
			a1 = ((x+1)*(x+1) + (x+1))/2;

			double h00 = data[(a + y)];
			double h01 = data[(a1 + y)];
			double h11 = data[(a1 + (y+1))];
			double h10;
			if (y+1 > x)
				h10 = h00 + h11 - h01;
			else
				h10 = data[((x*x + x)/2 + (y+1))];

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

void planet::init(char *n, camerahdl *cam)
{
	camera.ref = cam;
	next = NULL;
	prev = NULL;
	strcpy(name, n);
	for (unsigned int x = 0; x < strlen(n); x++)
		seed += int(n[x]);

	srand(seed);
	int c1 = 1, c2 = 0, index = 0;

	for (int x = 0; x < vps-1; x++)
	{
		for (int y = 0; y < x+1; y++)
		{
			indices[index++] = c1++;
			indices[index++] = c2++;
		}
		indices[index++] = c1++;
	}

	int count = 0;

	for (int x = 0; x < vps; x++)
		for (int y = 0; y < x+1; y++)
		{
			count = ((x*x + x)/2 + y)*2;
			texts[count + 0] = double(y)/double(vps-1);
			texts[count + 1] = double(x)/double(vps-1);
		}

	for (int x = 0; x < 20; x++)
	{
		data[x].init(this);
		data[x].seed = rand();
	}

	physics.init(cam->framerate);
	physics.radius = float(rand()%9000000 + 1000000);
	atmos_radius = physics.radius*1.025;

	vec    v0(1.0, 0.0, 0.0),
		   v1(0.447213595500,  0.894427191000, 0.0),
		   v2(0.447213595500,  0.276393202252, 0.850650808354),
		   v3(0.447213595500, -0.723606797748, 0.525731112119),
		   v4(0.447213595500, -0.723606797748, -0.525731112119),
		   v5(0.447213595500,  0.276393202252, -0.850650808354),
		   v6(-0.447213595500, -0.894427191000, 0.0),
		   v7(-0.447213595500, -0.276393202252, 0.850650808354),
		   v8(-0.447213595500,  0.723606797748, 0.525731112119),
		   v9(-0.447213595500,  0.723606797748, -0.525731112119),
		   v10(-0.447213595500, -0.276393202252, -0.850650808354),
		   v11(-1.0, 0.0, 0.0);

	data[0].generate(v2, v1, v0);
	data[1].generate(v3, v2, v0);
	data[2].generate(v4, v3, v0);
	data[3].generate(v5, v4, v0);
	data[4].generate(v1, v5, v0);
	data[5].generate(v2, v8, v1);
	data[6].generate(v3, v7, v2);
	data[7].generate(v4, v6, v3);
	data[8].generate(v5, v10, v4);
	data[9].generate(v1, v9, v5);
	data[10].generate(v8, v9, v1);
	data[11].generate(v7, v8, v2);
	data[12].generate(v6, v7, v3);
	data[13].generate(v10, v6, v4);
	data[14].generate(v9, v10, v5);
	data[15].generate(v10, v9, v11);
	data[16].generate(v9, v8, v11);
	data[17].generate(v8, v7, v11);
	data[18].generate(v7, v6, v11);
	data[19].generate(v6, v10, v11);

	for (int x = 0; x < 20; x++)
	{
		data[x].min = 0.0;
		data[x].max = 0.0;
	}

	ground_mat.init("res/planet_ground.vx", "res/planet_ground.ft");
	liquid_mat.init("res/planet_liquid.vx", "res/planet_liquid.ft");
	atmos_mat.init("res/planet_atmos.vx", "res/planet_atmos.ft");

	// Ground Shader Textures
	unsigned char *texdata = new unsigned char[32*32*32*4];

	noise3tex(texdata, 32);
	ground_mat.addtex(texdata, 32, 32, 32, true, GL_RGBA, GL_REPEAT);
	delete [] texdata;

	int width, height;

	texdata = rgba_tga("res/terrain00.tga", &width, &height);
	ground_mat.addtex(texdata, width, height, 1, true, GL_RGBA, GL_REPEAT);
	delete [] texdata;

	texdata = rgba_tga("res/terrain01.tga", &width, &height);
	ground_mat.addtex(texdata, width, height, 1, true, GL_RGBA, GL_REPEAT);
	delete [] texdata;

	texdata = rgba_tga("res/terrain02.tga", &width, &height);
	ground_mat.addtex(texdata, width, height, 1, true, GL_RGBA, GL_REPEAT);
	delete [] texdata;

	texdata = rgba_tga("res/terrain03.tga", &width, &height);
	ground_mat.addtex(texdata, width, height, 1, true, GL_RGBA, GL_REPEAT);
	delete [] texdata;
	// End Ground Shader Textures


	atm_optdepthsize = 1024;
	atm_optdepth = new unsigned char[atm_optdepthsize*3];
	atmos_mat.blank(atm_optdepthsize, 1, 1, GL_RGB, GL_CLAMP_TO_EDGE);

	texdata = rgba_tga("res/skycol.tga", "res/skycola.tga", &width, &height);
	atmos_mat.addtex(texdata, width, height, 1, false, GL_RGBA, GL_CLAMP_TO_EDGE);
	liquid_mat.addtex(texdata, width, height, 1, false, GL_RGBA, GL_CLAMP_TO_EDGE);
	ground_mat.addtex(texdata, width, height, 1, false, GL_RGBA, GL_CLAMP_TO_EDGE);
	delete [] texdata;

	unsigned char *front, *back, *right, *left, *top, *bottom;
	front = rgba_tga("res/front.tga", "res/front.tga", &width, &height);
	back = rgba_tga("res/back.tga", "res/back.tga", &width, &height);
	right = rgba_tga("res/right.tga", "res/right.tga", &width, &height);
	left = rgba_tga("res/left.tga", "res/left.tga", &width, &height);
	top = rgba_tga("res/bottom.tga", "res/bottom.tga", &width, &height);
	bottom = rgba_tga("res/top.tga", "res/top.tga", &width, &height);

	atmos_mat.addcubemap(front, back, right, left, top, bottom, width, height);
	liquid_mat.addcubemap(front, back, right, left, top, bottom, width, height);

	delete [] front;
	delete [] back;
	delete [] right;
	delete [] left;
	delete [] top;
	delete [] bottom;
}

void planet::release()
{
	for (int x = 0; x < 20; x++)
		data[x].release();

	ground_mat.release();
	liquid_mat.release();
	atmos_mat.release();

	if (atm_optdepth != NULL)
		delete [] atm_optdepth;
}

void planet::prepare()
{
	physics.update();
	camera.update(&physics);

	for (int x = 0; x < 20; x++)
	{
		data[x].detail();
		data[x].getheight(camera.vector, &camera.ground_height);
	}

	vec p = vec(0.0, -camera.distance, 0.0);
	vec v;
	double u, u0, u1, o, e, e0;
	double scale = double(atm_optdepthsize)/double(camera.ref->size);
	int x1;
	double x2;

	double a, b, c = camera.distance*camera.distance - atmos_radius*atmos_radius;
	double c2 = camera.distance*camera.distance - physics.radius*physics.radius;
	bool s = false;
	for (int x = 0; x < atm_optdepthsize; x++)
	{
		x1 = m_min(x/int(scale), camera.ref->size - 2);
		x2 = double(x - x1*int(scale))/scale;
		v = vec(&camera.ref->sphereverts[x1*3], 3)*(1.0 - x2) + vec(&camera.ref->sphereverts[(x1+1)*3], 3)*x2;

		a = dot(v, v);
		b = dot(v, p);
		u = sqrt(b*b - a*c);
		e = sqrt(b*b - a*c2);
		u0 = b - u;
		u0 = u0 < 0.0 ? 0.0 : u0;
		u1 = b + u;
		u1 = u1 < 0.0 ? 0.0 : u1;
		if (m_nan(u0))
			o = 0.0;
		else
			o = (u1 - u0)/a;

		atm_optdepth[x*3 + 0] = (unsigned char)(pow(m_e, -o*0.000028)*255.0);
		if (s)
			atm_optdepth[x*3 + 1] = 0;
		else
			atm_optdepth[x*3 + 1] = 255;
		atm_optdepth[x*3 + 2] = 0;
		s = false;
	}

	glBindTexture(GL_TEXTURE_1D, atmos_mat.first->texture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, atm_optdepthsize, GL_RGB, GL_UNSIGNED_BYTE, atm_optdepth);
}

void planet::render()
{
	vec pos = camera.vector*camera.distance;

	vec sun = vec(0.0, 1.0, 0.0);

	double ri = physics.radius*1.000001;
	double ro = ri - physics.radius + atmos_radius;

	vec q = -camera.vector*(camera.distance + ri - physics.radius);

	double a = 1.0/(ri*ri - ro*ro);
	double b = -a*ro*ro;

	atmos_mat.bind();
	glUniform3fARB(glGetUniformLocationARB(atmos_mat.program, "pltdir"), -camera.vector.x, -camera.vector.y, -camera.vector.z);
	glUniform1fARB(glGetUniformLocationARB(atmos_mat.program, "pltdist"), camera.distance);
	glUniform3fARB(glGetUniformLocationARB(atmos_mat.program, "sundir"), sun.x, sun.y, sun.z);
	glUniform1fARB(glGetUniformLocationARB(atmos_mat.program, "radius"), physics.radius);
	glUniformMatrix4fv(glGetUniformLocationARB(atmos_mat.program, "transform"), 1, GL_FALSE, camera.spheremat);
	camera.render();

	glClear(GL_DEPTH_BUFFER_BIT);

	ground_mat.bind();
	glUniform3fARB(glGetUniformLocationARB(ground_mat.program, "camera"), pos.x, pos.y, pos.z);
	glUniform3fARB(glGetUniformLocationARB(ground_mat.program, "sundir"), sun.x, sun.y, sun.z);
	glUniform1fARB(glGetUniformLocationARB(ground_mat.program, "radius"), physics.radius);
	glUniform1fARB(glGetUniformLocationARB(ground_mat.program, "a"), a);
	glUniform1fARB(glGetUniformLocationARB(ground_mat.program, "b"), b);
	glUniform3fARB(glGetUniformLocationARB(ground_mat.program, "q"), q.x, q.y, q.z);
	for (int x = 0; x < 20; x++)
		data[x].render();

	glDisable(GL_CULL_FACE);
	liquid_mat.bind();
	glUniform3fARB(glGetUniformLocationARB(liquid_mat.program, "camera"), pos.x, pos.y, pos.z);
	glUniform3fARB(glGetUniformLocationARB(liquid_mat.program, "sundir"), sun.x, sun.y, sun.z);
	glUniform1fARB(glGetUniformLocationARB(liquid_mat.program, "radius"), physics.radius);
	for (int x = 0; x < 20; x++)
		data[x].render_liquid();
	glEnable(GL_CULL_FACE);

	ground_mat.bind();
}
