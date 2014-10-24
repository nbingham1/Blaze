#include "planet.h"
#include "system.h"

void planet_node::init(planethdl *o)
{
	cull = false;

	children = NULL;
	parent = NULL;

	orgin = o;
}

void planet_node::generate(vec v1, vec v2, vec v3)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->mid = norm(v1 + v2 + v3);

	double length = 0.0;
	double x_d_vps = 0.0;
	double y_d_x = 0.0;
	int count = 0;

	vec n;

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

			for (int z = 0; z < 3; z++)
				verts[count + z] = verts[count + z]*orgin->radius;
		}

	for (int x = 0; x < vps; x++)
		for (int y = 0; y < x+1; y++)
		{
			count = ((x*x + x)/2 + y)*3;
			norms[count + 0] = 0.0;
			norms[count + 1] = 0.0;
			norms[count + 2] = 0.0;
		}
	// Not Finished
}

void planet_node::release()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			children[x].release();

		delete [] children;
	}

	parent = NULL;
	orgin = NULL;
}

void planet_node::detail()
{
	// Not Finished
}

void planet_node::split()
{
	if (children != NULL)
		for (int x = 0; x < 4; x++)
			children[x].split();
	else
	{
		vec vec1 = v1,
			vec2 = norm(v1 + v2),
			vec3 = norm(v1 + v3),
			vec4 = v2,
			vec5 = norm(v2 + v3),
			vec6 = v3;

		children = new planet_node[4];

		for (int x = 0; x < 4; x++)
		{
			children[x].init(orgin);
			children[x].parent = this;
		}

		children[0].generate(vec1, vec2, vec3);
		children[1].generate(vec2, vec4, vec5);
		children[2].generate(vec5, vec3, vec2);
		children[3].generate(vec3, vec5, vec6);
	}

	// Not Finished
}

