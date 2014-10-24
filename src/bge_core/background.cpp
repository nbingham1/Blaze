#include "../bge_mtrl.h"

#include "background.h"
#include "planet.h"

backgroundhdl::backgroundhdl()
{
	geometry_object = 0;
	indices_object = 0;
	background_vx = 0;
	background_ft = 0;
	background_pm = 0;
	background_tx = 0;
}

backgroundhdl::~backgroundhdl()
{
	geometry_object = 0;
	indices_object = 0;
	background_vx = 0;
	background_ft = 0;
	background_pm = 0;
	background_tx = 0;
}

void backgroundhdl::initialize()
{
	float geometry[24];
	GLuint indices[24];

	for (int x = 0; x < 2; x++)
		for (int y = 0; y < 2; y++)
			for (int z = 0; z < 2; z++)
			{
				geometry[(z*4 + y*2 + x)*3 + 0] = float(x)*2.0 - 1.0;
				geometry[(z*4 + y*2 + x)*3 + 1] = float(y)*2.0 - 1.0;
				geometry[(z*4 + y*2 + x)*3 + 2] = float(z)*2.0 - 1.0;
			}

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 2;

	indices[4] = 4;
	indices[5] = 6;
	indices[6] = 7;
	indices[7] = 5;

	indices[8] = 0;
	indices[9] = 4;
	indices[10] = 5;
	indices[11] = 1;

	indices[12] = 1;
	indices[13] = 5;
	indices[14] = 7;
	indices[15] = 3;

	indices[16] = 3;
	indices[17] = 7;
	indices[18] = 6;
	indices[19] = 2;

	indices[20] = 2;
	indices[21] = 6;
	indices[22] = 4;
	indices[23] = 0;


	glGenBuffers(1, &geometry_object);
	glBindBuffer(GL_ARRAY_BUFFER, geometry_object);
	glBufferData(GL_ARRAY_BUFFER, 12*8, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*8, geometry);

	glGenBuffers(1, &indices_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*24, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*24, indices);

	// Load Shaders
	int TextLen;
	char ErrText[1024];

	TextLen = 1024;
	background_vx = LoadGLSLShader(GL_VERTEX_SHADER, "res/bge_core/background.vx", ErrText, &TextLen);
	printf("%s", ErrText);

	TextLen = 1024;
	background_ft = LoadGLSLShader(GL_FRAGMENT_SHADER, "res/bge_core/background.ft", ErrText, &TextLen);
	printf("%s", ErrText);

	background_pm = glCreateProgram();
	glAttachShader(background_pm, background_vx);
	glAttachShader(background_pm, background_ft);
	glLinkProgram(background_pm);

	int width, height;
	unsigned char *front, *back, *right, *left, *top, *bottom;
	front = rgb_tga("res/bge_core/front.tga", &width, &height);
	back = rgb_tga("res/bge_core/back.tga", &width, &height);
	right = rgb_tga("res/bge_core/right.tga", &width, &height);
	left = rgb_tga("res/bge_core/left.tga", &width, &height);
	top = rgb_tga("res/bge_core/bottom.tga", &width, &height);
	bottom = rgb_tga("res/bge_core/top.tga", &width, &height);

	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &background_tx);

	glBindTexture(GL_TEXTURE_CUBE_MAP, background_tx);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, right);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, top);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, front);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, back);
	delete [] front;
	delete [] back;
	delete [] right;
	delete [] left;
	delete [] top;
	delete [] bottom;
}

void backgroundhdl::render()
{
	glUseProgram(background_pm);

	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, background_tx);
	glUniform1i(glGetUniformLocation(background_pm, "tex_0"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_object);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 12, NULL);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void backgroundhdl::release()
{
	if (geometry_object != 0)
		glDeleteBuffers(1, &geometry_object);
	if (indices_object != 0)
		glDeleteBuffers(1, &indices_object);

	if (background_tx != 0)
		glDeleteTextures(1, &background_tx);

	// Release the Shaders
	if (background_vx != 0)
		glDeleteShader(background_vx);
	if (background_ft != 0)
		glDeleteShader(background_ft);
	if (background_pm != 0)
		glDeleteShader(background_pm);
}
