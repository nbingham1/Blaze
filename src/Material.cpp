/*
 *  Material.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/2/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Material.h"

void Material::Init()
{
	textures = NULL;
	curr = NULL;
	Next = NULL;
}

void Material::AddTexture(char *name, char *type, int depth)
{
	if (textures == NULL)
	{
		textures = (Texture*)malloc(sizeof(Texture));
		textures->Next = NULL;
		curr = textures;
	}
	else
	{
		curr->Next = (Texture*)malloc(sizeof(Texture));
		curr->Next->Next = NULL;
		curr = curr->Next;
	}
	curr->LoadTexture(name, type, depth);
}

void Material::AddTexture(GLuint tex)
{
	if (textures == NULL)
	{
		textures = (Texture*)malloc(sizeof(Texture));
		textures->Next = NULL;
		curr = textures;
	}
	else
	{
		curr->Next = (Texture*)malloc(sizeof(Texture));
		curr->Next->Next = NULL;
		curr = curr->Next;
	}
	curr->Map = tex;
	curr->LOD = false;
	curr->Depth = 1;
}

void Material::Load(char *vert, char *frag)
{
	printf("loading shaders...\n");
	int TextLen;
	char ErrText[256];
	
	TextLen = 256;
	VertShad = LoadGLSLShader(GL_VERTEX_SHADER_ARB, vert, ErrText, &TextLen);
	//if(!VertShad || TextLen > 0)
	//{
	//	cout << "Vertex Error: ";
		cout << ErrText << endl;
	//	exit(0);
	//}

	TextLen = 256;
	FragShad = LoadGLSLShader(GL_FRAGMENT_SHADER_ARB, frag, ErrText, &TextLen);
	//if(!FragShad || TextLen > 0)
	//{
	//	cout << "Fragment Error: ";
		cout << ErrText << endl;
	//	exit(0);
	//}

	ShadProg = glCreateProgramObjectARB();
	glAttachObjectARB(ShadProg, VertShad);
	glAttachObjectARB(ShadProg, FragShad);
	glLinkProgramARB(ShadProg);
}

void Material::Load()
{
	printf("loading shaders...\n");
	int TextLen;
	char ErrText[128];
	
	TextLen = 128;
	VertShad = LoadGLSLShader(GL_VERTEX_SHADER_ARB, Vert, ErrText, &TextLen);
	//if(!VertShad || TextLen > 0)
	//{
		cout << ErrText << endl;
	//	exit(0);
	//}

	TextLen = 128;
	FragShad = LoadGLSLShader(GL_FRAGMENT_SHADER_ARB, Frag, ErrText, &TextLen);
	//if(!FragShad || TextLen > 0)
	//{
		cout << ErrText << endl;
	//	exit(0);
	//}

	ShadProg = glCreateProgramObjectARB();
	glAttachObjectARB(ShadProg, VertShad);
	glAttachObjectARB(ShadProg, FragShad);
	glLinkProgramARB(ShadProg);
}

void Material::Use()
{
	Texture *curr = textures;
	int x = 0;
	while (curr != NULL)
	{
		glActiveTexture(GL_TEXTURE0 + x);

		if (curr->Depth == 1)
		{
			glBindTexture(GL_TEXTURE_2D, curr->Map);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, curr->Map);
		}
		glUniform1iARB(glGetUniformLocationARB(ShadProg, ("tex_" + string(int_to_char(x))).c_str()), x);
		x++;
		curr = curr->Next;
	}
	
	glUseProgramObjectARB(ShadProg);
}

void Material::ReleaseTextures()
{
	Texture *Current = textures, *Prev;
	while (Current != NULL)
	{
		Prev = Current;
		Current = Current->Next;
		Prev->Release();
		free(Prev);
		Prev = NULL;
	}
	
	textures = NULL;
}

void Material::Release()
{
	Texture *Current = textures, *Prev;
	while (Current != NULL)
	{
		Prev = Current;
		Current = Current->Next;
		Prev->Release();
		free(Prev);
	}
	
	glDeleteObjectARB(VertShad);
	glDeleteObjectARB(FragShad);
	glDeleteObjectARB(ShadProg);
}

void Material::ImportNumber(GLdouble n, char *name)
{
	glUniform1fARB(glGetUniformLocationARB(ShadProg, name), GLfloat(n));
}

void Material::ImportVector(Vector v, char *name)
{
	glUniform3fARB(glGetUniformLocationARB(ShadProg, name), GLfloat(v.x), GLfloat(v.y), GLfloat(v.z));
}