void planet_node::merge()
{
	if (children != NULL)
	{
		for (int x = 0; x < 4; x++)
			if (children[x].children != NULL)
				children[x].merge();

		int u, v, c;
		for (int x = 0; x < vps; x+=2)
			for (int y = 0; y < x+1; y+=2)
			{
				c = (x*x + x)/2;
				u = x/2;
				v = y/2;
				for (int z = 0; z < 3; z++)
				{
					verts[((u*u + u)/2 + v)*3 + z] = children[0].verts[(c + y)*3 + z];
					norms[((u*u + u)/2 + v)*3 + z] = children[0].norms[(c + y)*3 + z];
				}

				u = x/2 + vps/2;
				v = y/2;
				for (int z = 0; z < 3; z++)
				{
					verts[((u*u + u)/2 + v)*3 + z] = children[1].verts[(c + y)*3 + z];
					norms[((u*u + u)/2 + v)*3 + z] = children[1].norms[(c + y)*3 + z];
				}

				u = (vps-1) - x/2;
				v = vps/2 - y/2;
				for (int z = 0; z < 3; z++)
				{
					verts[((u*u + u)/2 + v)*3 + z] = children[2].verts[(c + y)*3 + z];
					norms[((u*u + u)/2 + v)*3 + z] = children[2].norms[(c + y)*3 + z];
				}

				u = x/2 + vps/2;
				v = y/2 + vps/2;
				for (int z = 0; z < 3; z++)
				{
					verts[((u*u + u)/2 + v)*3 + z] = children[3].verts[(c + y)*3 + z];
					norms[((u*u + u)/2 + v)*3 + z] = children[3].norms[(c + y)*3 + z];
				}
			}

		for (int x = 0; x < 4; x++)
			children[x].release();

		delete children;
		children = NULL;
	}
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
			vec cpos = orgin->camerapos();
			for (int x = 0; x < vpn; x++)
			{
				v[x*3 + 0] = verts[x*3 + 0] + cpos.x;
				v[x*3 + 1] = verts[x*3 + 1] + cpos.y;
				v[x*3 + 2] = verts[x*3 + 2] + cpos.z;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			//glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, v);
			glTexCoordPointer(2, GL_DOUBLE, 0, orgin->texts);
			//glNormalPointer(GL_FLOAT, 0, norms);

			int count = 0;
			for (int x = 1; x < vps; x++)
			{
				glDrawElements(GL_TRIANGLE_STRIP, x*2+1, GL_UNSIGNED_INT, &orgin->indices[count]);
				count += x*2 + 1;
			}

			//glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
}

bool planethdl::generate(char *name, double *framerate)
{
	printf("Generating planet %s....", name);
	double s1;

	strcpy(this->name, name);
	for (unsigned int x = 0; x < strlen(name); x++)
		seed += double(name[x])/double(x);

	seed /= 512.0;

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
		data[x].init(this);

	physics.init(framerate);

	s1 = noise1(seed);
	radius = s1*9e14 + 1e14;
	atmradius = (noise1(s1) + 1.0)*radius;

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

	georesolution = res*res*2*4;
	geology = NULL;
	geology = new unsigned char[georesolution];
	if (geology == NULL)
	{
		printf("[error]\nNot enough memory, cleaning up.\n");
		release();
		return false;
	}

	envresolution = res*res*2*3;
	environment = NULL;
	environment = new unsigned char[envresolution];
	if (environment == NULL)
	{
		printf("[error]\nNot enough memory, cleaning up.\n");
		release();
		return false;
	}

	atmresolution = res*res*2*4;
	atmospherics = NULL;
	atmospherics = new unsigned char[atmresolution];
	if (atmospherics == NULL)
	{
		printf("[error]\nNot enough memory, cleaning up.\n");
		release();
		return false;
	}
	// Not Finished

	printf("[done]\n");
	return true;
}

bool planethdl::load(char *path)
{
	printf("Loading planet at %s....", path);
	FILE *fileptr;
	char str[256];

	sprintf(str, "%s/%s", path, "data.txt");
	fileptr = fopen((const char*)str, "r");
	if (fileptr == NULL)
	{
		printf("[error]\nFile not found: %s/data.txt\n", path);
		printf("Generating planet instead.\n");

		sprintf(str, "%c%c%c%c%c%c%c", rand()%(int('z') - int('a')) + int('a'),
									   rand()%(int('z') - int('a')) + int('a'),
									   rand()%(int('z') - int('a')) + int('a'),
									   rand()%(int('z') - int('a')) + int('a'),
									   rand()%(int('z') - int('a')) + int('a'),
									   rand()%(int('z') - int('a')) + int('a'),
									   rand()%(int('z') - int('a')) + int('a'));

		return generate(str, NULL);
	}

	while (!feof(fileptr))
	{
		better_fgets(str, 256, fileptr);

		if (strncmp(str, "Name: ", 6) == 0)
			sscanf(str, "Name: %s", name);
		else if (strncmp(str, "Radius: ", 8) == 0)
			sscanf(str, "Radius: %f", &radius);
		else if (strncmp(str, "Atm Radius: ", 12) == 0)
			sscanf(str, "Atm Radius: %f", &atmradius);
		else if (strncmp(str, "Seed: ", 6) == 0)
			sscanf(str, "Seed: %f", &seed);
	}

	fclose(fileptr);

	// Not Finished

	return true;
}

void planethdl::release()
{
	for (int x = 0; x < 20; x++)
		data[x].release();

	if (atmosphere != NULL)
	{
		glDeleteTextures(atmosdim[0]*atmosdim[1]*atmosdim[2], atmosphere);
		delete [] atmosphere;
		atmosphere = NULL;
	}

	if (ground != NULL)
	{
		glDeleteTextures(grounddim[0]*grounddim[1]*grounddim[2]*grounddim[3], ground);
		delete [] ground;
		ground = NULL;
	}

	if (geology != NULL)
	{
		delete [] geology;
		geology = NULL;
	}

	if (environment != NULL)
	{
		delete [] environment;
		environment = NULL;
	}

	if (atmospherics != NULL)
	{
		delete [] atmospherics;
		atmospherics = NULL;
	}
}

void planethdl::prepare()
{
	// Not Finished
}

void planethdl::render()
{
	// Not Finished
}

vec planethdl::camerapos()
{
	return orgin->camerapos();
}
