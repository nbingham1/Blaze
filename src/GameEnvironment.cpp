/*
	GameEnvironment.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 6/6/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GameEnvironment.h"

void SkyPlane::Init(string filename, int division, GLfloat Radius, GLfloat peakheight)
{
	SkyTexture = Load2DTexture(filename, false);
	
	NumVertices = 0;
		
	GLfloat PlaneSize = sqrt((2.0*Radius)*(2.0*Radius)*0.5);
	GLfloat delta = PlaneSize/GLfloat(division);
	GLfloat tex_delta = 1.0/GLfloat(division);
	int index = 0;
	int startvert;
	
	GLfloat x_dist, z_dist,
			x_height, z_height,
			height, sqr_radius;
	
	sqr_radius = Radius*Radius;
	
	for (int x = 0; x <= division; x++)
		for (int y = 0; y <= division; y++)
		{
			x_dist = (-0.5*PlaneSize) + (GLfloat(y)*delta);
			z_dist = (-0.5*PlaneSize) + (GLfloat(x)*delta);
			x_height = (x_dist*x_dist);
			z_height = (z_dist*z_dist);
			height = (x_height + z_height)/sqr_radius;
			
			vertices[(x*(division+1) + y)*3 + 0] = x_dist;
			vertices[(x*(division+1) + y)*3 + 1] = (1.0 - height)*peakheight - 50000;
			vertices[(x*(division+1) + y)*3 + 2] = z_dist;
			
			texcoord[(x*(division+1) + y)*2 + 0] = GLfloat(y)*tex_delta;
			texcoord[(x*(division+1) + y)*2 + 1] = GLfloat(x)*tex_delta;
		}
	for (int x = 0; x < division; x++)
		for (int y = 0; y < division; y++)
		{
			startvert = (x*(division+1) + y);
			
			indices[index++] = startvert;
			indices[index++] = startvert+1;
			indices[index++] = startvert + division + 1;
			
			indices[index++] = startvert+1;
			indices[index++] = startvert + division + 2;
			indices[index++] = startvert + division + 1;
		}
	NumVertices = division*division*6;
}

void SkyPlane::Render(Vector Translation)
{
	Position += Translation;
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glTranslatef(Position.x, Position.y, 0.0);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glBindTexture(GL_TEXTURE_2D, SkyTexture);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_BYTE, indices);
		
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void SkyPlane::Release()
{
	glDeleteTextures(1, &SkyTexture);
}

void SkyBox::Init(string filename, string filetype, GLfloat size)
{
	Size = size;
	Vector v1(size, size, size);
	Vector v2(-size, size, size);
	Vector v3(-size, -size, size);
	Vector v4(size, -size, size);
	Vector v5(size, size, -size);
	Vector v6(-size, size, -size);
	Vector v7(-size, -size, -size);
	Vector v8(size, -size, -size);
	
	string file = filename + "_rt." + filetype;
	rt = Load2DTexture(file, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	file = filename + "_lt." + filetype;
	lt = Load2DTexture(file, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	file = filename + "_ft." + filetype;
	ft = Load2DTexture(file, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	file = filename + "_bk." + filetype;
	bk = Load2DTexture(file, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	file = filename + "_up." + filetype;
	up = Load2DTexture(file, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	file = filename + "_dn." + filetype;
	dn = Load2DTexture(file, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	DispList = glGenLists(1);
	glNewList(DispList, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, rt);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(1, 1);
		glVertex3f(v5.x, v5.y, v5.z);
		glTexCoord2f(1, 0);
		glVertex3f(v8.x, v8.y, v8.z);
		glTexCoord2f(0, 0);
		glVertex3f(v4.x, v4.y, v4.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, lt);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v6.x, v6.y, v6.z);
		glTexCoord2f(1, 1);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(1, 0);
		glVertex3f(v3.x, v3.y, v3.z);
		glTexCoord2f(0, 0);
		glVertex3f(v7.x, v7.y, v7.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, bk);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v5.x, v5.y, v5.z);
		glTexCoord2f(1, 1);
		glVertex3f(v6.x, v6.y, v6.z);
		glTexCoord2f(1, 0);
		glVertex3f(v7.x, v7.y, v7.z);
		glTexCoord2f(0, 0);
		glVertex3f(v8.x, v8.y, v8.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, ft);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(1, 1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(1, 0);
		glVertex3f(v4.x, v4.y, v4.z);
		glTexCoord2f(0, 0);
		glVertex3f(v3.x, v3.y, v3.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, up);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(1, 1);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(1, 0);
		glVertex3f(v6.x, v6.y, v6.z);
		glTexCoord2f(0, 0);
		glVertex3f(v5.x, v5.y, v5.z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, dn);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(v3.x, v3.y, v3.z);
		glTexCoord2f(1, 1);
		glVertex3f(v4.x, v4.y, v4.z);
		glTexCoord2f(1, 0);
		glVertex3f(v8.x, v8.y, v8.z);
		glTexCoord2f(0, 0);
		glVertex3f(v7.x, v7.y, v7.z);
	glEnd();
	glEndList();
}

void SkyBox::Render()
{
	glPushMatrix();
	glTranslated(Position->x, Position->y, Position->z);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glCallList(DispList);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void SkyBox::Release()
{
	glDeleteLists(DispList, 1);
	glDeleteTextures(1, &rt);
	glDeleteTextures(1, &lt);
	glDeleteTextures(1, &ft);
	glDeleteTextures(1, &bk);
	glDeleteTextures(1, &up);
	glDeleteTextures(1, &dn);
}

void SkyDome::Init(string filename, int slices, GLfloat Radius, GLfloat dampener)
{
	SkyTexture = Load2DTexture(filename, false);
	
	int sides = slices;
	
	GLfloat angle;
	GLfloat PolyAngle = 1.0*pi/sides;
	GLfloat vx, vy, vz;
	int index = 0;
	for (int x = 0; x <= slices; x++)
	{
		angle = x*((pi*2.0)/slices);
		for (int y = 0; y <= sides; y++)
		{
			vx = cos(y*PolyAngle) * cos(angle);
			vy = dampener * sin(angle);
			vz = sin(y*PolyAngle) * cos(angle);
			
			vertices[(x * (sides + 1) + y)*3 + 0] = vx*Radius;
			vertices[(x * (sides + 1) + y)*3 + 1] = vy*Radius;
			vertices[(x * (sides + 1) + y)*3 + 2] = vz*Radius;
			
			norms[(x * (sides + 1) + y)*3 + 0] = vx;
			norms[(x * (sides + 1) + y)*3 + 1] = vy;
			norms[(x * (sides + 1) + y)*3 + 2] = vz;
			
			texcoord[(x * (sides + 1) + y)*2 + 0] = GLfloat(y)/GLfloat(sides);
			texcoord[(x * (sides + 1) + y)*2 + 1] = GLfloat(x)/GLfloat(slices);
		}
	}
	
	for (int x = 1; x <= slices; x++)
	{
		for (int y = 0; y <= sides; y++)
		{
			indices[index++] = x*(sides+1)+y;
			indices[index++] = (x-1)*(sides+1)+y;
		}
	}
	NumSides = sides;
	NumSlices = slices;
}

void SkyDome::Render(Vector Animation)
{
	glRotatef(90.0, 1.0, 0.0, 0.0);
	Position += Animation;
	glPushMatrix();
	glRotatef(Position.x, 1.0, 0.0, 0.0);
	glRotatef(Position.y, 0.0, 1.0, 0.0);
	glRotatef(Position.z, 0.0, 0.0, 1.0);
	glScalef(2.0, 1.0, 1.0);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, norms);
	glBindTexture(GL_TEXTURE_2D, SkyTexture);
	for (int x = 0; x < NumSlices; x++)
		glDrawElements(GL_TRIANGLE_STRIP, (NumSides+1)*2, GL_UNSIGNED_SHORT, &indices[x*(NumSides+1)*2]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
}
void SkyDome::Release()
{
	glDeleteTextures(1, &SkyTexture);
}

void Water::Init(string filename, int divs, Vector Min, Vector Max)
{		
	Reflect = Load2DTexture(filename, false);
	NumVerts = divs*divs;
	NumDivs = divs;
	for (int z = 0; z <= divs; z++)
	{
		for (int x = 0; x <= divs; x++)
		{
			vertices[(z*divs + x)*3 + 0] = (GLfloat(x)/GLfloat(divs))*(Max.x - Min.x) + Min.x;
			vertices[(z*divs + x)*3 + 1] = (Max.y - Min.y)/2.0 + Min.y - 1470.0;
			vertices[(z*divs + x)*3 + 2] = (GLfloat(z)/GLfloat(divs))*(Max.z - Min.z) + Min.z;
			
			texcoord[(z*divs + x)*2 + 0] = (GLfloat(x)/GLfloat(divs));
			texcoord[(z*divs + x)*2 + 1] = (GLfloat(z)/GLfloat(divs));
		}
	}
	int index = 0;
	for (int z = 0; z < divs-1; z++)
	{
		for (int x = 0; x < divs-1; x++)
		{
			indices[index++] = z*divs + x;
			indices[index++] = (z+1)*divs + x;
			indices[index++] = z*divs + (x+1);
			
			indices[index++] = (z+1)*divs + x;
			indices[index++] = (z+1)*divs + (x+1);
			indices[index++] = z*divs + (x+1);
		}
	}
	NumVerts = index;
	
	for (int x = 0; x < 8192; x++)
	{
		forces[x] = 0;
		velocity[x] = 0;
	}
	
	for (int x = 0; x < 5; x++)
		vertices[(rand()%(NumDivs*NumDivs))*3 + 1] -= 100.0;
	
	for (int x = 0; x < (NumDivs*NumDivs)*4; x+=4)
	{
		colors[x+0] = 0.2;
		colors[x+1] = 0.2;
		colors[x+2] = 1.0;
		colors[x+3] = 1.0;
	}
}

void Water::CalculateNormals()
{
	GLfloat *Vert, *Norm;
	GLfloat Temp;
	
	for (int x = 0; x < NumDivs; x++)
	{
		for (int y = 0; y < NumDivs; y++)
		{
			Vert = &vertices[(x*NumDivs + y)*3];
			Norm = &normals[(x*NumDivs + y)*3];
			
			Norm[0] = 0.0;
			Norm[1] = 250.0;
			Norm[2] = 0.0;
			
			if (x != 0)
			{
				if (y != 0)
				{
					Norm[0] += -Vert[(-NumDivs-1)*3 + 1];
					Norm[2] += -Vert[(-NumDivs-1)*3 + 1];
				}
				else
				{
					Norm[0] += -Vert[(-NumDivs)*3 + 1];
					Norm[2] += -Vert[(-NumDivs)*3 + 1];
				}
				
				Norm[0] += -Vert[(-NumDivs)*3 + 1]*2.0;
				
				if (y != (NumDivs-1))
				{
					Norm[0] += -Vert[(-NumDivs+1)*3 + 1];
					Norm[2] += Vert[(-NumDivs+1)*3 + 1];
				}
				else
				{
					Norm[0] += -Vert[(-NumDivs)*3 + 1];
					Norm[2] += Vert[(-NumDivs)*3 + 1];
				}
			}
			else
				Norm[0] += -4.0*(Vert[1]);
			
			if (y != 0)
				Norm[2] += -Vert[-2]*2.0;
			else
				Norm[2] += -Vert[1]*2.0;
			
			if (y != (NumDivs-1))
				Norm[2] += Vert[4]*2.0;
			else
				Norm[2] += Vert[1]*2.0;
			
			if (x != (NumDivs-1))
			{
				if (y != 0)
				{
					Norm[0]+=  Vert[(NumDivs-1)*3 + 1];
					Norm[2]+= -Vert[(NumDivs-1)*3 + 1];
				}
				else
				{
					Norm[0]+=  Vert[(NumDivs)*3 + 1];
					Norm[2]+= -Vert[(NumDivs)*3 + 1];
				}

				Norm[0] += Vert[(NumDivs)*3 + 1]*2.0f;

				if (y != NumDivs-1)
				{
					Norm[0] += Vert[(NumDivs+1)*3 + 1];
					Norm[2] += Vert[(NumDivs+1)*3 + 1];
				}
				else
				{
					Norm[0] += Vert[(NumDivs)*3 + 1];
					Norm[2] += Vert[(NumDivs)*3 + 1];
				}
			}
			else
				Norm[0]+= 4.0*Vert[1];
			
			Temp = sqrt(Norm[0]*Norm[0] + Norm[2]*Norm[2] + Norm[1]*Norm[1]);
			Norm[0] /= Temp;
			Norm[1] /= Temp;
			Norm[2] /= Temp;
		}
	}
	
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Vector Front = -1*Vector(m[2], m[6], m[10]);
	for (int x = 0; x < NumDivs; x++)
	{
		for (int y = 0; y < NumDivs; y++)
		{
			GLfloat incident_angle = acos(Dot(Vector(normals[(x*NumDivs + y)*3], normals[(x*NumDivs + y)*3 + 1], normals[(x*NumDivs + y)*3 + 2]), Front));
			GLfloat refract_angle = asin(sin(incident_angle)/1.3333);
			
			GLfloat coef_rs = pow((sin(incident_angle-refract_angle)/sin(incident_angle+refract_angle)), 2);
			GLfloat coef_rp = pow((1.3333*cos(incident_angle) - cos(refract_angle))/(1.3333*cos(incident_angle) + cos(refract_angle)), 2);
			
			colors[(x*NumDivs + y)*4 + 3] = (coef_rp + coef_rs)*0.5;
			Clamp(colors[(x*NumDivs + y)*4 + 3], 0.15, 0.5);
		}
	}
}

void Water::Update()
{	
	GLfloat d, temp, vert;
	
	//for (int x = 0; x < 20; x++)
		//vertices[(rand()%(NumDivs*NumDivs))*3 + 1] += -2.0;
	
	for(int z = 1; z < NumDivs-1; z++)
	{
		for(int x = 1; x < NumDivs-1; x++) 
		{
			temp = forces[(z*NumDivs) + x];
			vert = vertices[((z*NumDivs) + x)*3 + 1];

			d = vert - vertices[(((z-1)*NumDivs)+x)*3 + 1];
			forces[((z-1)*NumDivs)+x] += d;
			temp -= d;

			d = vert - vertices[((z*NumDivs)+(x-1))*3 + 1];
			forces[(z*NumDivs)+(x-1)] += d;
			temp -= d;

			d = vert - vertices[(((z+1)*NumDivs)+x)*3 + 1];
			forces[((z+1)*NumDivs)+x] += d;
			temp -= d;

			d = vert - vertices[((z*NumDivs)+(x+1))*3 + 1];
			forces[(z*NumDivs)+(x+1)] += d;
			temp -= d;

			d = (vert - vertices[(((z+1)*NumDivs)+(x+1))*3 + 1]);
			forces[((z+1)*NumDivs)+(x+1)] += d;
			temp -= d;

			d = (vert - vertices[(((z-1)*NumDivs)+(x-1))*3 + 1]);
			forces[((z-1)*NumDivs)+(x-1)] += d;
			temp -= d;

			d = (vert - vertices[(((z-1)*NumDivs)+(x+1))*3 + 1]);
			forces[((z-1)*NumDivs)+(x+1)] += d;
			temp -= d;

			d = (vert - vertices[(((z+1)*NumDivs)+(x-1))*3 + 1]);
			forces[((z+1)*NumDivs)+(x-1)] += d;
			temp -= d;

			forces[(z*NumDivs)+x] = temp;
		} 
	}

	for (int x = 0; x < NumDivs*NumDivs; x++)
	{
		velocity[x] += (forces[x]*.001);
		vertices[x*3 + 1] += velocity[x];
		//vertices[x*3 + 1] = -17990 + (vertices[x*3 + 1] - -17990)*.8;

		forces[x]= 0.0f;
	}
	
	CalculateNormals();
}

void Water::Render()
{	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	
	glBindTexture(GL_TEXTURE_2D, Reflect);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glEnable(GL_COLOR_MATERIAL);
	
	glDisable(GL_CULL_FACE);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
	glColorPointer(4, GL_FLOAT, 0, colors);
	
	glLockArraysEXT(0, NumVerts);
	
	glDrawElements(GL_TRIANGLES, NumVerts, GL_UNSIGNED_SHORT, indices);
	
	glUnlockArraysEXT();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glEnable(GL_CULL_FACE);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void Water::Release()
{
	glDeleteTextures(1, &Reflect);
}

void TreeType::Init(string filenames[NumTreeTypes])
{
	for (int x = 0; x < NumTreeTypes; x++)
		TreeTypes[x] = Load2DTexture(filenames[x], false);
}

void TreeType::Release()
{
	glDeleteTextures(NumTreeTypes, TreeTypes);
}

void Tree::Init(GLfloat ratio, GLuint type, Vector pos, Vector *Player)
{
	TreeType = type;
	YXRatio = ratio;
	LoadObj(&TreeModel, "res/Tree/Tree.obj", 0.5, 0.5,  0.5, 1);
	GenNormals(&TreeModel);
	GenDisplayLists(&TreeModel);
	SetCollisionGroundFunction(&TreeModel, &InAnimateObjectGroundCollision);
	PlayerPos = Player;
	TreeModel.Physics.Position = pos;
}

void Tree::Render()
{
	Update(&TreeModel, .0013);
	GLfloat Dist = Distance(TreeModel.Physics.Position, *PlayerPos);
	if (Dist < 0)
	{
		Vector TopRight, TopLeft, BottomRight, BottomLeft;
		
		GLfloat m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);
		
		Vector Up = Vector(m[0], m[4], m[8]);
		Vector Right;
		Right = Vector(0.0, 2.0, 0.0);
		GLfloat halfheight;
		halfheight = 100*YXRatio;
		TopRight = (Right+Up)*50*Vector(-2, YXRatio, -2) + TreeModel.Physics.Position;
		TopLeft = (Up-Right)*50*Vector(-2, YXRatio, -2) + TreeModel.Physics.Position;
		BottomRight = (Right-Up)*50*Vector(-2, YXRatio, -2) + TreeModel.Physics.Position;
		BottomLeft = (Right+Up)*-50*Vector(-2, YXRatio, -2) + TreeModel.Physics.Position;
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TreeType);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(TopRight.x, TopRight.y + halfheight, TopRight.z);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(BottomRight.x, BottomRight.y + halfheight, BottomRight.z);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(BottomLeft.x, BottomLeft.y + halfheight, BottomLeft.z);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(TopLeft.x, TopLeft.y + halfheight, TopLeft.z);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_CULL_FACE);
		RenderModel(&TreeModel, true);
		glEnable(GL_CULL_FACE);
	}
}

void GameEnvironment::InitWithSkyPlane(string filename, int size, GLfloat height, GLfloat scale, string mapname, Vector *PlayerPos)
{
	sky = 0;
	plane.Init(filename, size, scale, height);
	//LoadObj(&Map, mapname, 2000, 1000, 2000, 50);
	LoadHeightMap(&Map, mapname, "res/Terrain/Terrain.mtl", "res/Terrain/Terrain.phs", 128, 800, 80, 800, 10);
	GenNormals(&Map);
	GenDisplayLists(&Map);
	WaterLevel.Init("res/m_up.tga", 100, Map.Geometry->Min, Map.Geometry->Max);
	string s[1] = {"res/tree2.tga"};
	Trees.Init(s);
	t.Init(1.573, Trees.TreeTypes[Deciduous], Vector(1000, 6000.0, 2500), PlayerPos);
	
	glEnable(GL_FOG);
	GLfloat fogColor[] = {0.35, 0.35, 1.0, 1.0};
	glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, .0000001);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 5.0);
}

void GameEnvironment::InitWithSkyBox(string filename, string filetype, GLfloat size, string mapname, Vector *PlayerPos)
{
	sky = 1;
	box.Init(filename, filetype, size);
	box.Position = PlayerPos;
	//LoadObj(&Map, mapname, 2000, 1000, 2000, 50);
	LoadHeightMap(&Map, mapname, "res/Terrain/Terrain.mtl", "res/Terrain/Terrain.phs", 128, 80, 4, 80, 10);
	GenNormals(&Map);
	GenDisplayLists(&Map);
	WaterLevel.Init("res/m_up.tga", 120, Map.Geometry->Min, Map.Geometry->Max);
	string s[1] = {"res/tree2.tga"};
	Trees.Init(s);
	t.Init(1.573, Trees.TreeTypes[Deciduous], Vector(1000, 6000.0, 2500), PlayerPos);
	
	glEnable(GL_FOG);
	GLfloat fogColor[] = {0.35, 0.35, 1.0, 1.0};
	glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, .0000001);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 5.0);
}

void GameEnvironment::InitWithSkyDome(string filename, int slices, GLfloat Radius, GLfloat damper, string mapname, Vector *PlayerPos)
{
	sky = 2;
	dome.Init(filename, slices, Radius, damper);
	//LoadObj(&Map, mapname, 2000, 1000, 2000, 50);
	LoadHeightMap(&Map, mapname, "res/Terrain/Terrain.mtl", "res/Terrain/Terrain.phs", 128, 800, 80, 800, 10);
	GenNormals(&Map);
	GenDisplayLists(&Map);
	WaterLevel.Init("res/m_up.tga", 100, Map.Geometry->Min, Map.Geometry->Max);
	string s[1] = {"res/tree2.tga"};
	Trees.Init(s);
	t.Init(1.573, Trees.TreeTypes[Deciduous], Vector(1000, 6000.0, 2500), PlayerPos);
	
	glEnable(GL_FOG);
	GLfloat fogColor[] = {0.35, 0.35, 1.0, 1.0};
	glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, .0000001);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 5.0);
}

void GameEnvironment::Render(Vector Animation)
{
	glDisable(GL_FOG);
	if (sky == 0)
		plane.Render(Animation);
	else if (sky == 1)
		box.Render();
	else
		dome.Render(Animation);
	glEnable(GL_FOG);
	
	if (PlayerPos->y >= WaterLevel.vertices[1])
	{
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, 0.003);
	}
	else
		glFogf(GL_FOG_DENSITY, 0.006);
	
	RenderModel(&Map, false);
	if (PlayerPos->y > WaterLevel.vertices[1])
		glDisable(GL_FOG);
	else
		glEnable(GL_FOG);
	t.Render();
	WaterLevel.Update();
	WaterLevel.Render();
}

void GameEnvironment::Release()
{
	ReleaseModel(&Map);
	if (sky == 0)
	{
		plane.Release();
	}
	else if (sky == 1)
	{
		box.Release();
	}
	else
	{
		dome.Release();
	}
	
	Trees.Release();
	ReleaseModel(&t.TreeModel);
	WaterLevel.Release();
}
