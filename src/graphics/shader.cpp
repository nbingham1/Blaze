#include "shader.h"
#include "../base.h"
#include "opengl.h"

shaderhdl::shaderhdl()
{
	vertex = 0xFFFFFFFF;
	fragment = 0xFFFFFFFF;
	program = 0xFFFFFFFF;
}

shaderhdl::shaderhdl(const shaderhdl &s)
{
	vertex = s.vertex;
	fragment = s.fragment;
	program = s.program;
}

shaderhdl::shaderhdl(string v, string f, bool raw)
{
	if (raw)
	{
		vertex = load_source(v, GL_VERTEX_SHADER);
		fragment = load_source(f, GL_FRAGMENT_SHADER);
	}
	else
	{
		vertex = load_file(v, GL_VERTEX_SHADER);
		fragment = load_file(f, GL_FRAGMENT_SHADER);
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
}

shaderhdl::~shaderhdl()
{
}

GLuint shaderhdl::load_file(string filename, GLuint type)
{
	GLuint handle = 0;

	file fin(filename, "rt");
	if (!fin.is_open())
	{
		cerr << "Error: Could not find file: " << filename << endl;
		return 0;
	}
	string source = fin.read_file();
	const char* data = source.data();
	int length = source.length();
	fin.close();

	handle = glCreateShader(type);
	glShaderSource(handle, 1, &data, &length);
	glCompileShader(handle);

	char temp[1024];
	length = 1023;
	glGetShaderInfoLog(handle, 1023, &length, temp);
	if (length > 0)
		cerr << temp;

	return handle;
}

GLuint shaderhdl::load_source(string source, GLuint type)
{
	GLuint handle = 0;
	const char* data = source.data();
	GLint length = source.length();

	if (length == 0)
		return 0;

	handle = glCreateShader(type);
	glShaderSource(handle, 1, &data, &length);
	glCompileShader(handle);

	char temp[1024];
	length = 1023;
	glGetShaderInfoLog(handle, 1023, &length, temp);
	if (length > 0)
		cerr << temp;

	return handle;
}

void shaderhdl::release()
{
	if (vertex != 0xFFFFFFFF)
		glDeleteShader(vertex);
	if (fragment != 0xFFFFFFFF)
		glDeleteShader(fragment);
	if (program != 0xFFFFFFFF)
		glDeleteShader(program);
	vertex = 0xFFFFFFFF;
	fragment = 0xFFFFFFFF;
	program = 0xFFFFFFFF;
}

void shaderhdl::bind()
{
	glUseProgram(program);
}

void shaderhdl::uniform(string location, float data)
{
	glUniform1f(glGetUniformLocation(program, location.data()), data);
}

void shaderhdl::uniform(string location, vec1f data)
{
	glUniform1fv(glGetUniformLocation(program, location.data()), 1, data.data);
}

void shaderhdl::uniform(string location, vec2f data)
{
	glUniform2fv(glGetUniformLocation(program, location.data()), 2, data.data);
}

void shaderhdl::uniform(string location, vec3f data)
{
	glUniform3fv(glGetUniformLocation(program, location.data()), 3, data.data);
}

void shaderhdl::uniform(string location, vec4f data)
{
	glUniform4fv(glGetUniformLocation(program, location.data()), 4, data.data);
}

void shaderhdl::uniform(string location, int data)
{
	glUniform1i(glGetUniformLocation(program, location.data()), data);
}

void shaderhdl::uniform(string location, vec1i data)
{
	glUniform1iv(glGetUniformLocation(program, location.data()), 1, data.data);
}

void shaderhdl::uniform(string location, vec2i data)
{
	glUniform2iv(glGetUniformLocation(program, location.data()), 2, data.data);
}

void shaderhdl::uniform(string location, vec3i data)
{
	glUniform3iv(glGetUniformLocation(program, location.data()), 3, data.data);
}

void shaderhdl::uniform(string location, vec4i data)
{
	glUniform4iv(glGetUniformLocation(program, location.data()), 4, data.data);
}

void shaderhdl::uniform(string location, texturehdl data)
{
	glUniform1i(glGetUniformLocation(program, location.data()), data.identity);
}

shaderhdl &shaderhdl::operator=(shaderhdl s)
{
	vertex = s.vertex;
	fragment = s.fragment;
	program = s.program;
	return *this;
}
